#include <iostream>

#include "TFile.h"
#include "TH1F.h"

#include "HelloLib.h"
#include "RandomSingleton.h"

void HelloFn() {
  std::cout << "Hello world\n";

  TFile *_rootFile=new TFile("HelloWorld.root","RECREATE");

  TH1F *h=new TH1F("Test","Hist",10,0,10);

  for(unsigned i(0);i<100;i++) {
    h->Fill(RandomSingleton::random().Integer(10));
  }
  
  _rootFile->Write();
  _rootFile->Close();
}
