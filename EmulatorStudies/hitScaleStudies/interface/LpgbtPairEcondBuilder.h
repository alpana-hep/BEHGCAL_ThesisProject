#ifndef Emulation_HGCalBufferModel_LpgbtPairEcondBuilder_h
#define Emulation_HGCalBufferModel_LpgbtPairEcondBuilder_h

#include "FastControlDriver.h"
#include "FastControlStream.h"
#include "LpgbtPairDefinition.h"
#include "EcondAsicHeader.h"
#include "EcondAsicIdle.h"
#include "EcondAsicHitCounts.h"
#include "LpgbtPairEcondBuilderConfiguration.h"
#include "HgcrocModel.h"
#include "RamWord.h"
#include "EntityFastControl.h"

class LpgbtPairEcondBuilderAnalyser;

class LpgbtPairEcondBuilder : public EntityFastControl {
  friend LpgbtPairEcondBuilderAnalyser;
  
 public:
  LpgbtPairEcondBuilder();
  virtual ~LpgbtPairEcondBuilder();

  // Initialisation
  void setDefinitionTo(const EcondAsicDefinition &ead);
  EcondAsicDefinition& definition() const;

  void setConfigurationTo(const LpgbtPairEcondBuilderConfiguration &c);
  LpgbtPairEcondBuilderConfiguration& configuration();

  void setArgumentsTo(const Arguments &a);
  void initialise();
  
  // Connections
  void connect(FastControlDriver &fcd);
  void connect(FastControlStream &fcs); // Needed later, for modifying commands

  // Inputs
  void connectRxPortTo(Connection< std::array<uint32_t,2> > &a);
  void strobeRx();
 
  // Outputs
  void connectTxPortTo(Connection<RamWord> &w);
  void strobeTx();

  // Processing
  void process();

  void print() const;

 private:

  // Subprocessing
  void subProcessInput();
  void subProcessBuilder();
  void subProcessOutput();

  // Utilities
  void startPacketB();
  void setEcondAsicThrottle(EcondAsicIdle::BufferStatus s);
  void setEcondBeThrottle(unsigned b);

  // MIGRATE TO RELEVANT CLASSES?
  bool consistentWithIdle(uint32_t w);
  bool consistentWithHeader(uint32_t w);
  
  // Initialisation
  EcondAsicDefinition definition_;
  LpgbtPairEcondBuilderConfiguration configuration_;  

  // Configuration
  unsigned mediumBufferLevel_;
  unsigned highBufferLevel_;
  unsigned almostFullBufferLevel_;
  unsigned fullBufferLevel_;
  
  enum State {
    WaitForIdle,
    WaitForHeader,
    PayloadA,
    PayloadB,
    EndAA,
    EndAB,
    EndBA,
    EndBB,
    NumberOfStates
  };

  // Input
  Connection< std::array<uint32_t,2> > *rxElinkPairPtr_;

  // Output
  Connection<RamWord> *txRamWordPtr_;
  
  // Internal
  LpgbtPairEcondBuilderAnalyser *lpgbtPairEcondBuilderAnalyserPtr_;  
    
  // Overall state
  State state_;
  uint16_t wordCount_;
  EcondAsicHeader econdAsicHeader_;
  EcondAsicHitCounts econdAsicHitCounts_;
  unsigned waitForIdleCounter_;
  bool error_;

  // Interface between input and builder
  std::array<uint32_t,2> array_;
  bool validArray_;
  
  // Builder
  uint32_t arraySave_;
  bool arraySaveValid_;

  // Interface between builder and output
  RamWord ramWord_;
  bool validRamWord_;

  uint32_t idleWord_;
  EcondAsicIdle::BufferStatus econdBufferStatus_;
  bool bufferUnusable_;
  bool discardingPacket_;
  bool econdAsicThrottle_;
  bool econdBeThrottle_;

  uint32_t packetOutMinDelay_;

  // Packet checking
  std::vector<RamWord> econdPacket72Vector_;
  
  // Counts for summary print
  unsigned long numberOfTicks_;
  unsigned long numberOf40MhzTicks_;
  
  unsigned long numberOfInputWords_;
  unsigned long numberOfInputIdleWords_;
  unsigned long numberOfInputPacketWords_;
  unsigned long numberOfInputErrorWords_;
  unsigned long numberOfEcondBufferStatus_[4];
  
  unsigned long numberOfStartedPackets_;
  unsigned long numberOfFinishedPackets_;
  unsigned long numberOfCheckedPackets_;
  unsigned long numberOfStoredPackets_;
  unsigned long numberOfDiscardedPackets_;

  unsigned long numberOfOutputRamWords_;

  unsigned long maxMainBufferLevel_;
  unsigned long numberOfMainBufferLevel_[5];

  unsigned long numberOfEcondAsicThrottles_;
  unsigned long numberOfEcondBeThrottles_;

  double fractionOfIdleWords() const;
  double fractionOfPacketWords() const;
  double fractionOfErrorWords() const;
  double fractionOfEcondBufferStatus(EcondAsicIdle::BufferStatus s) const;
  double fractionOfCheckedPackets() const;
  double fractionOfStoredPackets() const;
  double fractionOfDiscardedPackets() const;
  double fractionOfMainBufferLevel(unsigned) const;
  
  // Printable state names
  static std::string stateName_[NumberOfStates];
};

#endif
