## Slink checks

**For figures shown in [19 Jan 2022 DAQ: Architecture and implementation](https://indico.cern.ch/event/1107853/contributions/4693732/attachments/2375160/4057222/HGCAL-190122.pdf)**.

Using git revision: ``

Perform two runs of the emulator, with different BE mapping files. The first from Taylan/Bora, and the second from a simple algorithm developed by Paul:

```sh
./bin/HGCalBufferModel numberOfBx=100000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"BEDAQ_192SlinksToLpGBT_16FPGA_Mapping.json\" number=1 outputname=\"HGCalBufferModel_FEV8_MCFD2V8_BE192_SL25.root\"
```
```sh
./bin/HGCalBufferModel numberOfBx=100000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"BEDAQ_192SlinksToLpGBT_16FPGA_Mapping.json\" number=1 outputname=\"HGCalBufferModel_FEV8_MCFD2V8_BETS16_SL25.root\"
```

Then to make comparison plots:

```sh
python scripts/genericPlotHists.py
```

**Update for 16 Feb:**

Adding:

```sh
./bin/HGCalBufferModel numberOfBx=100000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"BEDAQ_192SlinksToLpGBT_16FPGA_Mapping_V9.1.json\" feMappingFile=\"FeMappingV9.json\" hitModifierFile=\"	modifier_config_fixdensity2_V9.json\" number=1 outputname=\"HGCalBufferModel_FEV9_MCFD2V9_BE192V9p1_SL25.root\" | tee HGCalBufferModel_FEV9_MCFD2V9_BE192V9p1_SL25.log

./bin/HGCalBufferModel numberOfBx=100000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"TempSorting14.json\" feMappingFile=\"FeMappingV9.json\" hitModifierFile=\"modifier_config_fixdensity2_V9.json\" number=1 outputname=\"HGCalBufferModel_FEV9_MCFD2V9_BETS14_SL25.root\" | tee HGCalBufferModel_FEV9_MCFD2V9_BETS14_SL25.log

./bin/HGCalBufferModel numberOfBx=100000 filename=\"/eos/cms/store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root\" slink25GbitPerSec=true beMappingFile=\"TempSorting15.json\" feMappingFile=\"FeMappingV9.json\" hitModifierFile=\"modifier_config_fixdensity2_V9.json\" number=1 outputname=\"HGCalBufferModel_FEV9_MCFD2V9_BETS15_SL25.root\" | tee HGCalBufferModel_FEV9_MCFD2V9_BETS15_SL25.log
```
