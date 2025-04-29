# LDM to HDM (vice versa) translation

* This repository contains scripts & procedure to correct the average hits rate for inconsistencies in terms of HDM/LDM, presented between CMSSW and baseline geometries. 

* So there are some modules that are HDM in baseline but low density in CMSSW and vice-versa.

* No partial modules, or modules beyond 38th layer are considered in this geometry.

* Low density modules having thickness 300um are also excluded because of different s/n values.


Descriptions of scripts and steps -
### 1. Extract the average occupancies for all the modules that need HDM to LDM or vice versa transformation -
* readinghistogram.cc : this script reads the histograms from a root file "hitdata_***" containing distribution of number of hits in each events for each modules and also reads a text file named "econ_data_packet_hits_EOL_v8.txt".

* The reading format for input mapping file -
** "layer u,v,type, cmssw-type,shape, no, of lpgbt, lpgbt 1, lgpbt - elink-1, lpgbt-2, lpgbt-2elink, average data"

** We are only picking the modules which are in full shape and their types in CMSSW and baseline (5th and 4th column of above file are different.

* output of this file will be a txt file which contains all the information for each modules that you are going to need with the following format -

** "layer u,v,type, cmssw-type,shape,average hits, error on average, rms,rms_Error"

* To run the script -
```
cd scripts/
root -b -q 'readinghistogram.cc("../data/updatedNtuple_DataPacketDec20_histo.root","../data/signal_noise/econ_data_packet_hits_EOL_v8.txt")'
```
* Note - the path to input and output files are hardcoded in the script - please fix it before you run it.

### 2. Draw or create TGraphs showing average occupancies as a function of distance from beampipe - 
* Use "ldm_hdm_correction.cc" file to get "average hits occuapncies vs distance from the beampipe of that module" graphs for each layer and only for full shape modules.
* To run it
```
root -l 'ldm_hdm_correction.cc("data file from step 1","geometry file "geomnew_corrected.txt"")'
```

* output: a root file containing graphs of avergae vs r behavious for each layer and for hdm and ldm separately.

### 3. Fitting an exponential to these graphs -
* I have been using an exponential function with two parameters "exp[p0+p1*x]".
* script used "fit_function.cc"
* it will save all the graphs in png format and also will give a output txt file that contains fit function parameters of hdm/ldm fit functions for each layer.
* To run it -
```
root -b 'fit_function.cc("output of step-2")'
```

### 4. Get the corrected occupancies for modules with mismatched geometry
* script: correlation_ldm_hdm.cc
* inputs: "function_parametrs","geometry_file","average hits file that we got from step1"
* output: a txt file that contains all the geometry inconsistency and correxponding corrections.
* To run the script -
```
root -b 'correlation_ldm_hdm.cc("output of step-3","../data/geomnew_corrected.txt","Output of step-1")'
```


### 5. Make a coorelation plot and summary plot that are describing the results for this study.
* and that is done using the script: "ldm_hdm_correlation_graph.cc"
* output is a root file with correlation and summary plots
* To Run the script -
```
root -b 'ldm_hdm_correlation_graph.cc("output of the step-4")'
```
### 6. to compare it with fixdensity corrections:
* script: "fixdensity_comparison.cc"
* input: "fixdensity correction file","txt file from step 5"
* output: "root file containing comparison with fix density.

* Please check the data folder for the input & output files used & produced at various steps.
* Similarly steps are to be repeated for Noise only, Signal only, Signal+Noise - Only difference is in the step-1 where input root file has to be changed respectively for different types
