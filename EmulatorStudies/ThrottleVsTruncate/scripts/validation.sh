#!/bin/sh

echo "${EOS_ACCOUNT_PASSWORD}" | kinit -f ${EOS_ACCOUNT_USERNAME}@CERN.CH

mkdir results

python scripts/generateHitData.py \
  -i root://eoscms.cern.ch//store/user/agilbert/hgcal/NTuples/HGCalBEStudies_D49_ttbar_PU200_3iab_mipDef.root \
  --output hitData_D49_ttbar_PU200_3iab_mipDef \
  -e 100 \
  --modifier data/CMSSW/modifier_config_fixdensity2masking_partialfix.json

cp hitData_D49_ttbar_PU200_3iab_mipDef_occ_summary.txt results/