## Instrucitons to run the emulator for different cases:

### Setup the machinery-
```
cmsrel CMSSW_11_1_0_pre3
cd CMSSW_11_1_0_pre3/src
git clone -b throttleVsTruncation https://gitlab.cern.ch/kalpana/HGCalBufferModel.git .
cmsenv
make clean
make
```
### To run the emulator test-
```
./bin/HGCalBufferModel data/current_default_V10.json outputname=\"HGCalBufferModel_1000_job_1.root\" numberOfBx=500000 randomSeed=1 triggerRate=750 fileReadMode=4 | gzip -c > HGCalBufferModel_1000_job_1.log.gz
```

Depending upon what you want to run the test for, you can change the different arguments provided to the test. A large number of configurable arguments, so better take the reference from the code itself.

### For the explanation of arguments, see the follwing readme files

https://gitlab.cern.ch/kalpana/HGCalBufferModel/-/blob/master/README.md?ref_type=heads
Or
https://gitlab.cern.ch/kalpana/HGCalBufferModel/-/blob/master/src/Arguments.cc?ref_type=heads

### Throttle vs truncation

In these studies, one will have to scan many different variables and run the emulator for each of those cases.
In the file 'script/launch_jobs.py', see L99-L110: Jobs are prepared for scanning the econd buffer level threshold. Similar can be prepared to scan other variables.

### Launch the jobs :
```
python scripts/launch_jobs.py --job condor

```

### Read the event data
use 'test/ReadEventData.cpp' to read the output of the emulator run and create histogram after counting truncations and fill the histograms-
```
./bin/ReadEventData inputfile outputfile
```
### Scale up "Read event data" from multiple files and save it to the corresponding file-
```
python scripts/readProcess.py <path to the output from scripts/launch_jobs.py> <path to where the output of ./bin/ReadEventData should be saved>

```

Outputfile of the emulator-run contains a histograms named as FastControlDriver_Fractions which contains event throttle fractions in bin-22. So only files needed to create final throttle vs truncation plot- output files from emulator-run, and readEvent.

### Create the ROC curve for truncation vs throttling

```
python scripts/tmpAnalyseHists2.py <path to emulator run files> <path to output from readEvent data> <string for png file>

```

There are no simple steps to edit scripts/tmpAnalyseHists2.py. One has to go through the code and understand it and then make the changes. It reads multiple run file sper point, take the average and calculate the error.


### Steps to run an example using full chain-
```
python scripts/launch_jobs.py --job condor
```
If it does not submit the jobs, check if do_submission list inside the code contains the correct string which is being executed later on

Read the event data - count number of truncations

```
python scripts/readProcess.py /eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/DoubleThresholdSystem /eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/DoubleThresholdSystem/ProcessedFiles
```
Make ROC curve

```
python scripts/tmpAnalyseHists2.py /eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/DoubleThresholdSystem /eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/ThrottleVsTruncation/PseudoEvents/DoubleThresholdSystem/ProcessedFiles /eos/user/k/kalpana/www/folder/HGCAL_TDAQ/Plots/ThrottleVsTruncation/ Pseudo_TwoBufferLevels

```

 