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
#import covariance_module
def readRandGaus(min_,max_,fin):
    gc.enable()
    gc.collect()
    L = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/inputs/LowerTraingular_Matrix_scipy_method_15kby15k_LatestNtuples_D86_Fe10.txt")
    # print(corr1)
    # corr1[corr1 == -inf] = 0.1
    # corr1[~np.isfinite(corr1)] = 0.1
    # # # print(np.diagonal(corr1))
    mycache1 = {}                                                                         
    #L= covariance_module.covariance_()
    # L=la.cholesky(corr1, lower=True)
    #L[~np.isfinite(L)] = 0
    #L[L == -inf] = 0
    Rg_events = uproot.open(fin)#["tree"]
    layer=[]
    u=[]
    v=[]
    typee=[]
    nCells=[]
    nHalfHgcrocs=[]
    nMaxWords=[]
    data = np.loadtxt("/home/work/kalpana1/public/BE_DAQ_HGCAL_22/CMSSW_10_2_0_pre3/src/PseudoEventMachinery/luv_ncells_map_cmssw_D86_V10.txt")
    for im in range(4500):
        layer.append(int(data[im][0]))
        u.append(int(data[im][1]))
        v.append(int(data[im][2]))
        typee.append(int(data[im][3]))
        nCells.append(int(data[im][5]))
        nHalfHgcrocs.append(int(data[im][6]))
    RG_arrays=np.empty((13500,50000),dtype='float32')
    count=0
    for ib in range(0,13500,3):
        #if(ib<5066):
        name='%i_%i_%i_%i' %(layer[count],typee[count],u[count],v[count])                           
        a=np.array(Rg_events["tree"]["n10_Module_%s" %name].array()) #g_events.lazyarray("n10_Module_%s" %name,cache=mycache1)
        #elif(ib>=5066 and ib<10132):
        #name='%i_%i_%i_%i' %(layer[ib-5066],typee[ib-5066],u[ib-5066],v[ib-5066])
        b=np.array(Rg_events["tree"]["n20_Module_%s" %name].array()) #Rg_events.lazyarray("n20_Module_%s" %name,cache=mycache1)
        #elif(ib>=10132):
        #name='%i_%i_%i_%i' %(layer[ib-10132],typee[ib-10132],u[ib-10132],v[ib-10132])
        c=np.array(Rg_events["tree"]["n30_Module_%s" %name].array()) #Rg_events.lazyarray("n30_Module_%s" %name,cache=mycache1)
        RG_arrays[ib]=a[min_:max_]
        RG_arrays[ib+1]=b[min_:max_]
        RG_arrays[ib+2]=c[min_:max_]
        count+=1
        #RG_arrays[ib]=b[min_:max_]
    #np_RG_arrays = np.array(RG_arrays,dtype='float32')
    #print(RG_arrays[0])
    #RG_arrays=np.array(RG_arrays1)
    mycache1.clear()
    X=np.matmul(L,RG_arrays)
    print(X)
    return X
