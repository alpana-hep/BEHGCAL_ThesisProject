# LDM to HDM (vice versa) translation

* This repository contains scripts & procedure to correct the average hits rate for inconsistencies in terms of HDM/LDM, presented between CMSSW and baseline geometries. 

* So there are some modules that are HDM in baseline but low density in CMSSW and vice-versa.

* No partial modules, or modules beyond 38th layer are considered in this geometry.

* Low density modules having thickness 300um are also excluded because of different s/n values.


Descriptions of scripts:
* readinghistogram.cc : this script reads the histograms from a root file "hitdata_***" containing number of hits in each events for each modules and also reads a text file named "econ_data_packet_hits_EOL_v8.txt".
output of this file will be a txt file which contains all the information for each modules that you are gonna need . 
"layer u,v,type, cmssw-type,shape,average hits, error on average, rms,rms_Error"

2. in the second step, you use "ldm_hdm_correction.cc" file to get "average hits occuapncies vs distance from the beampipe of that module" graphs for each layer and only for full shape modules.
input : root -l 'ldm_hdm_correction.cc("data file from step 1","geometry file "geomnew_corrected.txt"")'
output: a root file containing graphs of avergae vs r behavious for each layer and for hdm and ldm separately.

3. now we need to fit a function to these graphs. I have been using an exponential function with two parameters "exp[p0+p1*x]".
script used "fit_function.cc"
it will save all the graphs in png format and also will give a output txt file that contains fit function parameters of hdm/ldm fit functions for each layer.

4. now we need to find out the modules that are inconsistent in geometry and are full shape obviously and find the corresponding corrections for those.
script: correlation_ldm_hdm.cc
inputs: "function_parametrs","geometry_file","average hits file that we got from step1"
output: a txt file that contains all the geometry inconsistency and correxponding corrections.

5. after we get a txt file from step 4, now all we need to do is make a coorelation plot and summary plot that are describing the results for this study. and that is done using the script:
"ldm_hdm_correlation_graph.cc"
output is a root file with correlation and summary plots

6. to compare it with fixdensity corrections:
script: "fixdensity_comparison.cc"
input: "fixdensity correction file","txt file from step 5"
output: "root file containing comparison with fix density.