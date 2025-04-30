import os,sys,re,fileinput,string,shutil
from datetime import date
count=0
min_count=0
# year=["Fall17","Autumn18","Summer16v3"]
# number=["2017","2018","2016"]
# #sample_tag=["GJets_DR","QCD_Jets","temp","temp","temp","temp","temp","temp"]
# sample_tag=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
# bkg_samples=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
bkg_samples=["UnCorrelate_Pseudo","Correlate_Psuedo","CMSSW"]
fname=["/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/UncorrleatedPseudoEve/PseudoNtuple_uncorrelated_D86_19072022_1M.root","/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root","/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_ttbar_D86_12_1_X_PU200_20211106_mipDef.root"]
BX_num =[1000000] #,50000,100000,200000,400000,600000,800000,1000000,1500000]
scale_num=[1,2,3,4]#,5,51,52,53]#,1,2,3,4]#4,5,51,52,53]#1000] #700,800,900]#1300,1400,1500,1600,1700,1800]
hitscale=[1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]#[100,200,300,400,500,600,700,800,900,1000]#1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]
import numpy as np
layer_number = np.arange(0,48,1)#[4,5,7,9,28,30,32,34,36]                                                                                   print(layer_number)
count1=0;
for j in range(len(bkg_samples)):    
    for l in range(1,2):#len(layer_number)):        
        for i in range(len(scale_num)):
            for k in range(len(hitscale)):
                count=BX_num[0]/1000
                scale=scale_num[i] #/1000
                print('scale', scale)
                hitScaling = hitscale[k]/100 #submitCondor_D86_UnCorrelate_Pseudo_BX100k_test_Addscale_1per_toLDM_53
                print(hitScaling)
                condorSubmit = "condor_submit/submitCondor_D86_%s_1MBX_test_MultiScale_%sper_%s"%(bkg_samples[j],hitScaling,scale)
                fname1 = "%s"%fname[j]#Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_5.root
                jobName = "D86_%s_BX%sk_%sper_case_%s"%(j,count,hitScaling,scale)
                outFile="/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/May19_2023/Out_emuRuns_D86_%s_1MBX_defaultConfig_hitScale_Multi_%sper_case_%s.root"%(bkg_samples[j],hitScaling,scale) #%s_%s" %(sample_tag[count1],year[count1])
                shutil.copyfile("proto_condor_submit",condorSubmit)
                for line in fileinput.FileInput(condorSubmit, inplace=1):
                    line=line.replace("BX", str(BX_num[0]))
                    line=line.replace("OutFile", str(outFile))
                    line=line.replace("inFile", str(fname1))
                    line=line.replace("JOBNAME", jobName)
                    line=line.replace("HitAdd_case",str(scale_num[i]))
                    line=line.replace("Type",str(bkg_samples[j]))
                    line=line.replace("HitScaling",str(hitscale[k]))
                    line=line.replace("HitLayer",str(layer_number[l]))
                    line=line.replace("ScaleVSMulti",str(2))
                    print line.rstrip()
                    
                
                print condorSubmit
            
