#ifndef Emulation_HGCalBufferModel_EventData_h
#define Emulation_HGCalBufferModel_EventData_h

#include <iostream>
#include <vector>

#include "SlinkData.h"


class EventData {
public:
  EventData();

  void addSlinkBoe(const SlinkBoe &boe);
  void addSlinkEoe(const SlinkEoe &eoe);
  void addLpgbtPairData(const LpgbtPairHeader &lph);
  void addEcondData(const EcondAsicHeader &eah);
  void clear();
  
  unsigned sizeInBytes() const;
  unsigned numberOfEcondAsicTruncations() const;
  unsigned numberOfEcondBeTruncations() const;

  std::vector<SlinkData> const& slinkDataVector() const;
  
  void print(const std::string &s="") const;
  
private:
  std::vector<SlinkData> slinkDataVector_;
};

#endif
