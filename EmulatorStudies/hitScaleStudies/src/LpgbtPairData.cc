#include <iostream>
#include <iomanip>
#include <cassert>

#include "LpgbtPairData.h"


LpgbtPairData::LpgbtPairData() {
}

LpgbtPairData::LpgbtPairData(const LpgbtPairHeader &lph) :
  lpgbtPairHeader_(lph) {
}

void LpgbtPairData::addEcondData(const EcondAsicHeader &eah) {
  econdDataVector_.push_back(EcondData(eah));
}

unsigned LpgbtPairData::sizeInBytes() const {
  unsigned n(sizeof(LpgbtPairHeader));
  for(unsigned i(0);i<econdDataVector_.size();i++) {
    n+=econdDataVector_[i].sizeInBytes();
  }
  return n;
}

unsigned LpgbtPairData::numberOfEcondAsicTruncations() const {
  unsigned n(0);
  for(unsigned i(0);i<econdDataVector_.size();i++) {
    if(econdDataVector_[i].truncated()) n++;
  }
  return n;
}

unsigned LpgbtPairData::numberOfEcondBeTruncations() const {
  /*
  unsigned n(0);
  for(unsigned i(0);i<econdDataVector_.size();i++) {
    //n+=econdDataVector_[i].numberOfEcondBeTruncations();
  }
  return n;
  */
  return lpgbtPairHeader_.numberOfAbsentEcondPackets();  
}

void LpgbtPairData::print(const std::string &s) const {
  std::cout << s << "LpgbtPairData::print() Size = " << sizeInBytes()
	    << " bytes, number ECON-D ASIC truncations = "
	    << numberOfEcondAsicTruncations()
	    << ", BE truncations = "
	    << numberOfEcondBeTruncations() << std::endl;
}
