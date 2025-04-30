#ifndef Emulation_HGCalBufferModel_LpgbtPairBuilderAnalyser_h
#define Emulation_HGCalBufferModel_LpgbtPairBuilderAnalyser_h

#include "TH1F.h"
#include "TH1I.h"

#include "LpgbtPairBuilder.h"

class LpgbtPairBuilderAnalyser {
 public:
  LpgbtPairBuilderAnalyser(const LpgbtPairBuilder *e);

  void initialise(bool a);
  void analyse();
  void finalise();

 private:
  const LpgbtPairBuilder *lpgbtPairBuilderPtr_;
  bool nonStaticsInitialised_;
  
  static bool staticsInitialised_;
  TH1F *h1EcondBuffer_[2];
  static TH1I *h1EcondBufferFslp_;
  static TH1F *h1AvgRamWordsFslp_;
};

#endif
