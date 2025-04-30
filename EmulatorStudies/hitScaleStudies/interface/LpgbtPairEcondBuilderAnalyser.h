#ifndef Emulation_HGCalBufferModel_LpgbtPairEcondBuilderAnalyser_h
#define Emulation_HGCalBufferModel_LpgbtPairEcondBuilderAnalyser_h

#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"

#include "LpgbtPairEcondBuilder.h"

class LpgbtPairEcondBuilderAnalyser {
 public:
  LpgbtPairEcondBuilderAnalyser(const LpgbtPairEcondBuilder *e);

  void initialise(bool a);
  void analyse();
  void finalise();

 private:
  const LpgbtPairEcondBuilder *lpgbtPairEcondBuilderPtr_;
  const EcondAsicDefinition *econdAsicDefinitionPtr_;

  //LpgbtPairEcondBuilder::State stateOld_;
  //LpgbtPairEcondBuilder::State stateOlder_;
  //LpgbtPairEcondBuilder::L1aState l1aStateOld_;
  
  bool nonStaticsInitialised_;
  TH1I *h1L1aQueueSize_;
  TH1I *h1EcondL1a_;
  TH1I *h1EcondThr_;
  TH1I *h1EcondBufferStatus_;
  TH1F *h1EcondBuffer_[2];
  
  static bool staticsInitialised_;
  static TH1I *h1EcondOrThr_;
  static TH1I *h1EcondBufferLuv_;
  static TH1F *h1EcondErrorFractionLuv_;
  static TH2F *h1L1aMatchTime_[8];
  static TH1I *h1PacketOutMinDelayLuv_;

  static TH1F *h1PacketWordsFractionLuv_;
  static TH1F *h1ErrorWordsFractionLuv_;
  static TH1F *h1DiscardedPacketsFractionLuv_;
  static TH1F *h1MainBufferLowFractionLuv_;
  static TH1F *h1MainBufferMediumFractionLuv_;
  static TH1F *h1MainBufferHighFractionLuv_;
  static TH1F *h1MainBufferAlmostFullFractionLuv_;
  static TH1F *h1EcondAsicThrottleNumberLuv_;
  static TH1F *h1EcondBeThrottleNumberLuv_;
};

#endif
