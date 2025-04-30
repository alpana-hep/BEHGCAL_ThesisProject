import ROOT
import plotting as plot
import argparse
import json
import tools

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


def GetSplitHists(hist, modifier, legend):
    res = []
    for ip in xrange(9):
        res.append(ROOT.TH1D(hist.GetName() + str(ip), '', hist.GetNbinsX(), hist.GetXaxis().GetBinLowEdge(1), hist.GetXaxis().GetBinUpEdge(hist.GetNbinsX())))
        res[-1].Reset()
    for ib in xrange(1, hist.GetNbinsX() + 1):
        av_words = hist.GetBinContent(ib)
        if av_words == 0:
            continue
        l, u, v = tools.DecodeModule(ib - 1)
        rule = modifier[(l, u, v)]
        if 'cmssw_type' not in rule:
            continue
        if (rule['cmssw_type'] == rule['type'] == 0) and (rule['cmssw_shape'] == rule['shape'] == 'F'):
            res[0].SetBinContent(ib, hist.GetBinContent(ib))
        elif rule['cmssw_type'] == rule['type'] == 1 and (rule['cmssw_shape'] == rule['shape'] == 'F'):
            res[1].SetBinContent(ib, hist.GetBinContent(ib))
        elif rule['cmssw_type'] >= 2 and rule['type'] >=2:
            res[2].SetBinContent(ib, hist.GetBinContent(ib))
        elif (rule['cmssw_type'] == rule['type'] == 0) and (rule['shape'] != 'F'):
            res[3].SetBinContent(ib, hist.GetBinContent(ib))
        elif (rule['cmssw_type'] == rule['type'] == 1) and (rule['shape'] != 'F'):
            res[4].SetBinContent(ib, hist.GetBinContent(ib))
        elif (rule['cmssw_type'] == 0 and rule['type'] == 1):
            res[5].SetBinContent(ib, hist.GetBinContent(ib))
        elif (rule['cmssw_type'] == 1 and rule['type'] == 0):
            res[6].SetBinContent(ib, hist.GetBinContent(ib))
        elif rule['cmssw_type'] == rule['type'] == 0 and (rule['cmssw_shape'] != 'F' and rule['shape'] == 'F'):
            res[7].SetBinContent(ib, hist.GetBinContent(ib))
        elif rule['cmssw_type'] == rule['type'] == 1 and (rule['cmssw_shape'] != 'F' and rule['shape'] == 'F'):
            res[8].SetBinContent(ib, hist.GetBinContent(ib))
        else:
            print '>> Stuck at %s: %s' % (tools.DecodeModule(ib - 1), rule)
    for i in range(9):
        col = SPLIT_COLORS[i]
        if isinstance(col, list):
            col = ROOT.TColor.GetColor(*col)
        plot.Set(res[i], MarkerStyle=SPLIT_MARKERS[i], MarkerColor=col)
    legend.AddEntry(res[0], 'HD-F', 'P')
    legend.AddEntry(res[1], 'LD-F', 'P')
    legend.AddEntry(res[2], 'Scint', 'P')
    legend.AddEntry(res[3], 'HD-P', 'P')
    legend.AddEntry(res[4], 'LD-P', 'P')
    legend.AddEntry(res[5], 'HD-F#rightarrowLD-F', 'P')
    legend.AddEntry(res[6], 'LD-F#rightarrowHD-F', 'P')
    legend.AddEntry(res[7], 'HD-P#rightarrowHD-F', 'P')
    legend.AddEntry(res[8], 'LD-P#rightarrowLD-F', 'P')
    return res


def GetHist(fileobj):
    filename, histname = fileobj.split(':')
    fin = ROOT.TFile(filename)
    hist2d = fin.Get(histname)
    fin.Close()
    return hist2d



parser = argparse.ArgumentParser()

parser.add_argument('--input', default='hitData_D49_ttbar_PU200_3iab_mipDef.root:trans_hits_vs_luv')
parser.add_argument('--divide-by', default=None)
parser.add_argument('--overlay', default=None)
parser.add_argument('--modifier', default=None)
parser.add_argument('--output', default='drawhits')
parser.add_argument('--y-range', default='0,500')
parser.add_argument('--y-title', default='Average hits')
parser.add_argument('--x-title', default='Module LUV')
parser.add_argument('--marker-size', default=0.2, type=float)

args = parser.parse_args()

if args.modifier is not None:
    modifier = {}
    with open(args.modifier) as jsonfile:
        modifier_tmp = json.load(jsonfile)
        for X in modifier_tmp:
            modifier[tuple(X['key'])] = dict(X)


hist2d = GetHist(args.input)
hist = hist2d.ProfileX()

if args.divide_by is not None:
    ratio2d = GetHist(args.divide_by)
    ratiohist = ratio2d.ProfileX()
    hist.Divide(ratiohist)
if args.overlay is not None:
    alternate = GetHist(args.overlay)
    althist = alternate.ProfileX()
    plot.Set(althist, MarkerSize=args.marker_size, MarkerColor=2, LineColor=2)
    # hist.Divide(ratiohist)

plot.Set(hist, MarkerSize=args.marker_size)

legend2 = ROOT.TLegend(0.12, 0.84, 0.96, 0.92, '', 'NBNDC')
legend2.SetFillColor(0)
legend2.SetNColumns(9)

canv = ROOT.TCanvas(args.output)
pads = plot.OnePad()

axis = hist.Clone()
axis.Reset()
axis.SetMinimum(float(args.y_range.split(',')[0]))
axis.SetMaximum(float(args.y_range.split(',')[1]))


axis.Draw()
plot.Set(axis.GetXaxis(), TitleSize=axis.GetXaxis().GetTitleSize() * 0.75, Title=args.x_title)
plot.Set(axis.GetYaxis(), TitleSize=axis.GetYaxis().GetTitleSize() * 0.75, Title=args.y_title)

hists = []
if args.modifier is not None:
    hists = GetSplitHists(hist, modifier, legend2)
    # hists[0].Print('range')
    for h in hists:
        plot.Set(h, MarkerSize=0.2)
        h.Draw('SAMEHISTP')
    legend2.Draw()
else:
    hist.Draw('SAMEHISTP')
if args.overlay is not None:
    althist.Draw('SAMEHISTP')

canv.Print('.pdf')
canv.Print('.png')
