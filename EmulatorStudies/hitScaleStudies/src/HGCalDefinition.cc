#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "HGCalDefinition.h"

HGCalDefinition::HGCalDefinition() {
}

void HGCalDefinition::setSectorTo(const SectorDefinition &s, bool copy) {
  sectorDefinitionArray_[0]=s;
  sectorDefinitionArray_[0].setSectorNumberTo(0);

  if(copy) {
    for(unsigned i(1);i<6;i++) {
      sectorDefinitionArray_[i]=s;
      sectorDefinitionArray_[i].setSectorNumberTo(i);
    }
  }
}

const std::array<SectorDefinition,6>& HGCalDefinition::sectorDefinitions() const {
  return sectorDefinitionArray_;
}

void HGCalDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<6;i++) {
    sectorDefinitionArray_[i].econdAsicDefinitions(v);
  }  
}

void HGCalDefinition::print() const {
  std::cout << "HGCalDefinition::print()" << std::endl;
  //for(unsigned i(0);i<sectorDefinitionArray_.size();i++) {
  sectorDefinitionArray_[0].print(" ");
  //}
}
