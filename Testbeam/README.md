# Instructions to run DAQ machinary
### Raw data analysis-

Instructions to analyze raw data analysis

Run log -
```
https://docs.google.com/spreadsheets/d/1Pibh-i9MiomyZILa7h2V5lKhBdji1kgAn2BIiGKzuFw/edit#gid=1238056000
```

EOS area where raw files are stored -
```
/eos/cms/store/group/dpg_hgcal/tb_hgcal/2023/BeamTestSep/HgcalBeamtestSep2023
```

Repository with analysis scripts, plotting scripts and instructions on how to run the scripts

Link to repository-
```
https://gitlab.cern.ch/kalpana/hgcal10glinkreceiver/-/tree/Alp_dev?ref_type=heads
```

Readme file with instructions-
```
https://gitlab.cern.ch/kalpana/hgcal10glinkreceiver/-/blob/Alp_dev/readme.md?ref_type=heads
```
#### Instructions -
## script to analyze DAQ packet at raw level-

One should use lxplus9 to run this machinery.
### Setup the machinery
```
git clone -b Alp_dev https://gitlab.cern.ch/kalpana/hgcal10glinkreceiver.git .

```

```
src/RelayCheck.cpp
```
This script takes the relay number as the input, look for this particular relay in the directory 'dat' and loop over all the runs in relay one by one and fill the decoded information in the histograms and save histogram to the root file.

Note- 'dat' is soft link which should be pointing to eos directory. You can create using the following command-

```
ln -s /eos/cms/store/group/dpg_hgcal/tb_hgcal/2023/BeamTestSep/HgcalBeamtestSep2023 dat
```

###To compile and run the script -
```
g++  -Icommon/inc src/RelayCheck.cpp -Ioffline/inc -Iexpert/inc -o bin/RelayCheck.exe -lyaml-cpp -lhdf5_cpp -lhdf5 `root-config --libs --cflags`
./bin/RelayCheck.exe  <Relay number>
```

Directories "common,	offline, expert" contain the suppporting classes that are being use in RelayCheck.cpp and other *.cpp files. These helps in decoding different header from different parts of the system.
For each event, the packet is a 124 64-bit words array. First element of this array would be Slink header, following this would be BE header and ECON-D headers.
(more details to be added later or you can refer to respective documents on EDMS)

Output of the above command will be a root file with all the checks and corresponding histograms.
Run over all the relays at the same time-

```
source Analy_Relay_2d.sh
```

## plotting scripts

'makePlots_2D.C' to create png/pdf for 2D plots and 'makePlots_1D.C' for 1D pltos. string vector  'baseline' defined in these scripts contains the list of all the plots that are to be made.

Arguments for these scripts-
```
root -q -b 'makePlots_1D.C("path to save png/pdf files","input root file with histograms","relay number")'
root -q	-b 'makePlots_2D.C("path to save png/pdf files","input root file with histograms","relay number")'

```
submit jobs for plotting all the plots0

```
source run_Relay_2d.sh
source run_Relay_1d.sh
```


## Example to run the analysis script and creates the plot-

```
./bin/RelayCheck2.exe Relay_check_rawHeaders_1695480564_link1.root 1 1695480564 > out_1695480564_link1.log &
root -q -b 'makePlots_1D.C("runWithDifferentFirmware","West","Relay_check_rawHeaders_1695480564_link2.root","1695480564")'
root -q -b 'makePlots_2D.C("runWithDifferentFirmware","West","Relay_check_rawHeaders_1695480564_link2.root","1695480564")'
```

Plotting scripts-
```
https://gitlab.cern.ch/kalpana/hgcal10glinkreceiver/-/blob/Alp_dev/makePlots_1D.C?ref_type=heads
https://gitlab.cern.ch/kalpana/hgcal10glinkreceiver/-/blob/Alp_dev/makePlots_2D.C?ref_type=heads
```

Plots on the webpage-
```
https://kalpana.web.cern.ch/kalpana/folder/HGCAL_TDAQ/Plots/Aug2023_TBHGCAL_CERN/rawDataStudies/OrbitCounter_MisMatch/
```
More details

gDoc for all the information (path, code, plots, summary)

https://docs.google.com/document/d/19zX4xVA6Y2sF5N6qavhSQRWzpF16jCBG9I_3waIQNB8/edit?usp=sharin

Gitlab repository with analyzer scripts plus plotting scripts

https://gitlab.cern.ch/kalpana/hgcal10glinkreceiver/-/tree/Alp_dev?ref_type=heads




### Here are the instructions to run over an example for HGCAL TB analysis-
* Pedetsal studies
gDoc for all the information-
```
https://docs.google.com/document/d/1oteC0YNgPVWrxyXO6WsO24KYG6qW4GTOIQMMr4alRf0/edit?usp=sharing
```
github link for all scripts-
```
https://github.com/alpana-hep/HGCAL_TB_Analysis_2023/tree/AnalysisScripts
```


