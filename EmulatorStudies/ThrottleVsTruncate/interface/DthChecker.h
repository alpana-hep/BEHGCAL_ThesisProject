#ifndef Emulation_HGCalBufferModel_DthChecker_h
#define Emulation_HGCalBufferModel_DthChecker_h

#include "Entity.h"
#include "FastControlDriver.h"
#include "SlinkDefinition.h"
#include "SlinkWord.h"
#include "SlinkBuilder.h"


class DthChecker : public Entity {
 public:
  class DthCheckerData {
  public:
  DthCheckerData() : active_(false), numberOfTicks_(0) {}
    bool active_;
    FastControlCounters counters_;
    unsigned long numberOfTicks_;
  };

  DthChecker();
  virtual ~DthChecker();

  // Initialisation
  void setDefinitionTo(const SlinkDefinition &d);
  void initialise();
  
  // Connections
  void connect(FastControlDriver &d);
  void connect(SlinkBuilder &b);

  // Inputs
  void connectRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(Connection< std::pair<bool,SlinkWord> > &w);
  void strobeRx();
 
  // Outputs
  void connectTxPortTo(Connection<DthCheckerData> &d);
  void strobeTx();

  // Processing
  void process();

  void print() const;

 private:

  // Initialisation
  SlinkDefinition slinkDefinition_;
  
  // Input
  Connection<FastControlCommands> *rxCommandsPtr_;
  Connection< std::pair<bool,SlinkWord> > *rxSlinkWordPtr_;

  Connection<DthCheckerData> *txDataPtr_;
  
  // Internal
  DthCheckerData dthCheckerData_;
  //bool active_;
  //FastControlCounters counters_;
  std::queue<FastControlCounters> countersQueue_;

  // Packet checking
  std::vector<SlinkWord> slinkWordVector_;


  std::vector<RamWord> slinkBuilderPacket72_; // ???

  // Counts for summary print
  //unsigned long numberOfTicks_;
  unsigned long numberOfWords_;
  unsigned long numberOfPackets_;
  unsigned long numberOfCheckedPackets_;
};

#endif
