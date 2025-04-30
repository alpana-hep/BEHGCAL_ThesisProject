import json
# import argparse
# import sys
# import ROOT
# import copy
# import plotlayer
# import plotting
import fileReaders


module_dict = fileReaders.Read2021GeoSourceFile('data/FE/geomdetailed10052021.txt')


# Now have to infer which modules are connected to 'c'
c_mods = dict()
f_mods = dict()
for i, mod in enumerate(module_dict):
    if mod['modules'][0]['shape'] == 'c' and mod['type'] == 1:
        c_mods[(mod['layer'], mod['modules'][0]['u'], mod['modules'][0]['v'])] = i
    if mod['modules'][0]['shape'] == 'F' and mod['type'] == 1:
        f_mods[(mod['layer'], mod['modules'][0]['u'], mod['modules'][0]['v'])] = i


mod_c_assignments = dict()
assigned_f = set()
mod_c_ambiguous = list()
for ic in range(3):
    print('Iteration %i' % ic)
    n_unmatched = 0
    for l, u, v in c_mods:
        cmod = (l, u, v)
        if cmod in mod_c_assignments:
            continue
        neighbours = [(u - 1, v), (u + 1, v), (u, v - 1), (u, v + 1), (u - 1, v - 1), (u + 1, v + 1)]
        foundmods = []
        for un, vn in neighbours:
            searchmod = (l, un, vn)
            if searchmod in f_mods and searchmod not in assigned_f:
                foundmods.append(searchmod)
        if len(foundmods) == 1:
            print('MATCH %s --> %s' % (cmod, foundmods))
            mod_c_assignments[cmod] = tuple(foundmods[0])
            assigned_f.add(foundmods[0])
        elif ic >= 2 and len(foundmods) >= 2:
            mod_c_ambiguous.append(cmod)
            mod_c_assignments[cmod] = tuple(foundmods[0])
            assigned_f.add(foundmods[0])
        else:
            n_unmatched += 1
            print('%s --> %s' % (cmod, foundmods))
    print('Matched: %i' % len(mod_c_assignments))
    print('Unmatched: %i' % n_unmatched)
    print('mod_c_ambiguous: %i' % len(mod_c_ambiguous))


for mod in assigned_f:
    module_dict[f_mods[mod]]["modules"][0]["shape"] = 'L'


with open('FeMappingV10Dummy2.json', 'w') as out_file:
    out_file.write(json.dumps(module_dict))
