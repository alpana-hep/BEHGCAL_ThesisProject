#ifndef Emulation_HGCalBufferModel_SlinkDefinition_h
#define Emulation_HGCalBufferModel_SlinkDefinition_h

#include <vector>

#include "LpgbtPairDefinition.h"

class SlinkDefinition {
 public:
  SlinkDefinition();

  void setSectorNumberTo(unsigned s);
  void setFpgaNumberTo(unsigned f);
  void setSlinkNumberTo(unsigned s);

  void addLpgbtPair(const LpgbtPairDefinition &d);

  const std::vector<LpgbtPairDefinition>& lpgbtPairDefinitions() const;

  unsigned numberOfLpgbtPairs() const;
  void lpgbtPairDefinitions(std::vector<LpgbtPairDefinition> &v) const;
  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;

  const LpgbtPairDefinition& lpgbtPairDefinition(unsigned lp) const;
  const LpgbtDefinition& lpgbtDefinition(unsigned lp, unsigned l) const;

  unsigned fs() const;
  
  uint32_t sourceId() const;
  
  std::string label() const;
  
  void print(const std::string &s="") const;

 private:
  unsigned sectorNumber_;
  unsigned fpgaNumber_;
  unsigned slinkNumber_;
  std::vector<LpgbtPairDefinition> lpgbtPairDefinitionVector_;
};

#endif
