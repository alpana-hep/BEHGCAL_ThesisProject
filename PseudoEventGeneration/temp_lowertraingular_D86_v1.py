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


corr1 = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/CoVMatrix_15kby15k_LatestNtuples_D86_Fe10_w100k_PaulChecks_01Mar22.txt")
print(corr1)
corr1[corr1 == -inf] = 0.1
corr1[~np.isfinite(corr1)] = 0.1
# diag = np.sqrt(np.diag(np.diag(corr1)))
# gaid = np.linalg.inv(diag)
# corl1 = np.matmul(S,gaid)
# corl = np.matmul(gaid,corl1)#np S @ gaid
# d = np.linalg.norm(corl) * np.finfo(corl.dtype).eps;
# print(d,'second')
# I = np.eye(len(corl));
# # corr1[corr1 == -inf] = 0.1
# # corr1[~np.isfinite(corr1)] = 0.1
# # diag = np.sqrt(np.diag(np.diag(corr1)))
# # gaid = np.linalg.inv(diag)
# # corl1 = np.matmul(S,gaid)
# # corl = np.matmul(gaid,corl1)#np S @ gaid
# # d = np.linalg.norm(corl) * np.finfo(corl.dtype).eps;
# # print(d,'second')
# I = np.eye(len(corl));
corr1[np.linalg.eigvals(corr1)<0]=0.1
#corr1[np.linalg.eig(corr1)<0]=abs(np.linalg.eig(corr1))
# try:
#     L = np.linalg.cholesky(corr1)
# except np.linalg.LinAlgError:
#     L = nearestPD(self.P)
regularized_X = corr1 + np.eye(corr1.shape[0]) * 1e-14
L=np.linalg.cholesky(regularized_X) #la.cholesky(corr1, lower=True) #np.linalg.cholesky(corr1) #,lower=True)
#L = la.cholesky(corr1+d*I, lower=True)

np.savetxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/inputs/PseudoEvents_LowerTraingular_Matrix_numpy_method_15kby15k_LatestNtuples_Fe10_D86_10March23.txt",L)

print(L)
print('...')
print(np.matmul(L,np.transpose(L)))

print('orig')
print(corr1)
