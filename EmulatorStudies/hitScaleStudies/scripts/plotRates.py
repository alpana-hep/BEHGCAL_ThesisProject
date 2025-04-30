import ROOT
import plotting as plot
import argparse
import json
import math
import tools

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)
ROOT.TH1.SetDefaultSumw2(True)
plot.ModTDRStyle(width=1000)



samples = [
    'D49_ttbar_PU140_3iab',
    'D49_ttbar_PU200_start',
    # 'D49_ttbar_PU200_1p5iab',
    'D49_ttbar_PU200_3iab',
    'D49_ttbar_PU200_4iab',
    'D49_ttbar_PU200_3iab_puintime',
    'D49_ttbar_PU200_3iab_puintime_nonoise',
    'D49_ttbar_PU0_3iab',
    'D49_ttbar_PU0_3iab_nonoise',
]

mips = [
'0.5', '1.0', '2.0', '5.0', '10.0'
]

hists = {

}

print 'Average data rate (Dec20): MB / event'
print '-------------------------------------'
print '%-50s %s' % ('', ''.join('%18s' % X for X in mips))
for sa in samples:
    hists[sa] = dict()
    rates = []
    for mip in mips:
        hists[sa][mip] = dict()
        mipstr = mip.replace('.', 'p')
        fin = ROOT.TFile('hitData_%s_mip%s.root' % (sa, mipstr))
        hists[sa][mip]['data_vs_layer'] = fin.Get('h2d_data_vs_layer').ProfileX().ProjectionX()
        hists[sa][mip]['hd_data_vs_layer'] = fin.Get('h2d_hd_data_vs_layer').ProfileX().ProjectionX()
        hists[sa][mip]['ld_data_vs_layer'] = fin.Get('h2d_ld_data_vs_layer').ProfileX().ProjectionX()
        hists[sa][mip]['data_vs_layer'].Scale(1. / 8.)
        hists[sa][mip]['hd_data_vs_layer'].Scale(1. / 8.)
        hists[sa][mip]['ld_data_vs_layer'].Scale(1. / 8.)
        h = fin.Get('h_total_data')
        rates.append((h.GetMean() / 8., h.GetMeanError() / 8.))
    print '%-50s %s' % (sa, ''.join(['%8.3f +/- %-4.3f' % X for X in rates]))

print '\nAverage total hit rate: 1k hits / event'
print '---------------------------------------'
print '%-50s %s' % ('', ''.join('%18s' % X for X in mips))
for sa in samples:
    rates = []
    for mip in mips:
        mipstr = mip.replace('.', 'p')
        fin = ROOT.TFile('hitData_%s_mip%s.root' % (sa, mipstr))
        h = fin.Get('h_total_hits')
        rates.append((h.GetMean(), h.GetMeanError()))
    print '%-50s %s' % (sa, ''.join(['%8.3f +/- %-4.3f' % X for X in rates]))



def MakeProfilePlot(drawlist, hists, output, label='data_vs_layer', ratioRange=[0., 1.]):
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
    col_idx = 0

    canv = ROOT.TCanvas(output, output)
    pads = plot.TwoPadSplit(0.35, 0.01, 0.01)
    pads[0].cd()
    hlist = []
    rlist = []
    for sa, mip in drawlist:
        h = hists[sa][mip][label]
        col = AUTO_COLOURS[col_idx]
        if col_idx < len(AUTO_COLOURS) - 1:
            col_idx += 1
        if isinstance(col, list):
            col = ROOT.TColor.GetColor(*col)
        plot.Set(h, LineColor=col, MarkerColor=col)
        hlist.append(h)
        rlist.append(plot.MakeRatioHist(h, hlist[0], True, False))
        # rlist[-1].Print('range')
        pads[0].cd()
        h.Draw('SAME')
        pads[1].cd()
        rlist[-1].Draw('SAME')
    pads[1].cd()
    pads[1].SetGrid(0, 1)
    plot.SetupTwoPadSplitAsRatio(
        pads, plot.GetAxisHist(
            pads[0]), plot.GetAxisHist(pads[1]), 'Ratio', True, ratioRange[0], ratioRange[1])

    pads[0].cd()
    hlist[0].GetYaxis().SetTitle('Dec20 rate (MB / event)')
    rlist[0].GetXaxis().SetTitle('Layer')

    legend = ROOT.TLegend(0.55, 0.65, 0.96, 0.93, '', 'NBNDC')
    for ip in xrange(len(drawlist)):
        legend.AddEntry(hlist[ip], '%s - %s MIP' % drawlist[ip], 'P')
    legend.Draw()

    canv.Print('.pdf')


for htype in ['data_vs_layer', 'hd_data_vs_layer', 'ld_data_vs_layer']:
    MakeProfilePlot(
        [
            ('D49_ttbar_PU200_3iab', '0.5'),
            ('D49_ttbar_PU200_3iab', '1.0'),
            ('D49_ttbar_PU200_3iab', '2.0'),
            ('D49_ttbar_PU200_3iab', '5.0'),
            ('D49_ttbar_PU200_3iab', '10.0')
        ], hists, '%s_mips' % htype, label=htype)

    MakeProfilePlot(
        [
            ('D49_ttbar_PU200_start', '0.5'),
            ('D49_ttbar_PU200_1p5iab', '0.5'),
            ('D49_ttbar_PU200_3iab', '0.5'),
            ('D49_ttbar_PU200_4iab', '0.5'),
        ], hists, '%s_ageing' % htype, label=htype, ratioRange=[0.8, 1.4])


    for mip in mips:
        MakeProfilePlot(
            [
                ('D49_ttbar_PU200_3iab', mip),
                ('D49_ttbar_PU200_3iab_puintime', mip),
                ('D49_ttbar_PU200_3iab_puintime_nonoise', mip),
                ('D49_ttbar_PU0_3iab', mip),
                ('D49_ttbar_PU0_3iab_nonoise', mip),
            ], hists, '%s_%s_scenario' % (htype, mip.replace('.', 'p')), label=htype, ratioRange=[0.0, 2.0])

# Data / layer : comparing ageing / scenarios


# modifier = {}
# with open(args.modifier) as jsonfile:
#     modifier_tmp = json.load(jsonfile)
#     for X in modifier_tmp:
#         modifier[tuple(X['key'])] = dict(X)

# hit_data = {}
# if args.hit_data is not None:
#     with open(args.hit_data) as module_file:
#         for line in module_file:
#             X = line.split()
#             layer = int(X[0])
#             u = int(X[1])
#             v = int(X[2])
#             av_hits = float(X[3])
#             prev_hits = float(X[4])
#             hit_data[layer, u, v] = (av_hits, prev_hits)

# fin = ROOT.TFile(args.file)

# legend = ROOT.TLegend(0.12, 0.85, 0.96, 0.93, '', 'NBNDC')
# legend.SetFillColor(0)
# legend.SetNColumns(8)
# legend2 = ROOT.TLegend(0.12, 0.77, 0.96, 0.85, '', 'NBNDC')
# legend2.SetFillColor(0)
# legend2.SetNColumns(9)

# # Additional splitting - type 0, 1, 2 / full (agree) / partial

# hists = []
# # With further splitting: BothLDM-F, BothHDM-F, LDM(CMSSW)-HDM(Mapping)-F, LDM-partial, HDM-partial
# alt_hists = []
# col_idx = 0
# n_elinks = 0
# n_additional_elinks = 0
# summary_file = ['# luv / l / u / v / av_words / elinks / av_words_div_%i / ceil_av_words_div_%i / av_hits / prev_av_hits | cmssw_type / type / cmssw_shape / shape / transform_method / transform_data' % (args.max, args.max)]
# for i in xrange(1, 9):
#     hists.append(fin.Get('%s%i' % (args.hist, i)))
#     col = AUTO_COLOURS[col_idx]
#     if col_idx < len(AUTO_COLOURS) - 1:
#         col_idx += 1
#     if isinstance(col, list):
#         col = ROOT.TColor.GetColor(*col)
#     plot.Set(hists[-1], LineColor=col, MarkerColor=col, LineWidth=1, MarkerSize=1.0)
#     legend.AddEntry(hists[-1], 'nElinks = %i' % i, 'P')
#     if args.check:
#         lines, n_elinks_x, n_additional_elinks_x, hist_cleaned = CheckHighRates(hists[-1], i, args.max, modifier, hit_data)
#         if args.draw_mode == 1:
#             alt_hists.append(GetSplitHists(hists[-1], modifier))
#         elif args.draw_mode == 2:
#             alt_hists.append(GetSplitHists(hist_cleaned, modifier))

#         summary_file.extend(lines)
#         n_elinks += n_elinks_x
#         n_additional_elinks += n_additional_elinks_x

# summary_file.append('# Total links defined: %i' % n_elinks)
# summary_file.append('# Additional links needed: %i' % n_additional_elinks)
# with open('rate_summary_file%s.txt' % args.post, 'w') as outfile:
#     outfile.write('\n'.join(summary_file))


# canv = ROOT.TCanvas(args.hist + args.post + str(args.draw_mode))
# pads = plot.OnePad()

# axis = hists[0].Clone()
# axis.Reset()
# axis.SetMinimum(0)
# axis.SetMaximum(130)
# axis.Draw()
# plot.Set(axis.GetXaxis(), TitleSize=axis.GetXaxis().GetTitleSize() * 0.75)
# plot.Set(axis.GetYaxis(), TitleSize=axis.GetYaxis().GetTitleSize() * 0.75)

# if args.draw_mode == 0:
#     for h in hists:
#         h.Draw('SAMEP')
# else:
#     legend2_hists = []
#     for h in alt_hists[0]:
#         legend2_hists.append(h.Clone())
#         plot.Set(legend2_hists[-1], LineColor=1, MarkerColor=1)
#     legend2.AddEntry(legend2_hists[0], 'HD-F', 'P')
#     legend2.AddEntry(legend2_hists[1], 'LD-F', 'P')
#     legend2.AddEntry(legend2_hists[2], 'Scint', 'P')
#     legend2.AddEntry(legend2_hists[3], 'HD-P', 'P')
#     legend2.AddEntry(legend2_hists[4], 'LD-P', 'P')
#     legend2.AddEntry(legend2_hists[5], 'HD-F#rightarrowLD-F', 'P')
#     legend2.AddEntry(legend2_hists[6], 'LD-F#rightarrowHD-F', 'P')
#     legend2.AddEntry(legend2_hists[7], 'HD-P#rightarrowHD-F', 'P')
#     legend2.AddEntry(legend2_hists[8], 'LD-P#rightarrowLD-F', 'P')
#     legend2.Draw()
#     for X in alt_hists:
#         for h in X:
#             h.Draw('SAMEP')

# hline = ROOT.TLine()
# plot.Set(hline, LineColor=16, LineStyle=2)
# if args.hist == 'AveragePacketWords':
#     for i in xrange(1, 9):
#         plot.DrawHorizontalLine(pads[0], hline, i * args.max)
# if args.hist == 'AveragePacketWordsNorm':
#     plot.DrawHorizontalLine(pads[0], hline, args.max)
#     plot.DrawHorizontalLine(pads[0], hline, 53.3)

# legend.Draw()

# plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.19)

# canv.Print('.pdf')
# canv.Print('.png')


# if args.hit_data is not None:
#     canv = ROOT.TCanvas(args.hist + args.post + '_hitratio' + str(args.draw_mode))
#     pads = plot.OnePad()

#     hit_hists = []
#     alt_hit_hists = []
#     for h in hit_hists:
#         hit_hists.append(GetHitRatioHist(h, hit_data))
#     for hlist in alt_hists:
#         for h in hlist:
#             alt_hit_hists.append(GetHitRatioHist(h, hit_data))

#     axis = hists[0].Clone()
#     axis.Reset()
#     axis.SetMinimum(0.4)
#     axis.SetMaximum(2.0)
#     axis.GetYaxis().SetTitle('Average nHits: current / previous')
#     # pads[0].SetGrid(0, 2)
#     axis.Draw()
#     for i in xrange(14):
#         plot.DrawHorizontalLine(pads[0], hline, 0.5 + i * 0.1)

#     if args.draw_mode == 0:
#         for h in hit_hists:
#             h.Draw('SAMEP')
#     else:
#         for h in alt_hit_hists:
#             h.Draw('SAMEP')
#         legend2.Draw()
#     # plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.25)
#     legend.Draw()

#     canv.Print('.pdf')
#     canv.Print('.png')
#     canv.SetName(canv.GetName() + '_zoom')
#     axis.SetMinimum(0.9)
#     axis.SetMaximum(1.2)
#     canv.Print('.pdf')
#     canv.Print('.png')

