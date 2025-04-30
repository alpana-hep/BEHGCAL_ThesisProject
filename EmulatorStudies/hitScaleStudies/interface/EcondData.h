#ifndef Emulation_HGCalBufferModel_EcondData_h
#define Emulation_HGCalBufferModel_EcondData_h

#include <cstdint>
#include <iostream>
#include <vector>

#include "EcondAsicHeader.h"

class EcondData {
public:
  EcondData();
  EcondData(const EcondAsicHeader &eah);

  void addEcondData(uint32_t d);

  unsigned sizeInBytes() const;
  bool truncated() const;
  
  void print(const std::string &s="") const;
  
private:
  EcondAsicHeader econdAsicHeader_;

#ifdef EcondData_IncludePayload  
  std::vector<uint32_t> payloadVector_;
#endif
};

#endif
