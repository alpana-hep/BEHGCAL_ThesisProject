import ROOT
import sys
import json
import os
from numpy import isin
import plotting as plot
# from pprint import pprint
from array import array
from glob import glob
# import numpy as np

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.AddDirectory(False)

ROOT.gROOT.ProcessLine('#include "interface/FastControlCommands.h"')
ROOT.gSystem.Load('lib/libHGCalBufferModel.so')
plot.ModTDRStyle()


def LoadConfig(fin):
    js = json.loads(fin.Get('config').GetString().Data())
    return js


def ReflowBins(h_in, bins=[], bin_labels=None):
    h_out = ROOT.TH1D(h_in.GetName(), '', len(bins), -0.5, -0.5 + len(bins))
    for ob, ib in enumerate(bins):
        h_out.SetBinContent(ob + 1, h_in.GetBinContent(ib))
        h_out.SetBinError(ob + 1, h_in.GetBinError(ib))
        if bin_labels is not None:
            h_out.GetXaxis().SetBinLabel(ob + 1, bin_labels[ob])
    return h_out


global_colors = [1, ROOT.kRed, ROOT.kOrange,
                 ROOT.kSpring-1, ROOT.kGreen+3, ROOT.kCyan+3, 4]


basedir = '/eos/cms/store/user/agilbert/hgcal/emulator'

outdir = sys.argv[1]
os.system('mkdir -p %s' % outdir)

dat = {}

scans = [
    ('triggerRate_default', 'triggerRate', 'Default'),
    ('triggerRate_regTrig', 'triggerRate', 'regTrig'),
    ('triggerRate_regTrigGaps', 'triggerRate', 'regTrigGaps'),
    ('triggerRate_avEvent', 'triggerRate', 'avEventRoundUp'),
    ('triggerRate_avEventRound', 'triggerRate', 'avEventRound'),
    ('triggerRate_avEventPacket', 'triggerRate', 'avEventPacket'),
    ('randomSeed_default', 'randomSeed', 'Default'),
    ('triggerRate_avEventRound_regTrig', 'triggerRate', 'regTrig&avEventRound'),
    ('test', 'triggerRate', 'test'),
]


read_hists = [
    'FastControlDriver_Fractions',
    'EcondAsic_AveragePacketWordsLuv',
    'EcondAsic_AverageN10HitsLuv',
    'EcondAsic_AverageN20HitsLuv',
    'EcondAsic_AverageN30HitsLuv',
    'EventSize',
    'EcondAsic_FractionTruncated',
    'EcondAsic_FractionTruncatedLuv',
    'LpgbtPairEcondBuilder_DiscardedPacketsFractionLuv'
]
read_hists_proc = [
  "nEvents",
  "nEcondTruncations",
  "nBETruncations",
  "n2DEcondTruncationsPerSlink",
  "n2DBETruncationsPerSlink",
  "n1DEcondTruncationsPerSlink",
  "n1DBETruncationsPerSlink",
  "nEcondTruncPackets",
  "nBETruncPackets",
]

for dirname, index_on, label in scans:
    files = glob('%s/%s/HGCalBufferModel_*.root' % (basedir, dirname))
    proc_files = glob('%s/%s/HGCalBufferModel_*_proc.root' % (basedir, dirname))
    dat[dirname] = dict()

    x_vals = []
    for fin in files:
        if 'proc' in fin:
            continue
        f = ROOT.TFile(fin)
        print(fin)
        cfg = LoadConfig(f)
        x_val = cfg[index_on]
        x_vals.append(x_val)
        dat[dirname][x_val] = dict()
        for h in read_hists:
            dat[dirname][x_val][h] = f.Get(h)
        f.Close()

    # if dirname not in['triggerRate_default', 'triggerRate_avEvent', 'triggerRate_avEventRound', 'triggerRate_avEventRound_regTrig', 'randomSeed_default']:
    #     continue

    for fin in proc_files:
        f = ROOT.TFile(fin)
        print(fin)
        cfg = LoadConfig(f)
        x_val = cfg[index_on]
        for h in read_hists_proc:
            dat[dirname][x_val][h] = f.Get(h)
        f.Close()

for dirname in dat:
    for x_val in dat[dirname]:
        plots = dat[dirname][x_val]
        if 'nEvents' not in plots:
            continue
        n_events = float(plots['nEvents'].GetBinContent(1))
        plots['fracEcondTruncations'] = plots['nEcondTruncations'].Clone()
        plots['fracEcondTruncations'].Scale(1. / n_events)
        plots['fracBETruncations'] = plots['nBETruncations'].Clone()
        plots['fracBETruncations'].Scale(1. / n_events)
        plots['frac1DEcondTruncationsPerSlink'] = plots['n1DEcondTruncationsPerSlink'].Clone()
        plots['frac1DEcondTruncationsPerSlink'].Scale(1. / n_events)
        plots['frac1DBETruncationsPerSlink'] = plots['n1DBETruncationsPerSlink'].Clone()
        plots['frac1DBETruncationsPerSlink'].Scale(1. / n_events)

# Construct a histogram of the frequency over emulator runs for a given slink to have non-zero truncation
if 'randomSeed_default' in dat:
    subdata = dat['randomSeed_default']
    h = subdata[1]['frac1DEcondTruncationsPerSlink'].Clone()
    h.Reset()
    for idx in subdata:
        hidx = subdata[idx]['frac1DEcondTruncationsPerSlink']
        for ib in range(1, hidx.GetNbinsX() + 1):
            if hidx.GetBinContent(ib + 1) > 0:
                h.SetBinContent(ib + 1, h.GetBinContent(ib + 1) + 1)
    subdata[1]['frequencyOfEcondTruncationsPerSlinkPerRun'] = h



def AnalyseFastControlCommands(dat, inputs, hist='FastControlDriver_Fractions', outdir='.', ytitle='Fraction'):
    nCommands = ROOT.FastControlCommands.Command.ExtendedRange
    commandNames = []
    for ic in range(nCommands):
        commandNames.append(ROOT.FastControlCommands.commandName(ic).strip())
    print(commandNames)

    drawlist = []
    for dirname, overlay_points, leg_label in inputs:
        x_vals = overlay_points
        if len(overlay_points) == 0:
            x_vals = sorted(dat[dirname].keys())

        for x_val in x_vals:
            drawlist.append(ReflowBins(dat[dirname][x_val][hist], range(
                1, nCommands + 1), bin_labels=commandNames))
            drawlist[-1]._dirname = dirname
            drawlist[-1]._x = x_val
            drawlist[-1]._leg_text = leg_label % x_val

    # First make plot showing all fractions for the chosen set of "overlay_points"

    colors = list(global_colors)
    plot.ModTDRStyle(width=800, height=600, b=0.25)
    h_store = {}
    canv = ROOT.TCanvas(hist)
    pads = plot.OnePad()
    pads[0].cd()
    for h in drawlist:
        h.LabelsOption('v')

    axis = drawlist[0].Clone()
    axis.Reset()
    plot.Set(axis, Minimum=0.01, Maximum=1)
    plot.Set(axis.GetYaxis(), Title=ytitle,
             TitleOffset=axis.GetYaxis().GetTitleOffset()*0.5)
    pads[0].SetLogy(True)
    pads[0].SetGrid(0, 1)
    axis.Draw()
    legend = ROOT.TLegend(0.15, 0.6, 0.5, 0.92, '', 'NBNDC')
    for h in drawlist:
        col = colors.pop(0)
        plot.Set(h, LineWidth=2, LineColor=col)
        h.Draw('HISTSAME')
        legend.AddEntry(h, h._leg_text, 'L')
    legend.Draw()
    for fmt in ['.pdf', '.png', '.svg']:
        canv.Print('%s/%s%s' % (outdir, canv.GetName(), fmt))


def GetTH1Sorted(h):
    res = []
    for i in range(1, h.GetNbinsX() + 1):
        res.append((i, h.GetBinContent(i)))
    return sorted(res, reverse=True, key=lambda x: x[1])


def GenericOverlayTH1(dat,
                      inputs,
                      hist='EcondAsic_AveragePacketWordsLuv',
                      wh=(800, 800),
                      grid=(0, 1),
                      legend=(0.55, 0.70, 0.94, 0.92),
                      normalize=False,
                      ratio=False,
                      outdir='.',
                      style='HIST',
                      outname=None,
                      ytitle=None,
                      xtitle=None,
                      logy=None,
                      graphbin=None,
                      aggr=None,
                      xrange=None,
                      markersize=0.7):
    if outname is None:
        outname = hist
    print('>>>>>>>>>>>> ' + outname)
    drawlist = []
    for dirname, overlay_points, leg_label in inputs:
        x_vals = overlay_points
        if len(overlay_points) == 0:
            x_vals = sorted(dat[dirname].keys())
        if graphbin is not None:
            y_vals = []
            for x_val in x_vals:
                if hist not in dat[dirname][x_val]:
                    print('Warning, %s not in %s/%s' % (hist, dirname, x_val))
                if graphbin == 'mean':
                    y_vals.append(dat[dirname][x_val][hist].GetMean())
                else:
                    y_vals.append(dat[dirname][x_val][hist].GetBinContent(graphbin))
            if aggr is not None:
                drawlist.append(ROOT.TH1D(dirname, '', aggr[0], aggr[1], aggr[2]))
                for y_val in y_vals:
                    drawlist[-1].Fill(y_val)
            else:
                drawlist.append(ROOT.TGraph(len(x_vals), array('d', x_vals), array('d', y_vals)))
                drawlist[-1].SetName(dirname)
                print ('Graph for ' + dirname)
                drawlist[-1].Print()
            drawlist[-1]._dirname = dirname
            drawlist[-1]._leg_text = leg_label
        else:
            for x_val in x_vals:
                if hist not in dat[dirname][x_val]:
                    print('Warning, %s not in %s/%s' % (hist, dirname, x_val))
                drawlist.append(dat[dirname][x_val][hist].Clone())
                if isinstance(drawlist[-1], ROOT.TH1I):
                    newhist = ROOT.TH1D()
                    drawlist[-1].Copy(newhist)
                    drawlist[-1] = newhist
                drawlist[-1]._dirname = dirname
                drawlist[-1]._x = x_val
                drawlist[-1]._leg_text = leg_label % x_val
                if normalize:
                    drawlist[-1].Scale(1. / drawlist[-1].Integral())

    colors = list(global_colors)
    plot.ModTDRStyle(width=wh[0], height=wh[1])
    canv = ROOT.TCanvas(outname)
    # pads = plot.OnePad()
    if ratio:
        pads = plot.TwoPadSplit(0.35, 0.01, 0.01)
    else:
        pads = plot.OnePad()
    pads[0].cd()
    if isinstance(drawlist[0], ROOT.TGraph):
        axis = plot.CreateAxisHist(drawlist[0], False)
    else:
        axis = drawlist[0].Clone()
    pads[0].cd()
    axis.Reset()
    if xrange is not None:
        axis.GetXaxis().SetRangeUser(xrange[0], xrange[1])
    if logy is not None:
        pads[0].SetLogy(True)
        axis.SetMinimum(logy)
    plot.Set(axis.GetYaxis(), TitleOffset=axis.GetYaxis().GetTitleOffset() * (1. - ((wh[0] - wh[1])*0.8)/wh[0]))

    axis_ratio = axis.Clone()
    # plot.Set(axis, Minimum=0.01, Maximum=1)
    if ytitle is not None:
        plot.Set(axis.GetYaxis(), Title=ytitle)
    if xtitle is not None:
        plot.Set(axis.GetXaxis(), Title=xtitle)

    # pads[0].SetLogy(True)
    pads[0].SetGrid(grid[0], grid[1])
    axis.Draw()
    legend = ROOT.TLegend(legend[0], legend[1],
                          legend[2], legend[3], '', 'NBNDC')
    rlist = []
    for h in drawlist:
        col = colors.pop(0)
        plot.Set(h, LineWidth=2, LineColor=col,
                 MarkerColor=col, MarkerSize=markersize)
        if not graphbin:
            rlist.append(plot.MakeRatioHist(h, drawlist[0], False, False))
        h.Draw(style+'SAME')
        legend.AddEntry(h, h._leg_text, 'P' if style == 'P' else 'L')
    if ratio:
        pads[1].cd()
        axis_ratio.Draw()
        for h in rlist:
            h.Draw(style+'SAME')
        pads[1].SetGrid(0, 1)
        plot.SetupTwoPadSplitAsRatio(
            pads, axis, axis_ratio, '', True, 0.0, 2.8)
    pads[0].cd()
    plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.25)
    legend.Draw()
    for fmt in ['.pdf', '.png', '.svg', '.root']:
        canv.Print('%s/%s%s' % (outdir, canv.GetName(), fmt))


leg_top_left = (0.17, 0.70, 0.50, 0.92)

def Plot2D(dat, input=('triggerRate_default', 750), hist='n2DEcondTruncationsPerSlink', outname='bus', titles=('x', 'y', 'z')):
    plot.ModTDRStyle(width=900, height=800, l = 0.15, b = 0.13, r = 0.23, t=0.08)
    h = dat[input[0]][input[1]][hist].Clone()
    N = h.GetNbinsX()
    for ix in range(N):
        ref_val = h.GetBinContent(ix + 1, N - ix)
        for iy in range(N):
            if ref_val > 0:
                h.SetBinContent(ix + 1, iy + 1, h.GetBinContent(ix + 1, iy + 1) / ref_val)
    h.SetContour(255)
    ROOT.gStyle.SetPalette(103)
    ROOT.TColor.InvertPalette()
    canv = ROOT.TCanvas(outname)
    pads = plot.OnePad()
    pads[0].cd()
    plot.Set(h.GetXaxis(), Title=titles[0])
    plot.Set(h.GetYaxis(), Title=titles[1])
    plot.Set(h.GetYaxis(), TitleOffset=h.GetYaxis().GetTitleOffset() * 0.7)
    plot.Set(h.GetZaxis(), Title=titles[2])
    # if zmax is not None:
    #     hist.SetMaximum(zmax)
    # if zmin is not None:
    #     hist.SetMinimum(zmin)
    h.Draw('COLZ')
    pads[0].GetFrame().Draw()
    for fmt in ['.pdf', '.png', '.svg']:
        canv.Print('%s/%s%s' % (outdir, outname, fmt))


Plot2D(dat, input=('triggerRate_default', 850), hist='n2DEcondTruncationsPerSlink',
    outname='n2DEcondTruncationsPerSlink_default_850',
    titles=('slink i', 'slink j', 'F(trunc. j | trunc. i)'))

AnalyseFastControlCommands(dat, inputs=[
    ('triggerRate_default', [750], 'Default, %s kHz'),
    ('triggerRate_regTrig', [755], 'regTrig, %s kHz'),
    ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
    ('triggerRate_avEventRound_regTrig', [755], 'regTrig & avEventRound, %s kHz'),
], outdir=outdir)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [750], 'Default, %s kHz'),
    # ('triggerRate_regTrig', [755], 'regTrig, %s kHz'),
    ('triggerRate_avEvent', [750], 'avEventRoundUp, %s kHz'),
    ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
    ('triggerRate_avEventPacket', [750], 'avEventPacket, %s kHz'),
], hist='EcondAsic_AveragePacketWordsLuv', ratio=True, wh=(1200, 800), style='P', outdir=outdir, markersize=0.2)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [750], 'Default, %s kHz'),
    ('triggerRate_regTrig', [755], 'regTrig, %s kHz'),
    ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
    ('triggerRate_avEventRound_regTrig', [755], 'regTrig & avEventRound, %s kHz'),
], hist='EcondAsic_FractionTruncated', wh=(800, 800), normalize=True, legend=leg_top_left, outdir=outdir)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [750], 'Default, %s kHz'),
    ('triggerRate_avEvent', [750], 'avEventRoundUp, %s kHz'),
    ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
], hist='EventSize', wh=(800, 800), normalize=True, legend=leg_top_left, outdir=outdir)


GenericOverlayTH1(dat, inputs=[
    ('randomSeed_default', [1], 'Default (%s)'),
], hist='frequencyOfEcondTruncationsPerSlinkPerRun', outname='frequencyOfEcondTruncationsPerSlinkPerRun', xtitle='slink index', ytitle='Number of emulator runs w/ trunc.',
    ratio=False, wh=(1200, 800), style='HIST', outdir=outdir)
dat['randomSeed_default'][1]['frequencyOfEcondTruncationsPerSlinkPerRun'].Print('range')

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
], hist='frac1DEcondTruncationsPerSlink', outname='frac1DEcondTruncationsPerSlink_vs_triggerRate', xtitle='slink index', ytitle='Frac. of events with ECON-D trunc.',
    ratio=False, wh=(1200, 800), style='P', logy=1E-4, outdir=outdir)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
], hist='frac1DBETruncationsPerSlink', outname='frac1DBETruncationsPerSlink_vs_triggerRate', xtitle='slink index', ytitle='Frac. of events with BE packet loss',
    ratio=False, wh=(1200, 800), style='P', logy=1E-4, outdir=outdir)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
], hist='EcondAsic_FractionTruncatedLuv', outname='EcondAsic_FractionTruncatedLuv_vs_triggerRate',
    ratio=False, wh=(1200, 800), style='P', logy=1E-4, outdir=outdir)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
], hist='LpgbtPairEcondBuilder_DiscardedPacketsFractionLuv', outname='LpgbtPairEcondBuilder_DiscardedPacketsFractionLuv_vs_triggerRate',
    ratio=True, wh=(1200, 800), style='P', outdir=outdir)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    ('triggerRate_avEvent', [], 'avEventRoundUp'),
    ('triggerRate_avEventRound', [], 'avEventRound'),
    ('triggerRate_regTrig', [], 'regTrigger'),
    ('triggerRate_avEventRound_regTrig', [], 'avEventRound+regTrigger'),
], hist='FastControlDriver_Fractions', outname='HgcrocTcdsThrottle_vs_triggerRate',
    graphbin=20, style='PL', outdir=outdir, ytitle='HgcrocTcdsThrottle fraction', xtitle='Trigger rate (kHz)', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    # ('triggerRate_avEvent', [], 'avEventRoundUp'),
    ('triggerRate_avEventRound', [], 'avEventRound'),
    ('triggerRate_avEventPacket', [], 'avEventPacket'),
    ('triggerRate_regTrig', [], 'regTrigger'),
    ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
    ('triggerRate_avEventRound_regTrig', [], 'avEventRound+regTrigger'),
], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerRate',
    graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate (kHz)', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    # ('triggerRate_avEvent', [], 'avEventRoundUp'),
    ('triggerRate_avEventRound', [], 'avEventRound'),
    ('triggerRate_avEventPacket', [], 'avEventPacket'),
], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerRate_forAvEvent',
    graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate (kHz)', legend=leg_top_left, logy=1E-4)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    ('triggerRate_regTrig', [], 'regTrigger'),
    ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerRate_forRegTrig',
    graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate (kHz)', legend=leg_top_left, logy=1E-4)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    # ('triggerRate_avEvent', [], 'avEventRoundUp'),
    ('triggerRate_avEventRound', [], 'avEventRound'),
    ('triggerRate_avEventPacket', [], 'avEventPacket'),
    ('triggerRate_regTrig', [], 'regTrigger'),
    ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
    ('triggerRate_avEventRound_regTrig', [], 'avEventRound+regTrigger'),
], hist='fracBETruncations', outname='fracBETruncations_vs_triggerRate',
    graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with BE packet loss', xtitle='Trigger rate (kHz)', legend=leg_top_left)


GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    # ('triggerRate_avEvent', [], 'avEventRoundUp'),
    ('triggerRate_avEventRound', [], 'avEventRound'),
    ('triggerRate_regTrig', [], 'regTrigger'),
    ('triggerRate_avEventRound_regTrig', [], 'avEventRound+regTrigger'),
], hist='nEcondTruncPackets', outname='avEcondTruncPackets_vs_triggerRate',
    graphbin='mean', style='PL', outdir=outdir, ytitle='Average ECON-D trunc. per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [725, 750, 775], 'Default'),
    ('triggerRate_avEventRound', [725, 750, 775], 'avEventRound'),
    ('triggerRate_regTrig', [727, 755, 769], 'regTrigger'),
    ('triggerRate_avEventRound_regTrig', [727, 755, 769], 'avEventRound+regTrigger'),
], hist='nEcondTruncPackets', outname='avEcondTruncPackets_vs_triggerRate_zoom',
    graphbin='mean', style='PL', outdir=outdir, ytitle='Average ECON-D trunc. per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [], 'Default'),
    # ('triggerRate_avEvent', [], 'avEventRoundUp'),
    ('triggerRate_avEventRound', [], 'avEventRound'),
    ('triggerRate_regTrig', [], 'regTrigger'),
    ('triggerRate_avEventRound_regTrig', [], 'avEventRound+regTrigger'),
], hist='nBETruncPackets', outname='avnBETruncPackets_vs_triggerRate',
    graphbin='mean', style='PL', outdir=outdir, ytitle='Average BE packet loss per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('triggerRate_default', [725, 750, 775], 'Default'),
    ('triggerRate_avEventRound', [725, 750, 775], 'avEventRound'),
    ('triggerRate_regTrig', [727, 755, 769], 'regTrigger'),
    ('triggerRate_avEventRound_regTrig', [727, 755, 769], 'avEventRound+regTrigger'),
], hist='nBETruncPackets', outname='avnBETruncPackets_vs_triggerRate_zoom',
    graphbin='mean', style='PL', outdir=outdir, ytitle='Average BE packet loss per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)


GenericOverlayTH1(dat, inputs=[
    ('randomSeed_default', [], 'randomSeed (750 kHz)'),
], hist='fracEcondTruncations', outname='dist_fracEcondTruncations_randomSeed', aggr=(50,0,0.02),
    graphbin=1, style='HIST', outdir=outdir, xtitle='Frac. of events with ECON-D trunc.', ytitle='Number of emulator runs', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('randomSeed_default', [], 'randomSeed (750 kHz)'),
], hist='fracBETruncations', outname='dist_fracBETruncations_randomSeed', aggr=(50,0,0.02),
    graphbin=1, style='HIST', outdir=outdir, xtitle='Frac. of events with BE packet loss', ytitle='Number of emulator runs', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('randomSeed_default', [], 'randomSeed (750 kHz)'),
], hist='nEcondTruncPackets', outname='dist_meanEcondTruncPackets_randomSeed', aggr=(50,0,0.02),
    graphbin='mean', style='HIST', outdir=outdir, xtitle='Average ECON-Ds trunc.', ytitle='Number of emulator runs', legend=leg_top_left)

GenericOverlayTH1(dat, inputs=[
    ('randomSeed_default', [], 'randomSeed (750 kHz)'),
], hist='nBETruncPackets', outname='dist_meanBETruncPackets_randomSeed', aggr=(50,0,0.02),
    graphbin='mean', style='HIST', outdir=outdir, xtitle='Average packets lost in BE', ytitle='Number of emulator runs', legend=leg_top_left)

for i in dat['randomSeed_default']:
    print('>> Random seed %i' % i)
    h = dat['randomSeed_default'][i]['nEcondTruncPackets']
    print(h.GetMean())
    if h.GetMean() > 0.01:
        # h.Print('range')
        GenericOverlayTH1(dat, inputs=[
            ('randomSeed_default', [i], 'Random seed %s')], outname='nEcondTruncPackets_%i' % i,
            hist='nEcondTruncPackets', ratio=False, style='HIST', outdir=outdir, xrange=(-0.5,19.5), logy=0.1, xtitle='Number of trunc. ECON-D packets per event', ytitle='Events')
