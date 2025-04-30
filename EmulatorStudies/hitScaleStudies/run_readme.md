## Instrucitons to run the emulator for different cases:

### Setup the machinery-
```
cmsrel CMSSW_11_1_0_pre3
cd CMSSW_11_1_0_pre3/src
git clone -b HitScaleStudies https://gitlab.cern.ch/kalpana/HGCalBufferModel.git .
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

### Scaling the hits

In these studies, I scan different ways to scale/change the occupancies from module and study its impact on data losses. arguments of the interest-
```
econdHitScaling - "If true, then scaling the hits by a number - econdHitScalingArg/1000"
econdHitScalingArg - " Scaling arguments"
econdHitAdditiveArg - " % fraction of nCells added to the occupancies "
econdHitlayerArg - Which layer we should change the hits
econdScintHitScalingArg - Turn it on if only wants to change scintillator hits
```
For more details: https://gitlab.cern.ch/kalpana/HGCalBufferModel/-/blob/HitScaleStudies/src/EcondAsic.cc

### Launch the jobs :
```
cd condor_jobs/
python makeCondorSubmit.py
python driver_submitjobs.py

```
Shell script -
```
run_myprog.sh
```

### Plotting scripts
Check the files under the 'PlottingScripts'. Some scripts make the overlay plots, some reads the histograms, or generate graphs.

https://gitlab.cern.ch/kalpana/HGCalBufferModel/-/tree/HitScaleStudies/PlottingScripts
