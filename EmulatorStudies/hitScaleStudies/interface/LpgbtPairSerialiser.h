#ifndef Emulation_HGCalBufferModel_LpgbtPairSerialiser_h
#define Emulation_HGCalBufferModel_LpgbtPairSerialiser_h

#include <vector>
#include <queue>

#include "Entity.h"
#include "Connection.h"
#include "LpgbtPairDefinition.h"
#include "FastControlDriver.h"
#include "FastControlCommands.h"
#include "EcondAsic.h"
#include "RamWord.h"
#include "SlinkWord.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"

class LpgbtPairSerialiser : public Entity {
 public:
  LpgbtPairSerialiser();
  virtual ~LpgbtPairSerialiser();

  // Initialisation
  void setDefinitionTo(const LpgbtPairDefinition &d);
  void initialise();

  // Connections
  void connect(EcondAsic &e);

  // Input
  void connectRxPortTo(Connection<uint32_t> &w);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection< std::array<uint32_t,3> > &a);
  void strobeTx();

  // Processing
  void process();

  void print() const;
    
 private:

  //Initialisation
  LpgbtPairDefinition lpgbtPairDefinition_;
  
  // Inputs
  std::vector< Connection<uint32_t>* > rxElinkPtrVector_;
  
  // Outputs
  Connection< std::array<uint32_t,3> > *txElinkArrayPtr_;  

  std::array<uint32_t,3> array_;
  bool evenTick_;
  unsigned elinkCounter_;
  unsigned elinkNumber_[14];
  unsigned elinkArrayNumber_[14][2];
  uint32_t elinkArray_[29]; // Extra word for unused ticks

  
  
  // 280 MHz building
  /*
  bool activeEvent_;
  SlinkEoe eoe_;
  unsigned nLpgbt_;
  SlinkWord slinkWord_;
  bool slinkFlag_;

  // 40 MHz L1A 
  FastControlCounters counters_;
  std::queue<SlinkBoe> boeQueue_;
  */
};

#endif
