import ROOT
import plotting as plot
import argparse
import json
import tools
import numpy as np

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

plot.ModTDRStyle(width=1000)


SPLIT_MARKERS = [24, 25, 26, 27, 28, 2, 5, 32, 30]
SPLIT_COLORS  = [
    [95, 173, 86],
    [242, 193, 78],
    [247, 129, 84],
    [81, 163, 163],
    [117, 72, 94],
    [19, 41, 61],
    [237, 37, 78],
    28,
    24
]

def GetMeanStdDev(hist):
    vals = []
    for b in range(1, hist.GetNbinsX() + 1):
        if hist.GetBinContent(b) > 0:
            vals.append(hist.GetBinContent(b))
    arr = np.array(vals)
    return np.mean(arr), np.std(arr)


def GetHist(fileobj):
    filename, histname = fileobj.split(':')
    fin = ROOT.TFile(filename)
    hist2d = fin.Get(histname)
    fin.Close()
    return hist2d


def makePlot(label, hists=list(), legend=dict(), logy=False, addStats=False, ymin=None, ymax=None):
    canv = ROOT.TCanvas(label)
    pads = plot.OnePad()
    pads[0].cd()
    leg = ROOT.TLegend(*(legend['pos'] + ['', 'NBNDC']))
    for hist in hists:
        h = GetHist(hist['path'])
        hist['h'] = h
        plot.Set(h, MarkerSize=0.9, MarkerColor=hist['MarkerColor'])
    axis = hists[0]['h'].Clone()
    if logy:
        pads[0].SetLogy(True)
        axis.SetMinimum(10)
    if ymin is not None:
        axis.SetMinimum(ymin)
    if ymax is not None:
        axis.SetMaximum(ymax)
    axis.Reset()
    axis.Draw()
    for hist in hists:
        hist['h'].Print('range')
        hist['h'].Draw('SAMEHISTP')
        if addStats:
            leg.AddEntry(hist['h'], hist['legend'] + ' %.1f / %.1f' % GetMeanStdDev(hist['h']), 'P')
        else:
            leg.AddEntry(hist['h'], hist['legend'], 'P')
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.19)
    leg.Draw()

    canv.Print('.pdf')
    canv.Print('.png')


# makePlot(label='testplot_AvgSlinkWordsFs',
#     hists = [
#         {'path': 'HGCalBufferModel_FEV8_MCFD2V8_BE192_SL25.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 2, 'legend': 'Taylan/Bora (V8)'},
#         {'path': 'HGCalBufferModel_FEV8_MCFD2V8_BETS16_SL25.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 4, 'legend': 'Simple (V8)'},
#         {'path': 'HGCalBufferModel_FEV9_MCFD2V9_BE192V9p1_SL25.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 6, 'legend': 'Simple (V9)'},
#         {'path': 'HGCalBufferModel_FEV9_MCFD2V9_BETS15_SL25.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 8, 'legend': 'Simple (V9) 15 FPGA'},
#         {'path': 'HGCalBufferModel_FEV9_MCFD2V9_BETS14_SL25.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 28, 'legend': 'Simple (V9) 14 FPGA'},
        
#     ],
#     legend = {
#         'pos': [0.6, 0.7, 0.9, 0.9]
#     },
#     addStats=True
# )

# makePlot(label='testplot_BufferFs',
#     hists = [
#         {'path': 'HGCalBufferModel_FEV8_MCFD2V8_BE192_SL25.root:SlinkBuilder_BufferFs', 'MarkerColor': 2, 'legend': 'Taylan/Bora (V8)'},
#         {'path': 'HGCalBufferModel_FEV8_MCFD2V8_BETS16_SL25.root:SlinkBuilder_BufferFs', 'MarkerColor': 4, 'legend': 'Simple (V8)'},
#         {'path': 'HGCalBufferModel_FEV9_MCFD2V9_BE192V9p1_SL25.root:SlinkBuilder_BufferFs', 'MarkerColor': 6, 'legend': 'Simple (V9)'},
#         {'path': 'HGCalBufferModel_FEV9_MCFD2V9_BETS15_SL25.root:SlinkBuilder_BufferFs', 'MarkerColor': 8, 'legend': 'Simple (V9) 15 FPGA'},
#         {'path': 'HGCalBufferModel_FEV9_MCFD2V9_BETS14_SL25.root:SlinkBuilder_BufferFs', 'MarkerColor': 28, 'legend': 'Simple (V9) 14 FPGA'},

#     ],
#     legend = {
#         'pos': [0.6, 0.7, 0.9, 0.9]
#     },
#     logy=True
# )



makePlot(label='testplot_AvgSlinkWordsFs',
    hists = [
        {'path': 'HGCalBufferModel_FEV8_BE_random.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 2, 'legend': 'Random'},
        {'path': 'HGCalBufferModel_FEV8_BE_simple.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 4, 'legend': 'Simple'},
        {'path': 'HGCalBufferModel_FEV8_BE_opt.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 6, 'legend': 'Optimized'},
        {'path': '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_default/HGCalBufferModel_750_job_1.root:SlinkBuilder_AvgSlinkWordsFs', 'MarkerColor': 8, 'legend': 'Simple (V9)'}
        
    ],
    legend = {
        'pos': [0.6, 0.7, 0.9, 0.9]
    },
    addStats=True
)

makePlot(label='testplot_BufferFs',
    hists = [
        {'path': 'HGCalBufferModel_FEV8_BE_random.root:SlinkBuilder_BufferFs', 'MarkerColor': 2, 'legend': 'Random'},
        {'path': 'HGCalBufferModel_FEV8_BE_simple.root:SlinkBuilder_BufferFs', 'MarkerColor': 4, 'legend': 'Simple'},
        {'path': 'HGCalBufferModel_FEV8_BE_opt.root:SlinkBuilder_BufferFs', 'MarkerColor': 6, 'legend': 'Optimized'},
        {'path': '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_default/HGCalBufferModel_750_job_1.root:SlinkBuilder_BufferFs', 'MarkerColor': 8, 'legend': 'Simple (V9)'}

        
    ],
    legend = {
        'pos': [0.6, 0.7, 0.9, 0.9]
    },
    logy=True
)

makePlot(label='testplot_LpgbtPairBuilder_AvgRamWordsFslp',
    hists = [
        {'path': 'HGCalBufferModel_FEV8_BE_random.root:LpgbtPairBuilder_AvgRamWordsFslp', 'MarkerColor': 2, 'legend': 'Random'},
        {'path': 'HGCalBufferModel_FEV8_BE_simple.root:LpgbtPairBuilder_AvgRamWordsFslp', 'MarkerColor': 4, 'legend': 'Simple'},
        {'path': 'HGCalBufferModel_FEV8_BE_opt.root:LpgbtPairBuilder_AvgRamWordsFslp', 'MarkerColor': 6, 'legend': 'Optimized'},
        {'path': '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_default/HGCalBufferModel_750_job_1.root:LpgbtPairBuilder_AvgRamWordsFslp', 'MarkerColor': 8, 'legend': 'Simple (V9)'}

        
    ],
    legend = {
        'pos': [0.6, 0.7, 0.9, 0.9]
    },
    addStats=True
)

makePlot(label='testplot_LpgbtPairBuilder_BufferFslp',
    hists = [
        {'path': 'HGCalBufferModel_FEV8_BE_random.root:LpgbtPairBuilder_BufferFslp', 'MarkerColor': 2, 'legend': 'Random'},
        {'path': 'HGCalBufferModel_FEV8_BE_simple.root:LpgbtPairBuilder_BufferFslp', 'MarkerColor': 4, 'legend': 'Simple'},
        {'path': 'HGCalBufferModel_FEV8_BE_opt.root:LpgbtPairBuilder_BufferFslp', 'MarkerColor': 6, 'legend': 'Optimized'},
        {'path': '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_default/HGCalBufferModel_750_job_1.root:LpgbtPairBuilder_BufferFslp', 'MarkerColor': 8, 'legend': 'Simple (V9)'}

        
    ],
    legend = {
        'pos': [0.6, 0.7, 0.9, 0.9]
    },
    logy=True
)



makePlot(label='testplot_LpgbtPairEcondBuilder_BufferLuv',
    hists = [
        {'path': 'HGCalBufferModel_FEV8_BE_random.root:LpgbtPairEcondBuilder_BufferLuv', 'MarkerColor': 2, 'legend': 'Random'},
        {'path': 'HGCalBufferModel_FEV8_BE_simple.root:LpgbtPairEcondBuilder_BufferLuv', 'MarkerColor': 4, 'legend': 'Simple'},
        {'path': 'HGCalBufferModel_FEV8_BE_opt.root:LpgbtPairEcondBuilder_BufferLuv', 'MarkerColor': 6, 'legend': 'Optimized'},
        {'path': '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_default/HGCalBufferModel_750_job_1.root:LpgbtPairEcondBuilder_BufferLuv', 'MarkerColor': 8, 'legend': 'Simple (V9)'}
        
    ],
    legend = {
        'pos': [0.6, 0.7, 0.9, 0.9]
    },
    logy=True
)