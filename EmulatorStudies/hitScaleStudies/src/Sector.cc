#include <iostream>
#include <cassert>

#include "Sector.h"


Sector::Sector() {
}

Sector::~Sector() {
}

void Sector::setDefinitionTo(const SectorDefinition &sd) {
  definition_=sd;

  const std::vector<FpgaDefinition>& vFd(definition_.fpgaDefinitions());
  for(unsigned f(0);f<vFd.size();f++) {
    if((f%2)==0) daqUnitPtrVector_.push_back(new DaqUnit());
    daqUnitPtrVector_.back()->setDefinitionTo(vFd[f]);
  }
}

// Arguments
/*
void Sector::setArgumentsTo(const Arguments &a) {
  for(unsigned f(0);f<fpgaPtrVector_.size();f++) {
    fpgaPtrVector_[f]->setArgumentsTo(a);
  }
}
*/
// Connections

void Sector::connect(FastControlDriver &fcd) {
  for(unsigned f(0);f<daqUnitPtrVector_.size();f++) {
    daqUnitPtrVector_[f]->connect(fcd);
  }
}

void Sector::connect(HgcrocAsics &has) {
  for(unsigned f(0);f<daqUnitPtrVector_.size();f++) {
    daqUnitPtrVector_[f]->connect(has);
  }
}

const std::vector<DaqUnit*>& Sector::daqUnit() {
  return daqUnitPtrVector_;
}

void Sector::print(const std::string &s) const {
  std::cout << s << "Sector::print() " << definition_.label() << std::endl;
  std::cout << s << " Number of DAQ units = "
	    << daqUnitPtrVector_.size() << std::endl;
  for(unsigned f(0);f<daqUnitPtrVector_.size();f++) {
    daqUnitPtrVector_[f]->print(s+" ");
  }
  std::cout << std::endl;
}
