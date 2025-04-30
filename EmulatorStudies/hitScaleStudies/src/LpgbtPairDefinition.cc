#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "LpgbtPairDefinition.h"

LpgbtPairDefinition::LpgbtPairDefinition() :
  sectorNumber_(0) {
}

void LpgbtPairDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
  for(unsigned i(0);i<lpgbtDefinitionVector_.size();i++) {
    lpgbtDefinitionVector_[i].setSectorNumberTo(sectorNumber_);
  }
}
void LpgbtPairDefinition::setFpgaNumberTo(unsigned f) {
  fpgaNumber_=f;
}

void LpgbtPairDefinition::setSlinkNumberTo(unsigned s) {
  slinkNumber_=s;
}

void LpgbtPairDefinition::setLpgbtPairNumberTo(unsigned l) {
  lpgbtPairNumber_=l;
}

void LpgbtPairDefinition::addLpgbt(const LpgbtDefinition &d) {
  assert(lpgbtDefinitionVector_.size()<2);
  if(lpgbtDefinitionVector_.size()==1) assert(d.singleLpgbt());
  
  lpgbtDefinitionVector_.push_back(d);
  lpgbtDefinitionVector_.back().setSectorNumberTo(sectorNumber_);
}

const std::vector<LpgbtDefinition>& LpgbtPairDefinition::lpgbtDefinitions() const {
  return lpgbtDefinitionVector_;
}

const LpgbtDefinition& LpgbtPairDefinition::lpgbtDefinition(unsigned l) const {
  assert(l<lpgbtDefinitionVector_.size());
  return lpgbtDefinitionVector_[l];
}

unsigned LpgbtPairDefinition::fslp() const {
  return 120*fpgaNumber_+10*slinkNumber_+lpgbtPairNumber_;
}

unsigned LpgbtPairDefinition::numberOfLpgbts() const {
  unsigned n(0);
  for(unsigned i(0);i<lpgbtDefinitionVector_.size();i++) {
    n+=(lpgbtDefinitionVector_[i].singleLpgbt()?1:2);
  }
  return n;
}

std::string LpgbtPairDefinition::label() const {
  std::ostringstream sout;
  sout << "Sector" << sectorNumber_ << std::setfill('0');
  
  if(lpgbtDefinitionVector_.size()==0) sout << "_NoLpgbt";
  if(lpgbtDefinitionVector_.size()==1) {
    if(lpgbtDefinitionVector_[0].singleLpgbt()) {
      sout << "_Lpgbt" << std::setw(4) << lpgbtDefinitionVector_[0].lpgbtNumber();      
    } else {
      return lpgbtDefinitionVector_[0].label();
    }
  } else {
    sout << "_Lpgbt" << std::setw(4) << lpgbtDefinitionVector_[0].lpgbtNumber();
    sout << "_Lpgbt" << std::setw(4) << lpgbtDefinitionVector_[1].lpgbtNumber();
  }
  
  return sout.str();
}

void LpgbtPairDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<lpgbtDefinitionVector_.size();i++) {
    lpgbtDefinitionVector_[i].econdAsicDefinitions(v);
  }  
}

void LpgbtPairDefinition::print(const std::string &s) const {
  std::cout << s << "LpgbtPairDefinition::print()  Sector "
	    << sectorNumber_ << ", FPGA " << fpgaNumber_
	    << ", Slink " << slinkNumber_
	    << ", lpGBT pair number " << lpgbtPairNumber_ << std::endl
	    << s << " Number of lpGBTs = " << numberOfLpgbts() << std::endl;
  
  for(unsigned i(0);i<lpgbtDefinitionVector_.size();i++) {
    lpgbtDefinitionVector_[i].print(s+"  ");
  }
}
