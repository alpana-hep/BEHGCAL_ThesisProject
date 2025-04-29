import gc
import sys
import ROOT
import scipy.linalg as la
import numpy as np
from random import seed
from random import random
from random import gauss
import root_numpy
from root_numpy import random_sample, array2tree, array2root
import uproot
from numpy import inf

corr1 = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/CoVMatrix_15kby15k_LatestNtuples_D86_Fe10_w100k_beforeCorrTotalNhits_01Mar22.txt")
corr2 = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/inputs/v1_correlationMatrix_15kby15k_LatestNtuples_D86_Fe10.txt")
# L1 = np.linalg.cholesky(corr1)
# L2 = np.linalg.cholesky(corr2)
# print(L1/corr2)
# L1= la.cholesky(corr1, lower=True)
# L2= la.cholesky(corr2, lower=True)
# corr1[corr1 == -inf] = 0.0
# corr2[corr2 == -inf] = 0.0

#diff_corr = corr2-corr1
#ratio_corr = corr2/corr1
#diff[diff == nan] = 0.0

# diff_L = L2-L1
# ratio_L = L2/L1
fout = ROOT.TFile("out_corrChecks_beforeCorrect.root", 'RECREATE')

layer=[]
u=[]
v=[]
typee=[]
nCells=[]
nHalfHgcrocs=[]
nMaxWords=[]
data = np.loadtxt("./luv_ncells_map_cmssw_D86_V10.txt") ## to get the luvtype information for each modules                                        
for im in range(4500):
    layer.append(int(data[im][0]))
    u.append(int(data[im][1]))
    v.append(int(data[im][2]))
    typee.append(int(data[im][3]))
    
count=0
hist_Origcorr_n10=[]
hist_Origcorr_n20=[]
hist_Origcorr_n30=[]

hist_Pseudocorr_n10=[]
hist_Pseudocorr_n20=[]
hist_Pseudocorr_n30=[]

hist_diff_OrigvsPseudo_n10=[]
hist_diff_OrigvsPseudo_n20=[]
hist_diff_OrigvsPseudo_n30=[]

for ib in range(4500):
    name1='OrigCorr_n10_Module_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_Origcorr_n10.append(ROOT.TH1F(name1,'Correlation matrix value -original:n10',15000,0,15000))#//,5000,0,5000))
    name1='PseudoCorr_n10_Module_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_Pseudocorr_n10.append(ROOT.TH1F(name1,'Correlation matrix value -Pseudo:n10',15000,0,15000))
    name1='Diff_Module_n10_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_diff_OrigvsPseudo_n10.append(ROOT.TH1F(name1,'Diff(orig-Pseudo):n10',15000,0,15000))

    name1='OrigCorr_n20_Module_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_Origcorr_n20.append(ROOT.TH1F(name1,'Correlation matrix value -original:n20',15000,0,15000))#//,5000,0,5000))                            
    name1='PseudoCorr_n20_Module_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_Pseudocorr_n20.append(ROOT.TH1F(name1,'Correlation matrix value -Pseudo:n20',15000,0,15000))
    name1='Diff_Module_n20_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_diff_OrigvsPseudo_n20.append(ROOT.TH1F(name1,'Diff(orig-Pseudo):n20',15000,0,15000))

    name1='OrigCorr_n30_Module_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_Origcorr_n30.append(ROOT.TH1F(name1,'Correlation matrix value -original:n30',15000,0,15000))#//,5000,0,5000))                            
    name1='PseudoCorr_n30_Module_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_Pseudocorr_n30.append(ROOT.TH1F(name1,'Correlation matrix value -Pseudo:n30',15000,0,15000))
    name1='Diff_Module_n30_%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    hist_diff_OrigvsPseudo_n30.append(ROOT.TH1F(name1,'Diff(orig-Pseudo):n30',15000,0,15000))
    count+=1

count=0
for im in range(0,13500,3):
    name='%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
    #hist_Origcorr_n10.append(
    for imm in range(13500):
        hist_Origcorr_n10[count].SetBinContent(imm,corr2[im][imm])
        hist_Origcorr_n20[count].SetBinContent(imm,corr2[im+1][imm])
        hist_Origcorr_n30[count].SetBinContent(imm,corr2[im+2][imm])
        hist_Pseudocorr_n10[count].SetBinContent(imm,corr1[im][imm])
        hist_Pseudocorr_n20[count].SetBinContent(imm,corr1[im+1][imm])
        hist_Pseudocorr_n30[count].SetBinContent(imm,corr1[im+2][imm])
        hist_diff_OrigvsPseudo_n10[count].SetBinContent(imm,corr2[im][imm]-corr1[im][imm])
        hist_diff_OrigvsPseudo_n20[count].SetBinContent(imm,corr2[im+1][imm]-corr1[im+1][imm])
        hist_diff_OrigvsPseudo_n30[count].SetBinContent(imm,corr2[im+2][imm]-corr1[im+2][imm])
    count+=1
    print(name)

fout.cd()
for ib in range(4500):
    hist_Origcorr_n10[ib].Write()
    hist_Origcorr_n20[ib].Write()
    hist_Origcorr_n30[ib].Write()

    hist_Pseudocorr_n10[ib].Write()
    hist_Pseudocorr_n20[ib].Write()
    hist_Pseudocorr_n30[ib].Write()

    hist_diff_OrigvsPseudo_n10[ib].Write()
    hist_diff_OrigvsPseudo_n20[ib].Write()
    hist_diff_OrigvsPseudo_n30[ib].Write()


# hist_Origcorr.Write()
# hist_AQTcorr.Write()
# hist_OrigcorrsVs_AQT.Write()
# hist_ratioOrigcorrsVs_AQT.Write()
# hist_OrigLowTraiCorr.Write()
# hist_AQTLowTraiCorr.Write()
# hist_OrigLowTraiCorrsVs_AQT.Write()
fout.Close()
# corr1[corr1 == -inf] = 0.1
# corr1[~np.isfinite(corr1)] = 0.1
# diag = np.sqrt(np.diag(np.diag(corr1)))
# gaid = np.linalg.inv(diag)
# corl1 = np.matmul(S,gaid)
# corl = np.matmul(gaid,corl1)#np S @ gaid
# d = np.linalg.norm(corl) * np.finfo(corl.dtype).eps;
# print(d,'second')
# I = np.eye(len(corl));
# #corr1[np.linalg.eigvals(corr1)<0]=0
# #corr1[np.linalg.eig(corr1)<0]=abs(np.linalg.eig(corr1))
# L=la.cholesky(corl+d*I, lower=True) #np.linalg.cholesky(corr1) #,lower=True)
# #la.cholesky(corr1+d*I, lower=True)
L = L1-corr2
np.savetxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/inputs/LowerTraingular_Matrix_numpy_method_15kby15k_LatestNtuples_D86_Fe10_corr_ratio_beforeVsAfter.txt",L)

print(L)
print('...')


#print(np.matmul(L,np.transpose(L)))
