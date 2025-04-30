#ifndef Emulation_HGCalBufferModel_LpgbtPairSplitter_h
#define Emulation_HGCalBufferModel_LpgbtPairSplitter_h

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
#include "LpgbtPairEcondBuilder.h"

class LpgbtPairSplitter : public Entity {
 public:
  LpgbtPairSplitter();
  virtual ~LpgbtPairSplitter();

  // Initialisation
  void setDefinitionTo(const LpgbtPairDefinition &d);
  void initialise();

  // Connections
  void connect(EcondAsic &e);
  void connect(std::vector<Connection<uint32_t>*> &v); // NOT NEEDED?
  void connect(LpgbtPairEcondBuilder &e);
  void connect(std::vector<Connection<std::array<uint32_t,2> > > &a);

  // Input
  void connectRxPortTo(Connection<uint32_t> &w);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection< std::array<uint32_t,2> > &a);
  void strobeTx();

  // Processing
  void process();

  void print() const;
    
 private:

  //Initialisation
  LpgbtPairDefinition lpgbtPairDefinition_;
  
  // Inputs
  std::vector<Connection<uint32_t>*> rxElinkPtrVector_;
  
  // Outputs
  std::vector<Connection< std::array<uint32_t,2> >*> txElinkArrayPtrVector_;

  std::array<uint32_t,2> array_;
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
