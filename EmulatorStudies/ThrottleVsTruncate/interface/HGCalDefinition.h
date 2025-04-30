#ifndef Emulation_HGCalBufferModel_HGCalDefinition_h
#define Emulation_HGCalBufferModel_HGCalDefinition_h

#include <array>

#include "SectorDefinition.h"

class HGCalDefinition {
 public:
  HGCalDefinition();

  void setSectorTo(const SectorDefinition &f, bool copy=true);

  const std::array<SectorDefinition,6>& sectorDefinitions() const;

  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;
  
  void print() const;

 private:
  std::array<SectorDefinition,6> sectorDefinitionArray_;
};

#endif
