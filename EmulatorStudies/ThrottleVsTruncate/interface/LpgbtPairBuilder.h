#ifndef Emulation_HGCalBufferModel_LpgbtPairBuilder_h
#define Emulation_HGCalBufferModel_LpgbtPairBuilder_h

#include <vector>
#include <string>
#include <queue>

#include "RandomSingleton.h"
#include "EntityFastControl.h"
#include "LpgbtPairDefinition.h"
#include "FastControlCommands.h"
#include "FastControlDriver.h"
#include "EcondBePreBuffer.h"
#include "EcondBeBuilder.h"
#include "EcondBeTrailer.h"
#include "EcondAsic.h"
#include "LpgbtPairEcondBuilder.h"
#include "BasicWord.h"
#include "RamWord.h"

class LpgbtPairBuilderAnalyser;

class LpgbtPairBuilder : public EntityFastControl {
 public:
  friend LpgbtPairBuilderAnalyser;

  LpgbtPairBuilder();
  virtual ~LpgbtPairBuilder();

  // Initialisation
  void setDefinitionTo(const LpgbtPairDefinition &d);
  void setArgumentsTo(const Arguments &a);
  void initialise();

  // Connections
  void connect(FastControlDriver &d);
  void connect(FastControlStream &s); // Needed later
  void connect(LpgbtPairEcondBuilder &e);

  // Input
  void connectRxPortTo(Connection<RamWord> &w);
  //void connectRxPortTo(Connection<LpgbtPairEcondBuilder::SideFifoFlag> &f);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection<RamWord> &w);
  void strobeTx();

  // Processing
  void process();

  //void enableAnalyser();

  void print() const;
  
 private:

  // Subprocessing
  void subProcessInput();
  void subProcessOutput();
  void firstWord();

  enum State {
    //Wait,
    AcquireEcond,
    //FakeEcond,
    GetHeaders,
    FirstWord,
    SendHeader,
    NumberOfStates
  };
  
  //Initialisation
  LpgbtPairDefinition definition_;
  std::vector<EcondAsicDefinition> econdAsicDefinitionVector_;

  unsigned numberOfEcondAsics_;  

  void outputPacketCheck();

  // Inputs
  std::vector< Connection<RamWord>* > rxRamWordPtrVector_;
  //std::vector< Connection<LpgbtPairEcondBuilder::SideFifoFlag>* > rxSideFifoFlagPtrVector_;

  // Outputs
  Connection<RamWord> *txRamWordPtr_;
  
  // Processing
  RamWord ramOutputWord_;
  bool validRamOutputWord_;
  
  State state_;
  std::vector< std::pair<unsigned,RamWord> > boolHeaderVector_;
  //LpgbtPairEcondBuilder::SideFifoFlag sideFifoFlag_;
  EcondAsicHeader econdAsicHeader_;

  unsigned outputEcond_;
  unsigned lastOutputEcond_;

  bool ebrSent_;
  unsigned fastControlDelay_; // MOVE TO FpgaFastControl!!!
  unsigned waitEcondCounter_;
  unsigned acquireEcondCounter_;
  
  // Analyser
  LpgbtPairBuilderAnalyser *lpgbtPairBuilderAnalyserPtr_;
  
  // Packet checkers
  std::vector< std::vector<RamWord> > econdAsicPacket72Vector_;
  std::vector<RamWord> lpgbtPairBuilderPacket72Vector_;

  // Statistics
  unsigned long numberOfTicks_;

  unsigned long numberOfInputBackpressureTicks_;
  unsigned long numberOfInputNoL1aTicks_;
  unsigned long numberOfInputNoHeaderTicks_;
  unsigned long numberOfInputNoDataTicks_;
  unsigned long numberOfInputHeaderTicks_;
  unsigned long numberOfInputDataTicks_;
  
  unsigned long numberOfInputRamWords_;
  unsigned long numberOfInputPackets_;
  unsigned long numberOfInputCheckedPackets_;

  std::vector<unsigned long> numberOfInputBackpressureTicksVector_;
  std::vector<unsigned long> numberOfInputNoL1aTicksVector_;
  std::vector<unsigned long> numberOfInputNoHeaderTicksVector_;
  std::vector<unsigned long> numberOfInputNoDataTicksVector_;
  std::vector<unsigned long> numberOfInputHeaderTicksVector_;
  std::vector<unsigned long> numberOfInputDataTicksVector_;
  std::vector<unsigned long> numberOfInputRamWordsVector_;
  std::vector<unsigned long> numberOfInputPacketsVector_;
  std::vector<unsigned long> numberOfInputCheckedPacketsVector_;
  std::vector<unsigned long> sizeOfInputBufferVector_;
  std::vector<unsigned long> maxOfInputBufferVector_;

  unsigned long numberOfDroppedRamWords_;
  unsigned long numberOfDroppedPackets_;

  std::vector<unsigned long> numberOfDroppedRamWordsVector_;
  std::vector<unsigned long> numberOfDroppedPacketsVector_;

  unsigned long numberOfTimeoutRamWords_;
  unsigned long numberOfTimeoutPackets_;

  std::vector<unsigned long> numberOfTimeoutRamWordsVector_;
  std::vector<unsigned long> numberOfTimeoutPacketsVector_;
  
  unsigned long numberOfOutputBackpressureTicks_;
  unsigned long numberOfOutputNoDataTicks_;
  unsigned long numberOfOutputDataTicks_;
  unsigned long numberOfOutputRamWords_;
  unsigned long numberOfOutputPackets_;
  unsigned long numberOfOutputCheckedPackets_;
  unsigned long sizeOfOutputBuffer_;
  unsigned long maxOfOutputBuffer_;
  
  // Printable state names
  static std::string stateName_[NumberOfStates];
};

#endif
