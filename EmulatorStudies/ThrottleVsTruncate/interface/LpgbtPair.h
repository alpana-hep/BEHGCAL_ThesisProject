#ifndef Emulation_HGCalBufferModel_LpgbtPair_h
#define Emulation_HGCalBufferModel_LpgbtPair_h

#include <vector>
#include <queue>

#include "Arguments.h"
#include "FastControlDriver.h"
#include "HgcrocAsics.h"
#include "LpgbtPairDefinition.h"
#include "LpgbtPair.h"
#include "LpgbtPairSplitter.h"
#include "LpgbtPairEcondBuilder.h"
#include "LpgbtPairBuilder.h"

class LpgbtPair {
 public:
  LpgbtPair();
  virtual ~LpgbtPair();

  // Initialisation
  void setDefinitionTo(const LpgbtPairDefinition &sd);
  void setArgumentsTo(const Arguments &a);
  
  // Connections
  void connect(FastControlDriver &fcd);
  void connect(HgcrocAsics &has);

  void connectTxPortTo(Connection<RamWord> &w);
  
  void print(const std::string &s="") const;
  
 private:
  
  // Initialisation
  LpgbtPairDefinition definition_;

  LpgbtPairSplitter *lpgbtPairSplitter_;
  LpgbtPairBuilder *lpgbtPairBuilder_;
  std::vector<FastControlStream*> fastControlStreamVector_;
  std::vector< std::vector<EcondAsic*> > econdAsicVectorVector_;
  std::vector< std::vector<LpgbtPairEcondBuilder*> > lpgbtPairEcondBuilderVectorVector_;
 };

#endif
