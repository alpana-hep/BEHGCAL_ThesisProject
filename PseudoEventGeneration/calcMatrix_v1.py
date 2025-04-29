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
data = np.loadtxt("./luv_ncells_map_cmssw_D49_V9.txt") ## to get the luvtype information for each modules
for im in range(4851):
    layer.append(int(data[im][0]))
    u.append(int(data[im][1]))
    v.append(int(data[im][2]))
    typee.append(int(data[im][3]))

# reading input array files
Events1=uproot.open("./PseudoEvents_Gen/D49/AfterQT_D49_100k_7854_Correct_23July22.root")#["EventTree"]

mycache = {}
orig_modules_arrays=np.empty((14553,100000),dtype='int32')
count=0
count1=0
count2=0
count3=0
count4=0
for ib in range(0,14553,3):    
        name='%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])
        print(name)
        a=np.array(Events1["tree"]["n10_Module_%s" %name].array()) #("n10_Module_%s" %name)[0:100000]
        #count+=1
        a = a[0:100000]
        if(np.std(np.array(a))==0):
            print(name)
            a=randn(100000)
        b=np.array(Events1["tree"]["n20_Module_%s" %name].array()) #Events1.array("n20_Module_%s" %name)[0:100000]
        if(np.std(np.array(b))==0):
            print(name)
            b=randn(100000)
        b = b[0:100000]
        c=np.array(Events1["tree"]["n30_Module_%s" %name].array()) #Events1.array("n30_Module_%s" %name)[0:100000]
        if(np.std(np.array(c))==0):
            print(name)
            c=randn(100000)
        c = c[0:100000]
        orig_modules_arrays[ib]=a
        orig_modules_arrays[ib+1]=b
        orig_modules_arrays[ib+2]=c
        print(name)
        count+=1
print(count1)
corr1 = np.cov(orig_modules_arrays)
np.savetxt("./CovMatrix_15kby15k_D49_AfterQT_25jul22.txt",corr1)

print(corr1)
#print(np.linalg.eig(corr1))
corr1[corr1 == -inf] = 0.1
#corr1[corr1 == -nan] = 0.1
#corr1[corr1 == -NaNs] = 0.1

# corr1[corr1<=0]= 0.01
corr1[~np.isfinite(corr1)] = 0.1
d = np.linalg.norm(corr1) * np.finfo(corr1.dtype).eps;
print(d,'second')
I = np.eye(len(corr1));

#corr1[np.linalg.eig(corr1)<0]=abs(np.linalg.eig(corr1))
#L= np.linalg.cholesky(
L=la.cholesky(corr1+d*I, lower=True)
print(L)
print('...')
print(np.matmul(L,np.transpose(L)))

# print(corr1)
# print(L)
np.savetxt("./LowerDecomp_correlationMatrix_15kby15k_D49_AfterQT_25jul22.txt",L)
