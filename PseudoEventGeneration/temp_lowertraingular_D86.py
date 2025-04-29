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


corr1 = np.loadtxt("/afs/cern.ch/user/k/kalpana/work/public/HGCAL_TDAQ/HGCAL_Buffer_Model/CMSSW_11_1_0_pre3/src/emulator_studies/PseudoEventMachinery/test_condor_jobs/v1_correlationMatrix_15kby15k_LatestNtuples_D86_Fe10.txt")
print(corr1)
corr1[corr1 == -inf] = 0.1
corr1[~np.isfinite(corr1)] = 0.1
# d = np.linalg.norm(corr1) * np.finfo(corr1.dtype).eps;
# print(d,'second')
# I = np.eye(len(corr1));
corr1[np.linalg.eigvals(corr1)<0]=0
L= la.cholesky(corr1, lower=True) #np.linalg.cholesky(corr1) #,lower=True)
#la.cholesky(corr1+d*I, lower=True)

#np.savetxt("/afs/cern.ch/user/k/kalpana/work/public/HGCAL_TDAQ/HGCAL_Buffer_Model/CMSSW_11_1_0_pre3/src/emulator_studies/LowerTraingular_Matrix_scipy_method_15kby15k_LatestNtuples_D86_Fe10.txt",L)

print(L)
print('...')
print(np.matmul(L,np.transpose(L)))

equal_arrays = np.array_equal(corr1,np.matmul(L,np.transpose(L)))
print(equal_arrays)
print('true')
