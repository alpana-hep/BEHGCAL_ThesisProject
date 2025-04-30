#include <iostream>
#include <iomanip>
#include <cassert>

#include "EcondData.h"


EcondData::EcondData() {
}

EcondData::EcondData(const EcondAsicHeader &eah) :
  econdAsicHeader_(eah) {
}

void EcondData::addEcondData(uint32_t) {
#ifdef EcondData_IncludePayload  
  payloadVector_.push_back(d);
#else
  //d=0; // Allow compilation???
#endif
}

unsigned EcondData::sizeInBytes() const {
  return sizeof(EcondAsicHeader)+
    sizeof(uint32_t)*econdAsicHeader_.payloadLength();
}

bool EcondData::truncated() const {
  return econdAsicHeader_.truncated();
}

void EcondData::print(const std::string &s) const {
  std::cout << s << "EcondData::print() Size = " << sizeInBytes()
	    << " bytes, truncated = "
	    << (truncated()?"true":"false") << std::endl;
}
