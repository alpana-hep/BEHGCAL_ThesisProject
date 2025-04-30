import json
import pandas as pd
# import numpy as np


def Read2021GeoSourceFile(filename):
    mb_dummy = 0
    res = []
    with open(filename) as module_file:
        for line in module_file:
            v = line.split()
            l = int(v[0])
            s = str(v[1])
            shape = s[0]
            itype = int(s[1] != 'I')
            center_x = float(v[2])
            center_y = float(v[3])
            irot = int(v[4])
            nv = int(v[5])
            vx = []
            vy = []
            icurr = 6
            for i in range(nv + 1):
                vx.append(float(v[icurr + 0]))
                vy.append(float(v[icurr + 1]))
                icurr += 2
            iv = int(v[-1])
            iu = int(v[-2])
            res.append({
                "layer": l,
                "MB": mb_dummy,
                "type": itype,
                "lpGBTs": [mb_dummy],
                "modules": [
                    # {"u": iu, "v": iv, "elinks": [1], "shape": shape, "vx": vx, "vy": vy}
                    {"u": iu, "v": iv, "elinks": [1], "shape": shape}
                ]
                })
            mb_dummy += 1
    return res




def ReadGeoSourceFile(filename):
    df = pd.read_csv(
        filename,
        sep=' ',
        comment='#'
    )
    layers = sorted(set(df['plane']))
    # print layers
    ldict = {l: i for i, l in enumerate(layers)}
    mapping = {
        "layers": [dict(global_index=x, module_dict={}) for x in layers]
    }
    # print mapping
    for idx, row in df.iterrows():
        npoints = row['nvertices']
        p_x = []
        p_y = []
        for ip in range(npoints + 1):
            p_x.append(row['vx_%i' % ip])
            p_y.append(row['vy_%i' % ip])
        mapping['layers'][ldict[row['plane']]]['module_dict'][(row['u'], row['v'])] = {
            "UV": (row['u'], row['v']),
            "type": int(not row['HDorLD']),
            'shape': row['itype'][0],
            'vx': p_x,
            'vy': p_y
        }

    return mapping


def ReadDaqModulesFile(filename):
    res = {}
    with open(filename) as module_file:
        for line in module_file:
            v = line.split()
            modtype = int(v[0])
            shape = v[1]
            ncells = int(v[2])
            ncalib = int(v[3])
            res[(modtype, shape)] = {
                'n_total_cells': ncells,
                'n_calib_cells': ncalib
            }
    return res


def BuildFeMappingLayoutDict(nlayers, file, reference_BH_file=None, module_dict=None, forceSilicon=None):

    mapping = {
        "layers": [dict() for x in range(nlayers)]
    }
    for i in xrange(nlayers):
        mapping['layers'][i].update({
            'global_index': i + 1,
            'module_dict': {}
            })

    # This part does not really work now we have the v8 mapping file and the number
    # of tiles can vary a lot due to the partials
    if reference_BH_file is not None:
        ref_BH = {}
        tilecounts = {}
        with open(reference_BH_file) as jsonfile:
            ref_BH = json.load(jsonfile)
            for layer in ref_BH['layers']:
                tilecounts[layer['global_index']] = dict()
                for tb in layer['scint_tileboards']:
                    tilecounts[layer['global_index']][tuple(tb['index'])] = len(tb['scint_tiles'])

    with open(file) as mapping_file:
        for line in mapping_file:
            v = line.split()
            global_index = int(v[0])
            UV = (int(v[1]), int(v[2]))
            modtype = int(v[3])
            modshape = v[4]
            n_lpGBT_DAQ = int(v[5])
            # n_lpGBT_trg = int(v[6])
            lpGBT_id1 = int(v[7])
            lpGBT_id1_elinks = int(v[8])
            lpGBT_id2 = int(v[9])
            lpGBT_id2_elinks = int(v[10])
            # if modtype == 1 and n_lpGBT_DAQ >= 1:
            if modtype <= 1 and modshape == 'F' and forceSilicon is not None:
                modtype = int(forceSilicon)
            mapping['layers'][global_index - 1]['module_dict'][(UV[0], UV[1])] = {
                "UV": UV,
                "type": modtype,
                'shape': modshape,
                'n_lpGBT_DAQ': n_lpGBT_DAQ,
                'lpGBT_id1': lpGBT_id1,
                'lpGBT_id1_elinks': lpGBT_id1_elinks,
                'lpGBT_id2': lpGBT_id2,
                'lpGBT_id2_elinks': lpGBT_id2_elinks
            }
            if modtype == 2 and reference_BH_file is not None:
                mapping['layers'][global_index - 1]['module_dict'][(UV[0], UV[1])]['n_tiles'] = tilecounts[global_index].get(UV)
            if modtype >= 2 and module_dict is not None:
                mapping['layers'][global_index - 1]['module_dict'][(UV[0], UV[1])]['n_tiles'] = module_dict.get((modtype, modshape))['n_total_cells']
    return mapping


def BuildFeMappingLayoutDictFromJSON(nlayers, file, module_dict=None, forceSilicon=None):

    mapping = {
        "layers": [dict() for x in range(nlayers)]
    }
    for i in xrange(nlayers):
        mapping['layers'][i].update({
            'global_index': i + 1,
            'module_dict': {}
            })

    with open(file) as mapping_file:
        js = json.load(mapping_file)
        for mb in js:
            global_index = mb['layer']
            modtype = mb['type']
            lpgbts = mb['lpGBTs']
            for mod in mb['modules']:
                UV = (mod['u'], mod['v'])
                modshape = mod['shape']
                thismodtype = modtype
                if thismodtype <= 1 and modshape == 'F' and forceSilicon is not None:
                    thismodtype = int(forceSilicon)
                n_lpGBT_DAQ = len([X for X in mod['elinks'] if X > 0])
                lpGBT_id1 = -1
                lpGBT_id1_elinks = -1
                lpGBT_id2 = -1
                lpGBT_id2_elinks = -1
                setid = 0
                for i, n in zip(lpgbts, mod['elinks']):
                    if n > 0:
                        if setid == 2:
                            raise RuntimeError('Trying to assign more than three lpGBTs')
                        if setid == 0:
                            lpGBT_id1 = i
                            lpGBT_id1_elinks = n
                            setid += 1
                        elif setid == 1:
                            lpGBT_id2 = i
                            lpGBT_id2_elinks = n
                            setid += 1
                mapping['layers'][global_index - 1]['module_dict'][(UV[0], UV[1])] = {
                    "UV": UV,
                    "type": thismodtype,
                    'shape': modshape,
                    'n_lpGBT_DAQ': n_lpGBT_DAQ,
                    'lpGBT_id1': lpGBT_id1,
                    'lpGBT_id1_elinks': lpGBT_id1_elinks,
                    'lpGBT_id2': lpGBT_id2,
                    'lpGBT_id2_elinks': lpGBT_id2_elinks
                }
                if thismodtype >= 2 and module_dict is not None:
                    mapping['layers'][global_index - 1]['module_dict'][(UV[0], UV[1])]['n_tiles'] = module_dict.get((modtype, modshape))['n_total_cells']
    return mapping

def BuildCMSSWLayoutDict(file_EE, file_FH, file_BH, replaceShape=True):
    PARTIALS_DICT = {
        -99: 'F',
        0: 'F',
        1: 'b',
        2: 'g',
        3: 'g',
        4: 'a',
        5: 'd',
        6: 'd',
        7: 'c',
        8: 'a',
        9: 'b'

    }
    global_to_list = -1
    cmssw = {
        'layers': list()
    }

    cmssw_EE = {}
    with open(file_EE) as jsonfile:
        cmssw_EE = json.load(jsonfile)

    cmssw_FH = {}
    with open(file_FH) as jsonfile:
        cmssw_FH = json.load(jsonfile)

    cmssw_BH = {}
    with open(file_BH) as jsonfile:
        cmssw_BH = json.load(jsonfile)

    nlayers_EE = len(cmssw_EE['layers'])
    print '> [BuildFullCMSSW] EE geometry has %i layers' % nlayers_EE
    nlayers_FH = len(cmssw_FH['layers'])
    print '> [BuildFullCMSSW] FH geometry has %i layers' % nlayers_FH
    nlayers_BH = len(cmssw_BH['layers'])
    print '> [BuildFullCMSSW] BH geometry has %i layers' % nlayers_BH

    nlayers_total = nlayers_EE + nlayers_FH
    print '> [BuildFullCMSSW] Creating geometry dictionary with %i layers' % nlayers_total

    for il in xrange(nlayers_total):
        cmssw['layers'].append({
            'list_index': il,
            'global_index': il + 1,
            'module_dict': dict()})

    for layer in cmssw_EE['layers']:
        for mod in layer['modules']:
            cmssw['layers'][layer['global_index'] + global_to_list]['module_dict'][(mod['UV'][0], mod['UV'][1])] = dict(mod)
    for layer in cmssw_FH['layers']:
        for mod in layer['modules']:
            cmssw['layers'][layer['global_index'] + global_to_list]['module_dict'][(mod['UV'][0], mod['UV'][1])] = dict(mod)
    for layer in cmssw_BH['layers']:
        for mod in layer['modules']:
            cmssw['layers'][layer['global_index'] + global_to_list]['module_dict'][(mod['UV'][0], mod['UV'][1])] = dict(mod)
    for layer in cmssw['layers']:
        for mod in layer['module_dict'].values():
            mod['orig_shape'] = mod['shape']
            if mod['shape'] in PARTIALS_DICT:
                mod['shape'] = PARTIALS_DICT[mod['shape']]
            else:
                raise RuntimeError('BuildFullCMSSW: module shape %s not found' % mod['shape'])
    return cmssw
