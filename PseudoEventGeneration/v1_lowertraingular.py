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


corr1 = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/inputs/v1_correlationMatrix_15kby15k_LatestNtuples_D86_Fe10.txt")#PseudoEventMachinery/CoVMatrix_15kby15k_LatestNtuples_D86_Fe10_w100k.txt")
corr2 = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/CoVMatrix_15kby15k_RandGaussian_D86_Fe10_w100k_04Mar23.txt")#inputs/v1_correlationMatrix_15kby15k_LatestNtuples_D86_Fe10.txt")#LowerDecomMatrix_correlationMatrix_15kby15k_LatestNtuples_D86_Fe10.txt")
# corr1[corr1 == -inf] = 0.1
# corr1[~np.isfinite(corr1)] = 0.1
#L1=la.cholesky(corr1, lower=True)
#L1 = np.linalg.cholesky(corr1)
#print(corr1==corr2)
print('before')
print(corr2)
print('after')
print(corr1)
print('ratio')
print(corr2/corr1)
corr1= corr1.round(1)
corr2=corr2.round(1)

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
L = corr2-corr1
L1 =la.cholesky(corr1, lower=True)
L2 =la.cholesky(corr2, lower=True)
l22 =L2-L1
np.savetxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/inputs/LowerTraingular_Matrix_numpy_method_15kby15k_LatestNtuples_D86_Fe10_corr_ratio_beforeVsAfter_Cov_10March23.txt",l22)
print('diff')
print(L)
print('...')
print(corr1==corr2)

print('before')
print(corr2)
print('after')
print(corr1)
print('ratio')
print(corr2/corr1)

print('LLt')
print(l22)
print(L1==L2)
#print(np.matmul(L,np.transpose(L)))
