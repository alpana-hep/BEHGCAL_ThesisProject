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
infname3=sys.argv[3]

out_fname = sys.argv[4] ## output file will contain similar 5k arrays saved after inverse qt
#max_=int(sys.argv[4])
#fout = ROOT.TFile(out_fname, 'RECREATE')
#tree = ROOT.TTree('tree', 'tree')
#Events=uproot.open(infname1)
#corrX_events = uproot.open(infname2)["tree"]
data = np.loadtxt(infname1) #"./luv_ncells_map_cmssw_D86_V10.txt")
data_20 =  np.loadtxt(infname2)
data_30 =  np.loadtxt(infname3)

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

layer_20=[]
u_20=[]
v_20=[]
typee_20=[]
nCells_20=[]
nHalfHgcrocs_20=[]
nMaxWords_20=[]

for im in range(len(data_20)):
    layer_20.append(int(data_20[im][0]))
    u_20.append(int(data_20[im][1]))
    v_20.append(int(data_20[im][2]))
    typee_20.append(int(data_20[im][3]))
    nCells_20.append(int(data_20[im][5]))
    nHalfHgcrocs_20.append(int(data_20[im][6]))

layer_30=[]
u_30=[]
v_30=[]
typee_30=[]
nCells_30=[]
nHalfHgcrocs_30=[]
nMaxWords_30=[]

for im in range(len(data_30)):
    layer_30.append(int(data_30[im][0]))
    u_30.append(int(data_30[im][1]))
    v_30.append(int(data_30[im][2]))
    typee_30.append(int(data_30[im][3]))
    nCells_30.append(int(data_30[im][5]))
    nHalfHgcrocs_30.append(int(data_30[im][6]))




signmaunCor_10 = np.sqrt(np.sum(np.square( nHalfHgcrocs)))
signmaCor_10 = np.sum(nHalfHgcrocs)
print("sigma_uncorr",signmaunCor_10)
print("sigma_corr",signmaCor_10)


signmaunCor_20 = np.sqrt(np.sum(np.square( nHalfHgcrocs_20)))
signmaCor_20 = np.sum(nHalfHgcrocs_20)

print("sigma_uncorr 20bit",signmaunCor_20)
print("sigma_corr 20bit ",signmaCor_20)

signmaunCor_30 = np.sqrt(np.sum(np.square( nHalfHgcrocs_30)))
signmaCor_30 = np.sum(nHalfHgcrocs_30)

print("sigma_uncorr 30bit",signmaunCor_30)
print("sigma_corr 30bit ",signmaCor_30)


    
# list_array=[]
# list_array.append(nCells[im])
# list_array.append(nHalfHgcrocs[im])
# print(min(list_array))
#with open(out_fname,'w') as f:
    
# for im in range(len(data)):
#     name='%i_%i_%i_%i' %(layer[im],typee[im],u[im],v[im])
#     print(name)
    
    # n10_hits= np.array(Events["tree"]["n10_Module_%s" %name].array())
    # n20_hits= np.array(Events["tree"]["n20_Module_%s" %name].array())        
    # n30_hits= np.array(Events["tree"]["n30_Module_%s" %name].array())
# f.write('%d\t'%(np.mean(n10_hits)))
# f.write('%d\t'%(np.std(n10_hits)))
#         f.write('%d\t'%(np.mean(n20_hits)))
#         f.write('%d\t'%(np.std(n20_hits)))
#         f.write('%d\t'%(np.mean(n30_hits)))
#         f.write('%d \n'%(np.std(n30_hits)))

# f.close()
    # list_array=[]
    # list_array.append(nCells[im])
    # list_array.append(nHalfHgcrocs[im])
    # print(min(list_array),'min')
    # cell_range= nHalfHgcrocs[im] #min(list_array)
    # n10_hits1=[]
    # n20_hits1=[]
    # n30_hits1=[]
    # for i in range(max_):
    #     n10 = n10_hits[i]
    #     n20 = n20_hits[i]
    #     n30 = n30_hits[i]
    #     if(n10>cell_range):
    #         print(n10, n20, n30, 'n10')
    #         n10 = cell_range
    #         print(n10, n20, n30, 'n10')
    #     if(n20>cell_range):
    #         print(n10, n20, n30, 'n20')
    #         n20 = cell_range
    #         print(n10, n20, n30, 'n20')
    #     if(n30>cell_range):
    #         print(n10, n20, n30, 'n30')
    #         n30 = cell_range
    #         print(n10, n20, n30, 'n30')
    #     if(n10+n20+n30>cell_range):
    #         print(n10, n20, n30, 'n30before sum')
    #         n30 = n30 - (n10+n20+n30-cell_range)
    #         print(n10, n20, n30, 'n30after sum')

    #     if(n30<0):
            
    #         n30 = 0            
    #     if(n10+n20+n30>cell_range):
    #         print(n10, n20, n30, 'n20before sum')
    #         n20 = n20 - (n10+n20+n30-cell_range)
    #         print(n10, n20, n30, 'n20after sum')
    #     if(n20<0):
    #         n20 =0

    #     if(n10+n20+n30>cell_range):
    #         print(n10, n20, n30, 'n10before sum')
    #         n10 = n10 - (n10+n20+n30-cell_range)
    #         print(n10, n20, n30, 'n10after sum')


    #     if(n10<0):
    #         n10 =0

    #     if(int(n10)>cell_range or int(n20)>cell_range or int(n30)>cell_range or int(n10+n20+n30)>cell_range):
    #         print(n10, n20, n30, 'final')
        
    #     n10_hits1.append(n10)
    #     n20_hits1.append(n20)
    #     n30_hits1.append(n30)

    
    # final_hits_n10=np.array(n10_hits1,dtype=[('n10_Module_%s' %name, np.int32)])
    # array2tree(final_hits_n10,tree=tree)
    # final_hits_n20=np.array(n20_hits1,dtype=[('n20_Module_%s' %name, np.int32)])
    # array2tree(final_hits_n20,tree=tree)
    # final_hits_n30=np.array(n30_hits1,dtype=[('n30_Module_%s' %name, np.int32)])
    # array2tree(final_hits_n30,tree=tree)



# fout.cd()
# tree.Write()
# fout.Close()
