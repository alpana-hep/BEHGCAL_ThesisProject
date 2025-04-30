#ifndef Emulation_HGCalBufferModel_EcondBePreBuffer_h
#define Emulation_HGCalBufferModel_EcondBePreBuffer_h

#include "Entity.h"
#include "FastControlDriver.h"
#include "LpgbtPairDefinition.h"
#include "LpgbtPairSerialiser.h"


class EcondBePreBuffer : public Entity {
 public:
  EcondBePreBuffer();
  virtual ~EcondBePreBuffer();

  // Initialisation
  void setDefinitionTo(const LpgbtPairDefinition &d);
  void initialise();
  
  // Connections
  //void connect(FastControlDriver &d);
  void connect(LpgbtPairSerialiser &l);

  // Inputs
  //void connectRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(Connection< std::array<uint32_t,3> > &a);
  void strobeRx();
 
  // Outputs
  //void connectTxPortTo(Connection< std::pair<uint32_t,RamWord> > &w);
  void connectTxPortTo(Connection<RamWord> &w);
  void strobeTx();

  // Processing
  void process();

  void print() const;

 private:
  
  class StateMachine {
  public:
    enum State {
      Idle,
      HeaderB1,
      HeaderA2,
      PayloadA,
      PayloadB,
      EndAA,
      EndAB,
      EndBA,
      EndBB
    };
    
    StateMachine();

    State state_;
    
    uint16_t wordCount_;
    uint8_t bufferRead_;
    uint8_t bufferWrite_;
    uint32_t buffer_[4];

    EcondAsicHeader econdAsicHeader_;

    bool error_;
  };
  
  // Initialisation
  LpgbtPairDefinition lpgbtPairDefinition_;

  // Input
  //Connection<FastControlCommands> *rxCommandsPtr_;
  Connection< std::array<uint32_t,3> > *rxArrayPtr_;

  // Output
  //Connection< std::pair<uint32_t,RamWord> > *txRamWordPtr_;
  std::vector< Connection<RamWord>* > txRamWordPtrVector_;
  

  // Internal
  std::array<uint32_t,3> array_;
  StateMachine stateMachine_[15];
  std::pair<uint32_t,RamWord> numberPlusRamWord_;

  /*
  EcondBePreBufferData dthCheckerData_;
  //bool active_;
  //FastControlCounters counters_;
  std::queue<FastControlCounters> countersQueue_;

  // Packet checking
  std::vector<SlinkWord> slinkWordVector_;


  std::vector<RamWord> slinkBuilderPacket72_; // ???
  */
  
  // Counts for summary print
  //unsigned long numberOfTicks_;
  unsigned long numberOfWords_;
  unsigned long numberOfPackets_;
};

#endif
