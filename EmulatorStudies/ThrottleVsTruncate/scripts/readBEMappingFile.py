import csv
import pprint
import sys
import json

filein = sys.argv[1]
fileout = sys.argv[2]

FPGAs = {}

with open(filein) as file:
    reader = csv.DictReader(file)
    for row in reader:
        print(row)
        fpga = int(row['FPGA']) - 1
        if fpga not in FPGAs:
            FPGAs[fpga] = {}
        slink = int(row['Slinks'])
        print(slink)
        if slink not in FPGAs[fpga]:
            FPGAs[fpga][slink] = {'Single': [], 'Double': []}
        FPGAs[fpga][slink][row['link variable']].append(int(row['DAQlpGBT']))


for fpga in FPGAs:
    for slink in FPGAs[fpga]:
        pair_dict = FPGAs[fpga][slink]
        pair_dict['merged'] = list()
        if len(pair_dict['Single']) % 2 == 1:
            pair_dict['Single'].append(-1)
        for i in range(len(pair_dict['Double'])):
            pair_dict['merged'].append([pair_dict['Double'][i], -1])
        for i in range(int(len(pair_dict['Single']) / 2)):
            pair_dict['merged'].append([pair_dict['Single'][2 * i + 0], pair_dict['Single'][2 * i + 1]])


res = []

for fpga in sorted(FPGAs.keys()):
    res_fpga = {
        "FPGA": {
            "INDEX": fpga,
            "slink": []
        }
    }
    for i, slink in enumerate(sorted(FPGAs[fpga].keys())):
        res_slink = [i, 0, 0., ["Lpgbt Index", FPGAs[fpga][slink]['merged']]]
        res_fpga["FPGA"]["slink"].append(res_slink)
    res.append(res_fpga)


pprint.pprint(FPGAs)
pprint.pprint(res)

with open(fileout, 'w') as file:
    file.write(json.dumps(res, indent=2))
