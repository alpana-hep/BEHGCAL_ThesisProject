#ifndef Emulation_HGCalBufferModel_SlinkBuilder_h
#define Emulation_HGCalBufferModel_SlinkBuilder_h

/*
This takes data from event buffers of all lpGBT pairs feeding into one 
Slink and combined them into a single Slink packet, including the Slink 
begin-of-event header and end-of-event trailer.

This does not yet handle backpressure on the output (usually to the Slink FIFO).
Currently crashes if this buffer is limited in size.
 */


#include <vector>
#include <queue>

#include "EntityFastControl.h"
#include "Connection.h"
#include "SlinkDefinition.h"
#include "FastControlDriver.h"
#include "LpgbtBuilder.h"
#include "FastControlCommands.h"
#include "RamWord.h"
#include "SlinkWord.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"
#include "LpgbtPair.h"
#include "SlinkBuilderConfiguration.h"

class SlinkBuilderAnalyser;

class SlinkBuilder : public EntityFastControl {
 public:
  friend SlinkBuilderAnalyser;

  SlinkBuilder();
  virtual ~SlinkBuilder();

  // Initialisation
  void setDefinitionTo(const SlinkDefinition &d);
  void setConfigurationTo(const SlinkBuilderConfiguration &c);
  void setArgumentsTo(const Arguments &a);
  void initialise();

  // Connections
  void connect(FastControlDriver &d);
  void connect(LpgbtPair &l);

  // Input
  //void connectRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(EventBuffer &w);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection< std::pair<bool,SlinkWord> > &w);
  void strobeTx();

  void print() const;
    
 private:
  static const unsigned eventBufferSize_;
  
  // Processing
  void process();
  //void subProcess40MHz();
  void subProcessInput();
  void subProcessBuilder();
  void subProcessBuilderOld(); // TEMP!
  void subProcessOutput();

  void inputPacketCheck(unsigned n, const RamWord &w);
  void outputPacketCheck(const std::pair<bool,SlinkWord> &w);

  //Initialisation
  SlinkDefinition definition_;
  SlinkBuilderConfiguration configuration_;
  //std::vector<LpgbtPairDefinition> lpgbtPairDefinitionVector_; OBSOLETE
  unsigned numberOfLpgbtPairs_;
  
  // Inputs
  //Connection<FastControlCommands> *rxCommandsPtr_;
  std::vector<EventBuffer*> rxRamWordPtrVector_;
  
  // Outputs
  Connection< std::pair<bool,SlinkWord> > *txSlinkWordPtr_;  

  // 360 MHz building
  std::vector<bool> activeInputVector_;
  bool activeEvent_;
  bool activeOutput_;
  SlinkBoe boe_;
  SlinkEoe eoe_;
  uint16_t eoeCrc_;
  unsigned nLpgbtPair_;
  SlinkWord slinkWord_[2];
  unsigned slinkWordReady_;
  bool slinkFlag_;

  // Input
  std::vector< std::queue<FastControlCounters> > inputL1aCountersQueueVector_;
  
  // Interface between input and builder
  //std::vector< std::queue<RamWord> > inputRamWordQueueVector_;
  std::vector< std::queue<BasicWord> > inputRamWordQueueVector_;

  // Interface between builder and output
  std::queue< std::pair<bool,SlinkWord> > outputSlinkWordQueue_;

  // 40 MHz L1A 
  /*
  FastControlCommands commands_;
  FastControlCounters counters_;
  */
  std::queue<SlinkBoe> boeQueue_;

  // Analyser
  SlinkBuilderAnalyser *slinkBuilderAnalyserPtr_;

  // Statistics
  unsigned long numberOfTicks_;
  unsigned long numberOf40MhzTicks_;
  unsigned long numberOf360MhzTicks_;

  unsigned long numberOfInputRamWords_;
  unsigned long numberOfInputBasicWords_;
  unsigned long numberOfInputPaddingWords_;
  unsigned long numberOfInputNoDataTicks_;
  unsigned long numberOfInputBackpressureTicks_;
  unsigned long numberOfInputStartedPackets_;
  unsigned long numberOfInputFinishedPackets_;
  unsigned long numberOfInputPackets_;
  unsigned long numberOfInputCheckedPackets_;

  std::vector<unsigned long> numberOfInputRamWordsVector_;
  std::vector<unsigned long> numberOfInputBasicWordsVector_;
  std::vector<unsigned long> numberOfInputPaddingWordsVector_;
  std::vector<unsigned long> numberOfInputNoDataTicksVector_;
  std::vector<unsigned long> numberOfInputBackpressureTicksVector_;
  std::vector<unsigned long> numberOfInputStartedPacketsVector_;
  std::vector<unsigned long> numberOfInputFinishedPacketsVector_;
  std::vector<unsigned long> numberOfInputPacketsVector_;
  std::vector<unsigned long> numberOfInputCheckedPacketsVector_;
  std::vector<unsigned long> sizeOfInputBufferVector_;
  std::vector<unsigned long> maxOfInputBufferVector_;

  unsigned long numberOfBuilderBackpressureTicks_;
  
  unsigned long maxOfOutputSlinkWordQueueSize_;
  unsigned long numberOfOutputBackpressureTicks_;
  unsigned long numberOfOutputDataTicks_;
  unsigned long numberOfOutputNoDataTicks_;
  unsigned long numberOfOutputSlinkWords_;
  unsigned long numberOfOutputPackets_;
  unsigned long numberOfOutputStartedPackets_;
  unsigned long numberOfOutputFinishedPackets_;
  unsigned long numberOfOutputCheckedPackets_;
  unsigned long sizeOfOutputBuffer_;
  unsigned long maxOfOutputBuffer_;
  
  // Input packet checking
  std::vector< std::vector<RamWord> > lpgbtPairPacketVector_;

  // Output packet checking
  std::vector<SlinkWord> slinkPacket_;
};

#endif
