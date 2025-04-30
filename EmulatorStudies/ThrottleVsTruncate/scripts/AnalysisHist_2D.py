import ROOT
import sys
import json
import os
from numpy import isin, mean, std, size, sqrt
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


global_colors = [1, ROOT.kMagenta,
                 ROOT.kBlue,ROOT.kOrange, ROOT.kViolet+2,ROOT.kCyan+3, ROOT.kOrange+2,ROOT.kGreen+2, 4]

global_markerStyle=[20,21,22,20,23,8,21,22]
global_markerSize=[6,6,6,2,2,2,2,2]
basedir = sys.argv[1]  #'/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation'
basedir1 = sys.argv[2] #'/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/ProcessedFiles'
outdir = sys.argv[3]
os.system('mkdir -p %s' % outdir)

dat = {}
## list of different scans done for different variables
scans = [
    #('econdbufferlevel_default_V10', 'econdMediumBufferLevel', 'Default'),
    # ('econdbufferlevel_triggerRate_780', 'econdMediumBufferLevel', 'Default'),
    # ('econdbufferlevel_triggerRate_780_econdUpdateIdleStatus', 'econdMediumBufferLevel', 'Default'),
    # ('triggerRate_default_V10', 'triggerRate', 'Default'),
#    ('triggerRate_default_V10_pseudo', 'triggerRate', 'Default'),
#('triggerRate_regTrig_V10_pseudo','triggerRate', 'regular'),
#    ('triggerRate_avEventPacket_regTrig_V10_pseudo','triggerRate','Regular'),
#    ('triggerRate_avEventPacket_V10_pseudo','triggerRate','Default'),
    ('econdHitScaling_1400_default_V10_pseudo','econdHighBufferLevel','Default','1400') ,#'econdHitScaling','Scaled_Occupancy'),
    #('econdHitScaling_1500_default_V10_pseudo','econdHighBufferLevel','Default','1500') ,  
     ('econdHitScaling_1600_default_V10_pseudo','econdHighBufferLevel','Default','1600') ,
    #  # ('econdHitScaling_1400_default_V10_pseudo','econdHighBufferLevel','Default','1400') ,
    # ('econdHitScaling_1300_default_V10_pseudo','econdHighBufferLevel','Default','1300') ,
    # ('econdHitScaling_1100_default_V10_pseudo','econdHighBufferLevel','Default','1100') ,
    # ('econdHitScaling_1200_default_V10_pseudo','econdHighBufferLevel','Default','1200') ,
    # ('econdHitScaling_1000_default_V10_pseudo','econdHighBufferLevel','Default','1000') ,

    #    ('triggerRate_default_800_V10_pseudo', 'econdHighBufferLevel', 'Default','800'),
    # ('triggerRate_default_850_V10_pseudo', 'econdHighBufferLevel', 'Default','850'),
    # ('triggerRate_default_900_V10_pseudo', 'econdHighBufferLevel', 'Default','900'),
    # ('triggerRate_default_950_V10_pseudo', 'econdHighBufferLevel', 'Default','950'),


   # ('triggerRate_regTrig', 'triggerRate', 'regTrig'),
    # # ('triggerRate_regTrigGaps', 'triggerRate', 'regTrigGaps'),
    # # ('triggerRate_avEvent', 'triggerRate', 'avEventRoundUp'),
    # # ('triggerRate_avEventRound', 'triggerRate', 'avEventRound'),
    # ('triggerRate_avEventPacket', 'triggerRate', 'avEventPacket'),
    # # ('randomSeed_default', 'randomSeed', 'Default'),
    # ('triggerRate_avEventPacket_regTrig', 'triggerRate', 'regTrig + avEvent'),
    # ('triggerRate_optimizedMapping', 'triggerRate', 'optimizedMapping'),
    # # ('test', 'triggerRate', 'test'),
]

do_plots = [
    "triggerRate_regTrig_V10_pseudo",
    "triggerRate_default_V10_pseudo",
    "econdHitScaling_Additive_fracNcells_V10_pseudo",
    "econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo",
    "econdHitScaling_default_V10_pseudo",
    "triggerRate_avEventPacket_V10_pseudo",
    "triggerRate_avEventPacket_regTrig_V10_pseudo",                                                                                                                         

]
## histograms to read from output of HgcalBufferModel
read_hists = [
    'SlinkBuilder_AvgSlinkWordsFs',
    'FastControlDriver_Fractions',
    'EcondAsic_AveragePacketWordsLuv',
    'EcondAsic_NumberOfElinksLuv',
    'EcondAsic_AverageN10HitsLuv',
    'EcondAsic_AverageN20HitsLuv',
    'EcondAsic_AverageN30HitsLuv',
    'EventSize',
    'EcondAsic_FractionTruncated',
    'EcondAsic_FractionTruncatedLuv',
    'LpgbtPairEcondBuilder_DiscardedPacketsFractionLuv'
]

## histograms to read from processed output from readEventData.cpp
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
  "localL1A_all_2000",
  "localL1A_econdtrunc_2000",
  "localL1A_all_10000",
  "localL1A_econdtrunc_10000"
]


for dirname, index_on, label , scaling in scans:
    files = glob('%s/HGCalBufferModel_%s_buffer_*.root' %(basedir,scaling)) #files = glob('%s/%s/HGCalBufferModel_*.root' % (basedir, dirname))
    proc_files = glob('%s/HGCalBufferModel_processed_%s_buffer_*.root' %(basedir1,scaling)) #proc_files = glob('%s/%s/HGCalBufferModel_processed_*.root' % (basedir, dirname))
    #print(files)
    dat[dirname] = dict()
    #print(dirname, dat[dirname])
    x_vals = set()
    for fin in files: ## looping over input files and read histograms
        if 'proc' in fin:
            continue
            
        #print('in the file loop')    
        f = ROOT.TFile(fin)
        #print(fin)
        cfg = LoadConfig(f)
        x_val = cfg[index_on]
        #print 'x_val', x_val, index_on
        x_vals.add(x_val)
        if x_val not in dat[dirname]:
            dat[dirname][x_val] = dict()
        for h in read_hists:
            #print 'hist name ' , h
            if h not in dat[dirname][x_val]:
                dat[dirname][x_val][h] = f.Get(h)
                dat[dirname][x_val][h]._sumCount = 1
                dat[dirname][x_val][h]._subhists = [f.Get(h).Clone()]
            else:
                dat[dirname][x_val][h].Add(f.Get(h))
                dat[dirname][x_val][h]._sumCount += 1
                dat[dirname][x_val][h]._subhists.append(f.Get(h).Clone())


        f.Close()
    print(x_vals)
    # if dirname not in['triggerRate_default', 'triggerRate_avEvent', 'triggerRate_avEventRound', 'triggerRate_avEventRound_regTrig', 'randomSeed_default']:
    #     continue

    for fin in proc_files:
        f = ROOT.TFile(fin)
        #print(fin)
        cfg = LoadConfig(f)
        x_val = cfg[index_on]
        hist_pool = {h: f.Get(h) for h in read_hists_proc}
        n_events = float(hist_pool['nEvents'].GetBinContent(1))
        hist_pool['fracEcondTruncations'] = hist_pool['nEcondTruncations'].Clone()
        hist_pool['fracEcondTruncations'].Scale(1. / n_events)
        hist_pool['fracBETruncations'] = hist_pool['nBETruncations'].Clone()
        hist_pool['fracBETruncations'].Scale(1. / n_events)
        hist_pool['frac1DEcondTruncationsPerSlink'] = hist_pool['n1DEcondTruncationsPerSlink'].Clone()
        hist_pool['frac1DEcondTruncationsPerSlink'].Scale(1. / n_events)
        hist_pool['frac1DBETruncationsPerSlink'] = hist_pool['n1DBETruncationsPerSlink'].Clone()
        hist_pool['frac1DBETruncationsPerSlink'].Scale(1. / n_events)

        for h in read_hists_proc + ['fracEcondTruncations', 'fracBETruncations']:
            # if h == 'nEcondTruncations':
            #     print('Alpana',h, dirname, hist_pool[h].GetBinContent(1))
                #print(x_val, dat[dirname])
            if h not in dat[dirname][x_val]:
                #print('Alpana2',h, dirname, hist_pool[h].GetBinContent(1))
                dat[dirname][x_val][h] = hist_pool[h]
                dat[dirname][x_val][h]._sumCount = 1
                dat[dirname][x_val][h]._subhists = [hist_pool[h].Clone()]
            else:
                #print('Alpana1',h, dirname, hist_pool[h].GetBinContent(1))
                dat[dirname][x_val][h].Add(hist_pool[h])
                dat[dirname][x_val][h]._sumCount += 1
                dat[dirname][x_val][h]._subhists.append(hist_pool[h].Clone())
        f.Close()
## define 2D plots
#h2D_overAllThrottleVsTruncation = ROOT.TH2F("overAllThrottleVsTruncation",";Frac. of events with #geq1 ECON-D truncated;Frac. of time system is busy",500,0,1,500,0,1)
for dirname in dat:
    hist_name ='h2D_overAllThrottleVsTruncation_%s' %dirname
    h2D_overAllThrottleVsTruncation = ROOT.TH2F(hist_name,";Frac. of events with #geq1 ECON-D truncated;Frac. of time system is busy",500,0,1,500,0,1)
    for x_val in dat[dirname]:
        plots = dat[dirname][x_val]
        for p in ['FastControlDriver_Fractions',
                  'SlinkBuilder_AvgSlinkWordsFs',
                  'EcondAsic_AveragePacketWordsLuv',
                  'EcondAsic_NumberOfElinksLuv',
                  'EcondAsic_FractionTruncatedLuv',
                  'fracEcondTruncations',
                  'fracBETruncations',
                  'nEcondTruncPackets',
                  'nBETruncPackets',
                  'localL1A_all_2000',
                  'localL1A_econdtrunc_2000',
                  'localL1A_all_10000',
                  'localL1A_econdtrunc_10000'
                  ]:
            plots[p].Scale(1. / plots[p]._sumCount)
        for p in ['FastControlDriver_Fractions',
                'fracEcondTruncations',
                  'fracBETruncations',
                  'nEcondTruncPackets',
                  'nBETruncPackets',
                  'localL1A_all_2000',
                  'localL1A_econdtrunc_2000',
                  'localL1A_all_10000',
                  'localL1A_econdtrunc_10000'
                  ]:
            for ib in range(1, plots[p].GetNbinsX() + 1):
                vals = [h.GetBinContent(ib) for h in plots[p]._subhists]
                #print('Error test', h, vals, std(vals),sqrt(size(vals)))
                plots[p].SetBinError(ib, std(vals) / sqrt(size(vals)))                
                #if p == 'fracEcondTruncations':
                    
                #print('Alpana-3', dirname, x_val, p, ib, plots[p].GetBinContent(ib), vals, mean(vals), std(vals)/sqrt(size(vals)) )
        X_val_2D = [h.GetBinContent(1) for h in plots['fracEcondTruncations']._subhists]
        Y_val_2D = [h.GetBinContent(22) for h in plots['FastControlDriver_Fractions']._subhists]
        print(X_val_2D, Y_val_2D, "Alpana- 2D test",x_val,h2D_overAllThrottleVsTruncation)
        for i in range (0, len(X_val_2D)):
            h2D_overAllThrottleVsTruncation.Fill(X_val_2D[i],Y_val_2D[i])
    if hist_name not in dat[dirname]:
        dat[dirname][2550][hist_name]= h2D_overAllThrottleVsTruncation
    print('alps ' , dat[dirname][2550], dirname)
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


def AnalyseEcondTruncation(hists, check_luv=[]):
    found_luv = []
    h_packetwords = hists['EcondAsic_AveragePacketWordsLuv'].Clone()
    h_elinks = hists['EcondAsic_NumberOfElinksLuv'].Clone()
    h_truncfrac = hists['EcondAsic_FractionTruncatedLuv'].Clone()
    h_packetwords_per_elink = h_packetwords.Clone()
    h_packetwords_per_elink.Divide(h_elinks)
    n_econds = 0
    # Can also make a few useful plots here:
    # 1D Hist of words / elink - for all ECON-Ds, and for those that exhibit truncation
    h_words_per_elink_all = ROOT.TH1F('EcondAsic_WordsPerElink_All', '', 50, -0.5, 49.5)
    h_words_per_elink_trunc = ROOT.TH1F('EcondAsic_WordsPerElink_Truncated', '', 50, -0.5, 49.5)
    for ib in range(1, h_packetwords.GetNbinsX() + 1):
        luv = ib - 1
        elinks = h_elinks.GetBinContent(ib)
        packetwords_per_elink = h_packetwords_per_elink.GetBinContent(ib)
        truncfrac = h_truncfrac.GetBinContent(ib)
        if elinks > 0:
            n_econds += 1
            h_words_per_elink_all.Fill(packetwords_per_elink)
        if (packetwords_per_elink > 0 and truncfrac > 0) or luv in check_luv:
            h_words_per_elink_trunc.Fill(packetwords_per_elink)
            found_luv.append(luv)
            print('LUV = %i, elinks=%i, packetwords_per_elink = %f, truncfrac = %f' % (luv, elinks, packetwords_per_elink, truncfrac))
    print('Total ECON-Ds = %i' % n_econds)
    hists[h_words_per_elink_all.GetName()] = h_words_per_elink_all
    hists[h_words_per_elink_trunc.GetName()] = h_words_per_elink_trunc
    return found_luv

## removed by Alpana - testing the machinery
# print(dat)
# problem_luvs = AnalyseEcondTruncation(dat['triggerRate_default'][750])
# for input in [
#     ('triggerRate_default', 750),
#     ('triggerRate_optimizedMapping', 750)]:
#     print('AnalyseEcondTruncation: %s' % (input,))
#     AnalyseEcondTruncation(dat[input[0]][input[1]], check_luv=problem_luvs)


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
                      legend=(0.75, 0.70, 0.98, 0.92),
                      normalize=False,
                      ratio=False,
                      outdir='.',
                      style='HIST',
                      outname=None,
                      ytitle=None,
                      xtitle=None,
                      logy=None,
                      logx=False,
                      graphbin=None,
                      aggr=None,
                      xrange=None,
                      ymin=None,
                      graphbin_x=None,
                      markersize=0.9):
    if outname is None:
        outname = hist
    print('>>>>>>>>>>>> ' + outname)
    drawlist = []
    for packed in inputs:
        dirname = packed[0]
        overlay_points = packed[1]
        leg_label = packed[2]
        if len(packed) > 3:
            histname = packed[3]
        else:
            histname = hist
        x_vals = overlay_points
        if len(overlay_points) == 0:
            x_vals = sorted(dat[dirname].keys())
        x_vals1 = sorted(dat[dirname].keys())    
        print(x_vals1,x_vals)
        x_vals_roc = []
        print('xvals', x_vals)
        if graphbin is not None:
            y_vals = []
            y_vals_err = []
            x_vals_err=[]
            for x_val in x_vals:
                if histname not in dat[dirname][x_val]:
                    print('Warning, %s not in %s/%s' % (histname, dirname, x_val))
                if graphbin == 'mean':
                    y_vals.append(abs(dat[dirname][x_val][histname].GetMean()))
                else:
                    y_vals.append(abs(dat[dirname][x_val][histname].GetBinContent(graphbin)))
                    y_vals_err.append(abs(dat[dirname][x_val][histname].GetBinError(graphbin)))
                    #y_vals_err = [] ## remove it later
                if(max(y_vals)==0):
                    y_vals=[0.00001,0.0000,0.0000,0.00001,0.0000,0.00001,0.0000,0.0001]
                    y_vals_err=[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
                print('x_val',x_val)
                if graphbin_x is not None:
                    x_vals_roc.append(abs(dat[dirname][x_val][graphbin_x[0]].GetBinContent(graphbin_x[1])))  
                    x_vals_err.append(abs(dat[dirname][x_val][graphbin_x[0]].GetBinError(graphbin_x[1])))
                    
            if(max(x_vals_roc)==0):
                x_vals_roc=[0.00001,0.0000,0.0000,0.00001,0.0000,0.00001,0.0000,0.0001]
                x_vals_err=[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
            print(x_vals_roc)
            if graphbin_x is not None:
                x_vals = x_vals_roc
            if aggr is not None:
                drawlist.append(ROOT.TH1D(dirname, '', aggr[0], aggr[1], aggr[2]))
                for y_val in y_vals:
                    drawlist[-1].Fill(y_val)
            else:
                if len(y_vals_err) > 0:
                #     x_vals_err.append(dat[dirname][x_val][graphbin_x[0]].GetBinError(graphbin_x[1]))# = [0.] * len(x_vals)
                    if(max(x_vals)==0):
                        x_vals=[0.00001,0.0000,0.0000,0.00001,0.0000,0.00001,0.0000,0.00001]
                        x_vals_err=[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
                    print('x_va',x_vals)
                    drawlist.append(ROOT.TGraphErrors(len(x_vals), array('d', x_vals), array('d', y_vals), array('d', x_vals_err), array('d', y_vals_err)))
                else:
                    if(max(x_vals)==0):
                        x_vals=[0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001,0.00001]

                    drawlist.append(ROOT.TGraph(len(x_vals), array('d', x_vals), array('d', y_vals)))
                drawlist[-1].SetName(dirname)
                print ('Graph for ' + dirname)
                drawlist[-1].Print()
            drawlist[-1]._dirname = dirname
            drawlist[-1]._leg_text = leg_label
            #latex = ROOT.TLatex(axis.GetX()[5], axis.GetY()[5],"my annotation");
        else:
            for x_val in x_vals:
                if histname not in dat[dirname][x_val]:
                    print('Warning, %s not in %s/%s' % (histname, dirname, x_val))
                drawlist.append(dat[dirname][x_val][histname].Clone())
                if isinstance(drawlist[-1], ROOT.TH1I):
                    newhist = ROOT.TH1D()
                    drawlist[-1].Copy(newhist)
                    drawlist[-1] = newhist
                drawlist[-1]._dirname = dirname
                drawlist[-1]._x = x_val
                print leg_label, x_val
                drawlist[-1]._leg_text = '%s_%i' %(leg_label,x_val)
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
        
        for i in range(0,len(x_vals1)):
            print 'temp', x_vals1[i], x_vals1[i]/2600.0,100.0*(x_vals1[i]/2600.0) 
            latex = ROOT.TLatex(drawlist[0].GetX()[i], drawlist[0].GetY()[i]+0.015*drawlist[0].GetY()[i],"%i" %(100.0*(x_vals1[i]/2600.0))+"%")
            latex.SetTextSize(0.027); latex.SetTextColor(colors[0])
            latex.SetTextAlign(80)
            drawlist[0].GetListOfFunctions().Add(latex)
        for i in range(0,len(x_vals1)):
            latex = ROOT.TLatex(drawlist[1].GetX()[i], drawlist[1].GetY()[i]+0.015*drawlist[1].GetY()[i],"%i" %(100.0*(x_vals1[i]/2600.0))+"%")
            latex.SetTextSize(0.02); latex.SetTextColor(colors[0])
            latex.SetTextAlign(80)
            drawlist[1].GetListOfFunctions().Add(latex)
        for i in range(0,len(x_vals1)):
            latex = ROOT.TLatex(drawlist[2].GetX()[i], drawlist[2].GetY()[i]+0.015*drawlist[2].GetY()[i],"%i" %(100.0*(x_vals1[i]/2600.0))+"%")
            latex.SetTextSize(0.015); latex.SetTextColor(colors[0])
            latex.SetTextAlign(80)
            drawlist[2].GetListOfFunctions().Add(latex)
        
        drawlist[0].SetMarkerSize(8)
        drawlist[0].SetMarkerStyle(8)
        # drawlist[0].GetXaxis().SetRangeUser(0,0.12);
        # drawlist[1].GetXaxis().SetRangeUser(0,0.12);
        # drawlist[2].GetXaxis().SetRangeUser(0,0.12);
        # for i in range(0,len(drawlist)):
        #     if(drawlist[i].GetXaxis().GetMaximum()==0.0):
        #         print('x_va',x_vals)
        #         drawlist[i].GetXaxis().SetMaximum(0.00001)
            
        drawlist[1].SetMarkerSize(8)
        drawlist[1].SetMarkerStyle(8)
        drawlist[2].SetMarkerSize(8)
        drawlist[2].SetMarkerStyle(8)
        drawlist[0].SetLineColor(colors[1])
        drawlist[1].SetLineColor(colors[2])
        drawlist[2].SetLineColor(colors[3])
        drawlist[0].SetMarkerColor(colors[1])
        drawlist[1].SetMarkerColor(colors[2])
        drawlist[2].SetMarkerColor(colors[3])
        
        #drawlist[0].SetMarkerSize(8)
        
    else:
        axis = drawlist[0].Clone()
    drawlist[0].GetXaxis().SetRangeUser(0,0.12);
    drawlist[1].GetXaxis().SetRangeUser(0,0.12);
    drawlist[2].GetXaxis().SetRangeUser(0,0.12);
    drawlist[1].SetMarkerSize(8)
    drawlist[1].SetMarkerStyle(8)
    drawlist[2].SetMarkerSize(8)
    drawlist[2].SetMarkerStyle(8)
    drawlist[0].SetLineColor(colors[1])
    drawlist[1].SetLineColor(colors[2])
    drawlist[2].SetLineColor(colors[3])
    drawlist[0].SetMarkerColor(colors[1])
    drawlist[1].SetMarkerColor(colors[2])
    drawlist[2].SetMarkerColor(colors[3])
    drawlist[0].GetXaxis().SetTitleSize(0.8)
    drawlist[0].GetYaxis().SetTitleSize(0.8)
    drawlist[1].GetXaxis().SetTitleSize(0.8)
    drawlist[1].GetYaxis().SetTitleSize(0.8)
    drawlist[2].GetXaxis().SetTitleSize(0.8)
    drawlist[2].GetYaxis().SetTitleSize(0.8)
    
    pads[0].cd()
    #axis.Reset()
    #axis.SetMaximum(0.4)
    # axis.SetMinimum(0.1)
    #axis.GetXaxis().SetRangeUser(0,0.12)
    # if xrange is not None:
    #     axis.GetXaxis().SetRangeUser(xrange[0], xrange[1])
    # if logy is not None:
    #     pads[0].SetLogy(True)
    #     axis.SetMinimum(logy)
    # if logx:
    #     pads[0].SetLogx(True)
    # if ymin is not None:
    #     axis.SetMaximum(0.03)#Minimum(ymin)
    #plot.Set(axis.GetYaxis(), TitleOffset=axis.GetYaxis().GetTitleOffset() * (1. - ((wh[0] - wh[1])*0.8)/wh[0]))
    
    # axis_ratio = axis.Clone()
    # plot.Set(axis, Minimum=0.0, Maximum=0.003)
    # if ytitle is not None:
    #     plot.Set(axis.GetYaxis(), Title=ytitle)
    # if xtitle is not None:
    #     plot.Set(axis.GetXaxis(), Title=xtitle)
    # if isinstance(drawlist[0], ROOT.TGraph):     
    #     latex = ROOT.TLatex(axis.GetX()[5], axis.GetY()[5],"my annotation"); axis.GetListOfFunctions().Add(latex);
    #     latex.SetTextSize(0.07); latex.SetTextColor(kRed);
    # pads[0].SetLogy(True)
    pads[0].SetGrid(grid[0], grid[1])
    #axis.Draw("ALP")
    mg = ROOT.TMultiGraph()
    axis51=  mg.GetXaxis();
    # axis51.SetRangeUser(0,0.12);
    # mg.GetXaxis().SetRangeUser(0,0.12)
    # gPad.Modified()
    # gPad.Update()
    # mg.SetMaximum(0.3)
    # mg.SetMinimum(0.0)
    mg.Add(drawlist[0])
    mg.Add(drawlist[1])
    mg.Add(drawlist[2])
    mg.Add(drawlist[3])
    mg.Add(drawlist[4])
    # mg.Add(drawlist[5])
    # mg.Add(drawlist[6])

    #plot.Set(mg.GetYaxis(), TitleOffset=mg.GetYaxis().GetTitleOffset() * (1. - ((wh[0] - wh[1])*0.8)/wh[0]))
    #axis_ratio = mg.Clone()
    #plot.Set(mg, Minimum=0.0, Maximum=0.003)
    if ytitle is not None:
        plot.Set(mg.GetYaxis(), Title=ytitle)
    if xtitle is not None:
        plot.Set(mg.GetXaxis(), Title=xtitle)
    # axis1 = plot.CreateAxisHist(mg, False)
    # axis1.GetXaxis().SetRangeUser(0,0.1)
    #mg.GetXaxis().SetRangeUser(0,0.12)
    #mg.SetMaximum(0.3)
    pads[0].Modified()
    pads[0].Update()
    axis51=  mg.GetXaxis();
    #axis51.SetRangeUser(0,0.12);
    mg.GetXaxis().SetTitleSize(0.8)
    mg.GetYaxis().SetTitleSize(0.8)

    mg.Draw("ALP")
#    mg.GetHistogram().SetMaxmimum(0.3)#GetYaxis().SetRangeUser(0.,0.3)
    #mg.GetYaxis().SetRangeUser(0,0.3)
    # mg.SetMinimum(0.0001)
    # mg.SetMaximum(0.3)
    pads[0].Modified()
    pads[0].Update()

    # drawlist[0].Draw("ALP")
    # drawlist[1].Draw("ALPsame")
    # drawlist[2].Draw("ALPsame")
    # axis.SetMaximum(0.3)
    # axis.SetMinimum(0.1)
    legend= ROOT.TLegend(0.45, 0.70, 0.96, 0.92 , '', 'NBNDC')
    legend.SetTextSize(0.04)
    legend.SetHeader("Default (750 kHz)");
    #legend.SetLineColor();
    
    #legend[0], legend[1],
    #                          legend[2], legend[3], '', 'NBNDC') 
    rlist = []
    print('dash ', colors)
    i=0
    for h in drawlist:
        i+=1
        col = colors[i]#.pop(0)
        plot.Set(h, LineWidth=2, LineColor=col,
                 MarkerColor=col, MarkerSize=markersize)
        if not graphbin:
            rlist.append(plot.MakeRatioHist(h, drawlist[0], False, False))
        h.Draw(style+'SAME')
        l1 = legend.AddEntry(h, h._leg_text, 'P' if 'P' in style else 'L')
        l1.SetTextColor(col);
        legend.SetTextSize(0.03)
    if ratio:
        pads[1].cd()
        axis_ratio.Draw()
        for h in rlist:
            h.Draw(style+'SAME')
        pads[1].SetGrid(0, 1)
        plot.SetupTwoPadSplitAsRatio(
            pads, axis, axis_ratio, '', True, 0.0, 2.8)
    pads[0].cd()
    #plot.FixTopRange(pads[0], plot.GetPadYMax(pads[0]), 0.25)
    legend.Draw()
    for fmt in ['.pdf', '.png', '.svg', '.root']:
        canv.Print('%s/%s%s' % (outdir, canv.GetName(), fmt))


leg_top_left = (0.17, 0.70, 0.50, 0.92)

def Plot2D(dat, input=('triggerRate_default', 750), hist='h2D_overAllThrottleVsTruncation', outname='bus', titles=('x', 'y', 'z')):
    plot.ModTDRStyle(width=900, height=800, l = 0.15, b = 0.13, r = 0.23, t=0.08)
    i=0
    ROOT.gStyle.SetPalette(103)
    ROOT.TColor.InvertPalette()
    canv = ROOT.TCanvas(outname)
    pads = plot.OnePad()
    for packed in input:
        dirname = packed[0]        
        hists = '%s_%s' %(hist,packed[0])
        #print(dat[packed[0]][packed[1]].keys())
        h = dat[packed[0]][packed[1]][hists].Clone()
        N = h.GetNbinsX()
        for ix in range(N):
            ref_val = h.GetBinContent(ix + 1, N - ix)
        for iy in range(N):
            if ref_val > 0:
                h.SetBinContent(ix + 1, iy + 1, h.GetBinContent(ix + 1, iy + 1) / ref_val)
        h.SetContour(255)
        h.GetXaxis().SetRangeUser(0,0.3)
        h.GetYaxis().SetRangeUser(0,0.3)
        # ROOT.gStyle.SetPalette(103)
        # ROOT.TColor.InvertPalette()
        # canv = ROOT.TCanvas(outname)
        # pads = plot.OnePad()
        #canv.cd()
        pads[0].cd()
        plot.Set(h.GetXaxis(), Title=titles[0])
        plot.Set(h.GetYaxis(), Title=titles[1])
        plot.Set(h.GetYaxis(), TitleOffset=h.GetYaxis().GetTitleOffset() * 0.7)
        plot.Set(h.GetZaxis(), Title=titles[2])
        # if zmax is not None:
        #     hist.SetMaximum(zmax)
        # if zmin is not None:
        #     hist.SetMinimum(zmin)
        print(i)
        if(i==0):
            h.Draw('COLZ')
        else:
            h.Draw('COLZ SAMES')
        i+=1
        #pads[0].GetFrame().Draw()
    for fmt in ['.pdf', '.png', '.svg']:
        canv.Print('%s/%s%s' % (outdir, outname, fmt))


Plot2D(dat, input=[
    ('econdHitScaling_1400_default_V10_pseudo', 2550),
    ('econdHitScaling_1600_default_V10_pseudo', 2550),
    ],
       hist='h2D_overAllThrottleVsTruncation',
       outname='n2DEcondTruncationsPerSlink_default_850',
       titles=('Frac. of events with #geq1 ECON-D truncated','Frac. of time system is busy', ''))

# AnalyseFastControlCommands(dat, inputs=[
#     ('triggerRate_default', [750], 'Default, %s kHz'),
#     ('triggerRate_regTrig', [755], 'regTrig, %s kHz'),
#     ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
#     ('triggerRate_avEventRound_regTrig', [755], 'regTrig & avEventRound, %s kHz'),
# ], outdir=outdir)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'Default, %s kHz'),
#     ('triggerRate_optimizedMapping', [750], 'Optimized, %s kHz')
# ], hist='SlinkBuilder_AvgSlinkWordsFs', ratio=False, wh=(1200, 800), style='HISTP', outdir=outdir, markersize=0.5)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'Default, %s kHz'),
#     ('triggerRate_optimizedMapping', [750], 'Optimized, %s kHz')
# ], hist='EcondAsic_NumberOfElinksLuv', ratio=True, wh=(1200, 800), style='HISTP', outdir=outdir, markersize=0.5)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'All ECON-Ds (%s kHz) - V9', 'EcondAsic_WordsPerElink_All'),
#     ('triggerRate_default', [750], 'Trunc. ECON-Ds (%s kHz) - V9', 'EcondAsic_WordsPerElink_Truncated'),
#     ('triggerRate_optimizedMapping', [750], 'All ECON-Ds (%s kHz) - V8', 'EcondAsic_WordsPerElink_All'),
# ], hist='EcondAsic_WordsPerElink', ratio=False, wh=(1200, 800), style='HIST', outdir=outdir, markersize=0.5, logy=0.5,
#     xtitle='Av. packet words / elink', ytitle='Number of ECON-Ds')


# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'All events (%s kHz)', 'localL1A_all_2000'),
#     ('triggerRate_default', [750], 'Trunc. events (%s kHz)', 'localL1A_econdtrunc_2000'),
# ], hist='localL1A_2000', ratio=False, wh=(1200, 800), style='HIST', outdir=outdir, markersize=0.5, logy=0.5,
#     xtitle='Local L1A rate (prev. 2000 BX)', ytitle='Events')

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'All events (%s kHz)', 'localL1A_all_10000'),
#     ('triggerRate_default', [750], 'Trunc. events (%s kHz)', 'localL1A_econdtrunc_10000'),
# ], hist='localL1A_10000', ratio=False, wh=(1200, 800), style='HIST', outdir=outdir, markersize=0.5, logy=0.5,
#     xtitle='Local L1A rate (prev. 10000 BX)', ytitle='Events')

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'Default, %s kHz'),
#     # ('triggerRate_regTrig', [755], 'regTrig, %s kHz'),
#     ('triggerRate_avEvent', [750], 'avEventRoundUp, %s kHz'),
#     ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
#     ('triggerRate_avEventPacket', [750], 'avEventPacket, %s kHz'),
# ], hist='EcondAsic_AveragePacketWordsLuv', ratio=True, wh=(1200, 800), style='P', outdir=outdir, markersize=0.2)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'Default, %s kHz'),
#     ('triggerRate_regTrig', [755], 'regTrig, %s kHz'),
#     ('triggerRate_avEventRound', [750], 'avEventRound, %s kHz'),
#     ('triggerRate_avEventRound_regTrig', [755], 'regTrig & avEventRound, %s kHz'),
# ], hist='EcondAsic_FractionTruncated', wh=(800, 800), normalize=True, legend=leg_top_left, outdir=outdir)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [750], 'Default, %s kHz'),
#     ('triggerRate_avEventPacket', [750], 'avEvent, %s kHz'),
# ], hist='EventSize', wh=(800, 800), normalize=True, legend=leg_top_left, outdir=outdir)


# GenericOverlayTH1(dat, inputs=[
#     ('randomSeed_default', [1], 'Default (%s)'),
# ], hist='frequencyOfEcondTruncationsPerSlinkPerRun', outname='frequencyOfEcondTruncationsPerSlinkPerRun', xtitle='slink index', ytitle='Number of emulator runs w/ trunc.',
#     ratio=False, wh=(1200, 800), style='HIST', outdir=outdir)
# dat['randomSeed_default'][1]['frequencyOfEcondTruncationsPerSlinkPerRun'].Print('range')

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
# ], hist='frac1DEcondTruncationsPerSlink', outname='frac1DEcondTruncationsPerSlink_vs_triggerRate', xtitle='slink index', ytitle='Frac. of events with ECON-D trunc.',
#     ratio=False, wh=(1200, 800), style='P', logy=1E-4, outdir=outdir)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
# ], hist='frac1DBETruncationsPerSlink', outname='frac1DBETruncationsPerSlink_vs_triggerRate', xtitle='slink index', ytitle='Frac. of events with BE packet loss',
#     ratio=False, wh=(1200, 800), style='P', logy=1E-4, outdir=outdir)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
# ], hist='EcondAsic_FractionTruncatedLuv', outname='EcondAsic_FractionTruncatedLuv_vs_triggerRate',
#     ratio=False, wh=(1200, 800), style='P', logy=1E-4, outdir=outdir)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [700, 725, 750, 775, 800], 'Default, %s kHz'),
# ], hist='LpgbtPairEcondBuilder_DiscardedPacketsFractionLuv', outname='LpgbtPairEcondBuilder_DiscardedPacketsFractionLuv_vs_triggerRate',
#     ratio=True, wh=(1200, 800), style='P', outdir=outdir)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     ('triggerRate_avEvent', [], 'avEventRoundUp'),
#     ('triggerRate_avEventRound', [], 'avEventRound'),
#     ('triggerRate_regTrig', [], 'regTrigger'),
#     ('triggerRate_avEventRound_regTrig', [], 'avEventRound+regTrigger'),
# ], hist='FastControlDriver_Fractions', outname='HgcrocTcdsThrottle_vs_triggerRate',
#     graphbin=20, style='PL', outdir=outdir, ytitle='HgcrocTcdsThrottle fraction', xtitle='Trigger rate (kHz)', legend=leg_top_left)

if 'triggerRate_default_V10_pseudo1' in do_plots:
    GenericOverlayTH1(dat, inputs=[
        # ('triggerRate_default_700_V10_pseudo', [], 'Trigger rate (kHz): 700'),                                                                                            
        # ('triggerRate_default_750_V10_pseudo', [], 'Trigger rate (kHz): 750'),                                                                                           
        ('triggerRate_default_800_V10_pseudo', [], 'Trigger rate (kHz): 800'),
        ('triggerRate_default_850_V10_pseudo', [], 'Trigger rate (kHz): 850'),
        ('triggerRate_default_900_V10_pseudo', [], 'Trigger rate (kHz): 900'),
        ('triggerRate_default_950_V10_pseudo', [], 'Trigger rate (kHz): 950'),
    ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='Frac. of time system is busy', xtitle='Frac. of events with #geq1 ECON-D truncated', legend=leg_top_left)

if 'econdHitScaling_default_V10_pseudo' in do_plots:
    GenericOverlayTH1(dat, inputs=[
        
        ('econdHitScaling_1600_default_V10_pseudo',[],'Data amount scale factor: 1.6') ,
        ('econdHitScaling_1500_default_V10_pseudo',[],'Data amount scale factor: 1.5') ,
        ('econdHitScaling_1400_default_V10_pseudo',[],'Data amount scale factor: 1.4') ,
        ('econdHitScaling_1300_default_V10_pseudo',[],'Data amount scale factor: 1.3') ,
        ('econdHitScaling_1200_default_V10_pseudo',[],'Data amount scale factor: 1.2') ,
        # ('econdHitScaling_1100_default_V10_pseudo',[],'Data amount scale factor: 1.1') ,
        ('econdHitScaling_1000_default_V10_pseudo',[],'Data amount scale factor: 1.0') ,
    ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='Frac. of time system is busy', xtitle='Frac. of events with #geq1 ECON-D truncated', legend=leg_top_left)
    #print(dat[econdbufferlevel_default_V10])
# GenericOverlayTH1(dat, inputs=[
# ('econdHitScaling_default_V10_pseudo',[], 'Scale Factor (F): 1.7'),
# ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
# ('econdHitScaling_default_V10',[], 'Default'),
# ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
# ('triggerRate_default_V10_pseudo',[], 'Default'),
# ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)
#triggerRate_avEventPacket_regTrig_V10_pseudo
# GenericOverlayTH1(dat, inputs=[                                                                                                                                     
# ('triggerRate_regTrig_V10_pseudo',[], 'Default'),                                                                                                                  ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)                      
# GenericOverlayTH1(dat, inputs=[
# ('triggerRate_avEventPacket_regTrig_V10_pseudo',[], 'Default'),
# ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)
# GenericOverlayTH1(dat, inputs=[
# ('triggerRate_avEventPacket_V10_pseudo',[], 'Default'),
# ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
# ('triggerRate_default_V10',[], 'Default'),
# ],hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4],graphbin=22,graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)


# GenericOverlayTH1(dat, inputs=[
#      ('econdbufferlevel_default_V10', [], 'Default (750 kHz)'),
# ], hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_750_%s'%sys.argv[4], xrange= [0.0003,0.003],ymin = 0.003,graphbin=22, graphbin_x=('fracEcondTruncations', 1), style='PL',outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left) 

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
# ], hist='nEcondTruncPackets', outname='NEconds_WithTruncations_750_%s'%sys.argv[4], xtitle='N-ECON-Ds with truncations',legend=leg_top_left,outdir=outdir,ytitle='no. of events')
# , xrange= [0.0003,0.003],ymin = 0.003,
#                   graphbin=22, graphbin_x=('fracEcondTruncations', 1), style='PL', outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('econdbufferlevel_triggerRate_780', [], 'Default (780 kHz)'),
#     ('econdbufferlevel_triggerRate_780_econdUpdateIdleStatus', [], 'Default (780 kHz) - econdUpdateIdleStatus'),
# ], hist='FastControlDriver_Fractions', outname='ROC_EcondAsicThrottle_vs_fracEcondTruncations_780',
#     graphbin=22, graphbin_x=('fracEcondTruncations', 1), style='PL', outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Frac. of events with ECON-D trunc.', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
# ], hist='FastControlDriver_Fractions', outname='EcondAsicThrottle_vs_triggerrate_%s'%sys.argv[4],
#     graphbin=22, style='PL', outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='Trigger rate', legend=leg_top_left, ymin = 0.003)



# GenericOverlayTH1(dat, inputs=[
#     ('econdbufferlevel_default_V10', [], 'Default'),
# ], hist='FastControlDriver_Fractions', outname='EcondAsicThrottle_vs_econdbufferlevel_%s'%sys.argv[4],
#     graphbin=22, style='PL', outdir=outdir, ytitle='EcondAsicThrottle fraction', xtitle='ECON-D medium buffer level', legend=leg_top_left, ymin = 0.003)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     # ('triggerRate_avEvent', [], 'avEventRoundUp'),
#     # ('triggerRate_avEventRound', [], 'avEventRound'),
#     ('triggerRate_avEventPacket', [], 'avEvent'),
#     ('triggerRate_regTrig', [], 'regTrigger'),
#     # ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
#     ('triggerRate_avEventPacket_regTrig', [], 'avEvent+regTrigger'),
#     ('triggerRate_optimizedMapping', [], 'Optimized (V8)'),
# ], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerRate',
#     graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate (kHz)', legend=leg_top_left, ymin=0)

# GenericOverlayTH1(dat, inputs=[
#     ('econdbufferlevel_default_V10', [], 'Default'),
# ], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_econdbufferlevel_%s'%sys.argv[4],
#                   graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='ECON-D Medium level', legend=leg_top_left, ymin = 0.003)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
# ], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerrate_%s'%sys.argv[4],
#     graphbin=22, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate', legend=leg_top_left, ymin = 0.003)


# GenericOverlayTH1(dat, inputs=[
#     ('econdbufferlevel_default_V10', [], 'Default'),
# ], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_econdbufferlevel',
#     graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='ECON-D Medium level', legend=leg_top_left, ymin=0)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     # ('triggerRate_avEvent', [], 'avEventRoundUp'),
#     ('triggerRate_avEventRound', [], 'avEventRound'),
#     ('triggerRate_avEventPacket', [], 'avEventPacket'),
# ], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerRate_forAvEvent',
#     graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate (kHz)', legend=leg_top_left, logy=1E-4)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     ('triggerRate_regTrig', [], 'regTrigger'),
#     ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
# ], hist='fracEcondTruncations', outname='fracEcondTruncations_vs_triggerRate_forRegTrig',
#     graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with ECON-D trunc.', xtitle='Trigger rate (kHz)', legend=leg_top_left, logy=1E-4)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     # ('triggerRate_avEvent', [], 'avEventRoundUp'),
#     # ('triggerRate_avEventRound', [], 'avEventRound'),
#     ('triggerRate_avEventPacket', [], 'avEvent'),
#     ('triggerRate_regTrig', [], 'regTrigger'),
#     # ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
#     ('triggerRate_avEventPacket_regTrig', [], 'avEvent+regTrigger'),
#     ('triggerRate_optimizedMapping', [], 'Optimized (V8)'),
# ], hist='fracBETruncations', outname='fracBETruncations_vs_triggerRate',
#     graphbin=1, style='PL', outdir=outdir, ytitle='Frac. of events with BE packet loss', xtitle='Trigger rate (kHz)', legend=leg_top_left)


# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     # ('triggerRate_avEvent', [], 'avEventRoundUp'),
#     # ('triggerRate_avEventRound', [], 'avEventRound'),
#     ('triggerRate_avEventPacket', [], 'avEvent'),
#     ('triggerRate_regTrig', [], 'regTrigger'),
#     # ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
#     ('triggerRate_avEventPacket_regTrig', [], 'avEvent+regTrigger'),
#     ('triggerRate_optimizedMapping', [], 'Optimized (V8)'),
# ], hist='nEcondTruncPackets', outname='avEcondTruncPackets_vs_triggerRate',
#     graphbin='mean', style='PL', outdir=outdir, ytitle='Average ECON-D trunc. per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [725, 750, 775], 'Default'),
#     ('triggerRate_avEventRound', [725, 750, 775], 'avEventRound'),
#     ('triggerRate_regTrig', [727, 755, 769], 'regTrigger'),
#     ('triggerRate_avEventRound_regTrig', [727, 755, 769], 'avEventRound+regTrigger'),
# ], hist='nEcondTruncPackets', outname='avEcondTruncPackets_vs_triggerRate_zoom',
#     graphbin='mean', style='PL', outdir=outdir, ytitle='Average ECON-D trunc. per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [], 'Default'),
#     # ('triggerRate_avEvent', [], 'avEventRoundUp'),
#     # ('triggerRate_avEventRound', [], 'avEventRound'),
#     ('triggerRate_avEventPacket', [], 'avEvent'),
#     ('triggerRate_regTrig', [], 'regTrigger'),
#     # ('triggerRate_regTrigGaps', [], 'regTriggerGaps'),
#     ('triggerRate_avEventPacket_regTrig', [], 'avEvent+regTrigger'),
#     ('triggerRate_optimizedMapping', [], 'Optimized (V8)'),
# ], hist='nBETruncPackets', outname='avnBETruncPackets_vs_triggerRate',
#     graphbin='mean', style='PL', outdir=outdir, ytitle='Average BE packet loss per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('triggerRate_default', [725, 750, 775], 'Default'),
#     ('triggerRate_avEventRound', [725, 750, 775], 'avEventRound'),
#     ('triggerRate_regTrig', [727, 755, 769], 'regTrigger'),
#     ('triggerRate_avEventRound_regTrig', [727, 755, 769], 'avEventRound+regTrigger'),
# ], hist='nBETruncPackets', outname='avnBETruncPackets_vs_triggerRate_zoom',
#     graphbin='mean', style='PL', outdir=outdir, ytitle='Average BE packet loss per event', xtitle='Trigger rate (kHz)', legend=leg_top_left)


# GenericOverlayTH1(dat, inputs=[
#     ('randomSeed_default', [], 'randomSeed (750 kHz)'),
# ], hist='fracEcondTruncations', outname='dist_fracEcondTruncations_randomSeed', aggr=(50,0,0.02),
#     graphbin=1, style='HIST', outdir=outdir, xtitle='Frac. of events with ECON-D trunc.', ytitle='Number of emulator runs', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('randomSeed_default', [], 'randomSeed (750 kHz)'),
# ], hist='fracBETruncations', outname='dist_fracBETruncations_randomSeed', aggr=(50,0,0.02),
#     graphbin=1, style='HIST', outdir=outdir, xtitle='Frac. of events with BE packet loss', ytitle='Number of emulator runs', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('randomSeed_default', [], 'randomSeed (750 kHz)'),
# ], hist='nEcondTruncPackets', outname='dist_meanEcondTruncPackets_randomSeed', aggr=(50,0,0.02),
#     graphbin='mean', style='HIST', outdir=outdir, xtitle='Average ECON-Ds trunc.', ytitle='Number of emulator runs', legend=leg_top_left)

# GenericOverlayTH1(dat, inputs=[
#     ('randomSeed_default', [], 'randomSeed (750 kHz)'),
# ], hist='nBETruncPackets', outname='dist_meanBETruncPackets_randomSeed', aggr=(50,0,0.02),
#     graphbin='mean', style='HIST', outdir=outdir, xtitle='Average packets lost in BE', ytitle='Number of emulator runs', legend=leg_top_left)

# for i in dat['randomSeed_default']:
#     print('>> Random seed %i' % i)
#     h = dat['randomSeed_default'][i]['nEcondTruncPackets']
#     print(h.GetMean())
#     if h.GetMean() > 0.01:
#         # h.Print('range')
#         GenericOverlayTH1(dat, inputs=[
#             ('randomSeed_default', [i], 'Random seed %s')], outname='nEcondTruncPackets_%i' % i,
#             hist='nEcondTruncPackets', ratio=False, style='HIST', outdir=outdir, xrange=(-0.5,19.5), logy=0.1, xtitle='Number of trunc. ECON-D packets per event', ytitle='Events')
