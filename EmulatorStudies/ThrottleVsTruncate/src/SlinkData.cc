#include <iostream>
#include <iomanip>
#include <cassert>

#include "SlinkData.h"


SlinkData::SlinkData() {
}

SlinkData::SlinkData(const SlinkBoe &boe) :
  slinkBoe_(boe) {
}

void SlinkData::addSlinkEoe(const SlinkEoe &eoe) {
  slinkEoe_=eoe;
}

void SlinkData::addLpgbtPairData(const LpgbtPairHeader &lph) {
  lpgbtPairDataVector_.push_back(LpgbtPairData(lph));
}

void SlinkData::addEcondData(const EcondAsicHeader &eah) {
  lpgbtPairDataVector_.back().addEcondData(eah);
}

unsigned SlinkData::sizeInBytes() const {
  unsigned n(sizeof(SlinkBoe)+sizeof(SlinkEoe));
  for(unsigned i(0);i<lpgbtPairDataVector_.size();i++) {
    n+=lpgbtPairDataVector_[i].sizeInBytes();
  }
  return n;
}

unsigned SlinkData::numberOfEcondAsicTruncations() const {
  unsigned n(0);
  for(unsigned i(0);i<lpgbtPairDataVector_.size();i++) {
    n+=lpgbtPairDataVector_[i].numberOfEcondAsicTruncations();
  }
  return n;
}

unsigned SlinkData::numberOfEcondBeTruncations() const {
  unsigned n(0);
  for(unsigned i(0);i<lpgbtPairDataVector_.size();i++) {
    n+=lpgbtPairDataVector_[i].numberOfEcondBeTruncations();
  }
  return n;
}

void SlinkData::print(const std::string &s) const {
  std::cout << s << "SlinkData::print() Size = " << sizeInBytes()
	    << " bytes, number ECON-D ASIC truncations = "
	    << numberOfEcondAsicTruncations()
	    << ", BE truncations = "
	    << numberOfEcondBeTruncations() << std::endl;
}

std::vector<LpgbtPairData> const& SlinkData::lpgbtPairDataVector() const {
  return lpgbtPairDataVector_;
}

  SlinkBoe const& SlinkData::slinkBoe() const {
    return slinkBoe_;
  }

  SlinkEoe const& SlinkData::slinkEoe() const {
    return slinkEoe_;
  }