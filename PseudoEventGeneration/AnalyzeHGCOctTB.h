#ifndef AnalyzeHGCOctTB_H
#define AnalyzeHGCOctTB_H
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "HGCNtupleVariables.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TLorentzVector.h"
#include "TProfile.h"
using namespace std;
class AnalyzeHGCOctTB : public HGCNtupleVariables {
 public:
  AnalyzeHGCOctTB(const TString &inputFileList = "foo.txt",		  
                  const char *outFileName = "histo.root",
		  const char * min_="",
		  const char * max_=""
                  // const char *dataset = "data", const char *config = "alpha",
                  // const char *energy = "-1"
		  );  //, const char* min_ = "-1",
                                               //const char* max_ ="-1");
  ~AnalyzeHGCOctTB();
  // Bool_t   FillChain(TChain *chain, TChain *chain2, TChain *chain3, const
  // TString &inputFileList);
  Bool_t FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void EventLoop(const char *, const char *);//const char *, const char *);  //, const char *,const char *);
  void BookHistogram(const char *);//, const char *energy);
  TFile *oFile;



  
};
#endif

#ifdef AnalyzeHGCOctTB_cxx

void AnalyzeHGCOctTB::BookHistogram(const char *outFileName
                                    ) {
  oFile = new TFile(outFileName, "recreate");
  
  int en = 1;//atoi(energy);
  float xlow = 0.0;
  float xhigh = en * 100 * 1.5;
  // int xbin = (xhigh-xlow)/6;
  int xbin = 200;  //*0.5*en;
  //// ====== BOOK HISTOGRAM HERE =============///////////////
}

AnalyzeHGCOctTB::AnalyzeHGCOctTB(
				 const TString &inputFileList, const char *outFileName, const char *min_, const char* max_) {  //,const char* min_, const char* max_) {

  TChain *tree = new TChain("tree");
  // TChain *tree2 = new TChain("trackimpactntupler/impactPoints");
  // TChain *tree3 = new TChain("bigtree");

  if (!FillChain(tree, inputFileList)) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " <<  std::endl;
  }

  //  int chi2_method = atoi(energy);
  HGCNtupleVariables::Init(tree);  //, tree2, tree3);
  //  HGCNtupleVariables::init_piTree();
  BookHistogram(outFileName);
}
Bool_t AnalyzeHGCOctTB::FillChain(
    TChain *chain,
    const TString &inputFileList) {  // TChain *chain2, TChain *chain3, const
                                     // TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if (!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input"
              << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while (1) {
    infile >> buffer;
    if (!infile.good()) break;
    // std::cout << "Adding tree from " << buffer.c_str() << std::endl;
    chain->Add(buffer.c_str());
    // chain2->Add(buffer.c_str());
    // chain3->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in chain  : " << chain->GetEntries()
            << std::endl;
  // std::cout << "No. of Entries in chain2 : " << chain2->GetEntries() <<
  // std::endl; std::cout << "No. of Entries in chain3 : " <<
  // chain3->GetEntries() << std::endl;

  return kTRUE;
}

Long64_t AnalyzeHGCOctTB::LoadTree(Long64_t entry) {
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class())) return centry;
  TChain *chain = (TChain *)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }

  // if (!fChain2) return -5;
  // Long64_t centry2 = fChain2->LoadTree(entry);
  // if (centry2 < 0) return centry2;
  // if (!fChain2->InheritsFrom(TChain::Class()))  return centry2;
  // TChain *chain2 = (TChain*)fChain2;
  // if (chain2->GetTreeNumber() != fCurrent) {
  //   fCurrent = chain->GetTreeNumber();
  //   //    Notify();
  // }

  // if (!fChain3) return -5;
  // Long64_t centry3 = fChain3->LoadTree(entry);
  // if (centry3 < 0) return centry3;
  // if (!fChain3->InheritsFrom(TChain::Class()))  return centry3;
  // TChain *chain3 = (TChain*)fChain3;
  // if (chain3->GetTreeNumber() != fCurrent) {
  //   fCurrent = chain->GetTreeNumber();
  //   //    Notify();
  // }

  // if (centry==centry2)
  return centry;
  // cout<<"centry = "<<centry<<endl;
  // if(centry>0)
  //   return centry;
  // else return -1;
}

AnalyzeHGCOctTB::~AnalyzeHGCOctTB() {
  // if (!fChain || !fChain2) return;
  // delete fChain->GetCurrentFile();
  // delete fChain2->GetCurrentFile();
  // oFile->cd();
  // oFile->Write();
  // oFile->Close();

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();
}

#endif

/*  LocalWords:  Nrechit EE R1 FH GetXaxis SetTitle Sumw2 TH2F reg3 NRechits
 */
/*  LocalWords:  GetYaxis SetTitleOffset
 */
