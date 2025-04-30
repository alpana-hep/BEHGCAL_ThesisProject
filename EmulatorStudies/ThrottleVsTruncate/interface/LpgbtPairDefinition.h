#ifndef Emulation_HGCalBufferModel_LpgbtPairDefinition_h
#define Emulation_HGCalBufferModel_LpgbtPairDefinition_h

#include <vector>
#include <string>

#include "LpgbtDefinition.h"


class LpgbtPairDefinition {
  
 public:
  LpgbtPairDefinition();

  void setSectorNumberTo(unsigned s);
  void setFpgaNumberTo(unsigned f);
  void setSlinkNumberTo(unsigned s);
  void setLpgbtPairNumberTo(unsigned l);
  
  void addLpgbt(const LpgbtDefinition &s);

  const std::vector<LpgbtDefinition>& lpgbtDefinitions() const;

  unsigned fslp() const;

  unsigned numberOfLpgbts() const;
  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;

  const LpgbtDefinition& lpgbtDefinition(unsigned l) const;

  std::string label() const;
  
  void print(const std::string &s="") const;

 private:
  unsigned sectorNumber_;
  unsigned fpgaNumber_;
  unsigned slinkNumber_;
  unsigned lpgbtPairNumber_;
  std::vector<LpgbtDefinition> lpgbtDefinitionVector_;
};

#endif
