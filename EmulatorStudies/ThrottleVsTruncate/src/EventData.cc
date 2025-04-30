#include <iostream>
#include <iomanip>
#include <cassert>

#include "EventData.h"


EventData::EventData() {
}

void EventData::addSlinkBoe(const SlinkBoe &boe) {
  slinkDataVector_.push_back(SlinkData(boe));
}

void EventData::addSlinkEoe(const SlinkEoe &eoe) {
  assert(slinkDataVector_.size()>0);
  slinkDataVector_.back().addSlinkEoe(eoe);
}

void EventData::addLpgbtPairData(const LpgbtPairHeader &lph) {
  assert(slinkDataVector_.size()>0);
  slinkDataVector_.back().addLpgbtPairData(lph);
}

void EventData::addEcondData(const EcondAsicHeader &lph) {
  assert(slinkDataVector_.size()>0);
  slinkDataVector_.back().addEcondData(lph);
}

void EventData::clear() {
  slinkDataVector_.resize(0);
}

unsigned EventData::sizeInBytes() const {
  unsigned n(0);
  for(unsigned i(0);i<slinkDataVector_.size();i++) {
    n+=slinkDataVector_[i].sizeInBytes();
  }
  return n;
}

unsigned EventData::numberOfEcondAsicTruncations() const {
  unsigned n(0);
  for(unsigned i(0);i<slinkDataVector_.size();i++) {
    n+=slinkDataVector_[i].numberOfEcondAsicTruncations();
  }
  return n;
}

unsigned EventData::numberOfEcondBeTruncations() const {
  unsigned n(0);
  for(unsigned i(0);i<slinkDataVector_.size();i++) {
    n+=slinkDataVector_[i].numberOfEcondBeTruncations();
  }
  return n;
}

std::vector<SlinkData> const& EventData::slinkDataVector() const {
  return slinkDataVector_;
}

void EventData::print(const std::string &s) const {
  std::cout << s << "EventData::print() Size = " << sizeInBytes()
	    << " bytes, number ECON-D ASIC truncations = "
	    << numberOfEcondAsicTruncations()
	    << ", BE truncations = "
	    << numberOfEcondBeTruncations() << std::endl;
}
