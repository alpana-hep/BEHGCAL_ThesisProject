#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "SlinkDefinition.h"

SlinkDefinition::SlinkDefinition() :
  sectorNumber_(0),
  fpgaNumber_(0),
  slinkNumber_(0) {
}

void SlinkDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
  for(unsigned i(0);i<lpgbtPairDefinitionVector_.size();i++) {
    lpgbtPairDefinitionVector_[i].setSectorNumberTo(s);
  }
}

void SlinkDefinition::setFpgaNumberTo(unsigned f) {
  fpgaNumber_=f;
}

void SlinkDefinition::setSlinkNumberTo(unsigned s) {
  slinkNumber_=s;
}

void SlinkDefinition::addLpgbtPair(const LpgbtPairDefinition &d) {
  lpgbtPairDefinitionVector_.push_back(d);
  lpgbtPairDefinitionVector_.back().setFpgaNumberTo(fpgaNumber_);
  lpgbtPairDefinitionVector_.back().setSlinkNumberTo(slinkNumber_);
  lpgbtPairDefinitionVector_.back().setLpgbtPairNumberTo(lpgbtPairDefinitionVector_.size()-1);
}

const std::vector<LpgbtPairDefinition>& SlinkDefinition::lpgbtPairDefinitions() const {
  return lpgbtPairDefinitionVector_;
}

unsigned SlinkDefinition::numberOfLpgbtPairs() const {
  return lpgbtPairDefinitionVector_.size();
}

void SlinkDefinition::lpgbtPairDefinitions(std::vector<LpgbtPairDefinition> &v) const {
  for(unsigned i(0);i<lpgbtPairDefinitionVector_.size();i++) {
    v.push_back(lpgbtPairDefinitionVector_[i]);
  }  
}

void SlinkDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<lpgbtPairDefinitionVector_.size();i++) {
    lpgbtPairDefinitionVector_[i].econdAsicDefinitions(v);
  }  
}

const LpgbtPairDefinition& SlinkDefinition::lpgbtPairDefinition(unsigned lp) const {
  assert(lp<lpgbtPairDefinitionVector_.size());
  return lpgbtPairDefinitionVector_[lp];
}

const LpgbtDefinition& SlinkDefinition::lpgbtDefinition(unsigned lp, unsigned l) const {
  assert(lp<lpgbtPairDefinitionVector_.size());
  return lpgbtPairDefinitionVector_[lp].lpgbtDefinition(l);  
}

unsigned SlinkDefinition::fs() const {
  return 12*fpgaNumber_+slinkNumber_;
}

uint32_t SlinkDefinition::sourceId() const {
  //return 0x00ce0000|sectorNumber_<<12|fpgaNumber_<<4|slinkNumber_;
  return 0x00ce0000|sectorNumber_<<13|fpgaNumber_<<8|slinkNumber_<<4|lpgbtPairDefinitionVector_.size();
}

std::string SlinkDefinition::label() const {
  std::ostringstream sout;
  sout << "Sector" << sectorNumber_ << std::setfill('0')
       << "_Fpga" << std::setw(2) << fpgaNumber_
       << "_Slink" << std::setw(2) << slinkNumber_;
  return sout.str();
}

void SlinkDefinition::print(const std::string &s) const {
  std::cout << s << "SlinkDefinition::print()  Sector " << sectorNumber_
	    << ", FPGA " << std::setw(2) << fpgaNumber_
	    << ", Slink " << std::setw(2) << slinkNumber_
	    << " (" << label() << ")"
	    << ", Source ID = 0x" << std::hex << std::setfill('0')
	    << std::setw(8) << sourceId()
	    << std::dec << std::setfill(' ') << std::endl
	    << s << " Number of lpGBT pairs = " << lpgbtPairDefinitionVector_.size()
	    << std::endl;
  
  for(unsigned i(0);i<lpgbtPairDefinitionVector_.size();i++) {
    lpgbtPairDefinitionVector_[i].print(s+"  ");
  }
}
