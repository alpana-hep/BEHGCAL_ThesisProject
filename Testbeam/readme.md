## script to analyze DAQ packet at raw level-
One can use lxplus9 to run this machinery.
'src/RelayCheck.cpp' takes the relay number as the input, look for this particular relay in the directory 'dat' and loop over all the runs in relay one by one. For each event, the packet is a 124 64-bit words array.
First element of this array would be Slink header, following this would be BE header and ECON-D headers. (more details to be added later or you can refer to respective documents on EDMS)

Note- 'dat' is soft link which should be pointing to eos directory. You can create using the following command-

```
ln -s /eos/cms/store/group/dpg_hgcal/tb_hgcal/2023/BeamTestSep/HgcalBeamtestSep2023 dat
```

###To compile and run the script -
```
g++  -Icommon/inc src/RelayCheck.cpp -Ioffline/inc -Iexpert/inc -o bin/RelayCheck.exe -lyaml-cpp -lhdf5_cpp -lhdf5 `root-config --libs --cflags`
./bin/RelayCheck.exe  <Relay number>
```

Directories "common,	offline, expert" contain the suppporting classes that are being use in RelayCheck.cpp file
