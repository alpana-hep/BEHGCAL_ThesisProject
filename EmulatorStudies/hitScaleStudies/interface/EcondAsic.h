#ifndef Emulation_HGCalBufferModel_EcondAsic_h
#define Emulation_HGCalBufferModel_EcondAsic_h

#include <cstdint>

#include <vector>
#include <queue>

#include "Arguments.h"
#include "Entity.h"
#include "BasicWord.h"
#include "FastControlDriver.h"
#include "FastControlStream.h"
#include "FastControlLpgbtWord.h"
#include "FastControlCommands.h"
#include "FastControlCounters.h"
#include "HgcrocModel.h"
#include "EcondAsicIdle.h"
#include "EcondAsicHeader.h"
#include "EcondAsicDefinition.h"
#include "EcondAsicConfiguration.h"

class EcondAsicAnalyser;

class EcondAsic : public Entity {
  friend EcondAsicAnalyser;

 public:
  EcondAsic();
  virtual ~EcondAsic();

  // Initialisation
  void setDefinitionTo(const EcondAsicDefinition &d);
  EcondAsicDefinition definition() const;
  void setConfigurationTo(const EcondAsicConfiguration &d);
  void setArgumentsTo(const Arguments &a);
  void initialise();
  
  // I/O
  void connect(FastControlDriver &d); // only one can be used
  void connect(FastControlStream &s); // only one can be used
  void connect(std::vector<Connection<uint32_t>*> &v);
  
  // Input
  void connectRxPortTo(Connection<FastControlLpgbtWord> &c);
  void connectRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(Connection< std::array<uint16_t,3> > &a);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection<uint32_t> &e);
  void strobeTx();

  // Processing
  void process();
  void subProcessBx();
  void subProcessL1a();
  void subProcessElinks();

  // Analysing
  void enableAnalyser();
  
  double averageN10Hits() const;
  double averageN20Hits() const;
  double averageN30Hits() const;
  double averageTotalHits() const;  
  double fractionOfToaHits() const;
  
  double averagePacketWords() const;
  double ratioWordsInToOut() const;

  unsigned maxBufferSize() const;
  double fractionOfBufferStatus(EcondAsicIdle::BufferStatus s) const;
  double fractionOfTruncatedPackets() const;
  double fractionOfCorruptIdleWords() const;
  double fractionOfCorruptPacketWords() const;
  double TruncatedPackets() const;
  void print() const;
  
 private:

  // Initialisation
  EcondAsicDefinition definition_;
  EcondAsicConfiguration configuration_;  

  // Input
  Connection<FastControlLpgbtWord> *rxLpgbtWordPtr_;
  Connection< std::array<uint16_t,3> > *rxHgcrocDataPtr_;
    
  // Output
  std::vector<Connection<uint32_t>* > txConnectionPtrVector_;
  unsigned lpgbtInputDelay_;
  std::vector<unsigned> lpgbtDelay_;

  // Output
  std::vector<uint32_t> vElink;

  // Internal
  EcondAsicAnalyser *econdAsicAnalyserPtr_;

  std::queue<bool> preL1aQueue_;
  std::queue<bool> calibrationQueue_;
  
  FastControlLpgbtWord lpgbtWord_;
  FastControlCommands commands_;
  FastControlCounters counters_;
  HgcrocModel hgcrocModel_;
  bool newHgcrocL1a_;
  bool forceIdleOut_;
  
  std::queue<FastControlCommands> l1aFastControlCommandsQueue_;

  std::vector<uint32_t> packetVector_;
  std::queue<BasicWord> bufferQueue_;

  EcondAsicIdle idleWord_;
  
  unsigned numberOfWordsInPacketOut_;
  EcondAsicHeader econdAsicHeaderOut_;
  uint32_t packetOutMinDelay_;
  uint32_t packetOutMaxDelay_;
  std::queue<uint32_t> packetOutDelayQueue_;
  
  // Statistics

  // subProcessBx
  unsigned long numberOfFakeL1as_;

  // subProcessL1a
  unsigned long numberOfTicks_;
  unsigned long numberOfN10Hits_;
  unsigned long numberOfN20Hits_;
  unsigned long numberOfN30Hits_;
  //unsigned long numberOfWords_;
  //unsigned long numberOfPackets_;
  //unsigned long numberOfTruncatedPackets_;
  //unsigned long numberOfPacketWords_;
  unsigned long numberOfPackets_;
  unsigned long numberOfPacketWordsIn_;
  unsigned long numberOfTruncatedPackets_;
  unsigned long numberOfPacketWords_;
  
  // subProcessElinks
  unsigned maxBufferSize_;
  unsigned long numberOfBufferStatus_[4];
  unsigned long numberOfWordsOut_;
  unsigned long numberOfIdleWordsOut_;
  unsigned long numberOfCorruptIdleWordsOut_;
  unsigned long numberOfPacketWordsOut_;
  unsigned long numberOfCorruptPacketWordsOut_;
};

#endif
