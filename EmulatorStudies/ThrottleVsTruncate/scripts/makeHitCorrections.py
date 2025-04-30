import json
import argparse
import sys
import ROOT
import copy
import plotlayer
import plotting
import fileReaders


FIXED_DENSITY_EXPONENTS_TOTAL = [
    0.419, 0.420, 0.437, 0.426, 0.436, 0.419, 0.424, 0.405, 0.407, 0.388, 0.389, 0.371, 0.371, 0.355, 0.356, 0.343, 0.345, 0.334, 0.338, 0.328, 0.332, 0.324, 0.327, 0.319, 0.323, 0.315, 0.320, 0.312, 0.316, 0.317, 0.324, 0.327, 0.327, 0.333, 0.346, 0.342, 0.337, 0.347, 0.359, 0.375, 0.433, 0.433, 0.433, 0.433, 0.433, 0.433, 0.433, 0.433, 0.433, 0.433
]

FIXED_DENSITY_EXPONENTS_TOA = [
    0.173, 0.217, 0.199, 0.237, 0.205, 0.232, 0.201, 0.225, 0.195, 0.220, 0.193, 0.218, 0.196, 0.220, 0.199, 0.223, 0.206, 0.228, 0.209, 0.231, 0.213, 0.230, 0.214, 0.229, 0.215, 0.230, 0.213, 0.231, 0.215, 0.215, 0.204, 0.196, 0.197, 0.196, 0.195, 0.196, 0.209, 0.211, 0.206, 0.206, 0.185, 0.185, 0.185, 0.185, 0.185, 0.185, 0.185, 0.185, 0.185, 0.185
]

FIXED_DENSITY_EXPONENTS_BXM1 = [
    0.294, 0.333, 0.335, 0.354, 0.345, 0.357, 0.345, 0.355, 0.341, 0.352, 0.336, 0.346, 0.335, 0.348, 0.336, 0.350, 0.341, 0.354, 0.343, 0.357, 0.343, 0.354, 0.348, 0.357, 0.349, 0.357, 0.345, 0.355, 0.343, 0.348, 0.335, 0.332, 0.334, 0.335, 0.349, 0.343, 0.351, 0.346, 0.335, 0.352, 0.203, 0.203, 0.203, 0.203, 0.203, 0.203, 0.203, 0.203, 0.203, 0.203
]


def ValidateRotations(cmssw):
    era = 0 if len(cmssw['layers']) == 50 else 1
    for layer in cmssw['layers']:
        il = layer['global_index']
        for key, mod in layer['module_dict'].iteritems():
            if mod['type'] <= 1:
                # Silicon
                rotated_uv = ROOT.pair(int, int)(tuple(mod['UV']))
                sector = ROOT.uvMapping(il, rotated_uv, era)
                rotated_uv = tuple(rotated_uv)
                r_key = (rotated_uv[0], rotated_uv[1])
                if r_key in layer['module_dict']:
                    r_mod = layer['module_dict'][r_key]
                    r_sector = ROOT.uvMapping(il, tuple(rotated_uv), era)
                    if r_sector != 0:
                        print '>> [RotationTestWrongSector] Layer %i, sector %i, rotation %s -> %s mapped to sector %i instead of 0' % (il, sector, key, r_key, r_sector)
                    if r_mod['type'] != mod['type']:
                        print '>> [RotationTestInconsistentType] Layer %i, sector %i, rotation %s -> %s has inconsistent type: %i (mapped), %i (orig)' % (il, sector, key, r_key, mod['type'], r_mod['type'])
                    if r_mod['n_total_cells'] != mod['n_total_cells']:
                        print '>> [RotationTestInconsistentCells] Layer %i, sector %i, rotation %s -> %s has inconsistent n_total_cells: %i (mapped), %i (orig)' % (il, sector, key, r_key, mod['n_total_cells'], r_mod['n_total_cells'])
                    if r_mod['n_unmasked_cells'] != mod['n_unmasked_cells']:
                        print '>> [RotationTestInconsistentCells] Layer %i, sector %i, rotation %s -> %s has inconsistent n_unmasked_cells: %i (mapped), %i (orig)' % (il, sector, key, r_key, mod['n_unmasked_cells'], r_mod['n_unmasked_cells'])
                    if r_mod['shape'] != mod['shape']:
                        print '>> [RotationTestInconsistentShape] Layer %i, sector %i, rotation %s -> %s has inconsistent shape: %s (mapped), %s (orig)' % (il, sector, key, r_key, mod['shape'], r_mod['shape'])

                else:
                    print '>> [RotationTestNotFound] Layer %i, sector %i, rotation %s -> %s not found in sector 0' % (il, sector, key, r_key)


def ValidateBH():
    cmssw_BH = {}
    with open('cmssw_BH.json') as jsonfile:
        cmssw_BH = json.load(jsonfile)

    ref_BH = {}
    with open('reference_BH.json') as jsonfile:
        ref_BH = json.load(jsonfile)

    print '>> %-50s %-10s %-10s' % ('', "Reference", "CMSSW")

    print '>> %-50s %-10i %-10i' % ('Number of layers', len(ref_BH['layers']), len(cmssw_BH['layers']))

    min_layers = min(len(ref_BH['layers']),len(cmssw_BH['layers']))
    for il in xrange(min_layers):
        cmssw_tiles_in_layer = 0
        ref_tiles_in_layer = 0
        ref_tiles = dict()
        for X in cmssw_BH['layers'][il]['modules']:
            cmssw_tiles_in_layer += X['n_total_cells']
        for tb in ref_BH['layers'][il]['scint_tileboards']:
            ref_tiles[tuple(tb['index'])] = len(tb['scint_tiles'])
            ref_tiles_in_layer += len(tb['scint_tiles'])
        print '>> %-50s %-10i %-10i' % ('Tiles in layer %i' %  ref_BH['layers'][il]['global_index'], ref_tiles_in_layer, cmssw_tiles_in_layer)


def GenerateRules(cmssw, mapping, module_dict, addCalibrationChannelCounts=None, cellCount='n_total_cells', mainStrategy='pessimistic'):
    res = copy.deepcopy(mapping)
    for il, layer in enumerate(res['layers']):
        for key, mod in layer['module_dict'].iteritems():
            cmssw_mod = cmssw['layers'][il]['module_dict'].get(key)
            mod['cmssw_orig_shape'] = cmssw_mod['orig_shape']
            mod['ScaleN30'] = 1.0
            if cmssw_mod is None:
                print '>> No CMSSW module for %i:%s, shape=%s' % (layer['global_index'], key, mod['shape'])
                mod['TransformMethod'] = 'Zero'
                # print mod
                continue


            if mod['type'] in [0, 1]:
                cmssw_shape = cmssw_mod['shape']
                mod['cmssw_shape'] = cmssw_shape
                mod['cmssw_type'] = cmssw_mod['type']
                if (mod['type'], mod['shape']) not in module_dict:
                    n_target = 0
                else:
                    n_target = module_dict.get((mod['type'], mod['shape']), 0)['n_total_cells']

            elif mod['type'] >= 2:
                mod['cmssw_shape'] = 'F'
                mod['cmssw_type'] = 2
                n_target = mod['n_tiles']
            
            n_src = cmssw_mod[cellCount]
            mod['cmssw_n_cells'] = n_src
            mod['n_cells'] = n_target

            if mainStrategy == 'notransform':
                mod['TransformMethod'] = 'NoTransform'
                continue

            if mod['cmssw_type'] == 1 and mod['type'] == 0:
                # LDM->HDM
                mod['ScaleN30'] = 0.5
            elif mod['cmssw_type'] == 0 and mod['type'] == 1:
                mod['ScaleN30'] = 2.0


            if mod['type'] in [0, 1] and mod['shape'] == 'c':
                mod['TransformMethod'] = 'Zero'
                mod['n_cells'] = 0
                continue

            if n_src == 0:
                # This can happen if we use n_unmasked
                mod['TransformMethod'] = 'Zero'
                # print mod
                continue

            if n_src == n_target:
                mod['TransformMethod'] = 'NoTransform'
                # print mod
                continue

            # For partials we do not attempt any LD/HD conversion
            if mod['type'] in [0, 1] and mod['cmssw_type'] == mod['type']:
                mod['TransformMethod'] = 'FixedOccupancy'
                mod['FixedOccupancyFrac'] = float(n_target) / float(n_src)
                continue

            if n_target > n_src:
                if mainStrategy == 'pessimistic':
                    mod['TransformMethod'] = 'FixedOccupancy'
                    mod['FixedOccupancyFrac'] = float(n_target) / float(n_src)
                    # print mod
                elif mainStrategy == 'optimistic':
                    mod['TransformMethod'] = 'FixedNumber'
                    mod['FixedNumberMax'] = n_target
                elif mainStrategy == 'fixdensity':
                    mod['TransformMethod'] = 'FixedDensity'
                    mod['FixedDensitySrc'] = float(n_src)
                    mod['FixedDensityTarget'] = float(n_target)
                elif mainStrategy == 'fixdensity2':
                    if mod['cmssw_type'] == 1 and mod['type'] == 0 and mod['cmssw_shape'] == 'F' and mod['shape'] == 'F':
                        mod['TransformMethod'] = 'FixedDensity2'
                        mod['FixedDensitySrc'] = float(n_src)
                        mod['FixedDensityTarget'] = float(n_target)
                        mod['FixedDensityExponentTotal'] = FIXED_DENSITY_EXPONENTS_TOTAL[il]
                        mod['FixedDensityExponentTOA'] = FIXED_DENSITY_EXPONENTS_TOA[il]
                        mod['FixedDensityExponentBXM1'] = FIXED_DENSITY_EXPONENTS_BXM1[il]
                    else:
                        mod['TransformMethod'] = 'FixedDensity'
                        mod['FixedDensitySrc'] = float(n_src)
                        mod['FixedDensityTarget'] = float(n_target)

                else:
                    raise RuntimeError('mainStrategy not valid')
                continue
            else:
                if mainStrategy == 'pessimistic':
                    mod['TransformMethod'] = 'FixedNumber'
                    mod['FixedNumberMax'] = n_target
                elif mainStrategy == 'optimistic':
                    mod['TransformMethod'] = 'FixedOccupancy'
                    mod['FixedOccupancyFrac'] = float(n_target) / float(n_src)
                elif mainStrategy == 'fixdensity':
                    mod['TransformMethod'] = 'FixedDensity'
                    mod['FixedDensitySrc'] = float(n_src)
                    mod['FixedDensityTarget'] = float(n_target)
                elif mainStrategy == 'fixdensity2':
                    mod['TransformMethod'] = 'FixedDensity'
                    mod['FixedDensitySrc'] = float(n_src)
                    mod['FixedDensityTarget'] = float(n_target)
                else:
                    raise RuntimeError('mainStrategy not valid')
                # print mod
                continue

            print '>> ERROR, how did I get here?'
            print cmssw_mod
            print mod

    counters = [0] * 11
    for il, layer in enumerate(res['layers']):
        for key, rule in layer['module_dict'].iteritems():
            if 'cmssw_type' not in rule:
                continue
            if (rule['cmssw_type'] == rule['type'] == 0) and (rule['cmssw_shape'] == rule['shape'] == 'F'):
                counters[0] += 1
            elif rule['cmssw_type'] == rule['type'] == 1 and (rule['cmssw_shape'] == rule['shape'] == 'F'):
                counters[1] += 1
            elif rule['cmssw_type'] >= 2 and rule['type'] >=2:
                counters[2] += 1
            elif (rule['cmssw_type'] == rule['type'] == 0) and (rule['shape'] != 'F'):
                counters[3] += 1
            elif (rule['cmssw_type'] == rule['type'] == 1) and (rule['shape'] != 'F'):
                counters[4] += 1
            elif (rule['cmssw_type'] == 0 and rule['type'] == 1):
                counters[5] += 1
            elif (rule['cmssw_type'] == 1 and rule['type'] == 0):
                counters[6] += 1
            elif rule['cmssw_type'] == rule['type'] == 0 and (rule['cmssw_shape'] != 'F' and rule['shape'] == 'F'):
                counters[7] += 1
            elif rule['cmssw_type'] == rule['type'] == 1 and (rule['cmssw_shape'] != 'F' and rule['shape'] == 'F'):
                counters[8] += 1

            if (rule['cmssw_type'] == 0 and rule['type'] == 1) and rule['cmssw_shape'] != rule['shape']:
                counters[9] += 1
                print res['layers'][il]['global_index'], key
            if (rule['cmssw_type'] == 1 and rule['type'] == 0) and rule['cmssw_shape'] != rule['shape']:
                counters[9] += 1
    print '>> Counters: %s' % counters
    return res


parser = argparse.ArgumentParser()
parser.add_argument('--strategy', default='pessimistic', choices=['pessimistic', 'optimistic', 'notransform', 'fixdensity', 'fixdensity2'])
parser.add_argument('--plots', action='store_true')
parser.add_argument('--forcetype', default=None, type=int)
parser.add_argument('--mapping', default='data/FE/FeMappingV9.json')
parser.add_argument('--cell-counts', default='n_total_cells')
parser.add_argument('--cmssw-dir', default='data/CMSSW')
args = parser.parse_args()

drawRate = False
drawEOLRate = False
drawRatio = False
doEE = False

col0 = ROOT.gStyle.GetColorPalette(0)
ncols = ROOT.gStyle.GetNumberOfColors()
min_rate = 0.
max_rate = 300.
if drawRatio:
    min_rate = 0
    max_rate = 4

hit_data = {}
# with open('reference_hits_official.json') as jsonfile:
#     hit_data = json.load(jsonfile)
# hit_data_EOL = {}
# with open('reference_hits_EOL.json') as jsonfile:
#     hit_data_EOL = json.load(jsonfile)
# if drawEOLRate:
#     hit_data = hit_data_EOL

# cmssw = BuildFullCMSSW('resources/cmssw_EE.json', 'resources/cmssw_FH.json', 'resources/cmssw_BH.json')
cmssw = fileReaders.BuildCMSSWLayoutDict('%s/cmssw_EE.json' % args.cmssw_dir, '%s/cmssw_FH.json' % args.cmssw_dir, '%s/cmssw_BH.json' % args.cmssw_dir)

n_cmssw_layers = len(cmssw['layers'])

ROOT.gROOT.ProcessLine('#include "HGCalBEStudies/NTupleProduction/interface/Rotations.h"')

ValidateRotations(cmssw)

# ValidateBH()
module_dict = fileReaders.ReadDaqModulesFile('data/FE/DaqModules.txt')

# mapping = fileReaders.BuildFeMappingLayoutDict(n_cmssw_layers, 'data/FE/FeMappingV9.txt', module_dict=module_dict, forceSilicon=args.forcetype)
mapping = fileReaders.BuildFeMappingLayoutDictFromJSON(n_cmssw_layers, args.mapping, module_dict=module_dict, forceSilicon=args.forcetype)
# mapping = BuildMapping(n_cmssw_layers, 'resources/FeMappingV8.txt', module_dict=module_dict, forceSilicon=args.forcetype)


rules = GenerateRules(cmssw, mapping, module_dict, mainStrategy=args.strategy, cellCount=args.cell_counts)

json_rules = list()

for layer in rules['layers']:
    for key, mod in layer['module_dict'].iteritems():
        newmod = copy.deepcopy(mod)
        # Have to sort type, layer, u, v
        newmod['key'] = (layer['global_index'], key[0], key[1])
        json_rules.append(newmod)

json_rules.sort(key=lambda x: x['key'])

for rule in json_rules:
    line = '%-3i %-3i %-3i %-3i %-3s %-3i %-4i %-3i %-3s %-4i %-20s' % (rule['key'][0], rule['key'][1], rule['key'][2], rule['cmssw_type'], rule['cmssw_shape'], rule['cmssw_orig_shape'], rule['cmssw_n_cells'], rule['type'], rule['shape'], rule['n_cells'], rule['TransformMethod'])
    if rule['TransformMethod'] == 'FixedNumber':
        line += ' FixedNumber:%i' % rule['FixedNumberMax']
    if rule['TransformMethod'] == 'FixedOccupancy':
        line += ' FixedOccupancyFrac:%.3f' % rule['FixedOccupancyFrac']
    if rule['TransformMethod'] == 'FixedDensity':
        line += ' FixedDensitySrc:%g FixedDensityTarget:%g' % (rule['FixedDensitySrc'], rule['FixedDensityTarget'])
    if rule['TransformMethod'] == 'FixedDensity2':
        line += ' FixedDensitySrc:%g FixedDensityTarget:%g ExponentTotal:%g ExponentTOA:%g' % (rule['FixedDensitySrc'], rule['FixedDensityTarget'], rule['FixedDensityExponentTotal'], rule['FixedDensityExponentTOA'])
    print line

# print jsondata
with open('modifier_config_%s.json' % args.strategy, 'w') as out_file:
    out_file.write(json.dumps(json_rules, separators=(',', ': '), indent=2))

if not args.plots:
    sys.exit(0)

assert(n_cmssw_layers in [47, 50])
era = 0 if n_cmssw_layers == 50 else 1

for il in xrange(n_cmssw_layers):
    ilG = cmssw['layers'][il]['global_index']

    if drawRate:
        plot = plotlayer.MakeLayerCanvas('hit_rate_%.2i' % ilG, -6, 12, -1, 17, palette=(min_rate, max_rate))
    elif drawRatio:
        plot = plotlayer.MakeLayerCanvas('hit_rate_ratio_%.2i' % ilG, -6, 12, -1, 17, palette=(min_rate, max_rate))
    else:
        plot = plotlayer.MakeLayerCanvas('geo_consistency_%.2i' % ilG, -6, 12, -1, 17)
    plot['cmssw'] = {}
    plot['mapping'] = {}
    plot['hit_data'] = {}

    # Need three colours for unmasked, partial_masked, fully_masked
    # x 2 for HDM and LDM

    latex = ROOT.TLatex()
    latex.SetTextFont(42)
    latex.SetTextSize(0.01)
    latex.SetTextAlign(22)
    for key, module in cmssw['layers'][il]['module_dict'].iteritems():
        # tk = key[0]
        UV = key

        in_region = (ROOT.uvSector(ilG, tuple(UV), era) == 0) if module['type'] <= 1 else (ROOT.scintSector(UV[1]) == 0)
        # if tk == 1:
        #     continue
        # in_region = False
        # if ilG <= 28 and UV[0] > 0 and UV[1] >= 0:
        #     in_region = True
        # elif ilG > 28 and (ilG % 2) == 1 and UV[0] >= 0 and UV[1] >= 0:
        #     in_region = True
        # elif ilG > 28 and (ilG % 2) == 0 and UV[0] >= 1 and UV[1] >= 1:
        #     in_region = True
        if in_region:
            if module['type'] <= 1:
                plot['cmssw'][UV] = plotlayer.MakeHexUV(ilG, UV)
            else:
                plot['cmssw'][UV] = plotlayer.MakeTrapIphi(UV[1], 10.5, 12)
            if drawRate:
                if UV not in hit_data['layers'][ilG - 1]['module_dict']:
                    rate = 0.
                else:
                    rate = hit_data['layers'][ilG - 1]['module_dict'][key]['average_hits']
                if rate > max_rate:
                    print 'Warning, got rate %f for max %f' % (rate, max_rate)
                    rate = max_rate
                col = int(col0 + (((rate - min_rate) / (max_rate - min_rate)) * ncols))
                plot['cmssw'][UV].SetFillColor(col)
            elif drawRatio:
                if UV not in hit_data_EOL['layers'][ilG - 1]['module_dict']:
                    num = 0
                else:
                    num = hit_data_EOL['layers'][ilG - 1]['module_dict'][key]['average_hits']
                if UV not in hit_data['layers'][ilG - 1]['module_dict']:
                    den = 0
                else:
                    den = hit_data['layers'][ilG - 1]['module_dict'][key]['average_hits']
                if num > 0 and den > 0:
                    rate = num / den
                else:
                    rate = 1.0
                # rate = hit_data_EOL['layers'][ilG - 1]['module_dict'][UV]['average_hits'] / hit_data['layers'][ilG - 1]['module_dict'][UV]['average_hits']
                if rate > max_rate:
                    print 'Warning, got ratio %f for max %f' % (rate, max_rate)
                    rate = max_rate
                col = int(col0 + (((rate - min_rate) / (max_rate - min_rate)) * ncols))
                plot['cmssw'][UV].SetFillColor(col)

            else:
                if module['n_masked_cells'] == 0 and module['n_unmasked_cells'] > 0:
                    if module['type'] == 0:
                        plot['cmssw'][UV].SetFillColor(ROOT.kGreen + 2)
                    else:
                        plot['cmssw'][UV].SetFillColor(ROOT.kGreen - 7)
                if module['n_masked_cells'] > 0:
                    # print module['shape']
                    if module['shape'] == 'F':
                        if module['type'] == 0:
                            plot['cmssw'][UV].SetFillColor(ROOT.kGreen + 2)
                        else:
                            plot['cmssw'][UV].SetFillColor(ROOT.kGreen - 7)
                        print ' >> Found partially masked module with shape F'
                    else:
                        if module['type'] == 0:
                            plot['cmssw'][UV].SetFillColor(ROOT.kYellow + 2)
                        else:
                            plot['cmssw'][UV].SetFillColor(ROOT.kYellow - 7)
                # if module['n_masked_cells'] > 0 and module['n_unmasked_cells'] == 0:
                #     if module['shape'] == -99:
                #         print ' >> Found fully masked module with shape -99'
                #     if module['type'] == 0:
                #         plot['cmssw'][UV].SetFillColor(ROOT.kRed + 2)
                #     else:
                #         plot['cmssw'][UV].SetFillColor(ROOT.kRed - 7)
            plot['cmssw'][UV].Draw('F')
            plot['cmssw'][UV].Draw()
            if not (drawRate or drawRatio):
                if module['type'] <= 1:
                    x, y = plotlayer.UVtoXY(ilG, UV[0], UV[1])
                    latex.DrawLatex(x, y, '%i, %i' % UV)
                    if module['shape'] != -99:
                        latex.DrawLatex(x, plotlayer.HexCentreLineY(x, y, -0.75), module['shape'])
                else:
                    x, y = plotlayer.IphiRToXY(UV[1], 11.25)
                    latex.DrawLatex(x, y, '%i, %i' % UV)
                    x, y = plotlayer.IphiRToXY(UV[1], 10.6)
                    if module['shape'] != -99:
                        latex.DrawLatex(x, y, module['shape'])


    if not (drawRate or drawRatio):
        for key, module in mapping['layers'][il]['module_dict'].iteritems():
            # tk = key[0]
            UV = key
            # if tk == 1:
            #     continue

            # WaferFull = 0, 'F'
            # WaferFive = 1, 'b'
            # WaferChopTwo = 2, 'g'
            # WaferChopTwoM = 3, 'g-'
            # WaferHalf = 4, 'a'
            # WaferSemi = 5, 'd'
            # WaferSemi2 = 6, 'd-'
            # WaferThree = 7, 'c'
            # WaferOut = 99
            if module['type'] <= 1:
                plot['mapping'][UV] = plotlayer.MakeHexUV(ilG, UV, 0.6)
            else:
                plot['mapping'][UV] = plotlayer.MakeTrapIphi(UV[1], 10.9, 11.6)
            if module['shape'] == 'F':
                if module['type'] == 0:
                    plot['mapping'][UV].SetFillColor(ROOT.kGreen + 2)
                else:
                    plot['mapping'][UV].SetFillColor(ROOT.kGreen - 7)
            if module['shape'] != 'F':
                if module['type'] == 0:
                    plot['mapping'][UV].SetFillColor(ROOT.kYellow + 2)
                else:
                    plot['mapping'][UV].SetFillColor(ROOT.kYellow - 7)
            if module['n_lpGBT_DAQ'] == 0:
                plot['mapping'][UV].SetLineColor(ROOT.kCyan)
                plot['mapping'][UV].SetLineWidth(2)

            # if module['n_masked_cells'] > 0 and module['n_unmasked_cells'] == 0:
            #     if module['type'] == 0:
            #         plot['mapping'][UV].SetFillColor(ROOT.kRed + 2)
            #     else:
            #         plot['mapping'][UV].SetFillColor(ROOT.kRed - 7)
            plot['mapping'][UV].Draw('F')
            plot['mapping'][UV].Draw()
            if module['type'] <= 1:
                x, y = plotlayer.UVtoXY(ilG, UV[0], UV[1])
            else:
                x, y = plotlayer.IphiRToXY(UV[1], 11.25)
            latex.DrawLatex(x, y, '%i, %i' % UV)
            if module['shape'] != 'F':
                latex.DrawLatex(x, plotlayer.HexCentreLineY(x, y, -0.35), module['shape'])


        # for key, module in hit_data['layers'][ilG - 1]['module_dict'].iteritems():
        #     tk = key[0]
        #     UV = key[1:]
        #     if tk == 1:
        #         continue
        #     if UV[0] > 0 and UV[1] >= 0 and module['has_masked_hits'] > 0 or module['has_undefined_hits'] > 0:
        #         plot['hit_data'][UV] = plotlayer.MakeHexUV(ilG, UV)
        #         plot['hit_data'][UV].SetLineColor(ROOT.kMagenta)
        #         plot['hit_data'][UV].SetLineWidth(2)
        #         plot['hit_data'][UV].Draw()

    # Legend
    legend = None
    legend_objs = {}
    if not (drawRate or drawRatio):
        legend_h = ROOT.TLegend(0.08, 0.78, 0.35, 0.9, '', 'NBNDC')
        legend = ROOT.TLegend(0.39, 0.7, 0.75, 0.9, '', 'NBNDC')
        legend_objs['HDM (full)'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['HDM (partial)'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['LDM (full)'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['LDM (partial)'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['LDM (fully masked)'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['nlpGBT == 0'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['CMSSW hits in masked cells'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['SmallHex'] = ROOT.TBox(0, 0, 1, 1)
        legend_objs['BigHex'] = ROOT.TBox(0, 0, 1, 1)
        plotting.Set(legend_objs['HDM (full)'], FillColor=(ROOT.kGreen + 2))
        plotting.Set(legend_objs['LDM (full)'], FillColor=(ROOT.kGreen - 7))
        plotting.Set(legend_objs['HDM (partial)'], FillColor=(ROOT.kYellow + 2))
        plotting.Set(legend_objs['LDM (partial)'], FillColor=(ROOT.kYellow - 7))
        plotting.Set(legend_objs['LDM (fully masked)'], FillColor=(ROOT.kRed - 7))
        plotting.Set(legend_objs['nlpGBT == 0'], LineColor=(ROOT.kCyan))
        plotting.Set(legend_objs['CMSSW hits in masked cells'], LineColor=(ROOT.kMagenta))
        plotting.Set(legend_objs['SmallHex'], LineColor=0)
        plotting.Set(legend_objs['BigHex'], LineColor=0)

        legend_h.AddEntry(legend_objs['BigHex'], 'CMSSW geometry', 'F')
        legend_h.AddEntry(legend_objs['SmallHex'], 'Mapping file', 'F')

        legend.AddEntry(legend_objs['HDM (full)'], 'HDM (full)', 'F')
        legend.AddEntry(legend_objs['LDM (full)'], 'LDM (full)', 'F')
        legend.AddEntry(legend_objs['HDM (partial)'], 'HDM (partial)', 'F')
        legend.AddEntry(legend_objs['LDM (partial)'], 'LDM (partial)', 'F')
        # legend.AddEntry(legend_objs['LDM (fully masked)'], 'LDM (fully masked)', 'F')
        legend.AddEntry(legend_objs['nlpGBT == 0'], 'nlpGBT == 0', 'L')
        # legend.AddEntry(legend_objs['CMSSW hits in masked cells'], 'Hits in undef/masked cells', 'L')
        legend.Draw()
        legend_h.Draw()
        legend_objs['X1'] = plotlayer.MakeHex((-5.1, 15.7), s=1)
        legend_objs['X2'] = plotlayer.MakeHex((-5.1, 14.5), s=0.6)
        plotting.Set(legend_objs['X1'], LineColor=1, FillColor=0)
        plotting.Set(legend_objs['X2'], LineColor=1, FillColor=0)
        legend_objs['X1'].Draw()
        legend_objs['X2'].Draw()

    plotting.DrawTitle(plot['pad'], 'Layer %i' % ilG, 1)
    plot['canvas'].Print('.pdf')
    plot['canvas'].Print('.png')

    cmssw_module_set = set()
    mapping_module_set = set()
    for X in cmssw['layers'][il]['module_dict'].keys():
        in_region = (ROOT.uvSector(ilG, tuple(X), era) == 0) if module['type'] <= 1 else (ROOT.scintSector(X[1]) == 0)
        if in_region and cmssw['layers'][il]['module_dict'][X]['n_total_cells'] > 0:
            cmssw_module_set.add(X)
    for X in mapping['layers'][il]['module_dict']:
            mapping_module_set.add(X)
    print '>> %-50s %-10i %-10i' % ('Modules in layer %i' %  cmssw['layers'][il]['global_index'], len(mapping_module_set), len(cmssw_module_set))

    print '    >> Only in mapping file: %s' % mapping_module_set.difference(cmssw_module_set)
    print '    >> Only in CMSSW: %s' % cmssw_module_set.difference(mapping_module_set)
    common_modules = mapping_module_set.intersection(cmssw_module_set)

    for mod in common_modules:
        modtype_cmssw = cmssw['layers'][il]['module_dict'][mod]['type']
        modtype_mapping = mapping['layers'][il]['module_dict'][mod]['type']
        if modtype_mapping != modtype_cmssw:
            print '   >> Module %s is type %i in the mapping file but type %i in CMSSW!' % (mod, modtype_mapping, modtype_cmssw)

    # for ieta, iphi in sorted(ref_tile_set.difference(cmssw_tile_set)):
    #     if ieta not in diff_dict:
    #         diff_dict[ieta] = list()
    #     diff_dict[ieta].append(iphi)
    # for ieta, phis in diff_dict.iteritems():
    #     print '  >> In reference only: ieta=%i, %i tiles in phi' % (ieta, len(phis))

