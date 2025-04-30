import os,sys,re,fileinput,string,shutil
from datetime import date
min_count=0
#year=["Fall17","Autumn18","Summer16v3"]
#number=["2017","2018","2016"]
#sample_tag=["GJets_DR","QCD_Jets","temp","temp","temp","temp","temp","temp"]
#bkg_samples=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
#bkg_samples=["ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets"]
#bkg_samples=["ZGJets","GJets","ZJets","QCD"]
#bkg_samples=["TTJets-HT","TTJets-inc"]
count=0;
bkg_samples=["UnCorrelate_Pseudo","Correlate_Psuedo","CMSSW"]
fname=["/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/UncorrleatedPseudoEve/PseudoNtuple_uncorrelated_D86_19072022_1M.root","/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_21072022_1M_covMatrix.root","/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_ttbar_D86_12_1_X_PU200_20211106_mipDef.root"]
#count1=0
BX_num =[1000000] #0,2000000,3000000,4000000,5000000]#,6000000,7000000,8000000,9000000,10000000]
scale_num=[1,2,3,4]#,5,51,52,53]#,1,2,3,4]#1,2,3,51,52,53,5]#,51,52,53]#700,800,900]#1300,1400,1500,1600,1700,1800]
hitscale=[1000]#,1100,1200,1300,1400,1500,1600,1700,1800,2000]#[100,200,300,400,500,600,700,800,900,1000]#[1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]#[100,200,300,400,500,600,800,1000]#[1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]#[100,200,300,400,500,600,800,1000]
#scale_um=["scinti_pshape","scinti_fshape","HDM_Fshape","HDM_pshape","LDM_Fshape","LDM_pshape"]
import numpy as np
layer_number = np.arange(0,48,1)#[4,5,7,9,28,30,32,34,36]
print(layer_number)
#submitCondor_D86_CMSSW_BX100k_test_MultiScale_10per_2
#triggerRate=[600,  625,  650,  675,  700,  725,  750,  775,  800,  825,  850,  875,  900,  925,  950,  975, 1000, 1025, 1050, 1075, 1100,1125, 1150, 1175,1200]
for j in range(1,len(bkg_samples)):
    for l in range(1,len(layer_number)):        
        for i in range(len(scale_num)):
            for k in range(len(hitscale)):
                count=BX_num[0]/1000
                scale = scale_num[i] #submitCondor_D86_CMSSW_BX100k_test_AddScale_10per_toLDM_4
                hitScaling = hitscale[k]/100
                condorSubmit = "condor_submit condor_submit/submitCondor_D86_%s_1MBX_test_MultiScale_%sper_%s_layer_%s"%(bkg_samples[j],hitScaling,scale,layer_number[l])#_%s_BX%sM_triggerRate_%skHz"%(bkg_samples[j],count,triggerRate[k]) condor_submit/checks_submitCondor_D86_UnCorrelate_Pseudo_BX100k_test_Addscale_5per_toLDM_1
                print condorSubmit
                os.system(condorSubmit)
            
