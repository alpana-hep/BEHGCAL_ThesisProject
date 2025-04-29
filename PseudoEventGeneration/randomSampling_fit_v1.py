# from io import StringIO
# from StringIO import StringIO
import ROOT
from ROOT import TTree, TBranch, TFile, TH1F
import numpy as np
import itertools
from root_numpy import random_sample, array2tree, array2root
import uproot
import sys 
#sys.exit()
min_ = int(sys.argv[1]) ##how many modules needs to be processed at a time and it is decided by min and max                                                                                               
max_ = int(sys.argv[2])

## reading module_info file
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
    #nMaxWords.append(int(data[im][6]))

## reading luvts file and storing the information
#### reading param_file                                                                                                        
layer_n10=[]
u_n10=[]
v_n10=[]
typee_n10=[]
constant1_n10=[]
constant2_n10=[]
constant3_n10=[]
mean_G_n10=[]
sigma_G_n10=[]
mean_P_n10=[]
mean_L_n10=[]
sigma_L_n10=[]
shape_n10=[]
data_n10 = np.loadtxt("./inputs/01July22_fitted_parameters_n10_D86_FeV10_mipDef_v2.txt")
for im in range(4500):

    layer_n10.append(int(data_n10[im][1]))
    u_n10.append(int(data_n10[im][2]))
    v_n10.append(int(data_n10[im][3]))
    typee_n10.append(int(data_n10[im][4]))
    constant1_n10.append(data_n10[im][5])
    mean_G_n10.append(data_n10[im][6])
    sigma_G_n10.append(data_n10[im][7])
    constant2_n10.append(data_n10[im][8])
    mean_P_n10.append(data_n10[im][9])
    constant3_n10.append(data_n10[im][10])
    mean_L_n10.append(data_n10[im][11])
    sigma_L_n10.append(data_n10[im][12])

layer_n20=[]
u_n20=[]
v_n20=[]
typee_n20=[]
constant1_n20=[]
constant2_n20=[]
constant3_n20=[]
mean_G_n20=[]
sigma_G_n20=[]
mean_P_n20=[]
mean_L_n20=[]
sigma_L_n20=[]
shape_n20=[]
data_n20 = np.loadtxt("./inputs/01July22_fitted_parameters_n20_D86_FeV10_mipDef_v2.txt")
for im in range(4500):

    layer_n20.append(int(data_n20[im][1]))
    u_n20.append(int(data_n20[im][2]))
    v_n20.append(int(data_n20[im][3]))
    typee_n20.append(int(data_n20[im][4]))
    constant1_n20.append(data_n20[im][5])
    mean_G_n20.append(data_n20[im][6])
    sigma_G_n20.append(data_n20[im][7])
    constant2_n20.append(data_n20[im][8])
    mean_P_n20.append(data_n20[im][9])
    constant3_n20.append(data_n20[im][10])
    mean_L_n20.append(data_n20[im][11])
    sigma_L_n20.append(data_n20[im][12])

layer_n30=[]
u_n30=[]
v_n30=[]
typee_n30=[]
constant1_n30=[]
constant2_n30=[]
constant2_n30=[]
constant3_n30=[]
mean_G_n30=[]
sigma_G_n30=[]
mean_P_n30=[]
mean_L_n30=[]
sigma_L_n30=[]
shape_n30=[]
data_n30 = np.loadtxt("./inputs/01July22_fitted_parameters_n30_D86_FeV10_mipDef_v2.txt")
for im in range(4500):

    layer_n30.append(int(data_n30[im][1]))
    u_n30.append(int(data_n30[im][2]))
    v_n30.append(int(data_n30[im][3]))
    typee_n30.append(int(data_n30[im][4]))
    constant1_n30.append(data_n30[im][5])
    mean_G_n30.append(data_n30[im][6])
    sigma_G_n30.append(data_n30[im][7])
    constant2_n30.append(data_n30[im][8])
    mean_P_n30.append(data_n30[im][9])
    constant3_n30.append(data_n30[im][10])
    mean_L_n30.append(data_n30[im][11])
    sigma_L_n30.append(data_n30[im][12])
    
out_fname = sys.argv[3] ## output file will contain similar 5k arrays saved after inverse qt                                                                                                              
fout = ROOT.TFile(out_fname, 'RECREATE')
# hist = ROOT.TH1F("hist","average n10 in CMSSW ntuple",500,0,500);

#fout = ROOT.TFile('RandomSample_from_fit_%dto%d_Latestntuples.root' %(min_,max_), 'RECREATE')
tree = ROOT.TTree('tree', 'tree')
M=4500
n=50000
# nhits10=[]
# nhits20=[]
# nhits30=[]
### ## sampling random numbers from the fit
seed_ = int(sys.argv[4])

gRandom=ROOT.TRandom3()
for im in range(4500):
    name='%i_%i_%i_%i' %(layer_n10[im],typee_n10[im],u_n10[im],v_n10[im])
    print(name)#, layer_n30[im],typee_n30[im],u_n30[im],v_n30[im])
    #print(constant1_n10[im],mean_G_n10[im],sigma_G_n10[im],constant2_n10[im],mean_P_n10[im],constant3_n10[im],mean_L_n10[im],sigma_L_n10[im])
    fit_func_n10= ROOT.TF1("fit_func_n10","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])")
    fit_func_n10.FixParameter(0,constant1_n10[im])
    fit_func_n10.FixParameter(1,mean_G_n10[im])
    fit_func_n10.FixParameter(2,sigma_G_n10[im])
    fit_func_n10.FixParameter(3,constant2_n10[im])
    fit_func_n10.FixParameter(4,mean_P_n10[im])
    fit_func_n10.FixParameter(5,constant3_n10[im])
    fit_func_n10.FixParameter(6,mean_L_n10[im])
    fit_func_n10.FixParameter(7,sigma_L_n10[im])
    f10=ROOT.TF1(fit_func_n10.Clone())
    #f10.SetRange(0,400)
    
    # nhitss_n10= np.array(random_sample(f10,n),dtype=[('n10_Module_%s' %name, np.float32)])
    # array2tree(nhitss_n10,tree=tree)

    fit_func_n20= ROOT.TF1("fit_func_n20","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])")
    fit_func_n20.FixParameter(0,constant1_n20[im])
    fit_func_n20.FixParameter(1,mean_G_n20[im])
    fit_func_n20.FixParameter(2,sigma_G_n20[im])
    fit_func_n20.FixParameter(3,constant2_n20[im])
    fit_func_n20.FixParameter(4,mean_P_n20[im])
    fit_func_n20.FixParameter(5,constant3_n20[im])
    fit_func_n20.FixParameter(6,mean_L_n20[im])
    fit_func_n20.FixParameter(7,sigma_L_n20[im])
    f20=ROOT.TF1(fit_func_n20.Clone())
    #f20.SetRange(0,400)
    # nhitss_n20= np.array(random_sample(f20,n),dtype=[('n20_Module_%s' %name, np.float32)])
    # array2tree(nhitss_n20,tree=tree)

    fit_func_n30= ROOT.TF1("fit_func_n30","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])")
    fit_func_n30.FixParameter(0,constant1_n30[im])
    fit_func_n30.FixParameter(1,mean_G_n30[im])
    fit_func_n30.FixParameter(2,sigma_G_n30[im])
    fit_func_n30.FixParameter(3,constant2_n30[im])
    fit_func_n30.FixParameter(4,mean_P_n30[im])
    fit_func_n30.FixParameter(5,constant3_n30[im])
    fit_func_n30.FixParameter(6,mean_L_n30[im])
    fit_func_n30.FixParameter(7,sigma_L_n30[im])
    f30=ROOT.TF1(fit_func_n30.Clone())
    # if(nCells[im]!=0):
    f10.SetRange(0,nCells[im]);
    f20.SetRange(0,nCells[im]);
    f30.SetRange(0,nCells[im]);
    # else:
    #     print(name)
    #     f10.SetRange(0,50);
    #     f20.SetRange(0,50);
    #     f30.SetRange(0,50);
    #print(name, layer_n30[im],typee_n30[im],u_n30[im],v_n30[im])
    #print(constant1_n30[im],mean_G_n30[im],sigma_G_n30[im],constant2_n30[im],mean_P_n30[im],constant3_n30[im],mean_L_n30[im],sigma_L_n30[im])
    gRandom.SetSeed(((im+1)*seed_)/(im+1))
    seedd_= seed_/(im+1)
    nhits10=random_sample(f10,n,seed=seedd_)
    nhits20=random_sample(f20,n,seed=seedd_)
    nhits30=random_sample(f30,n,seed=seedd_)
    # nhits10=[]
    # nhits20=[]
    # nhits30=[]

    # n10= 0
    # n20=0
    # n30=0
    # print(nCells[im],'   ncells')
    # for i in range(min_,max_):
    #     gRandom.SetSeed(((i+1)*seed_)/(im+1))
    #     n10 = int(f10.GetRandom(0,nCells[im]))
    #     n20 = int(f20.GetRandom(0,nCells[im]))
    #     n30 = int(f30.GetRandom(0,nCells[im]))
    #     if(typee[im]>=2):
    #         n20 =0

    #     if(n10>nCells[im]):
    #         print(n10, n20, n30, 'n10')
    #         n10 = nCells[im]
    #         print(n10, n20, n30, 'n10')
    #     if(n20>nCells[im]):
    #         print(n10, n20, n30, 'n20')
    #         n20 = nCells[im]
    #         print(n10, n20, n30, 'n20')
    #     if(n30>nCells[im]):
    #         print(n10, n20, n30, 'n30')
    #         n30 = nCells[im]
    #         print(n10, n20, n30, 'n30')
    #     if(n10+n20+n30>nCells[im]):
    #         print(n10, n20, n30, 'n30before sum')
    #         n30 = n30 - (n10+n20+n30-nCells[im])
    #         print(n10, n20, n30, 'n30after sum')

    #     if(n30<0):
    #         #or n10<0 or n20<0):
    #         n30 = 0
    #         # n20 =0
    #         # n10 =0
    #     if(n10+n20+n30>nCells[im]):
    #         print(n10, n20, n30, 'n20before sum')
    #         n20 = n20 - (n10+n20+n30-nCells[im])
    #         print(n10, n20, n30, 'n20after sum')
    #     if(n20<0):
    #         n20 =0

    #     if(n10+n20+n30>nCells[im]):
    #         print(n10, n20, n30, 'n10before sum')
    #         n10 = n10 - (n10+n20+n30-nCells[im])
    #         print(n10, n20, n30, 'n10after sum')

        
    #     if(n10<0):
    #         n10 =0
            
    #     if(int(n10)>nCells[im] or int(n20)>nCells[im] or int(n30)>nCells[im] or int(n10+n20+n30)>nCells[im]):
    #         print(n10, n20, n30, 'final')
    #     nhits10.append(n10) #f10.GetRandom(0,nCells[im]))
    #     nhits20.append(n20)#.GetRandom(0,nCells[im]))
    #     nhits30.append(n30) #.GetRandom(0,nCells[im]))
    nhitss_n10= np.array(nhits10,dtype=[('n10_Module_%s' %name, np.int32)])
    
    array2tree(nhitss_n10,tree=tree)
    nhitss_n20= np.array(nhits20,dtype=[('n20_Module_%s' %name, np.int32)])
    array2tree(nhitss_n20,tree=tree)
    nhitss_n30= np.array(nhits30,dtype=[('n30_Module_%s' %name, np.int32)])
    array2tree(nhitss_n30,tree=tree)
    # nhits10=[]
    # nhits20=[]
    # nhits30=[]
    #f30.SetRange(0,400)
    # nhitss_n30= np.array(random_sample(f30,n),dtype=[('n30_Module_%s' %name, np.float32)])
    # array2tree(nhitss_n30,tree=tree)
    # for i in range(n):
    #     nhitss_n30 = 
    if(im%100==0):
        print(im)

fout.cd()
#hist.Write()
#f10.Write()
tree.Write() 
fout.Close()
