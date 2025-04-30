import ROOT
import plotting as plot
from array import array
import argparse
import tools
import json

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

# These lines make HGCHitProvider available via PyROOT
ROOT.gROOT.ProcessLine('#include "interface/HGCHitProvider.h"')
ROOT.gROOT.ProcessLine('#include "interface/EcondAsicDefinition.h"')
ROOT.gROOT.ProcessLine('#include "interface/EcondAsicHitCounts.h"')
ROOT.gSystem.Load('lib/libHGCalBufferModel.so')
plot.ModTDRStyle()

parser = argparse.ArgumentParser()
parser.add_argument('--input', '-i', default='HGCalBEStudies.root', help='Input ROOT filename')
parser.add_argument('--output', '-o', default='hitData', help='Output ROOT filename')
parser.add_argument('--modifier', '-m', default='data/CMSSW/modifier_config_notransform.json', help='Modifier config json to convert hit numbers')
parser.add_argument('--mapping', '-a', default='data/FE/FeMappingV8.txt', help='Mapping file to define the modules')
parser.add_argument('--modules', default='data/FE/DaqModules.txt', help='File defining module properties')
parser.add_argument('--events', '-e', type=int, default=-1, help='Number of events to process (-1 for all events)')
parser.add_argument('--sector', '-s', type=int, default=0, help='Restrict sector (-1,-2,-3,1,2,3, 0=all)')
parser.add_argument('--no-read-unmasked', action='store_true', help='HGCHitProvider will transform the defined hits (instead of unmasked)')
parser.add_argument('--no-veto-scint', action='store_true', help='Include scintillator in the occ_summary file')
args = parser.parse_args()

modinfo = tools.ReadModuleFile(args.modules)
print modinfo

modifier = {}
with open(args.modifier) as jsonfile:
    modjson = json.load(jsonfile)
    for item in modjson:
        modifier[tuple(item['key'])] = dict(item)

hitprovider = ROOT.HGCHitProvider()
hitprovider.initialise(args.mapping, args.modifier)
hitprovider.setFile(args.input, 'beLinksNTuple/EventTree')
if args.no_read_unmasked:
    hitprovider.setReadUnmasked(False)

n_tot_evts = hitprovider.getEntries()
evts_to_proc = n_tot_evts if args.events == -1 else args.events

print '>> Using file %s and processing %i/%i pseudo events' % (args.input, evts_to_proc, n_tot_evts)

hists_words_trans = []

h_total_data = ROOT.TH1F('h_total_data', '', 200, 0, 40)
h_total_hits = ROOT.TH1F('h_total_hits', '', 200, 0, 110)


h2d_orig = {}
h2d_trans = {}


def makeLUVHist(name):
    return ROOT.TH2F(name, '', 50 * 256, 0, 50 * 256, 500, -0.5, 499.5)

h2d_orig_hits_vs_luv = makeLUVHist('orig_hits_vs_luv')
h2d_trans_hits_vs_luv = makeLUVHist('trans_hits_vs_luv')
h2d_orig_hits10_vs_luv = makeLUVHist('orig_hits10_vs_luv')
h2d_trans_hits10_vs_luv = makeLUVHist('trans_hits10_vs_luv')
h2d_orig_hits20_vs_luv = makeLUVHist('orig_hits20_vs_luv')
h2d_trans_hits20_vs_luv = makeLUVHist('trans_hits20_vs_luv')
h2d_orig_hits30_vs_luv = makeLUVHist('orig_hits30_vs_luv')
h2d_trans_hits30_vs_luv = makeLUVHist('trans_hits30_vs_luv')
h2d_orig_hitsbxm1_vs_luv = makeLUVHist('orig_hitsbxm1_vs_luv')
h2d_trans_hitsbxm1_vs_luv = makeLUVHist('trans_hitsbxm1_vs_luv')


for im in xrange(hitprovider.modules().size()):
    module = hitprovider.modules()[im]
    # Produces TH1s with name [layer]_[type]_[U]_[V}]
    name = '%i_%i_%i_%i' % (module.layer, module.type, module.u, module.v)
    hists_words_trans.append(ROOT.TH1F('trans_words_%s' % name, '', 1000, 0, 1000))


mods = hitprovider.modules()
nMods = len(mods)
modHalfRocs = [0] * nMods
modMaxCells = [0] *nMods
modEcondDefs = [0] * nMods
modLUV = [0] * nMods
for im in xrange(nMods):
    key = (mods[im].type, mods[im].s)
    modLUV[im] = tools.EncodeModule(mods[im].layer, mods[im].u, mods[im].v)
    if key not in modinfo:
        # print key
        modHalfRocs[im] = 0
    else:
        modMaxCells[im] = modinfo[key]['n_total_cells']
        modHalfRocs[im] = modinfo[key]['n_calib_cells']

    econdDef = ROOT.EcondAsicDefinition()
    econdDef.setDefinitionTo(mods[im].layer, mods[im].u, mods[im].v, mods[im].type, mods[im].s, 0)
    modEcondDefs[im] = econdDef
    if modHalfRocs[im] != econdDef.numberOfHalfHgcrocs():
        print modHalfRocs[im], econdDef.numberOfHalfHgcrocs()
# print modHalfRocs

maxWords = 0
nWordsTot = 0
evtsProc = 0
for ie in xrange(evts_to_proc):
    if ie % 100 == 0:
        print '>> Processed %i pseudo events' % ie
    hitprovider.processEvent(ie)

    if args.sector != 0 and hitprovider.eventTree().pseudo_event != args.sector:
            continue

    origHits10 = array('i', hitprovider.hits10(True))
    origHits20 = array('i', hitprovider.hits20(True))
    origHits30 = array('i', hitprovider.hits30(True))
    origHitsBXM1 = array('i', hitprovider.hitsBXM1(True))
    transHits10 = array('i', hitprovider.hits10(False))
    transHits20 = array('i', hitprovider.hits20(False))
    transHits30 = array('i', hitprovider.hits30(False))
    transHitsBXM1 = array('i', hitprovider.hitsBXM1(False))

    data_per_layer = array('d', [0.] * 50)
    total_data = 0
    hd_data_per_layer = array('d', [0.] * 50)
    ld_data_per_layer = array('d', [0.] * 50)
    total_hits = 0

    for im in xrange(nMods):
        if mods[im].s == 'c' and mods[im].type <= 1:
            continue
        nOrigTot = origHits10[im] + origHits20[im] + origHits30[im]
        nTransTot = transHits10[im] + transHits20[im] + transHits30[im]


        # Check if we're below the max number of hits
        if nOrigTot > modMaxCells[im] and mods[im].s != 'c':
            print '%i %i %i %i %s' % (mods[im].layer, mods[im].type, mods[im].u, mods[im].v, mods[im].s), nOrigTot, modMaxCells[im]
        if min(transHits10[im], transHits20[im], transHits30[im]) < 0:
            print '%i %i %i %i %s' % (mods[im].layer, mods[im].type, mods[im].u, mods[im].v, mods[im].s), transHits10[im], transHits20[im], transHits30[im]

        # nWords = daqRawDataPacketWordsJun20(modHalfRocs[im], transHits10[im] + transHits20[im], transHits30[im])
        nWordsRef = ROOT.EcondAsicHitCounts.daqRawDataPacketWordsDec20(modEcondDefs[im], int(transHits10[im]), int(transHits20[im]), int(transHits30[im]))
        nWordsRef = (int(nWordsRef + 1) / 2) * 2

        h2d_orig_hits_vs_luv.Fill(modLUV[im] + 0.5, nOrigTot)
        h2d_trans_hits_vs_luv.Fill(modLUV[im] + 0.5, nTransTot)
        h2d_orig_hits10_vs_luv.Fill(modLUV[im] + 0.5, origHits10[im])
        h2d_trans_hits10_vs_luv.Fill(modLUV[im] + 0.5, transHits10[im])
        h2d_orig_hits20_vs_luv.Fill(modLUV[im] + 0.5, origHits20[im])
        h2d_trans_hits20_vs_luv.Fill(modLUV[im] + 0.5, transHits20[im])
        h2d_orig_hits30_vs_luv.Fill(modLUV[im] + 0.5, origHits30[im])
        h2d_trans_hits30_vs_luv.Fill(modLUV[im] + 0.5, transHits30[im])
        h2d_orig_hitsbxm1_vs_luv.Fill(modLUV[im] + 0.5, origHitsBXM1[im])
        h2d_trans_hitsbxm1_vs_luv.Fill(modLUV[im] + 0.5, transHitsBXM1[im])


        hists_words_trans[im].Fill(nWordsRef)

        nWordsTot += nWordsRef
        data_per_layer[mods[im].layer - 1] += ((nWordsRef * 16. * 6.) / (1000. * 1000.))
        if mods[im].type == 0:
            hd_data_per_layer[mods[im].layer - 1] += ((nWordsRef * 16. * 6.) / (1000. * 1000.))
        elif mods[im].type == 1:
            ld_data_per_layer[mods[im].layer - 1] += ((nWordsRef * 16. * 6.) / (1000. * 1000.))
        total_data += ((nWordsRef * 16. * 6.) / (1000. * 1000.))
        total_hits += (transHits10[im] + transHits20[im] + transHits30[im])

    # print total_hits / 1000.
    h_total_hits.Fill(total_hits / 1000.)
    h_total_data.Fill(total_data)

    evtsProc += 1

avEvtWords = (float(nWordsTot) / float(evtsProc))
print '>> Mean 16-bit words per sector: %f' % (nWordsTot / evtsProc)
print '>> Data size / full event [Mb]: %f' % ((avEvtWords * 16. * 6.) / (1000. * 1000.))


prof_orig_hits_vs_luv = h2d_orig_hits_vs_luv.ProfileX()
prof_trans_hits_vs_luv = h2d_trans_hits_vs_luv.ProfileX()
prof_orig_hits10_vs_luv = h2d_orig_hits10_vs_luv.ProfileX()
prof_trans_hits10_vs_luv = h2d_trans_hits10_vs_luv.ProfileX()
prof_orig_hits20_vs_luv = h2d_orig_hits20_vs_luv.ProfileX()
prof_trans_hits20_vs_luv = h2d_trans_hits20_vs_luv.ProfileX()
prof_orig_hits30_vs_luv = h2d_orig_hits30_vs_luv.ProfileX()
prof_trans_hits30_vs_luv = h2d_trans_hits30_vs_luv.ProfileX()
prof_orig_hitsbxm1_vs_luv = h2d_orig_hitsbxm1_vs_luv.ProfileX()
prof_trans_hitsbxm1_vs_luv = h2d_trans_hitsbxm1_vs_luv.ProfileX()



with open(args.output + '_occ_summary.txt', 'w') as out_file:
    out_file.write('# l  u  v  type  shape  cells  cmssw_type  cmssw_shape  cmssw_cells  <nHitsTotal>  corrFactorTotal  <nHistsToA>  corrFactorToA  <nHitsBXM1>  corrFactorBXM1  <n32bWords>\n')
    for im in xrange(hitprovider.modules().size()):
        luvbin = int(modLUV[im]) + 1
        module = hitprovider.modules()[im]
        if module.type >= 2 and not args.no_veto_scint:
            continue
        av_rate_orig = prof_orig_hits_vs_luv.GetBinContent(luvbin)
        av_rate_trans = prof_trans_hits_vs_luv.GetBinContent(luvbin)
        av_rate_ratio = av_rate_trans / av_rate_orig if av_rate_orig > 0 else 0.
        av_rate_10_orig = prof_orig_hits10_vs_luv.GetBinContent(luvbin)
        av_rate_10_trans = prof_trans_hits10_vs_luv.GetBinContent(luvbin)
        av_rate_10_ratio = av_rate_10_trans / av_rate_10_orig if av_rate_10_orig > 0 else 0.
        av_rate_20_orig = prof_orig_hits20_vs_luv.GetBinContent(luvbin)
        av_rate_20_trans = prof_trans_hits20_vs_luv.GetBinContent(luvbin)
        av_rate_20_ratio = av_rate_20_trans / av_rate_20_orig if av_rate_20_orig > 0 else 0.
        av_rate_30_orig = prof_orig_hits30_vs_luv.GetBinContent(luvbin)
        av_rate_30_trans = prof_trans_hits30_vs_luv.GetBinContent(luvbin)
        av_rate_30_ratio = av_rate_30_trans / av_rate_30_orig if av_rate_30_orig > 0 else 0.
        av_rate_BXM1_orig = prof_orig_hitsbxm1_vs_luv.GetBinContent(luvbin)
        av_rate_BXM1_trans = prof_trans_hitsbxm1_vs_luv.GetBinContent(luvbin)
        av_rate_BXM1_ratio = av_rate_BXM1_trans / av_rate_BXM1_orig if av_rate_BXM1_orig > 0 else 0.
        av_words = hists_words_trans[im].GetMean()
        modifier_info = modifier[(module.layer, module.u, module.v)]
        target_shape = modifier_info['shape']
        target_type = modifier_info['type']
        if 'n_cells' in modifier_info:
            target_cells = float(modifier_info['n_cells'])
            cmssw_cells = float(modifier_info['cmssw_n_cells'])
            cmssw_shape = modifier_info['cmssw_shape']
            cmssw_type = modifier_info['cmssw_type']
        else:
            target_cells = 0
            cmssw_cells = 1
            cmssw_shape = '?'
            cmssw_type = 0

        out_file.write('%3i %3i %3i %3i %3s %3i %3i %3s %3i %10f %10f %10f %10f %10f %10f %10f\n' % (
            module.layer, module.u, module.v,
            target_type, target_shape, target_cells,
            cmssw_type, cmssw_shape, cmssw_cells,
            av_rate_orig, av_rate_ratio,
            av_rate_30_orig, av_rate_30_ratio,
            av_rate_BXM1_orig, av_rate_BXM1_ratio, av_words / 2.))



fout = ROOT.TFile(args.output + '.root', 'RECREATE')
fout.cd()
h2d_orig_hits_vs_luv.Write()
h2d_trans_hits_vs_luv.Write()
h2d_orig_hits10_vs_luv.Write()
h2d_trans_hits10_vs_luv.Write()
h2d_orig_hits20_vs_luv.Write()
h2d_trans_hits20_vs_luv.Write()
h2d_orig_hits30_vs_luv.Write()
h2d_trans_hits30_vs_luv.Write()
h2d_orig_hitsbxm1_vs_luv.Write()
h2d_trans_hitsbxm1_vs_luv.Write()

fout.Close()
