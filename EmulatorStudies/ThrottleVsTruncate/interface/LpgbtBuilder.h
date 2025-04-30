#ifndef Emulation_HGCalBufferModel_LpgbtBuilder_h
#define Emulation_HGCalBufferModel_LpgbtBuilder_h

/*
Either remove or handle FC commands; currently backing up.
 */

#include <vector>
#include <queue>

#include "RandomSingleton.h"
#include "Entity.h"
#include "LpgbtPairDefinition.h"
#include "FastControlCommands.h"
#include "FastControlDriver.h"
#include "EcondBePreBuffer.h"
#include "EcondBeBuilder.h"
#include "EcondBeTrailer.h"
#include "EcondAsic.h"
#include "BasicWord.h"
#include "RamWord.h"

class LpgbtBuilderAnalyser;


class LpgbtBuilderMiddleState {
 public:
  LpgbtBuilderMiddleState();

  bool writeBuffer_;
  std::queue<FastControlCounters> inputCountersQueue_;
  std::queue<FastControlCounters> middleCountersQueue_;
  std::queue<RamWord> ramWordQueue_;
  std::queue<RamWord> eventBuffer_;
  EcondBeTrailer econdBeTrailer_;

  unsigned maxRamWordQueueSize_;
};


class LpgbtBuilder : public Entity {
 public:
  friend LpgbtBuilderAnalyser;

  LpgbtBuilder();
  virtual ~LpgbtBuilder();

  void setDefinitionTo(const LpgbtPairDefinition &d);
  
  // Connections
  void connect(FastControlDriver &d);
  void connect(EcondBeBuilder &e);
  void connect(EcondBePreBuffer &e);

  // Input
  void connectRxPortTo(Connection<RamWord> &w);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection<RamWord> &w);
  void strobeTx();

  // Processing
  void process();
  void subProcess40Mhz();
  void subProcessInput();
  void subProcessMiddle();
  void subProcessOutput();

  void enableAnalyser();

  void print() const;
    

  // Unused?  
  void requestWord(bool b);
  void ramWordVector(const std::vector<RamWord> &v);

  const RamWord& ramWord() const;
  std::queue<RamWord>& ramWordQueue();
  
 private:

  //Initialisation
  LpgbtPairDefinition definition_;
  
  // Inputs
  Connection<FastControlCommands> *rxCommandsPtr_;
  std::vector< Connection<RamWord>* > rxRamWordPtrVector_;

  // Outputs
  Connection<RamWord> *txRamWordPtr_;
  
  // Processing
  FastControlCommands commands_;
  FastControlCounters counters_;
  std::queue<FastControlCounters> inputCountersQueue_;
  std::queue<FastControlCounters> countersQueue_;
  std::vector<EcondAsicDefinition> econdAsicDefinitionVector_;

  unsigned inputEcond_;
  unsigned middleEcond_;
  unsigned outputEcond_;
  unsigned nEcond_;
  std::queue<BasicWord> basicWordQueue_;
  //std::vector< std::queue<BasicWord> > basicWordQueueVector_;
  //std::vector< std::queue<RamWord> > ramWordQueueVector_;
  //std::vector<EcondBeTrailer> econdBeTrailerVector_;
  //std::vector< std::queue<RamWord> > eventBufferVector_;
  std::vector<LpgbtBuilderMiddleState> middleStateVector_;

  uint8_t validInputs_;

  // Analyser
  LpgbtBuilderAnalyser *singleLpgbtBuilderAnalyserPtr_;

  // Statistics
  unsigned long numberOfTicks_;

  unsigned long numberOfInputRamWords_;
  unsigned long numberOfInputPackets_;
  unsigned long numberOfInputCheckedPackets_;

  unsigned long numberOfOutputRamWords_;
  unsigned long numberOfOutputPackets_;
  unsigned long numberOfOutputCheckedPackets_;

  // Packet checker
  std::vector< std::vector<RamWord> > econdBePacket72Vector_;
  std::vector<RamWord> lpgbtPacket_;
};

#endif
