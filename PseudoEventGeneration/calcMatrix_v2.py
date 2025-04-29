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
from numpy.random import seed
from numpy.random import randn
                                                                                                                                                                         
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

# reading input array files
Events1=uproot.open("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/PseudoEvents_Gen/D86/AfterQT_D86_100k_7854_basedOnPseudoEvents_16March2023.root")#AfterQT_D86_100k_7854_Correct_23July22.root")#["EventTree"] PseudoEvents_Gen/D86/AfterQT_D86_1M_7854_CorrectedCorrelationMat_22Oct22.root

mycache = {}
orig_modules_arrays=np.empty((13500,100000),dtype='int32')
count=0
count1=0
count2=0
count3=0
count4=0
for ib in range(0,13500,3):
    
        name='%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
        print(name)
        a=np.array(Events1["tree"]["n10_Module_%s" %name].array()) #("n10_Module_%s" %name)[0:50000]
        #count+=1
        a = a[0:100000]
        b=np.array(Events1["tree"]["n20_Module_%s" %name].array()) #Events1.array("n20_Module_%s" %name)[0:50000]
        if(np.std(np.array(b))==0):
            print(name)
            b=randn(100000)
        b = b[0:100000]
        c=np.array(Events1["tree"]["n30_Module_%s" %name].array()) #Events1.array("n30_Module_%s" %name)[0:50000]
        c = c[0:100000]
        orig_modules_arrays[ib]=a
        orig_modules_arrays[ib+1]=b
        orig_modules_arrays[ib+2]=c
        print(name)
        count+=1
print(count1)
corr1 = np.cov(orig_modules_arrays)
np.savetxt("./PseudoBasedPseuodoEvents_CorrMatrix_15kby15k_LatestNtuples_D86_Fe10_w100k_PaulChecks_18Mar22_usingCovarianceMat.txt",corr1)
print(np.linalg.eig(corr1))
corr1[corr1 == -inf] = 0.1
#corr1[corr1<=0]= 0.01
corr1[~np.isfinite(corr1)] = 0.1
print('original')
print(corr1)
#corr1[np.linalg.eig(corr1)<0]=abs(np.linalg.eig(corr1))
#L= np.linalg.cholesky(
L=la.cholesky(corr1, lower=True)
print(L)
print('...')
print(np.matmul(L,np.transpose(L)))
# print(corr1)
# print(L)
np.savetxt("./PseudoBasedPseuodoEvents_usingCov_LowerDecomMatrix_correlationMatrix_15kby15k_LatestNtuples_D86_Fe10_18March23.txt",L)
