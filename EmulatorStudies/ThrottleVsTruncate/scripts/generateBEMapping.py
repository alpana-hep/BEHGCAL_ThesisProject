import argparse
import pandas as pd
import numpy as np
import json
import random
from pprint import pprint

parser = argparse.ArgumentParser()
parser.add_argument('--fe-mapping', default='data/FE/FeMappingV9.json')
parser.add_argument('--rates', default='hitData_D49_ttbar_PU200_3iab_mipDef_occ_summary.txt')
parser.add_argument('--mode', choices=['random', 'simple'], default='simple')
parser.add_argument('--fpgas', type=int, default=16)
parser.add_argument('--slinks', type=int, default=12)
parser.add_argument('--output', default='BeMappingV9_16FPGA_simple.json')
args = parser.parse_args()

# random.seed(1234567)

fe_mapping_file = args.fe_mapping
rates_file = args.rates

df_rates = pd.read_csv(rates_file, delim_whitespace=True, comment='#')
rate_dict = {}
for idx, row in df_rates.iterrows():
    rate_dict[(row['l'], row['u'], row['v'])] = row['<n32bWords>']

fe_mapping = []
if fe_mapping_file.endswith('.json'):
    with open(fe_mapping_file) as jsonfile:
        fe_mapping = json.load(jsonfile)

lpgbt_dict = {}

nFPGAs = args.fpgas
slinks_per_FPGA = args.slinks

n_slinks = nFPGAs * slinks_per_FPGA

def MakelpGBTDict(idxs):
    idx0 = idxs[0]
    if len(idxs) >= 2:
        idx1 = idxs[1]
        type = 'double'
    else:
        idx1 = -1
        type = 'single'
    return {
        "id0": idx0,
        "id1": idx1,
        "rate": 0.,
        "modules": [],
        "type": type
    }

def MakeSLINKs(n):
    res = [{"id": X, "lpGBTs": [], "rate": 0.} for X in range(n)]
    return res

def MakeFPGAs(n):
    res = [{"id": X, "SLINKs": [], "rate": 0.} for X in range(n)]
    return res


if fe_mapping_file.endswith('.json'):
    for mb in fe_mapping:
        key = mb['lpGBTs'][0]
        lpgbt_dict[key] = MakelpGBTDict(mb['lpGBTs'])
        for mod in mb["modules"]:
            mod_tuple = (mb['layer'], mod['u'], mod['v'])
            lpgbt_dict[key]["rate"] += rate_dict[mod_tuple]
            lpgbt_dict[key]["modules"].append(mod_tuple)
else:
    with open(fe_mapping_file) as mapping_file:
        lpgbts = {

        }
        known_pairs = []
        all_pair_lpgbts = set()
        for line in mapping_file:
            v = line.split()
            LUV = (int(v[0]), int(v[1]), int(v[2]))
            n_lpGBT_DAQ = int(v[5])
            lpGBT_id1 = int(v[7])
            lpGBT_id2 = int(v[9])
            if lpGBT_id1 not in lpgbts:
                lpgbts[lpGBT_id1] = list()
            lpgbts[lpGBT_id1].append(LUV)
            if n_lpGBT_DAQ == 2:
                if lpGBT_id2 not in lpgbts:
                    lpgbts[lpGBT_id2] = list()
                lpgbts[lpGBT_id2].append(LUV)
                known_pairs.append((lpGBT_id1, lpGBT_id2))
        for pair in known_pairs:
            sorted_pair = sorted(pair)
            key = sorted_pair[0]
            all_pair_lpgbts.add(sorted_pair[0])
            all_pair_lpgbts.add(sorted_pair[1])
            if key not in lpgbt_dict:
                lpgbt_dict[key] = MakelpGBTDict(sorted_pair)
                all_mods = set(lpgbts[sorted_pair[0]] + lpgbts[sorted_pair[1]])
                for mod_tuple in all_mods:
                    lpgbt_dict[key]["rate"] += rate_dict[mod_tuple]
                    lpgbt_dict[key]["modules"].append(mod_tuple)
                del lpgbts[sorted_pair[0]]
                del lpgbts[sorted_pair[1]]
        for key in lpgbts.keys():
            if key not in lpgbt_dict and key not in all_pair_lpgbts:
                lpgbt_dict[key] = MakelpGBTDict([key])
                all_mods = set(lpgbts[key])
                for mod_tuple in all_mods:
                    lpgbt_dict[key]["rate"] += rate_dict[mod_tuple]
                    lpgbt_dict[key]["modules"].append(mod_tuple)
        # pprint(lpgbt_dict)

all_lpgts = set()
for X in lpgbt_dict.values():
    all_lpgts.add(X['id0'])
    if X['id1'] >= 0:
        all_lpgts.add(X['id1'])
print(len(all_lpgts))
print(all_lpgts)

n_doubles = sum(X['type'] == 'double' for X in lpgbt_dict.values())
n_singles = sum(X['type'] == 'single' for X in lpgbt_dict.values())
print('nSingles = %i' % n_singles)
print('nDoubles = %i' % n_doubles)

slinks = MakeSLINKs(n_slinks)


lpgbts_to_assign = lpgbt_dict.values()
# print(lpgbts_to_assign)

while len(lpgbts_to_assign) > 0:
    if args.mode == 'simple':
        lpgbts_to_assign.sort(key=lambda X: X['rate'])  # lowest to highest
        slinks.sort(key=lambda X: (X['rate'], X['id'])) # lowest to highest
    elif args.mode == 'random':
        random.shuffle(lpgbts_to_assign)
    this_lpgbt = lpgbts_to_assign.pop(-1) # take the highest rate from the end
    slinks[0]['rate'] += this_lpgbt['rate']
    slinks[0]['lpGBTs'].append(this_lpgbt)
    if args.mode == 'random':
        # Just pop the first one we just filled, and move it to the backs
        slinks.append(slinks.pop(0))

# Now balance lpGBT pairs within each slink
for slink in slinks:
    # The final list of pairs
    lpgbtpairs = []
    lpgbts = slink['lpGBTs']

    # First just add all the doubles
    for lpgbt in filter(lambda X: X['type'] == 'double', lpgbts):
        lpgbtpairs.append({"id": [lpgbt['id0'], lpgbt['id1']], "rate": lpgbt['rate'], "modules": lpgbt['modules']})
    
    # Make a list of the singles with basic rate info
    lpgbtsingles = []
    has_sevens = False
    for lpgbt in filter(lambda X: X['type'] == 'single', lpgbts):
        if len(lpgbt['modules']) >= 7:
            has_sevens = True
        lpgbtsingles.append({"id": lpgbt['id0'], "rate": lpgbt['rate'], "modules": lpgbt['modules']})
    # If we have an odd number, add a dummy one
    if len(lpgbtsingles) % 2 == 1:
        lpgbtsingles.append({"id": -1, "rate": 0., "modules": list()})
    
    # Generate the empty list of paired singles we want to populate
    lpgbtsingles_paired = [{"id": [], "rate": 0., "modules": list()} for _ in range(len(lpgbtsingles) / 2)]

    # Now do the usual "largest first" filling
    while len(lpgbtsingles) > 0:
        if has_sevens:
            # Assing 6's and 7's as a matter of priority
            lpgbtsingles.sort(key=lambda X: (int(len(X['modules']) >= 6), X['rate']))
        else:
            lpgbtsingles.sort(key=lambda X: X['rate'])
        lpgbtsingles_paired.sort(key=lambda X: X['rate'])
        this_lpgbt = lpgbtsingles.pop(-1)
        target = None
        for ip in range(len(lpgbtsingles_paired)):
            if len(lpgbtsingles_paired[ip]['id']) < 2 and len(this_lpgbt['modules']) + len(lpgbtsingles_paired[ip]['modules']) <= 12:
                target = ip
                break
        if target is None:
            lpgbtsingles_paired.append({"id": [], "rate": 0., "modules": list()})
            target = -1
            pprint(slink)
            # raise RuntimeError('Not possible to assign lpGBT')

        lpgbtsingles_paired[target]['id'].append(this_lpgbt['id'])
        lpgbtsingles_paired[target]['rate'] += this_lpgbt['rate']
        lpgbtsingles_paired[target]['modules'].extend(this_lpgbt['modules'])
    
    for pair in lpgbtsingles_paired:
        if len(pair['id']) == 1:
            pair['id'].append(-1)
        if len(pair['id']) == 2:
            lpgbtpairs.append(pair)
        # If this one is now full, pop it and add it to the main list of pairs
        # if len(lpgbtsingles_paired[target]['id']) == 2:
        #     lpgbtpairs.append(lpgbtsingles_paired.pop(target))
    slink['lpGBTPairs'] = lpgbtpairs
    for pair in lpgbtpairs:
        if len(pair['modules']) > 12:
            pprint(slink)

slinks.sort(key=lambda X: (X['rate'], X['id']))
# pprint(slinks)

slink_rates = [X['rate'] for X in slinks]
mean_rate = np.mean(slink_rates)
print('>> slink average / stddev: %f/%s' % (mean_rate, np.std(slink_rates)))


fpgas = MakeFPGAs(nFPGAs)

slinks_to_assign = list(slinks)
while len(slinks_to_assign) > 0:
    slinks_to_assign.sort(key=lambda X: X['rate'])  # lowest to highest
    fpgas.sort(key=lambda X: (X['rate'], X['id'])) # lowest to highest
    this_slink = slinks_to_assign.pop(-1) # take the highest rate from the end
    fpgas[0]['rate'] += this_slink['rate']
    fpgas[0]['SLINKs'].append(this_slink)

fpga_rates = [X['rate'] for X in fpgas]
mean_fpga_rate = np.mean(fpga_rates)
print('>> FPGA average / stddev: %f/%s' % (mean_fpga_rate, np.std(fpga_rates)))

fpgas.sort(key=lambda X: X['id'])

res = []

for fpga in fpgas:
    res_fpga = {
        "FPGA": {
            "INDEX": fpga['id'],
            "slink": []
        }
    }
    for islink, slink in enumerate(fpga['SLINKs']):
        lpgbt_list = slink['lpGBTs']
        lpgbts_paired = [X['id'] for X in slink['lpGBTPairs']]

        # for pairinfo in slink['lpGBTPairs']:
        #     lpgbts_paired.append([pairinfo['id0'], pairinfo['id1']])
        # lpgbt_single = []
        # # first add the doubles
        # for lpgbt in filter(lambda X: X['type'] == 'double', lpgbt_list):
        #     lpgbts_paired.append([lpgbt['id0'], lpgbt['id1']])
        # # Then group the singles
        # for lpgbt in filter(lambda X: X['type'] == 'single', lpgbt_list):
        #     if len(lpgbt_single) == 2:
        #         lpgbts_paired.append(list(lpgbt_single))
        #         del lpgbt_single[:]
        #     lpgbt_single.append(lpgbt['id0'])
        # if len(lpgbt_single) == 1:
        #     lpgbt_single.append(-1)
        # if len(lpgbt_single) == 2:
        #     lpgbts_paired.append(lpgbt_single)

        res_slink = [islink, 0, 0., ["Lpgbt Index", lpgbts_paired]]
        res_fpga["FPGA"]["slink"].append(res_slink)
    res.append(res_fpga)

# pprint.pprint(FPGAs)
# pprint.pprint(res)

with open(args.output, 'w') as file:
    file.write(json.dumps(res, indent=2))
