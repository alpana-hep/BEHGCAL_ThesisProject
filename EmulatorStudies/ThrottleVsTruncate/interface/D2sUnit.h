#ifndef Emulation_HGCalBufferModel_D2sUnit_h
#define Emulation_HGCalBufferModel_D2sUnit_h

#include "Entity.h"
#include "FastControlDriver.h"
#include "SlinkDefinition.h"
#include "BasicWord.h"
#include "D2sWord.h"
#include "SlinkWord.h"
#include "SlinkBuilder.h"
#include "Slink.h"


class D2sUnit : public EntityFastControl {
 public:
  enum {
    D2sWordsPerTick=15,
    D2sBufferSize=1024 // REAL BUFFER SIZE IS 16M SlinkWords PER SLINK!
  };

  class D2sUnitData {
  public:
  D2sUnitData() : active_(false), numberOfTicks_(0) {}
    bool active_;
    FastControlCounters counters_;
    unsigned long numberOfTicks_;
  };

  D2sUnit();
  virtual ~D2sUnit();

  // Initialisation
  void setDefinitionTo(const SlinkDefinition &d);
  void setArgumentsTo(const Arguments &a);
  void initialise();
  
  // Connections
  void connect(FastControlDriver &d);
  void connect(Slink &s);

  // Inputs
  //void connectRxPortTo(Connection<FastControlCommands> &c);
  void connectRxPortTo(Connection< std::pair<bool,SlinkWord> > &w);
  void strobeRx();
 
  // Outputs
  void connectTxPortTo(Connection<D2sWord> &d);
  void strobeTx();

  // Processing
  void process();

  void subProcessInput();
  void subProcessBuilder();
  void subProcessD2sOutput();
  void subProcessCentralDaqOutput();

  void print() const;

 private:

  // Initialisation
  std::vector<SlinkDefinition> slinkDefinitionVector_;
  unsigned d2sBufferSize_;
  
  // Input
  //Connection<FastControlCommands> *rxCommandsPtr_;
  std::vector< Connection< std::pair<bool,SlinkWord> >* > rxSlinkWordPtrVector_;

  Connection<D2sWord> *txDataPtr_;
  
  // Internal
  D2sUnitData dthCheckerData_;
  //bool active_;
  //FastControlCounters counters_;
  //std::queue<FastControlCounters> countersQueue_;
  
  // Internal interface from input to builder subprocessors
  std::vector< std::queue< std::pair<bool,SlinkWord> > > slinkQueueVector_;

  // Builder subprocessor
  bool active_;
  unsigned inputSlink_;
  //unsigned orbitNumber_;
  unsigned orbitWordCount_;

  unsigned d2sWordCount_;
  D2sWord d2sWord_;
  
  // Internal interface from builder to output subprocessors
  std::queue<unsigned> wordCountQueue_;

  // Output subprocessor
  unsigned wordCountOut_;
  
  // Input packet checking
  std::vector<SlinkWord> slinkPacket_;

  // Output packet checking
  std::vector<SlinkWord> d2sPacketVector_;

  //unsigned d2sPacketLevel_;
  std::queue<BasicWord> cdQueue_;
  
  std::vector<RamWord> slinkBuilderPacket72_; // ???

  // Counts for summary print
  //unsigned long numberOfTicks_;
  unsigned long numberOfWords_;
  unsigned long numberOfPackets_;
};

#endif
