#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "FpgaDefinition.h"

FpgaDefinition::FpgaDefinition() :
  sectorNumber_(0),
  fpgaNumber_(0) {
}

void FpgaDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
  for(unsigned i(0);i<slinkDefinitionVector_.size();i++) {
    slinkDefinitionVector_[i].setSectorNumberTo(sectorNumber_);
  }
}

void FpgaDefinition::setFpgaNumberTo(unsigned f) {
  fpgaNumber_=f;
  for(unsigned i(0);i<slinkDefinitionVector_.size();i++) {
    slinkDefinitionVector_[i].setFpgaNumberTo(fpgaNumber_);
  }
}

void FpgaDefinition::addSlink(const SlinkDefinition &d) {
  assert(slinkDefinitionVector_.size()<12);
  
  slinkDefinitionVector_.push_back(d);
  slinkDefinitionVector_.back().setSectorNumberTo(sectorNumber_);
  slinkDefinitionVector_.back().setFpgaNumberTo(fpgaNumber_);
}

const std::vector<SlinkDefinition>& FpgaDefinition::slinkDefinitions() const {
  return slinkDefinitionVector_;
}

unsigned FpgaDefinition::numberOfLpgbts() const {
  unsigned n(0);
  for(unsigned i(0);i<slinkDefinitionVector_.size();i++) {
    n+=slinkDefinitionVector_[i].numberOfLpgbtPairs();
  }  
  return n;
}

void FpgaDefinition::lpgbtPairDefinitions(std::vector<LpgbtPairDefinition> &v) const {
  for(unsigned i(0);i<slinkDefinitionVector_.size();i++) {
    slinkDefinitionVector_[i].lpgbtPairDefinitions(v);
  }  
}

void FpgaDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<slinkDefinitionVector_.size();i++) {
    slinkDefinitionVector_[i].econdAsicDefinitions(v);
  }  
}

const SlinkDefinition& FpgaDefinition::slinkDefinition(unsigned s) const {
  assert(s<slinkDefinitionVector_.size());
  return slinkDefinitionVector_[s];
}

const LpgbtPairDefinition& FpgaDefinition::lpgbtPairDefinition(unsigned s, unsigned lp) const {
  assert(s<slinkDefinitionVector_.size());
  return slinkDefinitionVector_[s].lpgbtPairDefinition(lp);
}

const LpgbtDefinition& FpgaDefinition::lpgbtDefinition(unsigned s, unsigned lp, unsigned l) const {
  assert(s<slinkDefinitionVector_.size());
  return slinkDefinitionVector_[s].lpgbtDefinition(lp,l);
}

std::string FpgaDefinition::label() const {
  std::ostringstream sout;
  sout << "Sector" << sectorNumber_
       << "_Fpga" << std::setw(2) << std::setfill('0') << fpgaNumber_;
  return sout.str();
}

void FpgaDefinition::print(const std::string &s) const {
  std::cout << s << "FpgaDefinition::print()  Sector "
	    << sectorNumber_
	    << ", FPGA " << std::setw(2) << fpgaNumber_
	    << " (" << label() << ")" << std::endl
	    << s << " Number of Slinks = " << std::setw(2)
	    << slinkDefinitionVector_.size() << std::endl;
  
  for(unsigned i(0);i<slinkDefinitionVector_.size();i++) {
    slinkDefinitionVector_[i].print(s+"  ");
  }
}
