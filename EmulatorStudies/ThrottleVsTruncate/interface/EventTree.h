//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May  4 13:49:26 2020 by ROOT version 6.18/04
// from TTree EventTree/EventTree
// found on file: HGCalBEStudies_official.root
//////////////////////////////////////////////////////////

#ifndef EventTree_h
#define EventTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

using std::vector;

class EventTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong64_t       event;
   UInt_t          lumi;
   UInt_t          run;
   Int_t           pseudo_event;
   vector<int>     *layer;
   vector<int>     *wafer_u;
   vector<int>     *wafer_v;
   vector<int>     *wafer_u_orig;
   vector<int>     *wafer_v_orig;
   vector<int>     *type;
   vector<int>     *nhits10;
   vector<int>     *nhits20;
   vector<int>     *nhits30;
   vector<int>     *nhitsBXM1;
   vector<int>     *nhits10_defined;
   vector<int>     *nhits20_defined;
   vector<int>     *nhits30_defined;
   vector<int>     *nhitsBXM1_defined;
   vector<int>     *nhits10_unmasked;
   vector<int>     *nhits20_unmasked;
   vector<int>     *nhits30_unmasked;
   vector<int>     *nhitsBXM1_unmasked;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_run;   //!
   TBranch        *b_pseudo_event;   //!
   TBranch        *b_layer;   //!
   TBranch        *b_wafer_u;   //!
   TBranch        *b_wafer_v;   //!
   TBranch        *b_wafer_u_orig;   //!
   TBranch        *b_wafer_v_orig;   //!
   TBranch        *b_type;   //!
   TBranch        *b_nhits10;   //!
   TBranch        *b_nhits20;   //!
   TBranch        *b_nhits30;   //!
   TBranch        *b_nhitsBXM1;   //!
   TBranch        *b_nhits10_defined;   //!
   TBranch        *b_nhits20_defined;   //!
   TBranch        *b_nhits30_defined;   //!
   TBranch        *b_nhitsBXM1_defined;   //!
   TBranch        *b_nhits10_unmasked;   //!
   TBranch        *b_nhits20_unmasked;   //!
   TBranch        *b_nhits30_unmasked;   //!
   TBranch        *b_nhitsBXM1_unmasked;   //!

   EventTree(TTree *tree=0);
   virtual ~EventTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EventTree_cxx
EventTree::EventTree(TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree != nullptr) {
      Init(tree);
   }
}

EventTree::~EventTree(){ }

Int_t EventTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EventTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EventTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   layer = 0;
   wafer_u = 0;
   wafer_v = 0;
   wafer_u_orig = 0;
   wafer_v_orig = 0;
   type = 0;
   nhits10 = 0;
   nhits20 = 0;
   nhits30 = 0;
   nhitsBXM1 = 0;
   nhits10_defined = 0;
   nhits20_defined = 0;
   nhits30_defined = 0;
   nhitsBXM1_defined = 0;
   nhits10_unmasked = 0;
   nhits20_unmasked = 0;
   nhits30_unmasked = 0;
   nhitsBXM1_unmasked = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("pseudo_event", &pseudo_event, &b_pseudo_event);
   fChain->SetBranchAddress("layer", &layer, &b_layer);
   fChain->SetBranchAddress("wafer_u", &wafer_u, &b_wafer_u);
   fChain->SetBranchAddress("wafer_v", &wafer_v, &b_wafer_v);
   fChain->SetBranchAddress("wafer_u_orig", &wafer_u_orig, &b_wafer_u_orig);
   fChain->SetBranchAddress("wafer_v_orig", &wafer_v_orig, &b_wafer_v_orig);
   fChain->SetBranchAddress("type", &type, &b_type);
   fChain->SetBranchAddress("nhits10", &nhits10, &b_nhits10);
   fChain->SetBranchAddress("nhits20", &nhits20, &b_nhits20);
   fChain->SetBranchAddress("nhits30", &nhits30, &b_nhits30);
   fChain->SetBranchAddress("nhitsBXM1", &nhitsBXM1, &b_nhitsBXM1);
   fChain->SetBranchAddress("nhits10_defined", &nhits10_defined, &b_nhits10_defined);
   fChain->SetBranchAddress("nhits20_defined", &nhits20_defined, &b_nhits20_defined);
   fChain->SetBranchAddress("nhits30_defined", &nhits30_defined, &b_nhits30_defined);
   fChain->SetBranchAddress("nhitsBXM1_defined", &nhitsBXM1_defined, &b_nhitsBXM1_defined);
   fChain->SetBranchAddress("nhits10_unmasked", &nhits10_unmasked, &b_nhits10_unmasked);
   fChain->SetBranchAddress("nhits20_unmasked", &nhits20_unmasked, &b_nhits20_unmasked);
   fChain->SetBranchAddress("nhits30_unmasked", &nhits30_unmasked, &b_nhits30_unmasked);
   fChain->SetBranchAddress("nhitsBXM1_unmasked", &nhitsBXM1_unmasked, &b_nhitsBXM1_unmasked);
   Notify();
}

Bool_t EventTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EventTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EventTree::Cut(Long64_t /*entry*/)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EventTree_cxx
