from jobs import Jobs
import argparse
import os
import json

job_mgr = Jobs()
parser = argparse.ArgumentParser()
job_mgr.attach_job_args(parser)

# parser.add_argument('--files-per-job', '-f', type=int, default=100,
#                     help='Number of files to process per job')
# # parser.add_argument('--attributes', '-a', default='default',
# #                     help='Only run the samples matching the chosen attribute')
# parser.add_argument('--output', '-o', default='output',
#                     help='Main output file')
# parser.add_argument('--filelist', default='',
#                     help='Input filelist')
# # parser.add_argument('--production', '-p', default='wgamma_2018_v1',
# #                     help='Production tag, used as filelist prefix and output path')
# # parser.add_argument('--sequences', '-s', default='WGamma',
# #                     help='List of sequences to run')
# parser.add_argument('--tmp', '-t', default='',
#                     help='Write the job output in a temporary location')
# parser.add_argument('--mipThreshold', type=float, default=-1.,
#                     help='mipThreshold')
# # parser.add_argument('--merge-tasks', '-m', action='store_true',
# #                     help='Merge all tasks into one big submission')

args = parser.parse_args()
job_mgr.set_args(args)


nBx = 5E5
# With 5E5 BX, expect around 10K events. We have 120K events in the input file, so generate 12 jobs:
startPoints = [i * 10000 for i in range(10)]

# do_submissions = ['econdbufferlevel_default', 'optimizedMapping', 'triggerRate_default', 'triggerRate_avEventPacket', 'triggerRate_regTrig', 'triggerRate_avEventPacket_regTrig']
do_submissions = ['triggerRate_optimizedMapping', 'econdbufferlevel_triggerRate_780', 'econdbufferlevel_triggerRate_780_econdUpdateIdleStatus']
do_submissions = ['randomMappingV8', 'simpleMappingV8']
do_submissions = [
# "econdbufferlevel_default_V10",
# "econdbufferlevel_default_V10_pseudo",
   "econdHitScaling_default_V10_pseudo",
#"econdHitScaling_default_V10",
#"econdHitScaling_Additive_fracNcells_V10_pseudo",
# "econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo",

#"econdHitScaling_regTrig_V10_pseudo",
# "econdHitScaling_regTrig_V10",

# "econdHitScaling_15FPGA_V10_pseudo",
# "econdHitScaling_14FPGA_V10_pseudo",
# "triggerRate_default_V10_pseudo",
#"triggerRate_default_V10",
#"triggerRate_avEventPacket_V10_pseudo",
#"triggerRate_avEventPacket_V10",

#"triggerRate_regTrig_V10_pseudo",
# "triggerRate_regTrig_V10",

#"triggerRate_avEventPacket_regTrig_V10_pseudo",

# "triggerRate_avEventPacket_regTrig_V10",

# "econdHitScaling_default_V10",
 # "econdHitScaling_default_V10_pseudo",

]
writedir = '${TMPDIR}'


# if 'randomMappingV8' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/randomMappingV8'
#     os.system('mkdir -p %s' % resdir)
#     for rate in [750]:
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (rate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i feMappingFile=\\"FeMappingV8.txt\\" hitModifierFile=\\"modifier_config_fixdensity2_V8.json\\" beMappingFile=\\"BeMappingV8_16FPGA_random.json\\" triggerRate=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# if 'simpleMappingV8' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/simpleMappingV8'
#     os.system('mkdir -p %s' % resdir)
#     for rate in [750]:
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (rate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i feMappingFile=\\"FeMappingV8.txt\\" hitModifierFile=\\"modifier_config_fixdensity2_V8.json\\" beMappingFile=\\"BeMappingV8_16FPGA_simple.json\\" triggerRate=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)


# # V8 optimised mapping
# if 'triggerRate_optimizedMapping' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_optimizedMapping'
#     os.system('mkdir -p %s' % resdir)
#     for rate in range(720, 811, 30):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (rate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i feMappingFile=\\"FeMappingV8.txt\\" hitModifierFile=\\"modifier_config_fixdensity2_V8.json\\" beMappingFile=\\"BEDAQ_SlinksToLpGBT_16FPGA_Mapping_19032022_fixed_patched.json\\" triggerRate=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)
# filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0
if 'econdHitScaling_Additive_fracNcells_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_Additive_fracNcells_V10_pseudo/v1'
    os.system('mkdir -p %s' % resdir)
    scale_num=[1,2,3,4]
    for i in range(len(scale_num)):
        for level in range(100, 1100, 100):
            for bufflevel in range(1850, 2600, 100):
                for irng, startPoint in enumerate(startPoints):
                    basename = 'HGCalBufferModel_case%i_add_%i_buffer_%i_%i_job_%i' % (scale_num[i],level, bufflevel, bufflevel-400, irng + 1)
                    cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" outputname=\\"%s/%s.root\\" triggerRate=750 numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i econdHitAdditiveArg=%s econdHitScalingArg=true econdHighBufferLevel=%i econdMediumBufferLevel=%i slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, scale_num[i],bufflevel, bufflevel-400, writedir, basename)
                    cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                    job_mgr.job_queue.append(cmd)

if 'econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_Additive_fracNcells_V10_regTrig_pseudo/v1'
    os.system('mkdir -p %s' % resdir)
    scale_num=[1,2,3,4]
    for i in range(len(scale_num)):
        for level in range(100, 1100, 100):
            for bufflevel in range(1850, 2600, 100):
                for irng, startPoint in enumerate(startPoints):
                    actualrate = int(round(float(40000) / 53))
                    basename = 'HGCalBufferModel_case%i_add_%i_buffer_%i_%i_job_%i' % (scale_num[i],level, bufflevel, bufflevel-400, irng + 1)
                    cmd = './bin/HGCalBufferModel outputname=\\"%s/%s.root\\" triggerRate=%s numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i econdHitAdditiveArg=%s econdHitScalingArg=true econdHighBufferLevel=%i econdMediumBufferLevel=%i tcdsHllhcFilename=\\"Flat\\" triggerMethod=2053  filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0 slink25GbitPerSec=true  | gzip -c > %s/%s.log.gz' % (writedir, basename,actualrate, nBx, irng + 1, level, scale_num[i],bufflevel, bufflevel-400, writedir, basename)
                    cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                    job_mgr.job_queue.append(cmd)


if 'econdbufferlevel_default_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/DoubleThresholdSystem'#os/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation'
    os.system('mkdir -p %s' % resdir)
    for level in range(400, 2600, 150):
        for irng, startPoint in enumerate(startPoints):
            basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
            cmd = './bin/HGCalBufferModel /afs/cern.ch/user/k/kalpana/work/public/HGCAL_TDAQ/HGCAL_Buffer_Model/CMSSW_11_1_0_pre3/src/ThrottleVsTruncation/data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdMediumBufferLevel=%i econdHighBufferLevel=%i econdHitScalingArg=true econdHitScaling=1600 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, level, writedir, basename)
            cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
            job_mgr.job_queue.append(cmd)


if 'econdbufferlevel_default_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/DoubleThresholdSystem'#os/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation'                                                 
    os.system('mkdir -p %s' % resdir)
    for level in range(400, 2600, 150):
        for irng, startPoint in enumerate(startPoints):
            basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
            cmd = './bin/HGCalBufferModel data/current_default_V10_pseudo.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdMediumBufferLevel=%i econdHighBufferLevel=%i econdHitScalingArg=true econdHitScaling=1600 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, level, writedir, basename)
            cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
            job_mgr.job_queue.append(cmd)

# if 'econdbufferlevel_default_V10' in do_submissions:
#     resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/DoubleThresholdSystem'#'/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation'
#     #'/eos/cms/store/user/agilbert/hgcal/emulator/econdbufferlevel_default_v10'
#     #os.system('mkdir -p %s' % resdir)
#     for level in range(1850, 3201, 150):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_Hbl_%i_Mbl_%i_job_%i' % (level,level-472, irng + 1)
#             print basename
#             cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdMediumBufferLevel=%i econdHighBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level-472, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)
# job_mgr.flush_queue()

if 'econdHitScaling_regTrig_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_regTrig_V10_pseudo/' #no need to repeat
    os.system('mkdir -p %s' % resdir)
    for level in range(1000, 1801, 100):
        for bufflevel in range(1850, 2650, 100):
            nnarg = 53
            actualrate = int(round(float(40000) / nnarg))
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i econdHitScalingArg=true triggerRate=%i tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i econdHighBufferLevel=%i econdMediumBufferLevel=%i filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0 slink25GbitPerSec=true | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level,actualrate, nnarg, bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

if 'econdHitScaling_regTrig_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_regTrig_V10/v1'
    os.system('mkdir -p %s' % resdir)
    for level in range(1000, 1801, 100):
        for bufflevel in range(1850, 2650, 100):
            nnarg = 53
            actualrate = int(round(float(40000) / nnarg))
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0  outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i econdHitScalingArg=true triggerRate= %i tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level,actualrate, nnarg, bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)




if 'econdHitScaling_default_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_default_V10_pseudo/v1'
    os.system('mkdir -p %s' % resdir)
    for level in range(1300, 1801, 100):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0  outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i econdHitScalingArg=true econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)
                
if 'econdHitScaling_default_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/econdHitScaling_default_V10/v1'
    os.system('mkdir -p %s' % resdir)
    for level in range(1300, 1801, 100):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (level, bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i econdHitScalingArg=true econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

# if 'econdHitScaling_default_V10' in do_submissions:
#     resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/econdHitScaling_default_V10'
#     os.system('mkdir -p %s' % resdir)
#     for level in range(1000, 1701, 100):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)
# if 'econdHitScaling_15FPGA_V10_pseudo' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/econdHitScaling_15FPGA_V10_pseudo'
#     os.system('mkdir -p %s' % resdir)
#     for level in range(1000, 1401, 100):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default_V10_pseudo.json beMappingFile=\\"BeMappingV10_pseudo_15FPGA_simple.json\\" outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# if 'econdHitScaling_14FPGA_V10_pseudo' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/econdHitScaling_14FPGA_V10_pseudo'
#     os.system('mkdir -p %s' % resdir)
#     for level in range(1000, 1401, 100):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default_V10_pseudo.json beMappingFile=\\"BeMappingV10_pseudo_14FPGA_simple.json\\" outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdHitScaling=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# if 'econdbufferlevel_triggerRate_780' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/econdbufferlevel_triggerRate_780'
#     os.system('mkdir -p %s' % resdir)
#     for level in range(1850, 3201, 150):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdMediumBufferLevel=%i econdHighBufferLevel=%i triggerRate=780 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# if 'econdbufferlevel_triggerRate_780_econdUpdateIdleStatus' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/econdbufferlevel_triggerRate_780_econdUpdateIdleStatus'
#     os.system('mkdir -p %s' % resdir)
#     for level in range(1850, 3201, 150):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdMediumBufferLevel=%i econdHighBufferLevel=%i triggerRate=780 econdUpdateIdleStatus=true | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# if 'econdbufferlevel_econdHitScaling_1200' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/econdbufferlevel_econdHitScaling_1200'
#     os.system('mkdir -p %s' % resdir)
#     for level in range(1850, 3201, 150):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (level, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i econdMediumBufferLevel=%i econdHighBufferLevel=%i econdHitScaling=1200 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, level, level, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# if 'randomSeed_default' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/randomSeed_default'
#     os.system('mkdir -p %s' % resdir)
#     for seed in range(1, 51, 1):
#         basename = 'HGCalBufferModel_%i' % seed
#         cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i randomSeed=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, seed, writedir, basename)
#         cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#         job_mgr.job_queue.append(cmd)

# if 'triggerRate_default' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_default'
#     os.system('mkdir -p %s' % resdir)
#     # Do scans of L1 rate
#     for rate in range(720, 811, 30):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (rate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)
# print('')
if 'triggerRate_default_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_default_V10_pseudo/v1'
    os.system('mkdir -p %s' % resdir)
    # Do scans of L1 rate
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0  outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, bufflevel, bufflevel-400,writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

if 'triggerRate_default_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_default_V10/v1'
    os.system('mkdir -p %s' % resdir)
    # Do scans of L1 rate                                                                                                                                            
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, bufflevel, bufflevel-400,writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

# job_mgr.flush_queue()

# if 'triggerRate_avEventPacket' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_avEventPacket'
#     os.system('mkdir -p %s' % resdir)
#     for rate in range(720, 811, 30):
#         for irng, startPoint in enumerate(startPoints):
#             basename = 'HGCalBufferModel_%i_job_%i' % (rate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i randomSeed=%i triggerRate=%i fileReadMode=4 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

# print('')
if 'triggerRate_avEventPacket_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_V10_pseudo/v1'    
    os.system('mkdir -p %s' % resdir)
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0  outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=4 randomSeed=%i triggerRate=%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, bufflevel, bufflevel-400,writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)


if 'triggerRate_avEventPacket_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_V10/v1'
    os.system('mkdir -p %s' % resdir)
    for rate in range(600, 1000, 50):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (rate, bufflevel, bufflevel-400,irng + 1)
                cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=4 randomSeed=%i triggerRate=%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, bufflevel, bufflevel-400,writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

        # for irng, startPoint in enumerate(startPoints):
        #     basename = 'HGCalBufferModel_%i_job_%i' % (rate, irng + 1)
        #     cmd = './bin/HGCalBufferModel data/current_default_V10_pseudo.json outputname=\\"%s/%s.root\\" numberOfBx=%i randomSeed=%i triggerRate=%i fileReadMode=4 | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, rate, writedir, basename)
        #     cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
        #     job_mgr.job_queue.append(cmd)

# job_mgr.flush_queue()

# if 'triggerRate_regTrig' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_regTrig'
#     os.system('mkdir -p %s' % resdir)
#     # Do scans of L1 rate
#     # If we want to factor in the number of colliding bunches = 2748 / 3564
#     for rate in range(49, 56, 1):
#         for irng, startPoint in enumerate(startPoints):
#             nnarg = rate
#             actualrate = int(round(float(40000) / nnarg))
#             basename = 'HGCalBufferModel_%i_job_%i' % (actualrate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)
# print('')
if 'triggerRate_regTrig_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_regTrig_V10_pseudo/v1'
    os.system('mkdir -p %s' % resdir)
    # Do scans of L1 rate
    # If we want to factor in the number of colliding bunches = 2748 / 3564
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate, bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0  outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg,bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)


if 'triggerRate_regTrig_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_regTrig_V10/v1'
    os.system('mkdir -p %s' % resdir)
    # Do scans of L1 rate                                                                                                                                             
    # If we want to factor in the number of colliding bunches = 2748 / 3564                                                                                           
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate, bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg,bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

#job_mgr.flush_queue()

# if 'triggerRate_regTrigGaps' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_regTrigGaps'
#     os.system('mkdir -p %s' % resdir)
#     # Do scans of L1 rate
#     # If we want to factor in the number of colliding bunches = 2748 / 3564
#     for rate in range(38, 44, 1):
#         for irng, startPoint in enumerate(startPoints):
#             nnarg = rate
#             actualrate = int(round((float(40000) / float(nnarg)) * (2748. / 3564.)))
#             writedir = '${TMPDIR}'
#             basename = 'HGCalBufferModel_%i_job_%i' % (actualrate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i fileReadMode=1 randomSeed=%i triggerRate=%i triggerMethod=20%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)


# if 'triggerRate_avEventPacket_regTrig' in do_submissions:
#     resdir = '/eos/cms/store/user/agilbert/hgcal/emulator/triggerRate_avEventPacket_regTrig'
#     os.system('mkdir -p %s' % resdir)
#     # Do scans of L1 rate
#     for rate in range(49, 56, 1):
#         for irng, startPoint in enumerate([0]):
#             nnarg = rate
#             actualrate = int(round(float(40000) / nnarg))
#             basename = 'HGCalBufferModel_%i_job_%i' % (actualrate, irng + 1)
#             cmd = './bin/HGCalBufferModel data/current_default.json outputname=\\"%s/%s.root\\" numberOfBx=%i randomSeed=%i triggerRate=%i fileReadMode=4 tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg, writedir, basename)
#             cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
#             job_mgr.job_queue.append(cmd)

if 'triggerRate_avEventPacket_regTrig_V10_pseudo' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_regTrig_V10_pseudo/v1'
    os.system('mkdir -p %s' % resdir)
    # Do scans of L1 rate
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate,bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel filename=\\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root\\" slink25GbitPerSec=true beMappingFile=\\"BeMappingV10_16FPGA_simple.json\\" feMappingFile=\\"FeMappingV10.json\\" hitModifierFile=\\"modifier_config_notransform_V10.json\\" number=1 lpgbtPairEcondBuilderHighBufferFactor=10000 lpgbtPairEcondBuilderMediumBufferFactor=10000 hitModifierReadUnmasked=false centralDaq=true econdScintHitScalingArg=0  outputname=\\"%s/%s.root\\" numberOfBx=%i randomSeed=%i triggerRate=%i fileReadMode=4 tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg,bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)
if 'triggerRate_avEventPacket_regTrig_V10' in do_submissions:
    resdir = '/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/triggerRate_avEventPacket_regTrig_V10/v1'
    os.system('mkdir -p %s' % resdir)
    # Do scans of L1 rate                                                                                                                                            
    for rate in range(40, 66, 2):
        for bufflevel in range(1850, 2600, 100):
            for irng, startPoint in enumerate(startPoints):
                nnarg = rate
                actualrate = int(round(float(40000) / nnarg))
                basename = 'HGCalBufferModel_%i_buffer_%i_%i_job_%i' % (actualrate,bufflevel, bufflevel-400, irng + 1)
                cmd = './bin/HGCalBufferModel data/current_default_V10.json outputname=\\"%s/%s.root\\" numberOfBx=%i randomSeed=%i triggerRate=%i fileReadMode=4 tcdsHllhcFilename=\\"Flat\\" triggerMethod=20%i econdHighBufferLevel=%i econdMediumBufferLevel=%i | gzip -c > %s/%s.log.gz' % (writedir, basename, nBx, irng + 1, actualrate, nnarg,bufflevel, bufflevel-400, writedir, basename)
                cmd += '; cp %s/%s.* %s/' % (writedir, basename, resdir)
                job_mgr.job_queue.append(cmd)

job_mgr.flush_queue()
