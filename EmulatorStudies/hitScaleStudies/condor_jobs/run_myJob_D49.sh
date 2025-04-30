#!/bin/bash
# export PATH=/opt/apps/gcc-4.8.5-install/bin:/apps/Python-3.5.10-install-new/bin:/apps/cmake-3.19.4-install/bin:$PATH
# export LD_LIBRARY_PATH=/opt/apps/gmp-6.1.1-install/lib:/opt/apps/mpc-1.0.3-install/lib:/opt/apps/mpfr-3.1.4-install/lib:/opt/apps/gcc-4.8.5-install/lib:/opt/apps/gcc-4.8.5-install/lib64:/apps/Python-3.5.10-install-new/lib:$LD_LIBRARY_PATH
# export INCLUDE=/opt/apps/gmp-6.1.1-install/include:/opt/apps/mpc-1.0.3-install/include:/opt/apps/mpfr-3.1.4-install/include:/opt/apps/gcc-4.8.5-install/include:/apps/Python-3.5.10-install-new/include:$INCLUDE
# export PYTHONPATH=/apps/Python-3.5.10-install-new/lib:$PYTHONPATH
#cd /afs/cern.ch/user/k/kalpana/work/public/hgcalTestBeamOct2018/ForNitish/code_setup/CMSSW_11_1_0_pre3/src/Alps_Setup/Hgcal_testbeam_analysis_2021
cd /afs/cern.ch/user/k/kalpana/work/public/HGCAL_TDAQ/HGCAL_Buffer_Model/CMSSW_11_1_0_pre3/src/emulator_studies

# /home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/test_condor_jobs
# tar -zxvf /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/gcc-install.tar.gz

# export PATH=$PWD/gcc-4.8.5-install/bin:$PATH
# export LD_LIBRARY_PATH=$PWD/gmp-6.1.1-install/lib:$PWD/mpc-1.0.3-install/lib:$PWD/mpfr-3.1.4-install/lib:$PWD/gcc-4.8.5-install/lib:$PWD/gcc-4.8.5-install/lib64:$LD_LIBRARY_PATH
# export INCLUDE=$PWD/gmp-6.1.1-install/include:$PWD/mpc-1.0.3-install/include:$PWD/mpfr-3.1.4-install/include:$PWD/gcc-4.8.5-install/include:$INCLUDE

# tar -zxvf /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/Python-3.5.10-install-new.tar.gz
# export PATH=$PWD/Python-3.5.10-install-new/bin:$PATH
# export PYTHONPATH=$PWD/Python-3.5.10-install-new:$PYTHONPATH
# export LD_LIBRARY_PATH=$PWD/Python-3.5.10-install-new/lib:$LD_LIBRARY_PATH
# export HOME=$PWD

# tar -zxvf /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/root-6.18.04-install.tar.gz
# export PATH=$PWD/root-6.18.04-install/bin:$PATH
# source /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example/condor_jobs/root-6.18.04-install/bin/thisroot.sh
# cd /home/work/kalpana/public/work/BE_DAq/CMSSW_9_3_0/src/Condor_jobs/forAlpana/example

# Following two lines are just for cmsenv
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`


echo $PWD
#cd /home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/test_condor_jobs
#python temp.py
./bin/HGCalBufferModel /afs/cern.ch/user/k/kalpana/work/public/HGCAL_TDAQ/HGCAL_Buffer_Model/CMSSW_11_1_0_pre3/src/emulator_studies/data/current_default.json numberOfBx=$1 outputname=$2 filename=$3 hitModifierFile=$4 triggerRate=$5 #\"HGCalBufferModel\"



#centralDaq=true numberOfBx=$1 filename=\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/UncorrleatedPseudoEve/PseudoNtuple_uncorrelated_D86_19072022_1M.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\"  number=1 outputname=\"HGCalBufferModel_pseudo_FEV10_D86_BE16_192Slink_SL25_1MBX_fixdenv2.root\" hitModifierReadUnmasked=false | tee Psuedo_HGCalBufferModel_FEV10_BE192V9p1_SL25_1M.log

# ./bin/HGCalBufferModel centralDaq=true numberOfBx=1000000 filename=\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_20072022_1M_covMatrix.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\"  number=1 outputname=\"HGCalBufferModel_Corrpseudo_FEV10_D86_BE16_192Slink_SL25_1MBX_fixdenv2.root\" hitModifierReadUnmasked=false | tee Psuedo_HGCalBufferModel_FEV10_BE192V9p1_SL25_1M.log

# ./bin/HGCalBufferModel centralDaq=true numberOfBx=100000 filename=\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/UncorrleatedPseudoEve/PseudoNtuple_uncorrelated_D86_29062022_100k.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\"  number=1 outputname=\"HGCalBufferModel_pseudo_FEV10_D86_BE16_192Slink_SL25_100kBX_fixdenv2.root\" hitModifierReadUnmasked=false | tee Psuedo_HGCalBufferModel_FEV10_BE192V9p1_SL25_100k.log

# ./bin/HGCalBufferModel centralDaq=true numberOfBx=100000 filename=\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/CorrleatedPseudoEve/PseudoNtuple_Correlated_D86_20072022_1M_covMatrix.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\"  number=1 outputname=\"HGCalBufferModel_Corrpseudo_FEV10_D86_BE16_192Slink_SL25_100kBX_fixdenv2.root\" hitModifierReadUnmasked=false | tee Psuedo_HGCalBufferModel_FEV10_BE192V9p1_SL25_100kBX.log


# ./bin/HGCalBufferModel  centralDaq=true numberOfBx=100000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_ttbar_D86_12_1_X_PU200_20211106_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\" number=1 outputname=\"HGCalBufferModel_CMSSW_fixdenv2_FEV10_BE16_192Slink_SL25.root\" hitModifierReadUnmasked=false | tee CMSSW_HGCalBufferModel_FEV10_MCFD2V11_BE192V9p1_SL25.log


# ./bin/HGCalBufferModel  centralDaq=true numberOfBx=1000000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_ttbar_D86_12_1_X_PU200_20211106_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\" number=1 outputname=\"HGCalBufferModel_CMSSW_fixdev2_FEV10_BE16_192Slink_SL25_1M.root\" hitModifierReadUnmasked=false | tee CMSSW_HGCalBufferModel_FEV10_MCFD2V11_BE192V9p1_1M_SL25.log





# ./bin/HGCalBufferModel data/current_default_V10.json numberOfBx=100000 filename=\"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/PseudoNtuple_uncorrelated_D86_29062022_100k.root\" slink25GbitPerSec=true beMappingFile=\"BeMappingV10_16FPGA_simple.json\" feMappingFile=\"FeMappingV10.json\" hitModifierFile=\"modifier_config_fixdensity2_V10.json\"  number=1 outputname=\"HGCalBufferModel_pseudo_FEV10_MCFD2V9_BE16_192Slink_SL25_v1.root\" hitModifierReadUnmasked=false | tee Pseudo_HGCalBufferModel_FEV10_MCFD2V11_BE192V9p1_SL25.log
#./bin/HGCalBufferModel centralDaq=true numberOfBx=1000000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_
#PU200_3iab_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"BEDAQ_192SlinksToLpGBT_16FPGA_Mapping_V9.1.json\" feMappingFile=\"FeMappingV9.json\" hitModifierFile=\"modifier_config_fixdensity2_V9.json\"  number=1 outputname=\"HGCalBufferModel_CMSSW_FEV9_MCFD2V9_BE16_192Slink_SL25_1M.root\" hitModifierReadUnmasked=false | tee /afs/cern.ch/user/k/kalpana/work/public/HGCAL_TDAQ/HGCAL_Buffer_Model/CMSSW_11_1_0_pre3/src/emulator_studies/CMSSW_HGCalBufferModel_FEV9_MCFD2V11_BE192V9p1_SL25_1M.log
#./analyzehgcocttb $1 $2 $3 $4 $5

#file_list/skimmed_ntuples_0to1M.txt out_temp.root sim alpha 1 0 1
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_10.txt Out_T5qqqHG_2350_10_v17.root signal  T5bbbbZg_2350_10
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_50.txt Out_T5qqqHG_2350_50_v17.root signal T5bbbbZg_2350_50
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_100.txt Out_T5qqqHG_2350_100_v17.root signal T5bbbbZg_2350_100
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5bbbbZg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_400.txt Out_T5qqqHG_2350_400_v17.root signal T5bbbbZg_2350_400
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_600.txt Out_T5qqqHG_2350_600_v17.root signal T5bbbbZg_2350_600
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_1000.txt Out_T5qqqHG_2350_1000_v17.root signal T5bbbbZg_2350_1000
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_1200.txt Out_T5qqqHG_2350_1200_v17.root signal T5bbbbZg_2350_1200
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_2000.txt Out_T5qqqHG_2350_2000_v17.root signal T5bbbbZg_2350_2000
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_1800.txt Out_T5qqqHG_2350_1800_v17.root signal T5bbbbZg_2350_1800
# ./analyzeLightBSM ./runList/filelist/runList_T5bbbbZg_2350_2200.txt Out_T5qqqHG_2350_2200_v17.root signal T5bbbbZg_2350_2200




# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1700.txt out_pMSSM_MCMC_86_7257_Mg1700_BDTcut.root signal MCMC_86_7257_1700
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1600.txt out_pMSSM_MCMC_86_7257_Mg1600_BDTcut.root signal MCMC_86_7257_1600
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1500.txt out_pMSSM_MCMC_86_7257_Mg1500_BDTcut.root signal MCMC_86_7257_1500
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1400.txt out_pMSSM_MCMC_86_7257_Mg1400_BDTcut.root signal MCMC_86_7257_1400
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1800.txt out_pMSSM_MCMC_86_7257_Mg1800_BDTcut.root signal MCMC_86_7257_1800
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg1900.txt out_pMSSM_MCMC_86_7257_Mg1900_BDTcut.root signal MCMC_86_7257_1900
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257_Mg2000.txt out_pMSSM_MCMC_86_7257_Mg2000_BDTcut.root signal MCMC_86_7257_2000
# ./analyzeLightBSM ./runList/runfilelist_pMSSM_MCMC_86_7257.txt out_pMSSM_MCMC_86_7257_Mg1750_BDTcut.root signal MCMC_86_7257_1750

  

# ./Analyzelightbsm ./runList/filelist/runList_T5qqqqHg_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5qqqqHg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_300.txt Out_T5qqqHG_2350_300_v17.root signal T5qqqqHg_2350_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_1500.txt Out_T5qqqHG_2350_1500_v17.root signal T5qqqqHg_2350_1500
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_127.txt Out_T5qqqHG_2350_127_v17.root signal T5qqqqHg_2350_127
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_150.txt Out_T5qqqHG_2350_150_v17.root signal T5qqqqHg_2350_150
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_175.txt Out_T5qqqHG_2350_175_v17.root signal T5qqqqHg_2350_175
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5qqqqHg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_300.txt Out_T5qqqHG_2350_300_v17.root signal T5qqqqHg_2350_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHg_2350_1500.txt Out_T5qqqHG_2350_1500_v17.root signal T5qqqqHg_2350_1500


# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_150.txt Out_T5qqqHG_2300_150_v17.root signal T5qqqqHg_2300_150
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_175.txt Out_T5qqqHG_2300_175_v17.root signal T5qqqqHg_2300_175
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_200.txt Out_T5qqqHG_2300_200_v17.root signal T5qqqqHg_2300_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_300.txt Out_T5qqqHG_2300_300_v17.root signal T5qqqqHg_2300_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2300_1500.txt Out_T5qqqHG_2300_1500_v17.root signal T5qqqqHg_2300_1500
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_127.txt Out_T5qqqHG_2350_127_v17.root signal T5qqqqHg_2350_127
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_150.txt Out_T5qqqHG_2350_150_v17.root signal T5qqqqHg_2350_150
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_175.txt Out_T5qqqHG_2350_175_v17.root signal T5qqqqHg_2350_175
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_200.txt Out_T5qqqHG_2350_200_v17.root signal T5qqqqHg_2350_200
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_300.txt Out_T5qqqHG_2350_300_v17.root signal T5qqqqHg_2350_300
# ./analyzeLightBSM ./runList/filelist/runList_T5qqqqHG_2350_1500.txt Out_T5qqqHG_2350_1500_v17.root signal T5qqqqHg_2350_1500

# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_10.txt Out_T5ttttZG_2200_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_50.txt Out_T5ttttZG_2200_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_200.txt Out_T5ttttZG_2200_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2200_1500.txt Out_T5ttttZG_2200_1500_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_10.txt Out_T5ttttZG_2350_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_50.txt Out_T5ttttZG_2350_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_200.txt Out_T5ttttZG_2350_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T5ttttZG_2350_1500.txt Out_T5ttttZG_2350_1500_v17.root signal temp

# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_10.txt Out_T6ttZg_1400_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_50.txt Out_T6ttZg_1400_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_200.txt Out_T6ttZg_1400_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1400_1000.txt Out_T6ttZg_1400_1000_v17.root signal temp

# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_10.txt Out_T6ttZg_1500_10_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_50.txt Out_T6ttZg_1500_50_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_200.txt Out_T6ttZg_1500_200_v17.root signal temp
# ./analyzeLightBSM runList/filelist/runList_T6ttZg_1500_1000.txt Out_T6ttZg_1500_1000_v17.root signal temp





#python3 ./datapackets/quantile_transf_condorjobs.py $1 $2 $3
# python3 quantile_transf_condorjobs.py $1 $2 $3
#python3 randomSampling_fit.py
#python3 matrix_multiplication.py $1 $2 $3 
#python3 randomSampling_fit.py $1 $2 
#python3  myprog.py $1 $2 $3

#python3 myprog.py $1 $2 $3
