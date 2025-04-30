#ifndef Emulation_HGCalBufferModel_EcondBeBuilder_h
#define Emulation_HGCalBufferModel_EcondBeBuilder_h

#include <cstdint>

#include <vector>
#include <queue>

#include "Entity.h"
#include "EcondAsicDefinition.h"
#include "FastControlDriver.h"
#include "FastControlStream.h"
#include "FastControlLpgbtWord.h"
#include "Connection.h"
#include "RamWord.h"
#include "FastControlCommands.h"
#include "FastControlCounters.h"
#include "EcondAsicHeader.h"
#include "EcondAsic.h"
#include "EcondBeHeader.h"


class EcondBeBuilder : public Entity {
 public:
  enum State {
    Idle,
    Header,
    Body,
    Trailer
  };
  
  EcondBeBuilder();
  virtual ~EcondBeBuilder();

  // Initialisation
  void setDefinitionTo(const EcondAsicDefinition &d);
  void setReadOffsetTo(unsigned o);

  // Connections
  void connect(FastControlDriver &d);
  void connect(FastControlStream &s);
  void connect(EcondAsic &e);

  // Inputs
  void connectDriverRxPortTo(Connection<FastControlCommands> &c);
  void connectStreamRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(Connection<uint32_t> &e);

  void strobeRx();

  // Outputs
  //void connectTxPortTo(Connection<FastControlLpgbtWord> &c);
  void connectTxPortTo(Connection<FastControlRequests> &r);
  void connectTxPortTo(Connection<RamWord> &w);

  void strobeTx();
  
  // Processing
  void process();
  void subProcess40MHz();
  void subProcess280MHzA(const unsigned elink);
  void subProcess280MHzB();




  unsigned numberOfElinks() const;

  const std::vector<uint32_t>& elinks() const;

  void elinks(const std::vector<uint32_t> &e);

  void newInputs(bool b);
  
  void fcCommands(const FastControlCommands &g, const FastControlCommands &l);

  std::queue<RamWord>& ramWordQueue();
  

  const RamWord& ramWord() const;
  

  void print() const;

 private:

  // Initialisation
  EcondAsicDefinition fModuleDefinition;
  int elinkReadReset_;

  // Inputs
  Connection<FastControlCommands> *rxCommandsPtr_[2];
  std::vector< Connection<uint32_t>* > rxElinkPtrVector_;
  
  // Outputs
  //Connection<FastControlLpgbtWord> *txLpgbtWordPtr_;
  Connection<FastControlRequests> *txRequestsPtr_;
  std::vector< Connection<RamWord>* > txConnectionPtrVector_;

  // Processing
  State state_;
  FastControlCounters fastControlCounters_[2];
  std::queue<EcondBeHeader> econdBeHeaderQueue_[2];
  
  FastControlLpgbtWord lpgbtWord_;
 
  RamWord fRamWord;
  std::queue<RamWord> ramWordQueue_;
    
  // Internal use

  std::queue<FastControlCommands> vFastControlCommands[2];
  std::queue<FastControlCounters> vFastControlCounters[2];

  uint32_t elinkArray[14];
  std::queue<uint32_t> nonIdleWords_;
  std::queue<uint8_t> nonIdleFlags_;



  std::vector<uint32_t> econAsicPacket32_;
  std::vector<uint32_t> econBuilderPacket32_;
  
  int elinkRead_;
  unsigned fPacketWords;
  unsigned packetLength_;
  bool econdAsicBufferThrottle_;
  EcondAsicHeader econdAsicHeader_;
};

#endif
