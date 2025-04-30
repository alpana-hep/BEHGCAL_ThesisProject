#ifndef Emulation_HGCalBufferModel_Slink_h
#define Emulation_HGCalBufferModel_Slink_h

#include <vector>
#include <queue>

#include "Arguments.h"
#include "FastControlDriver.h"
#include "HgcrocAsics.h"
#include "SlinkDefinition.h"
#include "LpgbtPair.h"
#include "SlinkBuilder.h"
#include "SlinkSender.h"
#include "DthChecker.h"

class Slink {
 public:
  Slink();
  virtual ~Slink();

  // Initialisation
  void setDefinitionTo(const SlinkDefinition &sd);
  void setArgumentsTo(const Arguments &a);
  const SlinkDefinition& definition() const;
  
  // Connections
  void connect(FastControlDriver &fcd);
  void connect(HgcrocAsics &has);
  void connect(DthChecker &dc);
  
  void connectTxPortTo(Connection< std::pair<bool,SlinkWord> > &w);

  void print(const std::string &s="") const;
  
 private:

  // Initialisation
  SlinkDefinition definition_;

  SlinkBuilder *slinkBuilderPtr_;
  //SlinkSender *slinkSenderPtr_; Obsolete
  std::vector<LpgbtPair*> lpgbtPairPtrVector_;
};

#endif
