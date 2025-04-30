#ifndef Emulation_HGCalBufferModel_CentralDaq_h
#define Emulation_HGCalBufferModel_CentralDaq_h

#include <vector>
#include <queue>

#include "TH1I.h"
#include "TH2I.h"

#include "Arguments.h"
#include "Entity.h"
#include "SectorDefinition.h"
//#include "FastControlDriver.h"
#include "Sector.h"
#include "D2sWord.h"
#include "EventData.h"

class CentralDaq : Entity {
 public:
  CentralDaq();
  virtual ~CentralDaq();

  // Initialisation
  void setDefinitionTo(const SectorDefinition &sd);
  void setArgumentsTo(const Arguments &a);
  void initialise();

  // Connections
  //void connect(FastControlDriver &fcd);
  void connect(Sector &s);
  void connect(D2sUnit &d);

  void process();
  void subProcessInput();
  void subProcessBuilder();
  void subProcessBuilderHalf(uint32_t w0, uint32_t w1);
  void subProcessOutput();
  
  void print() const;
  
 private:

  //Initialisation
  const SectorDefinition *definition_;
  std::vector<const SlinkDefinition*> slinkDefinitionVector_;

  // Connections
  void connectRxPortTo(Connection<D2sWord> &w);
  void strobeRx();

  std::vector< Connection<D2sWord>* > rxD2sWordPtrVector_;

  // subProcessInput
  D2sWord d2sWord_;
  //std::vector<bool> inputActiveVector_;
  std::vector< std::queue< std::pair<bool,SlinkWord> > > boolSlinkWordQueueVector_;

  // subProcessBuilder
  bool builderActive_;
  unsigned d2sActive_;

  unsigned iSlink_;
  unsigned nSlinks_;

  //unsigned iSlinkWord_;
  //unsigned nSlinkWords_;

  //unsigned numberOfEconds_;
  //unsigned slinkNumber_;
  //unsigned lpgbtPairNumber_;
  std::vector<unsigned> numberOfLpgbtPairsVector_;
  std::vector<unsigned> numberOfSlinksVector_;

  unsigned iLpgbtPair_;
  unsigned nLpgbtPairs_;
  LpgbtPairHeader lpgbtPairHeader_;

  unsigned iEcondAsic_;
  unsigned nEcondAsics_;
  EcondAsicHeader econdAsicHeader_;
  
  unsigned iPayloadWord_;
  unsigned nPayloadWords_;

  // subProcessOutput
  bool completeEvent_;
  EventData eventData_;

  TH1I *hEventSize_;
  TH1I *hDroppedPackets_;
  TH2I *hDroppedPacketsVsEventSize_;
  
  TTree *outputTree_;
};

#endif
