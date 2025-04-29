#define allinone_cxx
#include "allinone.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <vector>
void allinone::Loop()
{
  //   In a ROOT session, you can do:
  //      root> .L tree.C
  //      root> tree t
  //      root> t.GetEntry(12); // Fill t data members with entry number 12
  //      root> t.Show();       // Show values of entry 12
  //      root> t.Show(16);     // Read and show values of entry 16
  //      root> t.Loop();       // Loop on all entries
  //
  // reading mapping file
  std::ifstream mapping_file;
  mapping_file.open("",ios::in);
  struct modulemap {
    unsigned int  layer, u, v, type;
    char shape;
    int  nDaqElinks1, nDaqElinks2;
    int  idxDaqLpgbt1, idxDaqLpgbt2;
    unsigned nDaqLpgbt,nTpgLpgbt;
    int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
  };
  std::vector <modulemap> map_vec;
  std::vector<int> map_layer;
  std::vector<int> map_wafer_u;
  std::vector<int> map_wafer_v;
  std::vector<int> map_type;
  std::vector<int> dummy_zeros;
  if(!mapping_file.is_open())
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      map_vec.clear();
      map_vec.clear();
      map_layer.clear();
      map_wafer_u.clear();
      map_wafer_v.clear();
      map_type.clear();
      dummy_zeros.clear();

      while (!mapping_file.eof()) {
        unsigned int  layer, u, v, type,typee;
        char shape;
	int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
        int  idxDaqLpgbt1, idxDaqLpgbt2;
        int nDaqElinks1, nDaqElinks2;
        unsigned nDaqLpgbt,nTpgLpgbt;
        float average_data_per_econ,Error_means,rms,rms_error;


        mapping_file >> layer >> u >> v >> type >>typee  >> idxTpgLpgbt1 >> idxTpgLpgbt2 ;
	map_layer.push_back(layer);
        map_wafer_u.push_back(u);
        map_wafer_v.push_back(v);
        map_type.push_back(type);
        dummy_zeros.push_back(0);
      }
      mapping_file.close();
    }

  std::vector<int> layer;                                                                                                    
  std::vector<int> wafer_u;                                                                                                    
  std::vector<int> wafer_v;                                                                                                   
  std::vector<int> type;                                                                                                     
  std::vector<int> nhits30_defined;                                                                                           
  std::vector<int> nhits20_defined;
  std::vector<int> nhitsBXM1_defined;      
  std::vector<int> nhits10_defined;
  ULong64_t event;
  TFile *fout = new TFile("BE_compatible_ntuple_allinone.root", "RECREATE");
  TTree *tnew = new TTree("EventTree", "my event tree");
  
  tnew->Branch("event",&event);
  tnew->Branch("layer",&layer);
  tnew->Branch("wafer_v",&wafer_v);
  tnew->Branch("wafer_u",&wafer_u);
  tnew->Branch("type",&type);
  tnew->Branch("nhits10_defined",&nhits10_defined);
  tnew->Branch("nhits20_defined",&nhits20_defined);
  tnew->Branch("nhits30_defined",&nhits30_defined);
  tnew->Branch("nhitsBXM1_defined",&nhitsBXM1_defined);


  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  //     Long64_t nentries = 1000;
  std::cout << "nentries " << nentries << std::endl;

  Long64_t nbytes = 0, nb = 0;   
  int decade = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;      
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;


    //for(int ii=0; ii<10; ii++)
    //clear the vectors
    layer.clear();
    wafer_v.clear();
    wafer_u.clear();
    type.clear();
    nhits10_defined.clear();
    nhits30_defined.clear();
    nhits20_defined.clear();
    nhitsBXM1_defined.clear();
    // if(jentry<10) 
    // std::cout << jentry<< " " <<  Module_1_0_1_2 << std::endl;
    layer   = map_layer;
    wafer_u = map_wafer_u;
    wafer_v = map_wafer_v;
    type    = map_type;
    nhitsBXM1_defined = dummy_zeros;
    event = jentry;
