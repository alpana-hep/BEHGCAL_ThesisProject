#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "FastControlStreamDefinition.h"

FastControlStreamDefinition::FastControlStreamDefinition() :
  sectorNumber_(0),
  streamNumber_(0) {
}

void FastControlStreamDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
  for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
    econdAsicDefinitionVector_[i].setSectorNumberTo(sectorNumber_);
  }
  //for(unsigned i(0);i<econtAsicDefinitionVector_.size();i++) {
  //econtAsicDefinitionVector_[i].setSectorNumberTo(sectorNumber_);
  //}
}

unsigned FastControlStreamDefinition::lpgbtNumber() const {
  return lpgbtNumber_;
}

void FastControlStreamDefinition::setLpgbtNumberTo(unsigned l) {
  lpgbtNumber_=l;
}

unsigned FastControlStreamDefinition::streamNumber() const {
  return streamNumber_;
}

void FastControlStreamDefinition::setStreamNumberTo(unsigned s) {
  streamNumber_=s;
}

void FastControlStreamDefinition::setNumbersTo(unsigned l, unsigned s) {
  lpgbtNumber_=l;
  streamNumber_=s;
}

void FastControlStreamDefinition::addModule(const EcondAsicDefinition &d) {
  addEcondAsic(d);
}

void FastControlStreamDefinition::addEcondAsic(const EcondAsicDefinition &d) {
  econdAsicDefinitionVector_.push_back(d);
  econdAsicDefinitionVector_.back().setSectorNumberTo(sectorNumber_);
}
/*
void FastControlStreamDefinition::addEcontAsic(const EcontAsicDefinition &d) {
  econtAsicDefinitionVector_.push_back(d);
  econtAsicDefinitionVector_.back().setSectorNumberTo(sectorNumber_);
}
*/
const std::vector<EcondAsicDefinition>& FastControlStreamDefinition::moduleDefinition() const {
  return econdAsicDefinition();
}

const std::vector<EcondAsicDefinition>& FastControlStreamDefinition::econdAsicDefinition() const {
  return econdAsicDefinitionVector_;
}
/*
const std::vector<EcontAsicDefinition>& FastControlStreamDefinition::econtAsicDefinition() const {
  return econtAsicDefinitionVector_;
}
*/
std::string FastControlStreamDefinition::label() const {
  char c('A');
  c+=streamNumber_;

  std::ostringstream sout;
  sout << "Sector" << sectorNumber_ << std::setfill('0')
       << "_Lpgbt" << std::setw(4) << lpgbtNumber_ << c;
  return sout.str();
}

void FastControlStreamDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
    v.push_back(econdAsicDefinitionVector_[i]);
  }  
}
/*
void FastControlStreamDefinition::econtAsicDefinitions(std::vector<EcontAsicDefinition> &v) const {
  for(unsigned i(0);i<econtAsicDefinitionVector_.size();i++) {
    v.push_back(econtAsicDefinitionVector_[i]);
  }  
}
*/
void FastControlStreamDefinition::print(const std::string &s) const {
  std::cout << s << "FastControlStreamDefinition::print()  Sector "
	    << sectorNumber_
	    << ", lpGBT " << std::setw(4) << lpgbtNumber_
	    << ", fast control stream " << streamNumber_
	    << " (" << label() << ")" << std::endl
	    << s << " Number of ECON-D ASICs = "
	    << econdAsicDefinitionVector_.size() << std::endl;
  
  for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
    econdAsicDefinitionVector_[i].print(s+"  ");
  }
  /*
  std::cout << s << " Number of ECON-T ASICs = "
	    << econtAsicDefinitionVector_.size() << std::endl;
  
  for(unsigned i(0);i<econtAsicDefinitionVector_.size();i++) {
    econtAsicDefinitionVector_[i].print(s+"  ");
  }
  */
}
