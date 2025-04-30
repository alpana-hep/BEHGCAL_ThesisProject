#ifndef Emulation_HGCalBufferModel_FastControlDriverAnalyser_h
#define Emulation_HGCalBufferModel_FastControlDriverAnalyser_h

#include "TH1F.h"
#include "TH1D.h"
#include "TH1I.h"

#include "FastControlDriver.h"


class FastControlDriverAnalyser {
 public:
  FastControlDriverAnalyser(const FastControlDriver *e);

  void initialise(bool a);
  void analyse();
  void finalise();

  void defineOrbitHists(unsigned o);

 private:
  const FastControlDriver *fastControlDriverPtr_;

  TH1D *h1HgcrocBuffer_;
  TH1I *h1L1aBcs_;
  TH1I *h1AllBcs_;
  TH1I *h1CollisionBcs_;
  unsigned throttleActive_;
  unsigned activeBxOld_;
  unsigned long numberOfTicks_;
  
  std::vector<TH1F*> orbitTh1fBcrVector_;
  std::vector<TH1F*> orbitTh1fEcrVector_;
  std::vector<TH1F*> orbitTh1fPreVector_;
  std::vector<TH1F*> orbitTh1fL1aVector_;
  std::vector<TH1F*> orbitTh1fCliVector_;
  std::vector<TH1F*> orbitTh1fCleVector_;
  std::vector<TH1F*> orbitTh1fThrVector_;
  std::vector<TH1I*> orbitTh1iHboVector_;
  
  std::vector<TH1F*> orbitTh1fTcdsL1aVector_;
  std::vector<TH1F*> orbitTh1fTcdsThrVector_;
  std::vector<TH1F*> orbitTh1fBeL1aVector_;
  std::vector<TH1F*> orbitTh1fBeThrVector_;
  
  TH1I* th1iThrottleLength_;
  TH1I* th1iActiveL1a_;

  TH1I *h1Counts_;  
  TH1F *h1Fractions_;  
};

#endif
