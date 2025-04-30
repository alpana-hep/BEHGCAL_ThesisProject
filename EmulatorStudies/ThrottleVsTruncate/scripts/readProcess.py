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
def TFileIsGood(filename):
    """Performs a series of tests on a TFile to ensure that it can be opened
    without errors

    Args:
        filename: `str` The name of the TFile to check

    Returns:
        `bool` True if the file can opened, is not a zombie, and if ROOT did
        not need to try and recover the contents
    """
    fin = ROOT.TFile(filename)
    if not fin:
        return False
    if fin and not fin.IsOpen():
        return False
    elif fin and fin.IsOpen() and fin.IsZombie():
        fin.Close()
        return False
    elif fin and fin.IsOpen() and fin.TestBit(ROOT.TFile.kRecovered):
        fin.Close()
        # don't consider a recovered file to be OK
        return False
    else:
        fin.Close()
        return True



# basedir = sys.argv[1] #'/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/'

# outdir = sys.argv[2] #'/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/ProcessedFiles'
#os.system('mkdir -p %s' % outdir)
dat = {}

scans = [#"triggerRate_regTrig_V10_pseudo",
    #"triggerRate_default_V10_pseudo",
    "econdHitScaling_Additive_fracNcells_V10_pseudo",
    #"temp",
    "econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo",    
    #"econdHitScaling_regTrig_V10_pseudo",
    # "econdHitScaling_default_V10",
# "econdHitScaling_15FPGA_V10_pseudo",                                                                                                                              
# "econdHitScaling_14FPGA_V10_pseudo",                                                                                                                               
# "triggerRate_default_V10_pseudo",
# "triggerRate_default_V10",
# "triggerRate_avEventPacket_V10_pseudo",
# "triggerRate_avEventPacket_V10",
# "triggerRate_regTrig_V10_pseudo",
# "triggerRate_regTrig_V10",
# "triggerRate_avEventPacket_regTrig_V10_pseudo",
# "triggerRate_avEventPacket_regTrig_V10",
    # ('triggerRate_default', 'triggerRate', 'Default'),
    # ('triggerRate_regTrig', 'triggerRate', 'regTrig'),
    # ('triggerRate_regTrigGaps', 'triggerRate', 'regTrigGaps'),
    # ('triggerRate_avEvent', 'triggerRate', 'avEventRoundUp'),
    # ('triggerRate_avEventRound', 'triggerRate', 'avEventRound'),
    # ('triggerRate_avEventPacket', 'triggerRate', 'avEventPacket'),
    # ('randomSeed_default', 'randomSeed', 'Default'),
    # ('triggerRate_avEventRound_regTrig', 'triggerRate', 'regTrig&avEventRound'),
    # ('test', 'triggerRate', 'test'),
]

# for dirname, index_on, label in scans:
#     print basedir, dirname
files_in =[] # glob('%s/HGCalBufferModel_*.root' %basedir)
files_out =[]
#proc_files = glob('%s/%s/HGCalBufferModel_*_proc.root' % (basedir, dirname))
startPoints = [i * 10000 for i in range(10)]
# for level in range(1000, 1701, 100):
#     for irng, startPoint in enumerate(startPoints):
#         basename = 'HGCalBufferModel_pseudo_%i_job_%i' % (level, irng + 1)
#         basename2 = 'HGCalBufferModel_processed_%i_job_%i' % (level, irng + 1)
#         print basename
#         print basename2
#         outputname = '%s/%s.root' %(basedir, basename)
#         outputname2 = '%s/%s.root' %(outdir,basename2)
#         files_in.append(outputname)
#         files_out.append(outputname2)
# if 'temp' in scans:
#     basedir= '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/D86/'
#     outdir= '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/D86/ProcessedFiles'
#     os.system('mkdir -p %s' % outdir)
    
if 'econdHitScaling_Additive_fracNcells_V10_pseudo' in scans:
    basedir= '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_Additive_fracNcells_V10_pseudo/'
    outdir= '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_Additive_fracNcells_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    scale_num=[1,2,3]
    add = [100,200,400,600]
    for i in range(len(scale_num)):
        for level in range(len(add)): #100, 1100, 100):
            for bufflevel in range(1850, 2600, 100):
                for irng, startPoint in enumerate(startPoints):
                    basename = 'HGCalBufferModel_case%i_add_%i_buffer_%i_%i_job_%i' % (scale_num[i],add[level], bufflevel, bufflevel-400, irng + 1)
                    basename2 = 'HGCalBufferModel_processed_case%i_add_%i_buffer_%i_%i_job_%i' % (scale_num[i],add[level], bufflevel, bufflevel-400, irng + 1)
                    print basename
                    print basename2
                    outputname = '%s/%s.root' %(basedir, basename)
                    outputname2 = '%s/%s.root' %(outdir,basename2)
                    files_in.append(outputname)
                    files_out.append(outputname2)


if 'econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo/'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    scale_num=[1,2,3]
    add = [100,200,400,600]

    for i in range(len(scale_num)):
        for level in range(len(add)): #100, 1100, 100):
            for bufflevel in range(1850, 2600, 100):
                for irng, startPoint in enumerate(startPoints):
                    actualrate = int(round(float(40000) / 53))
                    basename = 'HGCalBufferModel_case%i_add_%i_buffer_%i_%i_job_%i' % (scale_num[i],add[level], bufflevel, bufflevel-400, irng + 1)
                    basename2 = 'HGCalBufferModel_processed_case%i_add_%i_buffer_%i_%i_job_%i' % (scale_num[i],add[level], bufflevel, bufflevel-400, irng + 1)
                    print basename
                    print basename2
                    outputname = '%s/%s.root' %(basedir, basename)
                    outputname2 = '%s/%s.root' %(outdir,basename2)
                    files_in.append(outputname)
                    files_out.append(outputname2)

if 'econdHitScaling_default_V10_pseudo' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_default_V10_pseudo'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_default_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for level in range(1000, 1801, 100):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'econdHitScaling_regTrig_V10_pseudo' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_regTrig_V10_pseudo'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_regTrig_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for level in range(1000, 1801, 100):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)


if 'econdHitScaling_default_V10' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_default_V10'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_default_V10/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for level in range(1000, 1801, 100):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_default_V10_pseudo' in scans:
    basedir =  '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_default_V10_pseudo'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_default_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    # Do scans of L1 rate                                                                                                                                            
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_default_V10' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_default_V10'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_default_V10/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    # Do scans of L1 rate                                                                                                                                            
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                basename2 = 'HGCalBufferModel_processed_ %i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_avEventPacket_V10_pseudo' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_V10_pseudo'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_avEventPacket_V10' in scans:
    basedir= '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_V10/'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_V10/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_regTrig_V10_pseudo' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_regTrig_V10_pseudo'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_regTrig_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate, bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (actualrate, bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_regTrig_V10' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_regTrig_V10'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_regTrig_V10/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate, bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (actualrate, bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

if 'triggerRate_avEventPacket_regTrig_V10_pseudo' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_regTrig_V10_pseudo'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_regTrig_V10_pseudo/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)
    
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate,bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (actualrate,bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)
if 'triggerRate_avEventPacket_regTrig_V10' in scans:
    basedir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_regTrig_V10'
    outdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_regTrig_V10/ProcessedFiles'
    os.system('mkdir -p %s' % outdir)

    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate,bufflevel, bufflevel-400, irng + 1)
                basename2 = 'HGCalBufferModel_processed_%i_buffer_%i_%i_job_%i' % (actualrate,bufflevel, bufflevel-400, irng + 1)
                print basename
                print basename2
                outputname = '%s/%s.root' %(basedir, basename)
                outputname2 = '%s/%s.root' %(outdir,basename2)
                files_in.append(outputname)
                files_out.append(outputname2)

# if 'triggerRate_default_V10_pseudo' in scans:
#     for level in range(600,1000,50):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             basename2 = 'HGCalBufferModel_processed_%i_job_%i' % (level, irng + 1)
#             print basename
#             print basename2
#             outputname = '%s/%s.root' %(basedir, basename)
#             outputname2 = '%s/%s.root' %(outdir,basename2)
#             files_in.append(outputname)
#             files_out.append(outputname2)

# if 'triggerRate_regTrig_V10_pseudo' in scans:
#     for level in range(40, 66, 2):
#         nnarg = level
#         actualrate = int(round(float(40000) / nnarg))        
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (actualrate, irng + 1)
#             basename2 = 'HGCalBufferModel_processed_%i_job_%i' % (actualrate, irng + 1)
#             print basename
#             print basename2
#             outputname = '%s/%s.root' %(basedir, basename)
#             outputname2 = '%s/%s.root' %(outdir,basename2)
#             files_in.append(outputname)
#             files_out.append(outputname2)
        
# for fin in files:
#     outfile = 'Proccessed_%s'%fin
#     files_out.append(outfile)
#     #     print(fin)
#     #     files_out
for ij in range(0,len(files_in)):
    print './bin/ReadEventData %s %s' %(files_in[ij],files_out[ij])
    if(TFileIsGood(files_in[ij])):
        os.system('./bin/ReadEventData %s %s'%(files_in[ij],files_out[ij]))    
    else:
        print('cant open root File', files_in[ij])
    #i+=1
