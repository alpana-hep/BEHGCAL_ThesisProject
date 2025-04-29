import os,sys,re,fileinput,string,shutil
#from datetime import date
min_count=0
year=["Fall17","Autumn18","Summer16v3"]
number=["2017","2018","2016"]
#sample_tag=["GJets_DR","QCD_Jets","temp","temp","temp","temp","temp","temp"]
bkg_samples=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
#bkg_samples=["ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets"]
#bkg_samples=["ZGJets","GJets","ZJets","QCD"]
#bkg_samples=["TTJets-HT","TTJets-inc"]
count=0;
for i in range(0,100000,10000):
    Min_ = i
    seed_ = (12*(i+1))/10000
    i_ =i/10000
    Max_ = i+10000
    for j in range(0,100000,10000):
        min_ = j

        max_= j+10000
        ##count1+=1                                                                                                                       
        print(min_,max_)
        seed_ = (12*(i+1))/10000
        i_ =i/10000
        condorSubmit ="condor_submit condor_submit/submitCondor_D86_corr_10k_%d_%d_%d_%d"%(Min_,Max_,min_,max_)

#         #condorSubmit = "condor_submit condor_submit/submitCondor_D49_randgaus_10k_%d_%d"%(i_, seed_) #%d_10k_seed_%d"%(i_1,seed_)
#         #condorSubmit = "condor_submit condor_submit/submitCondor_D49_corr_10k_%d_%d"%(min_, max_)
        print condorSubmit
        os.system(condorSubmit)
#         # seed_ = (17*(i-1))/10000
#         # condorSubmit = "condor_submit condor_submit/submitCondor_D49_randgaus_10k_%d_%d"%(i_, seed_) #%d_10k_seed_%d"%(i_1,seed_)             
#         # print condorSubmit
#         # os.system(condorSubmit)
        
        
#condorSubmit ="condor_submit/submitCondor_D49_corr_10k_0_10000_0_10000"
