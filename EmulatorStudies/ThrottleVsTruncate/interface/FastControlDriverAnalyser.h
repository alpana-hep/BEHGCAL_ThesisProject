#ifndef Emulation_HGCalBufferModel_FastControlDriverAnalyser_h
#define Emulation_HGCalBufferModel_FastControlDriverAnalyser_h

#include "TH1F.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2I.h"
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
  TH1I *h1Throttle_bits_;
  TH1I *h1LiveThrottle_bits_;
  TH1D *h1_deadTimewithTime_;
  TH2D *h2d_deadTimeVsTime_;
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
  TH1I* h1Counts_liveL1As_;
  TH1F *h1Fractions_;  
  TH2I *h2Throttle_bits_HGCROCvsECOND_;
  TH2I *h2Throttle_bits_HGCROCvsBE_;
  TH2I *h2Throttle_bits_ECONDvsBE_;
  TH1F *h1Fractions_liveThrottle_;
  TH2I *h2LiveThrottle_bits_HGCROCvsECOND_;
  TH2I *h2LiveThrottle_bits_HGCROCvsBE_;
  TH2I *h2LiveThrottle_bits_ECONDvsBE_;

};

#endif
