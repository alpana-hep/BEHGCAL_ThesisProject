#define HGCNtupleVariables_cxx
#include "HGCNtupleVariables.h"

#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

#include <iostream>
//#include <TProfile.h>

using namespace std;



// double HGCNtupleVariables::shower_comparisons(TProfile* shower, TH1F* hist) {
//  TH1F* HGCNtupleVariables::shower_comparisons(TProfile* shower, TH1F* hist) {
//    // TFile* file0 =
//    TFile::Open("/home/shubham/work/HGCAL/CERNTB/niramay/niramay_github/forked_repo/E_Pi_Comparison/functions/Template_150_GeV.root");
//    // TH1F* ref_hist = (TH1F*)file0->Get("hist_ref_template");
//    //TH1F* hist = new TH1F("hist","hist",30,0,30);
//    for(int i = 1; i <= shower->GetNbinsX(); i++){
//      hist->SetBinContent(i,shower->GetBinContent(i));
//    }
//    return hist;
//    //return (hist->Chi2Test(ref_h,"CHI2/NDF"));
//  }

// float HGCNtupleVariables::find_official_calib(int layer, int en_chan){
//   std::pair<int, int> temp_find;
//   temp_find = std::make_pair(layer,en_chan);
//   std::map<std::pair<int,int>, float >::iterator it =
//   offical_calib_map.find(temp_find); if(it != offical_calib_map.end()){
//     return it->second;
//   }
//   else {
//     return -1.0;
//   }
// }

// float HGCNtupleVariables::find_my_calib(int layer, int en_chan){
//   std::pair<int, int> temp_find;
//   temp_find = std::make_pair(layer,en_chan);
//   std::map<std::pair<int,int>, float >::iterator it =
//   my_calib_map.find(temp_find); if(it != my_calib_map.end()){
//     return it->second;
//   }
//   else {
//     return -1.0;
//   }
// }
