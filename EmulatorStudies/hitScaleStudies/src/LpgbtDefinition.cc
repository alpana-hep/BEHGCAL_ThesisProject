#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "Backtrace.h"
#include "LpgbtDefinition.h"


LpgbtDefinition::LpgbtDefinition() :
  sectorNumber_(0),
  lpgbtNumberVector_(1) {
  lpgbtNumberVector_[0]=0;
}

void LpgbtDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
  for(unsigned i(0);i<streamVector_.size();i++) {
    streamVector_[i].setSectorNumberTo(sectorNumber_);
  }
}

// MORE STUFF HERE

void LpgbtDefinition::setLpgbtNumberTo(unsigned n) {
  lpgbtNumberVector_.resize(1);
  lpgbtNumberVector_[0]=n;

  for(unsigned i(0);i<streamVector_.size();i++) {
    streamVector_[i].setLpgbtNumberTo(lpgbtNumberVector_[0]);
  }
}

void LpgbtDefinition::setLpgbtNumbersTo(unsigned n0, unsigned n1) {
  lpgbtNumberVector_.resize(2);
  lpgbtNumberVector_[0]=n0;
  lpgbtNumberVector_[1]=n1;

  for(unsigned i(0);i<streamVector_.size();i++) {
    streamVector_[i].setLpgbtNumberTo(lpgbtNumberVector_[0]);
  }
}

unsigned LpgbtDefinition::lpgbtNumber(unsigned i) const {
  assert(i<lpgbtNumberVector_.size());
  return lpgbtNumberVector_[i];
}

bool LpgbtDefinition::singleLpgbt() const {
  return lpgbtNumberVector_.size()==1;
}

void LpgbtDefinition::addFastControlStream(const FastControlStreamDefinition &d) {
  streamVector_.push_back(d);
  streamVector_.back().setLpgbtNumberTo(lpgbtNumberVector_[0]);
  //backtraceAssert(streamVector_.size()<5);
}

const std::vector<FastControlStreamDefinition>& LpgbtDefinition::fastControlStreamDefinitions() const {
  return streamVector_;
}

uint64_t LpgbtDefinition::uniqueId() const {

  // Get list of ECON-Ds and check for problems
  std::vector<EcondAsicDefinition> v;
  econdAsicDefinitions(v);
  backtraceAssert(v.size()<8);
  if(v.size()==0) return 0;

  // Find layer
  uint64_t layer(v[0].layer());
  backtraceAssert(layer<256);

  // Initialise the id
  uint64_t id(layer<<56);

  for(unsigned e(0);e<v.size();e++) {

    // Check layer is the same for all ECON-Ds
    backtraceAssert(v[e].layer()==layer);

    // Check u and v are in right range
    uint64_t ue(v[e].u());
    backtraceAssert(ue<16);
    uint64_t ve(v[e].v());
    backtraceAssert(ve<16);

    // Store in id
    id|=(ue<<(8*e+4))|(ve<<(8*e));
  }
  
  return id;
}

std::string LpgbtDefinition::uniqueIdString() const {
  std::ostringstream sout;
  sout << std::hex << std::setfill('0') << std::setw(16) << uniqueId();
  return sout.str();
}

std::string LpgbtDefinition::label() const {
  std::ostringstream sout;
  sout << "Sector" << sectorNumber_ << std::setfill('0')
       << "_Lpgbt" << std::setw(4) << lpgbtNumberVector_[0];
  return sout.str();
}

void LpgbtDefinition::econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const {
  for(unsigned i(0);i<streamVector_.size();i++) {
    streamVector_[i].econdAsicDefinitions(v);
  }
}
/*
void LpgbtDefinition::econtAsicDefinitions(std::vector<EcontAsicDefinition> &v) const {
  for(unsigned i(0);i<streamVector_.size();i++) {
    streamVector_[i].econtAsicDefinitions(v);
  }
}
*/
void LpgbtDefinition::print(const std::string &s) const {
  std::cout << s << "LpgbtDefinition::print()  Sector "
	    << sectorNumber_;
  
  if(singleLpgbt()) {
    std::cout << ", Single lpGBT number = " << lpgbtNumberVector_[0];
  } else {
    std::cout << ", Double lpGBT numbers = " << lpgbtNumberVector_[0]
	      << ", " << lpgbtNumberVector_[1];
  }
  std::cout << " (" << label() << "), unique id = 0x"
	    << uniqueIdString() << std::endl;
  
  std::cout << s << " Number of fast control streams = "
	    << streamVector_.size() << std::endl;
  
  for(unsigned i(0);i<streamVector_.size();i++) {
    streamVector_[i].print(s+"  ");
  }
}
