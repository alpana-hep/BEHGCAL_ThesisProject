#ifndef Emulation_HGCalBufferModel_HgcrocAsics_h
#define Emulation_HGCalBufferModel_HgcrocAsics_h

#include "Entity.h"
#include "EcondAsic.h"
#include "HGCHitProvider.h"
#include <string>

class HgcrocAsics : public Entity {
 public:
  HgcrocAsics();
  virtual ~HgcrocAsics();

  // Initialisation
  void setArgumentsTo(const Arguments &a);
  void initialise();

  void setupHitProvider(std::string const& config);

  inline HGCHitProvider *hitProvider() { return hits_; }

  void addDefinition(const EcondAsicDefinition &d);

  // Connections
  void connect(EcondAsic &e);

  // Outputs
  void connectTxPortTo(Connection< std::array<uint16_t,3> > &a);
  void strobeTx();

  // Processing
  void process();

  void print() const;

 private:

  // Initialisation
  bool initialised_;
  std::vector<EcondAsicDefinition> econdAsicDefinitionVector_;

  HGCHitProvider *hits_;
  bool switchToRandom_;
  
  // Output
  std::vector< Connection< std::array<uint16_t,3> >* > txNumbersPtrVector_;

  // Statistics
  unsigned long numberOfEvents_;
};

#endif
