import sys
import ROOT
import scipy.linalg as la
import numpy as np
from random import seed
from random import random
from random import gauss
from numpy import inf
import root_numpy
from root_numpy import random_sample, array2tree, array2root
import uproot
from sklearn.preprocessing import QuantileTransformer, quantile_transform
from numpy.random import seed
from numpy.random import randn
#from rootpy.tree import Tree, TreeModel, FloatCol, IntCol, ULongCol
#from rootpy import stl

M=4500 ## total number of modules                                                                                              
n= 50000 ## maximum length of arrays                                                                                         
## input aruguments
infname1=sys.argv[1]
infname2=sys.argv[2]

out_fname = sys.argv[3] ## output file will contain similar 5k arrays saved after inverse qt                                        
fout = ROOT.TFile(out_fname, 'RECREATE')
## output tree ###                                                                                                             
tree = ROOT.TTree('tree', 'tree')
min_=int(sys.argv[4])
max_=int(sys.argv[5])
### saving layer, u,v information                                                                                              
# events = uproot3.open("./luv_tree.root")["Eventtree"]
# layer = events.array("layer")
# u=events.array("U")
# v=events.array("V")
# typee= events.array("type")
layer=[]
u=[]
v=[]
typee=[]
nCells=[]
nHalfHgcrocs=[]
nMaxWords=[]

data = np.loadtxt("./luv_ncells_map_cmssw_D86_V10.txt")
for im in range(4500):
    layer.append(int(data[im][0]))
    u.append(int(data[im][1]))
    v.append(int(data[im][2]))
    typee.append(int(data[im][3]))
    nCells.append(int(data[im][5]))
    nHalfHgcrocs.append(int(data[im][6]))


##reading montecarlo simulated file                                                                                            
print('readin after fit hits file')
Events=uproot.open(infname1)#["tree"]
corrX_events = uproot.open(infname2)#["tree"]
# Events=uproot3.open("./../condor_jobs/condor_output/temp_submit/datapackets_afterfit_1M_00_01.root")["tree"]
# corrX_events = uproot3.open("./../condor_jobs/condor_output/temp_submit/run01/matmulti_0to1M.root")["tree"]

#sys.exit()
mycache={}
mycache1={}
## perform qt on afit arrays and inv qt on corrX arrays
qt= QuantileTransformer(n_quantiles=50000, output_distribution='normal', random_state=0,subsample = 50000)
qt1= QuantileTransformer(n_quantiles=50000, output_distribution='normal', random_state=0,subsample = 50000)

count=0
for im in range(M):
    name='%i_%i_%i_%i' %(layer[im],typee[im],u[im],v[im])    
    temp10_v2= np.array(Events["tree"]["n10_Module_%s" %name].array())#Events.lazyarray("n10_Module_%s" %name,cache=mycache)
    Nhits_np_n10= np.array(temp10_v2)#[min_:max_])
    Nhits_np_n10=Nhits_np_n10.reshape(-1,1)
    X_trans_qt_n10 = qt.fit_transform(Nhits_np_n10) #.transform(Nhits_np_n10)
    count+=1
    # final_hits_n101=np.array(X_trans_qt_n10,dtype=[('1n10_Module_%s' %name, np.float32)])
    # array2tree(final_hits_n101,tree=tree)    
    a_n10=np.array(corrX_events["tree"]["n10_Module_%s" %name].array()) #corrX_events.lazyarray("n10_Module_%s" %name,cache=mycache1)
    corrX_n10 =np.array(a_n10)#[min_:max_])    
    print(corrX_n10)
    corrX_n10=corrX_n10.reshape(-1,1)
    corrX_trans_qt_n10 = qt1.fit_transform(corrX_n10)
    # final_hits_n102=np.array(X_trans_qt_n10,dtype=[('2n10_Module_%s' %name, np.float32)])
    # array2tree(final_hits_n102,tree=tree)    
    X_inv_gauss_qt_n10= qt.inverse_transform(corrX_trans_qt_n10)  #+np.mean(Nhits_np_n10)    
    final_hits_n10=np.array(X_inv_gauss_qt_n10,dtype=[('n10_Module_%s' %name, np.int32)])
    array2tree(final_hits_n10,tree=tree)    
    temp20_v2=np.array(Events["tree"]["n20_Module_%s" %name].array())#Events.lazyarray("n20_Module_%s" %name,cache=mycache)
    Nhits_np_n20= np.array(temp20_v2)#[min_:max_])
    Nhits_np_n20[Nhits_np_n20 == -inf] = 0.1
    Nhits_np_n20[Nhits_np_n20 == inf] = 0.1
    Nhits_np_n20[~np.isfinite(Nhits_np_n20)]=0.1
    Nhits_np_n20=Nhits_np_n20.reshape(-1,1)                                                                                                                             
    X_trans_qt_n20 = qt.fit(Nhits_np_n20).transform(Nhits_np_n20)
    count+=1
    a_n20=np.array(corrX_events["tree"]["n20_Module_%s" %name].array()) #corrX_events.lazyarray("n20_Module_%s" %name,cache=mycache1)
    corrX_n20 =np.array(a_n20)#[min_:max_])    
    corrX_n20=corrX_n20.reshape(-1,1)
    corrX_trans_qt_n20 = qt1.fit_transform(corrX_n20)
    X_inv_gauss_qt_n20= qt.inverse_transform(corrX_trans_qt_n20)
    final_hits_n20=np.array(X_inv_gauss_qt_n20,dtype=[('n20_Module_%s' %name, np.int32)])
    array2tree(final_hits_n20,tree=tree)

    #t# quantile transformation on n30                                                                                                                               
    temp30_v2=np.array(Events["tree"]["n30_Module_%s" %name].array())#Events.lazyarray("n30_Module_%s" %name,cache=mycache)
    Nhits_np_n30= np.array(temp30_v2)#[min_:max_])
    Nhits_np_n30=Nhits_np_n30.reshape(-1,1)                                                                                                                             
    X_trans_qt_n30 = qt.fit(Nhits_np_n30).transform(Nhits_np_n30)
    count+=1
    a_n30=np.array(corrX_events["tree"]["n30_Module_%s" %name].array()) #corrX_events.lazyarray("n30_Module_%s" %name,cache=mycache1)
    corrX_n30 =np.array(a_n30)#[min_:max_])
    corrX_n30=corrX_n30.reshape(-1,1)
    corrX_trans_qt_n30 = qt1.fit_transform(corrX_n30)
    X_inv_gauss_qt_n30= qt.inverse_transform(corrX_trans_qt_n30)
    final_hits_n30=np.array(X_inv_gauss_qt_n30,dtype=[('n30_Module_%s' %name, np.int32)])
    array2tree(final_hits_n30,tree=tree)
    # corrX_n10 = np.array(corrX_n10)
    # corrX_n20= np.array(corrX_n20)
    # X = np.stack((corrX_n10,corrX_n20))
    # print(np.cov(X)) #corrX_n10,corrX_n20))
    # Y = np.stack((corrX_trans_qt_n10,corrX_trans_qt_n20))
    # print(np.cov(Y))
    # #print(np.cov(corrX_trans_qt_n10,corrX_trans_qt_n20))
    # Z = np.stack((X_inv_gauss_qt_n10,X_inv_gauss_qt_n20))
    # print(np.cov(Z))

    #print(np.cov(X_inv_gauss_qt_n10,X_inv_gauss_qt_n20))
          
    # b=Events.lazyarray("Module_%s" %name,cache=mycache)
    # Nhits_np= np.array(b)
    # Nhits_np=Nhits_np.reshape(-1,1)
    # X_trans_qt1 = qt.fit(Nhits_np).transform(Nhits_np)
    # count+=1
    # a=corrX_events.lazyarray("Module_%s" %name,cache=mycache1)
    # corrX =np.array(a)
    # corrX=corrX.reshape(-1,1)
    # X_inv_gauss_qt1= qt.inverse_transform(corrX)
    # X_inv_gauss_qt1 = X_inv_gauss_qt1.astype('int64')
    # final_hits=np.array(X_inv_gauss_qt1,dtype=[('Module_%s' %name, np.int64)])
    # array2tree(final_hits,tree=tree)
    if(im%100==0):
        print(im)
    #sys.exit()
print(count)


fout.cd()
tree.Write()
fout.Close()
