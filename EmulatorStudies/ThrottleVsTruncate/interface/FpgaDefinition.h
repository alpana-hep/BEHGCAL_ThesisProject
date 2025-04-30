#ifndef Emulation_HGCalBufferModel_FpgaDefinition_h
#define Emulation_HGCalBufferModel_FpgaDefinition_h

#include <vector>
#include <string>

#include "SlinkDefinition.h"


class FpgaDefinition {
  
 public:
  FpgaDefinition();

  void setSectorNumberTo(unsigned s);
  void setFpgaNumberTo(unsigned f);
  
  void addSlink(const SlinkDefinition &s);

  const std::vector<SlinkDefinition>& slinkDefinitions() const;

  unsigned numberOfLpgbts() const;
  void lpgbtPairDefinitions(std::vector<LpgbtPairDefinition> &v) const;
  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;

  const SlinkDefinition& slinkDefinition(unsigned s) const;
  const LpgbtPairDefinition& lpgbtPairDefinition(unsigned s, unsigned l) const;
  const LpgbtDefinition& lpgbtDefinition(unsigned s, unsigned lp, unsigned l) const;

  std::string label() const;
  
  void print(const std::string &s="") const;

 private:
  unsigned sectorNumber_;
  unsigned fpgaNumber_;
  std::vector<SlinkDefinition> slinkDefinitionVector_;
};

#endif
