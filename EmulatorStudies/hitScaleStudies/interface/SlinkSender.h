#ifndef Emulation_HGCalBufferModel_SlinkSender_h
#define Emulation_HGCalBufferModel_SlinkSender_h

#include <vector>
#include <queue>

#include "Entity.h"
#include "Connection.h"
#include "SlinkDefinition.h"
#include "FastControlDriver.h"
#include "FastControlCommands.h"
#include "EcondAsic.h"
#include "RamWord.h"
#include "SlinkBuilder.h"
#include "SlinkWord.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"

class SlinkSender : public Entity {
 public:
  SlinkSender();
  virtual ~SlinkSender();

  // Initialisation
  void setDefinitionTo(const SlinkDefinition &d);
  void initialise();

  // Connections
  void connect(SlinkBuilder &b);

  // Input
  void connectRxPortTo(Connection< std::pair<bool,SlinkWord> > &w);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection< std::pair<bool,SlinkWord> > &w);
  void strobeTx();

  // Processing
  void process();

  void print() const;
    
 private:

  //Initialisation
  SlinkDefinition slinkDefinition_;
  
  // Inputs
  Connection< std::pair<bool,SlinkWord> > *rxSlinkWordPtr_;
  
  // Outputs
  Connection< std::pair<bool,SlinkWord> > *txSlinkWordPtr_;

  // Internal
  bool validBoolSlinkWord_;
  std::pair<bool,SlinkWord> boolSlinkWord_;

  unsigned long numberOfTicks_;
  unsigned long sizeOfInputBuffer_;
  unsigned long maxOfInputBuffer_;
  unsigned long sizeOfOutputBuffer_;
  unsigned long maxOfOutputBuffer_;
};

#endif
