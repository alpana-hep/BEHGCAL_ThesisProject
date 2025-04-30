#ifndef Emulation_HGCalBufferModel_SlinkBuilderAnalyser_h
#define Emulation_HGCalBufferModel_SlinkBuilderAnalyser_h

#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"

#include "SlinkBuilder.h"

class SlinkBuilderAnalyser {
 public:
  SlinkBuilderAnalyser(const SlinkBuilder *e);

  void initialise(bool a);
  void analyse();
  void finalise();

 private:
  const SlinkBuilder *slinkBuilderPtr_;
  
  bool nonStaticsInitialised_;
  TH1F *h1EcondBuffer_[2];
  
  static bool staticsInitialised_;
  static TH1I *h1SlinkFifoFs_;
  static TH1F *h1AvgSlinkWordsFs_;
  static TH2F *h2OccupancyVsAvgSlinkWords_;
};

#endif
