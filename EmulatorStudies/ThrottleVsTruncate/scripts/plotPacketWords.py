import ROOT
import plotting as plot
import argparse
import json
import math
import tools

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

plot.ModTDRStyle(width=1000)


AUTO_COLOURS = [
    [95, 173, 86],
    [242, 193, 78],
    [247, 129, 84],
    [81, 163, 163],
    [117, 72, 94],
    # [19, 41, 61],
    [237, 37, 78],
    28,
    24
]



def CheckHighRates(hist, elinks, threshold, modifier=None, hit_data=None):
    hist_cleaned = hist.Clone()
    hist_cleaned.Reset()
    n_elinks = 0
    n_additional_elinks = 0
    lines = []
    for ib in xrange(1, hist.GetNbinsX() + 1):
        # Calculate the number of elinks ourselves
        av_words = hist.GetBinContent(ib)
        if av_words == 0:
            continue
        n_elinks += elinks
        elinks_calc = av_words / threshold
        elinks_calc_ceil = math.ceil(elinks_calc)
        if elinks_calc_ceil > elinks:
            hist_cleaned.SetBinContent(ib, hist.GetBinContent(ib))
        if elinks_calc_ceil >= elinks:
            n_additional_elinks += int(elinks_calc_ceil) - int(elinks)
            l, u, v = tools.DecodeModule(ib - 1)
            line = '%-8i %-2i %-2i %-2i %-7.1f %-3i %-7.2f %-3i' % (ib - 1, l, u, v, av_words, elinks, elinks_calc, elinks_calc_ceil)
            if hit_data != None:
                line +=  ' %-7.2f %-7.2f' % (hit_data[l, u, v][0], hit_data[l, u, v][1])
            if modifier != None:
                rule = modifier[(l, u, v)]
                if 'cmssw_type' not in rule:
                    continue
                eline = '%-3i %-3i %-3s %-3s %-20s' % (rule['cmssw_type'], rule['type'], rule['cmssw_shape'], rule['shape'], rule['TransformMethod'])
                if rule['TransformMethod'] == 'FixedNumber':
                    eline += ' FixedNumber:%i' % rule['FixedNumberMax']
                if rule['TransformMethod'] == 'FixedOccupancy':
                    eline += ' FixedOccupancyFrac:%.3f' % rule['FixedOccupancyFrac']
                line += ' | '
                line += eline
            lines.append(line)
            # print line
    return lines, n_elinks, n_additional_elinks, hist_cleaned


SPLIT_MARKERS = [24, 25, 26, 27, 28, 2, 5, 32, 30]

def GetSplitHists(hist, modifier):
    res = []
    for ip in xrange(9):
        res.append(hist.Clone())
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
    plot.Set(res[0], MarkerStyle=SPLIT_MARKERS[0])
    plot.Set(res[1], MarkerStyle=SPLIT_MARKERS[1])
    plot.Set(res[2], MarkerStyle=SPLIT_MARKERS[2])
    plot.Set(res[3], MarkerStyle=SPLIT_MARKERS[3])
    plot.Set(res[4], MarkerStyle=SPLIT_MARKERS[4])
    plot.Set(res[5], MarkerStyle=SPLIT_MARKERS[5])
    plot.Set(res[6], MarkerStyle=SPLIT_MARKERS[6])
    plot.Set(res[7], MarkerStyle=SPLIT_MARKERS[7])
    plot.Set(res[0], MarkerStyle=SPLIT_MARKERS[8])
    return res


def GetHitRatioHist(hist, hit_data):
    hnew = hist.Clone()
    hnew.Reset()
    for ib in xrange(1, hist.GetNbinsX() + 1):
        if hist.GetBinContent(ib) == 0:
            continue
        l, u, v = tools.DecodeModule(ib - 1)
        h_ratio = hit_data[l, u, v][0] / hit_data[l, u, v][1] if hit_data[l, u, v][1] > 0. else 0.
        hnew.SetBinContent(ib, h_ratio)
    return hnew


parser = argparse.ArgumentParser()

parser.add_argument('--hist', default='AveragePacketWords')
parser.add_argument('--post', default='')
parser.add_argument('--file', default='SectorEcondAsicTest_23k.root')
parser.add_argument('--check', action='store_true')
parser.add_argument('--max', default=41, type=int)
parser.add_argument('--modifier', default='data/CMSSW/modifier_config.json')
parser.add_argument('--hit-data', default=None)
parser.add_argument('--draw-mode', type=int, default=0)

args = parser.parse_args()

modifier = {}
with open(args.modifier) as jsonfile:
    modifier_tmp = json.load(jsonfile)
    for X in modifier_tmp:
        modifier[tuple(X['key'])] = dict(X)

hit_data = {}
if args.hit_data is not None:
    with open(args.hit_data) as module_file:
        for line in module_file:
            X = line.split()
            layer = int(X[0])
            u = int(X[1])
            v = int(X[2])
            av_hits = float(X[3])
            prev_hits = float(X[4])
            hit_data[layer, u, v] = (av_hits, prev_hits)

fin = ROOT.TFile(args.file)

legend = ROOT.TLegend(0.12, 0.85, 0.96, 0.93, '', 'NBNDC')
legend.SetFillColor(0)
legend.SetNColumns(8)
legend2 = ROOT.TLegend(0.12, 0.77, 0.96, 0.85, '', 'NBNDC')
legend2.SetFillColor(0)
legend2.SetNColumns(9)

# Additional splitting - type 0, 1, 2 / full (agree) / partial

hists = []
# With further splitting: BothLDM-F, BothHDM-F, LDM(CMSSW)-HDM(Mapping)-F, LDM-partial, HDM-partial
alt_hists = []
col_idx = 0
n_elinks = 0
n_additional_elinks = 0
summary_file = ['# luv / l / u / v / av_words / elinks / av_words_div_%i / ceil_av_words_div_%i / av_hits / prev_av_hits | cmssw_type / type / cmssw_shape / shape / transform_method / transform_data' % (args.max, args.max)]
for i in xrange(1, 9):
    hists.append(fin.Get('%s%i' % (args.hist, i)))
    col = AUTO_COLOURS[col_idx]
    if col_idx < len(AUTO_COLOURS) - 1:
        col_idx += 1
    if isinstance(col, list):
        col = ROOT.TColor.GetColor(*col)
    plot.Set(hists[-1], LineColor=col, MarkerColor=col, LineWidth=1, MarkerSize=1.0)
    legend.AddEntry(hists[-1], 'nElinks = %i' % i, 'P')
    if args.check:
        lines, n_elinks_x, n_additional_elinks_x, hist_cleaned = CheckHighRates(hists[-1], i, args.max, modifier, hit_data)
        if args.draw_mode == 1:
            alt_hists.append(GetSplitHists(hists[-1], modifier))
        elif args.draw_mode == 2:
            alt_hists.append(GetSplitHists(hist_cleaned, modifier))

        summary_file.extend(lines)
        n_elinks += n_elinks_x
        n_additional_elinks += n_additional_elinks_x

summary_file.append('# Total links defined: %i' % n_elinks)
summary_file.append('# Additional links needed: %i' % n_additional_elinks)
with open('rate_summary_file%s.txt' % args.post, 'w') as outfile:
    outfile.write('\n'.join(summary_file))


canv = ROOT.TCanvas(args.hist + args.post + str(args.draw_mode))
pads = plot.OnePad()

axis = hists[0].Clone()
axis.Reset()
axis.SetMinimum(0)
axis.SetMaximum(130)
axis.Draw()
plot.Set(axis.GetXaxis(), TitleSize=axis.GetXaxis().GetTitleSize() * 0.75)
plot.Set(axis.GetYaxis(), TitleSize=axis.GetYaxis().GetTitleSize() * 0.75)

if args.draw_mode == 0:
    for h in hists:
        h.Draw('SAMEP')
else:
    legend2_hists = []
    for h in alt_hists[0]:
        legend2_hists.append(h.Clone())
        plot.Set(legend2_hists[-1], LineColor=1, MarkerColor=1)
    legend2.AddEntry(legend2_hists[0], 'HD-F', 'P')
    legend2.AddEntry(legend2_hists[1], 'LD-F', 'P')
    legend2.AddEntry(legend2_hists[2], 'Scint', 'P')
    legend2.AddEntry(legend2_hists[3], 'HD-P', 'P')
    legend2.AddEntry(legend2_hists[4], 'LD-P', 'P')
    legend2.AddEntry(legend2_hists[5], 'HD-F#rightarrowLD-F', 'P')
    legend2.AddEntry(legend2_hists[6], 'LD-F#rightarrowHD-F', 'P')
    legend2.AddEntry(legend2_hists[7], 'HD-P#rightarrowHD-F', 'P')
    legend2.AddEntry(legend2_hists[8], 'LD-P#rightarrowLD-F', 'P')
    legend2.Draw()
    for X in alt_hists:
        for h in X:
            h.Draw('SAMEP')

hline = ROOT.TLine()
plot.Set(hline, LineColor=16, LineStyle=2)
if args.hist == 'AveragePacketWords':
    for i in xrange(1, 9):
        plot.DrawHorizontalLine(pads[0], hline, i * args.max)
if args.hist == 'AveragePacketWordsNorm':
    plot.DrawHorizontalLine(pads[0], hline, args.max)
    plot.DrawHorizontalLine(pads[0], hline, 53.3)

legend.Draw()

plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.19)

canv.Print('.pdf')
canv.Print('.png')


if args.hit_data is not None:
    canv = ROOT.TCanvas(args.hist + args.post + '_hitratio' + str(args.draw_mode))
    pads = plot.OnePad()

    hit_hists = []
    alt_hit_hists = []
    for h in hit_hists:
        hit_hists.append(GetHitRatioHist(h, hit_data))
    for hlist in alt_hists:
        for h in hlist:
            alt_hit_hists.append(GetHitRatioHist(h, hit_data))

    axis = hists[0].Clone()
    axis.Reset()
    axis.SetMinimum(0.4)
    axis.SetMaximum(2.0)
    axis.GetYaxis().SetTitle('Average nHits: current / previous')
    # pads[0].SetGrid(0, 2)
    axis.Draw()
    for i in xrange(14):
        plot.DrawHorizontalLine(pads[0], hline, 0.5 + i * 0.1)

    if args.draw_mode == 0:
        for h in hit_hists:
            h.Draw('SAMEP')
    else:
        for h in alt_hit_hists:
            h.Draw('SAMEP')
        legend2.Draw()
    # plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.25)
    legend.Draw()

    canv.Print('.pdf')
    canv.Print('.png')
    canv.SetName(canv.GetName() + '_zoom')
    axis.SetMinimum(0.9)
    axis.SetMaximum(1.2)
    canv.Print('.pdf')
    canv.Print('.png')

