#ifndef Emulation_HGCalBufferModel_EntityFastControl_h
#define Emulation_HGCalBufferModel_EntityFastControl_h

#include <queue>

#include "FastControlCommands.h"
#include "FastControlRequests.h"
#include "FastControlCounters.h"
#include "HgcrocModel.h"
#include "Connection.h"
#include "Entity.h"

class EntityFastControl : public Entity {
 public:
  EntityFastControl(const std::string &n, Clocks::Frequency f);
  virtual ~EntityFastControl();

  // Input
  void connectRxPortTo(Connection<FastControlCommands> &c);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection<FastControlRequests> &r);
  void strobeTx();

  // Processing
  void subProcess40Mhz(bool qL1a=false);

  void print() const;
    
 protected:

  // Inputs
  Connection<FastControlCommands> *rxCommandsPtr_;

  // Outputs
  Connection<FastControlRequests> *txRequestsPtr_;
   
  // Processing
  FastControlCommands commands_;
  FastControlRequests requests_;
  FastControlCounters counters_;
  HgcrocModel hgcrocModel_;
  std::queue<FastControlCounters> l1aCountersQueue_;
};

#endif
