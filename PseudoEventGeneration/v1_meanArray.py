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
infname1=sys.argv[1]
infname2=sys.argv[2]
out_fname = sys.argv[3] ## output file will contain similar 5k arrays saved after inverse qt
max_=int(sys.argv[4])
fout = ROOT.TFile(out_fname, 'RECREATE')
tree = ROOT.TTree('tree', 'tree')
Events=uproot.open(infname1)
#corrX_events = uproot.open(infname2)["tree"]
data = np.loadtxt(infname2) #"./luv_ncells_map_cmssw_D86_V10.txt")

layer=[]
u=[]
v=[]
typee=[]
nCells=[]
nHalfHgcrocs=[]
nMaxWords=[]

for im in range(len(data)):
    layer.append(int(data[im][0]))
    u.append(int(data[im][1]))
    v.append(int(data[im][2]))
    typee.append(int(data[im][3]))
    nCells.append(int(data[im][5]))
    nHalfHgcrocs.append(int(data[im][6]))


# list_array=[]
# list_array.append(nCells[im])
# list_array.append(nHalfHgcrocs[im])
# print(min(list_array))
for im in range(len(data)):
    name='%i_%i_%i_%i' %(layer[im],typee[im],u[im],v[im])
    print(name)
    n10_hits= np.array(Events["tree"]["n10_Module_%s" %name].array())
    n20_hits= np.array(Events["tree"]["n20_Module_%s" %name].array())
    n30_hits= np.array(Events["tree"]["n30_Module_%s" %name].array())
    # list_array=[]
    # list_array.append(nCells[im])
    # list_array.append(nHalfHgcrocs[im])
    # print(min(list_array),'min')
    cell_range= nHalfHgcrocs[im] #min(list_array)
    n10_hits1=[]
    n20_hits1=[]
    n30_hits1=[]
    for i in range(max_):
        n10 = n10_hits[i]
        n20 = n20_hits[i]
        n30 = n30_hits[i]
        if(n10>cell_range):
            print(n10, n20, n30, 'n10')
            n10 = cell_range
            print(n10, n20, n30, 'n10')
        if(n20>cell_range):
            print(n10, n20, n30, 'n20')
            n20 = cell_range
            print(n10, n20, n30, 'n20')
        if(n30>cell_range):
            print(n10, n20, n30, 'n30')
            n30 = cell_range
            print(n10, n20, n30, 'n30')
        if(n10+n20+n30>cell_range):
            print(n10, n20, n30, 'n30before sum')
            n30 = n30 - (n10+n20+n30-cell_range)
            print(n10, n20, n30, 'n30after sum')

	if(n30<0):
            
            n30 = 0            
        if(n10+n20+n30>cell_range):
            print(n10, n20, n30, 'n20before sum')
            n20 = n20 - (n10+n20+n30-cell_range)
            print(n10, n20, n30, 'n20after sum')
        if(n20<0):
            n20 =0

        if(n10+n20+n30>cell_range):
            print(n10, n20, n30, 'n10before sum')
            n10 = n10 - (n10+n20+n30-cell_range)
            print(n10, n20, n30, 'n10after sum')


        if(n10<0):
            n10 =0

        if(int(n10)>cell_range or int(n20)>cell_range or int(n30)>cell_range or int(n10+n20+n30)>cell_range):
            print(n10, n20, n30, 'final')
        
        n10_hits1.append(n10)
        n20_hits1.append(n20)
        n30_hits1.append(n30)

    
    final_hits_n10=np.array(n10_hits1,dtype=[('n10_Module_%s' %name, np.int32)])
    array2tree(final_hits_n10,tree=tree)
    final_hits_n20=np.array(n20_hits1,dtype=[('n20_Module_%s' %name, np.int32)])
    array2tree(final_hits_n20,tree=tree)
    final_hits_n30=np.array(n30_hits1,dtype=[('n30_Module_%s' %name, np.int32)])
    array2tree(final_hits_n30,tree=tree)



fout.cd()
tree.Write()
fout.Close()
