
#import numpy as np
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
#from sklearn.preprocessing import QuantileTransformer
from numpy.random import seed
from numpy.random import randn
#from rootpy.tree import Tree, TreeModel, FloatCol, IntCol, ULongCol
from rootpy import stl
min_ = int(sys.argv[2])
seed_=int(sys.argv[1])
out_fname=sys.argv[3]

fout = ROOT.TFile(out_fname, 'RECREATE')

tree = ROOT.TTree('tree', 'tree')

layer=[]
u=[]
v=[]
typee=[]
nCells=[]
nHalfHgcrocs=[]
nMaxWords=[]
nMinWords=[]
data = np.loadtxt("./luv_ncells_map_cmssw_D49_V9.txt")
for im in range(4851):
    layer.append(int(data[im][0]))
    u.append(int(data[im][1]))
    v.append(int(data[im][2]))
    typee.append(int(data[im][3]))
    nCells.append(int(data[im][5]))
    nHalfHgcrocs.append(int(data[im][6]))

random_number=[]
random_list=[]
n = 100000
seed_1 = seed_
seeed = seed_1/n
#seeed = int(seed%n)
for im in range(4851):
    seed(im+seeed)
    random_number=randn(n)
    #name='%i_%i_%i_%i' %(layer_M[im],typee_M[im],u_M[im],v_M[im])
    #if(im<5066):
    name='%i_%i_%i_%i' %(layer[im],typee[im],u[im],v[im])
    nhitss_10= np.array(random_number,dtype=[('n10_Module_%s' %name, np.float32)])
    array2tree(nhitss_10,tree=tree)
    
    seed((im+1)+seeed)
    random_number=randn(n)
    
    nhitss_20= np.array(random_number,dtype=[('n20_Module_%s' %name, np.float32)])
    array2tree(nhitss_20,tree=tree)
    seed((im+2)+seeed)
    nhitss_30= np.array(random_number,dtype=[('n30_Module_%s' %name, np.float32)])
    array2tree(nhitss_30,tree=tree)

    #array2tree(nhitss,tree=tree)


fout.cd()
tree.Write()
fout.Close()
