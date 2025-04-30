#ifndef Emulation_HGCalBufferModel_LpgbtPairData_h
#define Emulation_HGCalBufferModel_LpgbtPairData_h

#include <iostream>
#include <vector>

#include "LpgbtPairHeader.h"
#include "EcondData.h"

class LpgbtPairData {
public:
  LpgbtPairData();
  LpgbtPairData(const LpgbtPairHeader &lph);

  void addEcondData(const EcondAsicHeader &eah);

  unsigned sizeInBytes() const;
  unsigned numberOfEcondAsicTruncations() const;
  unsigned numberOfEcondBeTruncations() const;
  
  void print(const std::string &s="") const;
  
private:
  LpgbtPairHeader lpgbtPairHeader_;
  std::vector<EcondData> econdDataVector_;
};

#endif
