#ifndef Emulation_HGCalBufferModel_SlinkData_h
#define Emulation_HGCalBufferModel_SlinkData_h

#include <iostream>
#include <vector>

#include "SlinkBoe.h"
#include "SlinkEoe.h"
#include "LpgbtPairData.h"


class SlinkData {
public:
  SlinkData();
  SlinkData(const SlinkBoe &boe);

  void addSlinkEoe(const SlinkEoe &eoe);
  void addLpgbtPairData(const LpgbtPairHeader &lph);
  void addEcondData(const EcondAsicHeader &eah);
  
  unsigned sizeInBytes() const;
  unsigned numberOfEcondAsicTruncations() const;
  unsigned numberOfEcondBeTruncations() const;
  
  void print(const std::string &s="") const;

  std::vector<LpgbtPairData> const& lpgbtPairDataVector() const;
  SlinkBoe const& slinkBoe() const;
  SlinkEoe const& slinkEoe() const;
  
private:
  SlinkBoe slinkBoe_;
  SlinkEoe slinkEoe_;
  std::vector<LpgbtPairData> lpgbtPairDataVector_;
};

#endif
