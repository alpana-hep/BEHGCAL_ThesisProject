# BEHGCAL_ThesisProject
This repository stores all the instrunctions codes and information to reproduce the results summarized in BE AN

## LDM to HDM occupancy transformation -
```
cd LDMtoHDM_translation
cd scripts
root -b -q 'readinghistogram.cc("../data/updatedNtuple_DataPacketDec20_histo.root","../data/signal_noise/econ_data_packet_hits_EOL_v8.txt")'
root -l 'ldm_hdm_correction.cc("data file from step 1","geometry file "geomnew_corrected.txt"")'
root -b 'fit_function.cc("output of step-2")'
root -b 'correlation_ldm_hdm.cc("output of step-3","../data/geomnew_corrected.txt","Output of step-1")'
root -b 'ldm_hdm_correlation_graph.cc("output of the step-4")'
```
* for details of above commands - check Readme file in LDMtoHDM_translation directory

## Pseudo event generation
```
cd PseudoEventGeneration
cmsrel CMSSW_11_1_0_pre3
cd CMSSW_11_1_0_pre3/src
cmsenv
git init
git clone ssh://git@gitlab.cern.ch:7999/agilbert/HGCalBufferModel.git .
python scripts/generateHitData.py  -i <input root ntuple> --output hitData_${TYPE} -e 500 --modifier data/CMSSW/modifier_config_${TYPE}.json 2>&1 | tee hit_averages_EOL_${TYPE}.txt; done
cp https://gitlab.cern.ch/kalpana/HGCalBufferModel/-/blob/Datapackets/arraytree_EconData.py .
python  arraytree_EconData.py  -i <input root ntuple> --output hitData_${TYPE} -e 500 --modifier data/CMSSW/modifier_config_${TYPE}.json
root -b 'n10_fitting.C("occupancy_histogram_file_n10.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'
root -b 'n20_fitting.C("occupancy_histogram_file_n20.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'	
root -b 'n30_fitting.C("occupancy_histogram_file_n30.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'
python3 calCorrMatrix.py
python3 genRandon.py
python3 matrix_multiplication_D86.py $min_ $max_ $out_fname $fin
python3 randomSampling_fit_v1.py 0 100000
python3 quantile_transf_condorjobs_D86_1M.py $infile_fitrandomSamples $infile_correlatedrandomNumber $outfile $min_ $max_
root -l
.L tree_try5k_v1.C
allinone M
m.Loop()

```
* for details of above commands - check Readme file in PseudoEventGeneration


# Emulator based studies
Check the readme file under EmulatorStudies

