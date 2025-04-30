import json


def uvSector(layer, uv, era):
    sector = 0
    transition = 28 if era == 0 else 33
    if layer <= transition:  # CE-E
        if (uv[0] > 0 and uv[1] >= 0):
            sector = 0
        elif (uv[0] >= uv[1] and uv[1] < 0):
            sector = 2
        else:
            sector = 1
    elif ((layer % 2) == 1):  # CE-H Odd
        if (uv[0] >= 0 and uv[1] >= 0):
            sector = 0
        elif (uv[0] > uv[1] and uv[1] < 0):
            sector = 2
        else:
            sector = 1
    else:  # CE-H Even
        if (uv[0] >= 1 and uv[1] >= 1):
            sector = 0
        elif (uv[0] >= uv[1] and uv[1] < 1):
            sector = 2
        else:
            sector = 1
    return sector


def scintSector(iphi):
    sector = 0
    if (iphi < 12):
        sector = 0
    elif (iphi >= 12 and iphi < 24):
        sector = 1
    else:
        sector = 2
    return sector


def FilterRegion(layout, era):
    for layerinfo in layout['layers']:
        layer = layerinfo['global_index']
        to_delete = []
        for uv, module in layerinfo['module_dict'].iteritems():
            in_region = (uvSector(layer, tuple(uv), era) == 0) if module['type'] <= 1 else (scintSector(uv[1]) == 0)
            if not in_region:
                to_delete.append(uv)
        for d in to_delete:
            del layerinfo['module_dict'][d]


def EncodeModule(l, u, v):
    return 256 * (l - 1) + 16 * u + v


# Convert from LUV to L, U, V
def DecodeModule(idm):
    l = (idm / 256) + 1
    u = (idm % 256) / 16
    v = (idm % 256) % 16
    return (l, u, v)


def ReadModuleFile(filename):
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


# For reading data/FE/geomnew_corrected_withmult.txt
def ReadOrigGeoFile(filename):
    res = {}
    with open(filename) as module_file:
        for line in module_file:
            X = line.split()
            layer = int(X[0])
            u = int(X[6])
            v = int(X[7])
            av_hits = float(X[8])
            res[layer, u, v] = av_hits
    return res


# For reading the Mapping files
def BuildMapping(nlayers, file, reference_BH_file=None, module_dict=None):

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


# Read Andre's JSON file with the motherboard and wagon configs
def ReadMBJson(filename):
    with open(filename) as jsonfile:
        data = json.load(jsonfile)

        res = [dict() for X in xrange(50)]

        for mb in data:
            # print mb
            layer = int(mb[0]["LAYER"]) - 1
            layer_id = len(res[layer])
            res[layer][layer_id] = {
                'I': [],
                'O': []
            }
            for mod in mb:
                res[layer][layer_id][mod["PLACE"]].append((mod['U'], mod['V']))

        for il in xrange(28):
            nmb = len(res[il])
            for im in xrange(nmb):
                layer_id = nmb + im
                res[il][layer_id] = {
                    'I': [],
                    'O': []
                }
                for mod in res[il][im]['I']:
                    res[il][layer_id]['I'].append((mod[0] - mod[1], mod[0]))
                for mod in res[il][im]['O']:
                    res[il][layer_id]['O'].append((mod[0] - mod[1], mod[0]))
    return res


# Read the data rate summary file produced by the plotPacketWords.py script
def ReadRateSummaryFile(filename):
    res = [dict() for X in xrange(50)]
    with open(filename) as module_file:
        for line in module_file:
            if line.startswith('#'):
                continue
            v = line.split()
            if int(v[12]) >= 2:
                # Skip scintillator for now
                continue
            res[int(v[1]) - 1][int(v[2]), int(v[3])] = {
                'ref_elinks': int(v[5]),
                'elinks': int(v[7]),
                'type': int(v[12]),
                'shape': v[14]
            }
            # print res[int(v[1])][v[2], v[3]]
    return res
