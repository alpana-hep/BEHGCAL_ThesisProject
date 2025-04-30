#ifndef Emulation_HGCalBufferModel_EcondAsicAnalyser_h
#define Emulation_HGCalBufferModel_EcondAsicAnalyser_h

#include "TH1F.h"
#include "TH1I.h"
#include "TH2I.h"

#include "EcondAsic.h"
#include "EcondAsicDefinition.h"

class EcondAsicAnalyser {
 public:
  EcondAsicAnalyser(EcondAsic *e);

  void initialise(bool a);
  void analyse();
  void finalise();

 private:
  EcondAsic *econdAsicPtr_;
  const EcondAsicDefinition *econdAsicDefinitionPtr_;

  TH1F *h1EcondBuffer_[2];
  TH1I *h1EcondAsicL1a_[2];
  TH1I *h1EcondAsicThr_;
  TH1I *h1PacketOutDelay_;
 
  static bool staticsInitialised_;

  static TH1I *h1NumberOfElinks_;  
  static TH1I *h1NumberOfElinksLuv_;  

  static TH1I *h1AverageN10Hits_;
  static TH1F *h1AverageN10HitsLuv_;
  static TH1I *h1AverageN20Hits_;
  static TH1F *h1AverageN20HitsLuv_;
  static TH1I *h1AverageN30Hits_;
  static TH1F *h1AverageN30HitsLuv_;
  static TH1I *h1AverageTotalHits_;
  static TH1F *h1AverageTotalHitsLuv_;
  
  static TH1I *h1AveragePacketWords_;
  static TH1F *h1AveragePacketWordsLuv_;
  static TH1I *h1AveragePacketBits_;
  static TH1F *h1AveragePacketBitsLuv_;
  
  static TH1I *h1LinkPacketOccupancy_;
  static TH1F *h1LinkPacketOccupancyLuv_;
  
  static TH1I *h1MaxBuffer_;
  static TH1I *h1MaxBufferLuv_;
  static TH1D *h1MaxBuffer_BX_;
  static TH1D *h1MaxBuffer_BX_Luv_;
  static TH1I *h1FractionBufferStatus_[4];
  static TH1F *h1FractionBufferStatusLuv_[4];
  
  static TH1I *h1FractionTruncatedPackets_;
  static TH1F *h1FractionTruncatedPacketsLuv_;
  static TH1I *h1FractionCorruptIdleWords_;
  static TH1F *h1FractionCorruptIdleWordsLuv_;
  static TH1I *h1FractionCorruptPacketWords_;
  static TH1F *h1FractionCorruptPacketWordsLuv_;
  static TH1I *h1NumberFakeL1as_;
  static TH1F *h1NumberFakeL1asLuv_;

  static TH2I *h2TruncationLpo_;
  static TH1I *h1PacketOutMinDelayLuv_;
  static TH1I *h1PacketOutMaxDelayLuv_;
};

#endif
