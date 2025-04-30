# Pseudo event generation method
This branch contains the step by step instructions to get the CMSSW based pseudo event generation with correct modeling of correlation.

Link to the slides showing the work flow of pseudo event generation (See update from Alpana)

https://indico.cern.ch/event/1014822/


## 1. Step-0.0  Preparing input files needed (a) occupancy histograms for all the modules , (b) occpuancy arrays in all three formats (c) mapping files

```
cmsrel CMSSW_11_1_0_pre3
cd CMSSW_11_1_0_pre3/src
cmsenv
git init
git clone ssh://git@gitlab.cern.ch:7999/agilbert/HGCalBufferModel.git .
python scripts/generateHitData.py  -i <input root ntuple> --output hitData_${TYPE} -e 500 --modifier data/CMSSW/modifier_config_${TYPE}.json 2>&1 | tee hit_averages_EOL_${TYPE}.txt; done
```

* where {Type} = for TYPE in notransform fixdensity pessimistic optimistic

* You will get a root file named hitData_${TYPE}.root which will have all the 1-d occupancy distributions for all modules
* Output files stored in this path :
```

```
* (b)  occpuancy arrays in all three formats (to be run three times) - this is used as input to calculate correlation matrix
```
cp https://gitlab.cern.ch/kalpana/HGCalBufferModel/-/blob/Datapackets/arraytree_EconData.py .
python  arraytree_EconData.py  -i <input root ntuple> --output hitData_${TYPE} -e 500 --modifier data/CMSSW/modifier_config_${TYPE}.json
```
*(Repeat for three times and get three root files containing one branch for each module) 
*Output files stored in this path 
```

```
## 2. Step 1.0: Fit the occupancy distributions & save the fit functions parameters in a txt file (different file for different occupancy format [n10,n20,n30]

* You can do this locally in your computer or you can use the CMSSW_11_1_0_pre3 version
```
root -b 'n*_fitting.C("occupancy_histogram_file*.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'  // where * = 10,20,30
```
* where * = 10,20,30
```
root -b 'n10_fitting.C("occupancy_histogram_file_n10.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'	
root -b 'n20_fitting.C("occupancy_histogram_file_n20.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'
root -b 'n30_fitting.C("occupancy_histogram_file_n30.root","inputs/luv_ncells_map_cmssw_D86_V10.txt")'
```

* This scripts fits the occupancy distributions and saves the plots and write the parameters in the txt files. Check for bad fits & accordingly change the initial values of the parameters in the code

## 2. Step 1.1: Calculating the covariance matrix[15k X 15k] and storing it into a txt file
* This step can be done parallely
Or For D86 geometry
```
python3 calCorrMatrix.py
```

* This will save the correlation matrix into a txt file

## Step2: Generating random gaussian numbers [n = 1M or whatever value you want] and save these to a root file so that these can be correlated after multiplying it with the correlation matrix
```
python3 genRandon.py
```
* {generates root file containing uncorrelated random number with gaussian distribution [0:1], mind the random seed}

## Step 3 : To correlate these random numbers
```
python3 matrix_multiplication_D86.py $min_ $max_ $out_fname $fin // see the code to understand the argument meaning
```

* this will generate a root file containing one branch for each module

## Step 4 : Sampling random numbers from the fit function and save them in a root file
```
python3 randomSampling_fit_v1.py 0 100000

```

## Step 5: This step is to read correlated random numbers and random sampled numbers from the fit function and use quantile tranformation to transfer correlated numbers to fitted random sampled space
```
python3 quantile_transf_condorjobs_D86_1M.py $infile_fitrandomSamples $infile_correlatedrandomNumber $outfile $min_ $max_
```
* This will give us a root file which will contain one branch containing pseudo correlated events for each module.

## Step 6 : The final step is to convert the output from step 5 to a ntuple compatible to the emulator code.
```
root -l
.L tree_try5k_v1.C
allinone M
m.Loop()
```

* You will get the final ntuple compatible with emulator code


### Plotting scripts -
```
cd plottingScripts/

```


* Additionally some intial correlation studies were done and corresponding scripts can be found here
```
cmsrel CMSSW_11_1_0_pre3
cd CMSSW_11_1_0_pre3/src
cmsenv
git clone -b correlation_studies https://gitlab.cern.ch/kalpana/HGCalBufferModel.git .
make
cd scripts
```

Under scripts directory , several scripts are there using which different correlations were calculated
