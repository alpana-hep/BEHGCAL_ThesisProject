
## make 1D overlay plots varying trigger rate
python scripts/tmpAnalyseHists_trig_1D_v1.py /eos/user/k/kalpana/folder/HGCAL_TDAQ/Plots/ThrottleVsTruncation/ final
##make 1D vs trigger rate - frca Econd trunc vs trigger rate
python scripts/tmpAnalyseHists_trig_1D.py /eos/user/k/kalpana/folder/HGCAL_TDAQ/Plots/ThrottleVsTruncation/ final

## Make ROC curve
python scripts/tmpAnalyseHists_trig.py /eos/user/k/kalpana/folder/HGCAL_TDAQ/Plots/ThrottleVsTruncation/ final
