import fileReaders
import tools
import pprint
import sys
import json
from collections import defaultdict

assert(len(sys.argv) >= 3)

filein = sys.argv[-2]
fileout = sys.argv[-1]

with open(filein) as jsonfile:
    js = json.load(jsonfile)

daqmodules = fileReaders.ReadDaqModulesFile('data/FE/DaqModules.txt')

mappingV8 = fileReaders.BuildFeMappingLayoutDict(50, 'data/FE/FeMappingV8.txt', module_dict=daqmodules, forceSilicon=None)

pair_lookup = {}

for layer_dict in mappingV8['layers']:
    for uv, mod in layer_dict['module_dict'].iteritems():
        if mod['type'] == 0 and mod['lpGBT_id1'] >= 0 and mod['lpGBT_id2'] >= 0:
            print (mod['lpGBT_id1'], mod['lpGBT_id2'])
            pair_lookup[mod['lpGBT_id1']] = mod['lpGBT_id2']

all_lpgbts = set()
for d in js:
    for slink in d['FPGA']['slink']:
        for pairs in slink[3][1]:
            print(pairs)
            if pairs[0] >= 0:
                all_lpgbts.add(pairs[0])
            if pairs[1] >= 0:
                all_lpgbts.add(pairs[1])


for d in js:
    for slink in d['FPGA']['slink']:
        for pairs in slink[3][1]:
            print(pairs)
            if pairs[0] >= 0 and pairs[1] == -1 and pairs[0] in pair_lookup and pair_lookup[pairs[0]] not in all_lpgbts:
                pairs[1] = pair_lookup[pairs[0]]
                print('Linked %i to %i' % (pairs[1], pairs[0]))
                del pair_lookup[pairs[0]]

print(pair_lookup)

with open(fileout, 'w') as out_file:
    out_file.write(json.dumps(js, separators=(',', ': '), indent=2))

