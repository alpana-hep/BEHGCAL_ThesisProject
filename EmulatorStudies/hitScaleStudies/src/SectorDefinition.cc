#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "SectorDefinition.h"

SectorDefinition::SectorDefinition() : sectorNumber_(0) {
}

void SectorDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
  for(unsigned i(0);i<fpgaDefinitionVector_.size();i++) {
    fpgaDefinitionVector_[i].setSectorNumberTo(sectorNumber_);
  }  
}

void SectorDefinition::addFpga(const FpgaDefinition &d) {
  assert(fpgaDefinitionVector_.size()<28);
  fpgaDefinitionVector_.push_back(d);
  fpgaDefinitionVector_.back().setSectorNumberTo(sectorNumber_);
}

const std::vector<FpgaDefinition>& SectorDefinition::fpgaDefinitions() const {
  return fpgaDefinitionVector_;
}

void SectorDefinition::slinkDefinitions(std::vector<SlinkDefinition> &v) const {
  for(unsigned i(0);i<fpgaDefinitionVector_.size();i++) {
    const std::vector<SlinkDefinition> &w(fpgaDefinitionVector_[i].slinkDefinitions());
    for(unsigned j(0);j<w.size();j++) {
      v.push_back(w[j]);
    }
  }
}

void SectorDefinition::lpgbtPairDefinitions(std::vector<LpgbtPairDefinition> &v) const {
  for(unsigned i(0);i<fpgaDefinitionVector_.size();i++) {
    fpgaDefinitionVector_[i].lpgbtPairDefinitions(v);
  }  
}

void SectorDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<fpgaDefinitionVector_.size();i++) {
    fpgaDefinitionVector_[i].econdAsicDefinitions(v);
  }  
}

const FpgaDefinition& SectorDefinition::fpgaDefinition(unsigned f) const {
  assert(f<fpgaDefinitionVector_.size());
  return fpgaDefinitionVector_[f];
}

const SlinkDefinition& SectorDefinition::slinkDefinition(unsigned f, unsigned s) const {
  assert(f<fpgaDefinitionVector_.size());
  return fpgaDefinitionVector_[f].slinkDefinition(s);
}

const LpgbtPairDefinition& SectorDefinition::lpgbtPairDefinition(unsigned f, unsigned s, unsigned lp) const {
  assert(f<fpgaDefinitionVector_.size());
  return fpgaDefinitionVector_[f].lpgbtPairDefinition(s,lp);
}

const LpgbtDefinition& SectorDefinition::lpgbtDefinition(unsigned f, unsigned s, unsigned lp, unsigned l) const {
  assert(f<fpgaDefinitionVector_.size());
  return fpgaDefinitionVector_[f].lpgbtDefinition(s,lp,l);
}

std::string SectorDefinition::label() const {
  std::ostringstream sout;
  sout << "Sector" << sectorNumber_;
  return sout.str();
}

void SectorDefinition::print(const std::string &s) const {
  std::cout << s << "SectorDefinition::print()  Sector "
	    << sectorNumber_ << " (" << label() << ")" << std::endl
	    << s << " Number of FPGAs = " << std::setw(2)
	    << fpgaDefinitionVector_.size()
	    << std::endl;
  
  for(unsigned i(0);i<fpgaDefinitionVector_.size();i++) {
    fpgaDefinitionVector_[i].print(s+"  ");
  }
}
