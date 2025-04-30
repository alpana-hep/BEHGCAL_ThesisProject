#ifndef Emulation_HGCalBufferModel_FastControlStream_h
#define Emulation_HGCalBufferModel_FastControlStream_h

#include <vector>

#include "Arguments.h"
#include "Entity.h"
#include "Connection.h"
#include "FastControlStreamDefinition.h"
#include "FastControlDriver.h"
#include "FastControlStreamDefinition.h"
#include "FastControlStreamConfiguration.h"
#include "FastControlCommands.h"
#include "FastControlLpgbtWord.h"
#include "FastControlRequests.h"

class FastControlStream : public Entity {
 public:
  FastControlStream();
  virtual ~FastControlStream();

  // Initialisation
  void setDefinitionTo(const FastControlStreamDefinition &d);
  void setConfigurationTo(const FastControlStreamConfiguration &c);
  void setArgumentsTo(const Arguments &a);
  void initialise();
  
  // Connections
  void connect(FastControlDriver &d);

  // Input
  void connectRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(Connection<FastControlRequests> &r);
  void strobeRx();

  // Output
  void connectTxPortTo(Connection<FastControlLpgbtWord> &w);
  void connectTxPortTo(Connection<FastControlCommands> &c);
  void connectTxPortTo(Connection<FastControlRequests> &r);
  void strobeTx();

  // Processing
  void process();
  void subProcessRequests();
  void subProcessCommands();

  void print() const;
  
 private:
  
  // Initialisation
  FastControlStreamDefinition definition_;
  FastControlStreamConfiguration configuration_;
  
  // Input
  Connection<FastControlCommands> *rxCommandsPtr_;
  std::vector< Connection<FastControlRequests>* > rxRequestsPtrVector_;

  // Output
  Connection<FastControlRequests> *txRequestsPtr_;
  std::vector< Connection<FastControlLpgbtWord>* > txLpgbtWordPtrVector_;
  std::vector< Connection<FastControlCommands>* > txCommandsPtrVector_;

  // Internal
  unsigned long numberOfL1as_;
  unsigned long numberOfNzsL1as_;
  unsigned noZeroSuppressionOffset_;
  unsigned initialCommandsQueueSize_;
  unsigned initialRequestsQueueSize_;

  FastControlCommands globalCommands_;
  FastControlCommands localCommands_;
  FastControlCommands droppedCommands_;
  FastControlLpgbtWord lpgbtWord_;
  FastControlRequests requests_;
};

#endif
