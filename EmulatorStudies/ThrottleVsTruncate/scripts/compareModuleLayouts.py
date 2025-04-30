import fileReaders
import tools
import pprint
from collections import defaultdict

cmssw = fileReaders.BuildCMSSWLayoutDict('data/CMSSW/cmssw_EE.json', 'data/CMSSW/cmssw_FH.json', 'data/CMSSW/cmssw_BH.json')

n_cmssw_layers = len(cmssw['layers'])
assert(n_cmssw_layers in [47, 50])
era = 0 if n_cmssw_layers == 50 else 1
tools.FilterRegion(cmssw, era)

daqmodules = fileReaders.ReadDaqModulesFile('data/FE/DaqModules.txt')

mappingV8 = fileReaders.BuildFeMappingLayoutDict(50, 'data/FE/FeMappingV8.txt', module_dict=daqmodules, forceSilicon=None)
mappingV9 = fileReaders.BuildFeMappingLayoutDictFromJSON(50, 'data/FE/FeMappingV9.json', module_dict=daqmodules, forceSilicon=None)

origgeo = fileReaders.ReadGeoSourceFile('hgcal_modmap/geometries/v11.5/geometry.hgcal.txt')
# Compare layers

def CompareLayouts(layout1, layout2, label1='Layout1', label2='Layout2'):
    l1 = set([X['global_index'] for X in layout1['layers']])
    l2 = set([X['global_index'] for X in layout2['layers']])
    l1map = {X['global_index']: i for (i, X) in enumerate(layout1['layers'])}
    l2map = {X['global_index']: i for (i, X) in enumerate(layout2['layers'])}
    layersToCheck = list(l1.intersection(l2))
    print 'Layers in both: %s (%i)' % (l1.intersection(l2), len(l1.intersection(l2)))
    print 'Layers only in %s: %s' % (label1, l1.difference(l2))
    print 'Layers only in %s: %s' % (label2, l2.difference(l1))

    inconsistencies = defaultdict(list)
    for layer in layersToCheck:
        print 'Checking modules in layer %i' % layer
        l1mods = set(layout1['layers'][l1map[layer]]['module_dict'].keys())
        l2mods = set(layout2['layers'][l2map[layer]]['module_dict'].keys())
        intersec = l1mods.intersection(l2mods)
        print 'Modules in both: (%i)' % (len(intersec))
        for mod in intersec:
            mod1 = layout1['layers'][l1map[layer]]['module_dict'][mod]
            mod2 = layout2['layers'][l2map[layer]]['module_dict'][mod]
            if not (mod1['type'] == mod2['type'] and mod1['shape'] == mod2['shape']):
                if (mod1['type'] == mod2['type']) and mod1['shape'] in ['F', 'L', 'R'] and mod2['shape'] in ['F', 'L', 'R']:
                    inconsistencies['FLR_mismatch'].append((layer, mod[0], mod[1]))
                elif (mod1['shape'] == mod2['shape']) and mod1['type'] == 1 and mod2['type'] == 0:
                    inconsistencies['LDtoHD'].append((layer, mod[0], mod[1]))
                elif (mod1['shape'] == mod2['shape']) and mod1['type'] == 0 and mod2['type'] == 1:
                    inconsistencies['HDtoLD'].append((layer, mod[0], mod[1]))
                else:
                    inconsistencies['other'].append((layer, mod[0], mod[1]))
                # print '%-10s %-3s %-3s | %-3s %-3s' % (mod, mod1['type'], mod1['shape'], mod2['type'], mod2['shape'])
        inconsistencies['OnlyIn%s' % label1].extend([(layer, X[0], X[1]) for X in l1mods.difference(l2mods)])
        inconsistencies['OnlyIn%s' % label2].extend([(layer, X[0], X[1]) for X in l2mods.difference(l1mods)])
    # pprint.pprint(dict(inconsistencies))
    for key in inconsistencies:
        print '[%s]: %s' % (key, inconsistencies[key])
    # print dict(inconsistencies)



# CompareLayouts(cmssw, mapping, label1='CMSSW', label2='FeMappingV8')

CompareLayouts(mappingV8, mappingV9, label1='FeMappingV8', label2='FeMappingV9')
