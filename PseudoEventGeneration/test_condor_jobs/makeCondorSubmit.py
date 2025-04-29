import os,sys,re,fileinput,string,shutil
from datetime import date
count=0
min_count=0
# year=["Fall17","Autumn18","Summer16v3"]
# number=["2017","2018","2016"]
# #sample_tag=["GJets_DR","QCD_Jets","temp","temp","temp","temp","temp","temp"]
# sample_tag=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
# bkg_samples=["GJets_DR","QCD","ZGJets","ZJets","TTGJets","TTJets-HT","WGJets","WJets","TTJets-inc"]
#bkg_samples=["GJets_DR","QCD"]
count1=0;
for i in range(0,110000,10000):
    Min_=i
    Max_ = i+10000
    inFile="/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/Random_stdGaus_uncorrelated/D49/RandNumber_D49_10k_%d_%d_.root"%(Min_,Max_)
    for j in range(0,100000,10000):
        min_ = j
        
        max_= j+10000
        ##count1+=1
        print(min_,max_)
        seed_ = (12*(i+1))/10000                
        i_ =i/10000
        condorSubmit ="condor_submit/submitCondor_D49_corr_10k_%d_%d_%d_%d"%(Min_,Max_,min_,max_)
        jobName = "Corr_%d_%d"%(min_,max_) #random_D49_seed7856_29062022.root
        #inFile="/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/RandNumber_D49_10k_10000_20000_.root"
        outFile="/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/Correlated_random/D49/1M/CorreNumber_D49_10k_%d_%d_%d_%d_.root"%(Min_,Max_,min_,max_)#/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/Random_stdGaus_uncorrelated/\
            #D49/RandNumber_D49_10k_%d_%d_.root"%(min_,max_)
        #/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/Correlated_random/D49/CorreNumber_D49_10k_%d_%d_.root"%(min_,max_) #%s_%s" %(sample_tag[count1],year[count1])
        shutil.copyfile("proto_condor_submit",condorSubmit)
        for line in fileinput.FileInput(condorSubmit, inplace=1):
            line=line.replace("max_", str(max_))
            line=line.replace("min_", str(min_))
            line=line.replace("outfile", str(outFile))
            line=line.replace("infile", str(inFile))
            #line=line.replace("
            # line=line.replace("MAX", str(j))
            # line=line.replace("outfile", fname)
            # line=line.replace("filelist",fname1)
            # line=line.replace("physicsList",number[count1])
            # line=line.replace("sample_tag",sample_tag[count])
            #line=line.replace("IN1",fname1)
            #    line=line.replace("IN2",fname2)
            line=line.replace("JOBNAME", jobName)
            print line.rstrip()
            
        print condorSubmit
