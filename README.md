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