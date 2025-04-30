#ifndef Emulation_HGCalBufferModel_LpgbtBuilderAnalyser_h
#define Emulation_HGCalBufferModel_LpgbtBuilderAnalyser_h

#include "TH1F.h"
#include "TH1I.h"

#include "LpgbtBuilder.h"


class LpgbtBuilderAnalyser {
  enum {
    NumberOfBx=1000
  };

 public:
  LpgbtBuilderAnalyser(const LpgbtBuilder *b);

  void analyse();

 private:
  const LpgbtBuilder *singleLpgbtBuilderPtr_;

  unsigned nClock0280_;
  TH1I *th1iActiveInputs_[8];
};

#endif
