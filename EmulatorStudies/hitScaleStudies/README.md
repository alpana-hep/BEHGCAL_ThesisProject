## Code organisation

The code is compiled simply by invoking `make`. To delete the build files run `make clean`.

The source code for executables is located in the `test` directory and should have the `.cpp` extension. A file `test/X.cpp` will produce the executable `bin/X`. Header files should go in `interface`. All the library source code files in `src` should have the `.cc` extension, and will be compiled into a shared library, `lib/libHGCalBufferModel.so`. The executables will be linked against this library.

## Input summary

The table below summarises the available input ROOT and configuration files, and shows which can be combined in a consistent way (reading from left to right).

<table>
    <thead>
        <tr>
            <th>CMSSW</th>
            <th>NTuple</th>
            <th>FE Mapping</th>
            <th>Hit Modifier</th>
            <th>BE Mapping</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=1>47 layer (D86) </td>
            <td rowspan=1>HGCalBEStudies_ttbar_D86_12_1_X_PU200_20211106_mipDef.root</td>
            <td rowspan=1>FeMappingV10.json</td>
            <td rowspan=1>modifier_config_fixdensity2_V10.json</td>
            <td rowspan=1>BeMappingV10_[14/15/16]FPGA_simple.json BeMappingV10_16FPGA_random.json</td>
        </tr>
        <tr>
            <td rowspan=4>50 layer (D49)</td>
            <td rowspan=2>HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root (after partial corrections)</td>
            <td rowspan=1>FeMappingV9.json</td>
            <td rowspan=1>modifier_config_fixdensity2_V9.json</td>
            <td rowspan=1>BEDAQ_192SlinksToLpGBT_16FPGA_Mapping_V9.1.json TempSorting15.json TempSorting14.json</td>
        </tr>
        <tr>
            <!-- <td rowspan=3>50 layer (D49)</td> -->
            <!-- <td rowspan=2>HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root (after partial corrections)</td> -->
            <td rowspan=1>FeMappingV8.txt</td>
            <td rowspan=1>modifier_config_fixdensity2_V8.json</td>
            <td rowspan=3>BEDAQ_SlinksToLpGBT_16FPGA_Mapping_19032022_fixed_patched.json BEDAQ_192SlinksToLpGBT_16FPGA_Mapping.json BEDAQ_224SlinksToLpGBT_Mapping.json BEDAQ_312SlinksToLpGBT_Mapping.json BEDAQ_336SlinksToLpGBT_Mapping.json</td>
        </tr>
        <tr>
            <!-- <td rowspan=3>50 layer (D49)</td> -->
            <td rowspan=1>HGCalBEStudies_ttbar_D49_1120pre1_PU200_eolupdate_Jan21.root (before partial corrections)</td>
            <td rowspan=2>FeMappingV8.txt</td>
            <td rowspan=2>modifier_config_fixdensity2_BeforeNewPartialCorr.json</td>
            <!-- <td rowspan=1></td> -->
        </tr>
        <tr>
            <!-- <td rowspan=3>50 layer (D49)</td> -->
            <td rowspan=1>data/CMSSW/pseudo_ntuples_D49.txt (before partial corrections)</td>
            <!-- <td rowspan=1>FeMappingV8.txt</td> -->
            <!-- <td rowspan=1>modifier_config_fixdensity2_BeforeNewPartialCorr.json</td> -->
            <!-- <td rowspan=1></td> -->
        </tr>
    </tbody>
</table>

Note that for samples labelled with "before partial corrections" it is necessary to run with an extra option, setting `hitModifierReadUnmasked` to `false`. This instructs HGCHitProvider to read the branches containing the full, raw numbers of CMSSW hits, instead of the ones using our custom partial masking.

In the BE mappings, `BEDAQ_SlinksToLpGBT_16FPGA_Mapping_19032022_fixed_patched.json ` is an improved version of `BEDAQ_192SlinksToLpGBT_16FPGA_Mapping.json`.
## NTuple information

| Process | Conditions | Number of pseudo-events | File (local) | File (xrootd) |
| --- | --- | --- | --- | --- |
| TTBar | EOL | 110814 | /eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D86_ttbar_12_1_X_PU200_20210920_mipDef.root | root://eoscms.cern.ch//store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D86_ttbar_12_1_X_PU200_20210920_mipDef.root |
| TTBar | EOL | 119700 | /eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root | root://eoscms.cern.ch//store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root |
| TTBar | EOL | 60000 | /eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_ttbar_D49_1120pre1_PU200_eolupdate_Jan21.root | root://eoscms.cern.ch//store/user/agilbert/hgcal/NTuples/HGCalBEStudies_ttbar_D49_1120pre1_PU200_eolupdate_Jan21.root |
| NuGun | EOL-NoPU | 60000 | /eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_FlatRandomEGunProducer_D49_1120pre2_noPU_eolupdate_Jan21.root | root://eoscms.cern.ch//store/user/agilbert/hgcal/NTuples/HGCalBEStudies_FlatRandomEGunProducer_D49_1120pre2_noPU_eolupdate_Jan21.root |


## Ntuple content

| Branch | Type | Notes |
| -------| -----| ------|
| `event` | `long int` | EDM event number |
| `lumi` | `int` | EDM lumi section number |
| `run` | `int` | EDM run number |
| `pseudo_event` | `int` | Six possible values: `[+/-][1/2/3]`, sign indicates +/-z |
| `layer` | `vector<int>` | HGCAL layer 1-50 |
| `wafer_u` | `vector<int>` | Mapped wafer `u` index (`eta` for tileboard) |
| `wafer_v` | `vector<int>` | Mapped wafer `v` index (`phi` for tileboard) |
| `wafer_u_orig` | `vector<int>` | Original `u` index |
| `wafer_v_orig` | `vector<int>` | Original `v` index |
| `type` | `vector<int>` | 0=HDM, 1=LDM, 2-11=Scint |
| `nhits20` | `vector<int>` | Number of 20-bit hits (without TOA) |
| `nhits30` | `vector<int>` | Number of 30-bit hits (with TOA) |
| `nhitsBXM1` | `vector<int>` | Number of hits in which BX-1 > 2.5 MIP |
| `nhits20_defined` | `vector<int>` | As above, but only counting hits from valid geometry DetIds  |
| `nhits30_defined` | `vector<int>` | As above, but only counting hits from valid geometry DetIds |
| `nhitsBXM1_defined` | `vector<int>` | As above, but only counting hits from valid geometry DetIds |
| `nhits20_unmasked` | `vector<int>` | As above, but only counting hits from valid geometry DetIds that are not masked  |
| `nhits30_unmasked` | `vector<int>` | As above, but only counting hits from valid geometry DetIds that are not masked |
| `nhitsBXM1_unmasked` | `vector<int>` | As above, but only counting hits from valid geometry DetIds that are not masked |


## HGCalBufferModel usage

Configuration is via JSON file(s). All options have hardcoded default values. To see these, run:

```sh
./bin/HGCalBufferModel -h
```

Which produces the output:
```
{
  "beMappingFile": "BEDaq_SlinkMapping.json",
  "beMappingFileArg": false,
  "calOrbitPeriod": 0,
  "calOrbitPeriodArg": false,
  "debugCheckBits": 65535,
  "debugCheckBitsArg": false,
  "debugPrintBits": 0,
  "debugPrintBitsArg": false,
  "doAnalysisBits": 0,
  "doAnalysisBitsArg": false,
  "econdBufferLimit": 4000,
  "econdBufferLimitArg": false,
  "feMappingFile": "FeMappingV8.txt",
  "feMappingFileArg": false,
  "filename": "NullFilename",
  "filenameArg": false,
  "hitModifierFile": "modifier_config_fixdensity2_V8.json",
  "hitModifierFileArg": false,
  "hitScaleFactor": 1000,
  "hitScaleFactorArg": false,
  "label": "NullLabel",
  "labelArg": false,
  "number": 0,
  "numberArg": false,
  "numberOfBx": 1,
  "numberOfBxArg": false,
  "numberOfOrbitsArg": false,
  "nzsProbability": 0,
  "nzsProbabilityArg": false,
  "outputname": "NullOutputName.root",
  "outputnameArg": false,
  "program": "./bin/HGCalBufferModel",
  "randomSeed": 4294967295,
  "randomSeedArg": false,
  "slink25GbitPerSec": false,
  "slink25GbitPerSecArg": false,
  "triggerMethod": 0,
  "triggerMethodArg": false,
  "triggerRate": 750,
  "triggerRateArg": false
}
```

There are two methods for overriding the default options:

1. Pass the names of one or more JSON files as arguments. Each JSON file should contain a single JSON object (dictionary), in which the key values should correspond to one of the pre-defined options. If multiple JSON file names are given, each is parsed in turn, from left to right, and options specified in multiple files will take the value from the final file in which they appear.

2. Specify an option name and value directly as an argument, in the form `[option name]=[value]`. To ensure the value type is interpreted correctly, string values must be surrounded by double quotes, and it may be necessary to prevent the shell expanding these first, e.g.: `./bin/HGCalBufferModel filename=\"file.root\"`, or `./bin/HGCalBufferModel 'filename="file.root"'`.

Both approaches above can be combined. This can be useful when starting with a JSON file containing a large set of options, with one or two needing a modification for a particular run. Instead of creating a new JSON file, those one or two options can be overridden on the command line, e.g.: `./bin/HGCalBufferModel defaults.json filename=\"file.root\"`.

A full example, in which only a few options need to be changed from the default:

```
./bin/HGCalBufferModel numberOfBx=10000 \
 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root\" \
 slink25GbitPerSec=true \
 beMappingFile=\"BEDAQ_192SlinksToLpGBT_16FPGA_Mapping.json\" \
 number=1
```

A few special behaviours are implemented when parsing options:

 -  The bit-field arguments `debugCheckBits`, `debugPrintBits` and `doAnalysisBits` are stored internally as integers. However, it can be more convenient to input these as hex values. These can be given directly as an integer, e.g. `debugCheckBits=65535`, or as a string, e.g. `debugCheckBits=\"0xFFFF\"`, which will automatically be converted to an integer.
 - For each option `[X]` specified either in an input JSON file or as a command line argument, the parser will check if an option with label `[X]Arg` exists, and if so, set its value to `true`. In this way, it is possible to check later whether the option `[X]` was changed from its default value.

The final set of options are encoded in a JSON string and stored in the output ROOT file. A script is provided to print this option string to the screen:

```
python scripts/printConfig.py outputfile.root [new_config.json]
```

The second, optional, argument is the name a JSON file that will be created with the config extracted from this file. This can then be used as input for a new run.

## HGCHitProvider usage

Input can be a single TFile or a plain text file containing a list of TFiles:

```cpp
// single file
hitProvider.setFile("file.root", "beLinksNTuple/EventTree");

// list of files
hitProvider.setFileList("files.txt", "beLinksNTuple/EventTree");
```

By default, it will process events sequentially starting at event zero. Call `nextEvent()` to load the current event from disk into memory. The current event can be adjusted if needed:

```cpp
hitProvider.setFile("file.root", "beLinksNTuple/EventTree");
hitProvider.setCurrentEvent(100);
bool eventOK = true; // nextEvent() will return false if we have reached the end of the input files
eventOK = hitProvider.nextEvent(); // loads event 100
eventOK = hitProvider.nextEvent(); // loads event 101 ...
```

A random read mode is also available. A configurable-size random list of event numbers is generated, and these are then processed sequentially upon calling `nextEvent`. Once the end of the list is reached a new random list is generated automatically. There is therefore no limit on the number of events that can be processed. Example usage:

```cpp
hitProvider.setReadMode(1); // 0 = default (sequential), 1 = random
hitProvider.setRandomFrac(0.01); // Generates random batches of [randomFrac] x [Total] events and processes these sequentially. In this example process a random 1% of events in each pass through the input files.
```

## Scripts

### NTuple workflow

Generate masking json file: `python scripts/plotMasking.py`

Run CMSSW job to generate CMSSW json files: `cmsRun test/exampleConfig.py events=1 files=resources/files_ttbar_D49_1120pre1_PU200_eolupdate_qua_20200723_x.txt output=HGCalBEStudies_TTbar_EOL.root`



### generateHitData.py

Demonstration of reading the flat ntuples via `HGCHitProvider` in python. Can be configured to use a particular modifier config, e.g:

```sh
for TYPE in notransform fixdensity pessimistic optimistic; do python scripts/generateHitData.py -i root://eoscms.cern.ch//store/user/agilbert/hgcal/NTuples/HGCalBEStudies_EOL_9k_130620.root --output hitData_${TYPE} -e 500 --modifier data/CMSSW/modifier_config_${TYPE}.json 2>&1 | tee hit_averages_EOL_${TYPE}.txt; done
```

The output ROOT file contains histograms over the original and transformed numbers of hits - one for each module. A text file is also produced containing the average number of hits for each module, as well as the averages from the previous studies that led to the current mapping file.

### plotPacketWords.py

Plots the histogram output of `SectorEcondAsicTest.cpp`.

Arguments:

 - `--hist [AveragePacketWords/AveragePacketWordsNorm]`: name of the histogram input to draw
 - `--file [SectorEcondAsicTest.root]`: input ROOT file
 - `--check`: run checks on rates per elink
 - `--post`: postfix string to label output files
 - `--modifier`: hit modifier config file - should be consistent with the one used in the input ROOT file
 - `--draw-mode [0/1/2]`: Set to `1` to draw different markers for different module types, set to `2` to only plot modules that are over-budget

Optional input: `hitData.txt` output from `generateHitData.py` above can supplied via `--hit-data` option to create an additional plot of the average hit rates from our study compared to those recorded previously.

Example usage:

```sh
for TYPE in fixdensity; do for MODE in 1 2; do python scripts/plotPacketWords.py --hist AveragePacketWords --file SectorEcondAsicTest_V8_EOL_${TYPE}.root --check --post _EOL_${TYPE} --modifier data/CMSSW/modifier_config_${TYPE}.json --draw-mode ${MODE} --hit-data hitData_${TYPE}.txt; done; done
```

### plotConstraints.py

Visual representation and summary of elink rate violations.

Arguments:

 - `--rate-summary-file [rate_summary_file_EOL_fixdensity.txt]`: output from `plotPacketWords.py` above
 - `--mapping-file [data/FE/FeMappingV8.txt]`: standard mapping file
 - `-layout-file [data/FE/MbLayoutV1.json]`: json file which gives association of modules to wagons and engines
