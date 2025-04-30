#ifndef Emulation_HGCalBufferModel_SectorDefinition_h
#define Emulation_HGCalBufferModel_SectorDefinition_h

#include <vector>
#include <string>

#include "FpgaDefinition.h"

class SectorDefinition {
 public:
  SectorDefinition();

  void setSectorNumberTo(unsigned s);
  
  void addFpga(const FpgaDefinition &f);

  const std::vector<FpgaDefinition>& fpgaDefinitions() const;

  void slinkDefinitions(std::vector<SlinkDefinition> &v) const;
  void lpgbtPairDefinitions(std::vector<LpgbtPairDefinition> &v) const;
  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;

  const FpgaDefinition& fpgaDefinition(unsigned f) const;
  const SlinkDefinition& slinkDefinition(unsigned f, unsigned s) const;
  const LpgbtPairDefinition& lpgbtPairDefinition(unsigned f, unsigned s, unsigned lp) const;
  const LpgbtDefinition& lpgbtDefinition(unsigned f, unsigned s, unsigned lp, unsigned l) const;
  
  std::string label() const;
  
  void print(const std::string &s="") const;

 private:
  unsigned sectorNumber_;
  std::vector<FpgaDefinition> fpgaDefinitionVector_;
};

#endif
