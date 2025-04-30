# HGCalBufferModel basic overview

[[_TOC_]]

## Introduction
HGCalBufferModel blah.

## Code classes

### Infrastructure

| | |
| :--- | :--- |
| Arguments | General `main()` argument handler, using `getopt`. Using this means the arguments are consistent for all executables. |
| `Clocks`| Generates the clock edges for the enabled frequencies. The frequencies which can be used are specified in the `Frequency` `enum`. |
| `Connection`| Provides templated "safe" data transfer between `Entity` processing blocks, meaning the output values from each block (which are the input values of other blocks) are not updated until all `Entity`es registered to be executed on the current clock edge have been run. |
| `Entity`| Base class for processing blocks; provides `Clock` and `Connection` interfaces. |
| `RandomSingleton`| Gives single random number generator for use throughout all `Entity`es, so that only one seed is needed.|
| `GlobalConfiguration`| Handles configuration values used widely, i.e. not just in one `Entity`.|
| `FastControlCounter`| Uses a sequence of `FastControlCommands` objects to keep track of BX, orbit, and L1Accept counters.|

### `Entity` objects

| | |
| :--- | :--- |
| `FastControlDriver`| Central processing block (partially corresponding to TCDS and partially to BE) which receives FC requests and issues FC commands to the FC streams. Handles HL-LHC beam orbit structure, trigger rules, calibration pulses, pre-L1Accepts, etc. Has `FastControlAnalyser` and `FastControlConfiguration` classes. Has `FastControlCommands` and `FastControlRequests` data structure classes.|
| `FastControlStream`| `Entity` corresponding to group of ECON-Ds which have the same FC commands. Fans out `FastControlCommands` after being temporarily converted to `FastControlLpgbtWord` objects for transmission to the FE; this can in principle cause loss of some fast commands. It also merges incoming `FastControlRequests`. Has `FastControlStreamConfiguration` class. Applies the "no zero-suppression" flag to L1Accepts.
| `DthChecker` | `Entity` corresponding to the DTH, although it only checks the received Slink packet for consistency and does not pass it on further. This corresponds to no backpressure.|

### Fast control

| | |
| :--- | :--- |
| `StreamConfiguration`| |

### HGCROC model

| | |
| :--- | :--- |
| `HGCHitProvider`| |
| `HgcrocAsics`| |
| `EventTree`| |
|`HgcrocModel` | This keeps track of the HGCROC buffer occupancy and discards L1Accepts if it overflows.|


### ECON-D model

| | |
| :--- | :--- |
| `EcondAsic` | ECON-D model entity, which simulates a single ECON-D. Takes in `FastControlLpgbtWord` objects (and converts these to `FastControlCommands` objects) and numbers of hits from `HgcrocAsics`, and outputs an array of elink words. Has `EcondAsicAnalyser` and `EcondAsicConfiguration` classes. Has `EcondAsicHeader` data structure class.|

### BE event builder buffer

| | |
| :--- | :--- |
| `LpgbtBuilder`| |
| `LpgbtPairSerialiser`| |

### BE Slink buffer

| | |
| :--- | :--- |
| `SlinkBuilder` | Entity which brings together all data packets from the lpGBT mapped to a given Slink and sends the total packet to the DTH. Receives `FastControlCommands` (to know when the L1Accepts occur) and `RamWords` from the lpGBT buffers, and outputs `SlinkWords`.|

### Data structures

| | |
| :--- | :--- |
| `WordFlags` | 8-bit status flags for data words.|
| `BasicWord` | `WordFlags` with a 32-bit data word.|
| `RamWord` | `WordFlags` with a two 32-bit data words, used for BRAM and URAM words.|
| `PacketCreator`| Creates packets of various data types.|
| `PacketChecker`| Checks packets of various data types.|
| `SlinkWord`| 128-bit word (four 32-bit words), used for the Slink.|
| `SlinkBoe`| `SlinkWord` specialised to begin-of-event Slink word.|
| `SlinkEoe`| `SlinkWord` specialised to end-of-event Slink word.|
|`LpgbtWord`| Placeholder for implementing reduction in FC commands when compressing to 6b8b (or whatever is used); should convert to and from a `FastControlCommands` object and keep track of which commands were dropped. :warning: **NOT YET IMPLEMENTED; DOES A SIMPLE COPY OF COMMANDS.**|

## Mapping

### FE mapping
Defines how the FE modules are mapped onto elinks and then lpGBT links, either
in single or paired lpGBTs. Definer reads in the FE file.

### BE mapping
Defines how the single lpGBTs are mapped into pairs and how all pairs are mapped to FPGAs and Slinks.

### Definitions
Takes mapping and creates `Entity` and `Connection` objects with correct structure.
The structure is a tree of nested objects in the order:
1. HGCal,
1. Sector (6 per HGCal),
1. Fpga (up to 28 per Sector),
1. Slink (up to 12 per Fpga),
1. Lpgbt(Pair),
1. FastControlStream,
1. EcondAsic.

:warning: **The use of Lpgbt and LpgbtPair is not yet fully implemented.**

| | |
| :--- | :--- |
| `HGCalDefinition`| |
| `SectorDefinition`| |
| `FpgaDefinition`| |
| `SlinkDefinition`| |
| `LpgbtDefinition`| |
| `LpgbtPairDefinition`| |
| `FastControlStreamDefinition`| |
| `EcondAsicDefinition`| |

# Executables
### `HGCalBufferModel`
### `SectorEcondAsicTest`