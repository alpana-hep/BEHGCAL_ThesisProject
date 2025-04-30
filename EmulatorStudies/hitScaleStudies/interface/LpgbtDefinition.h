#ifndef Emulation_HGCalBufferModel_LpgbtDefinition_h
#define Emulation_HGCalBufferModel_LpgbtDefinition_h

#include <vector>
#include <string>

#include "FastControlStreamDefinition.h"

class LpgbtDefinition {
 public:
  LpgbtDefinition();

  void setSectorNumberTo(unsigned s);

  void setLpgbtNumberTo(unsigned n);
  void setLpgbtNumbersTo(unsigned n0, unsigned n1);
  unsigned lpgbtNumber(unsigned i=0) const;

  bool singleLpgbt() const;
  
  void addFastControlStream(const FastControlStreamDefinition &d);

  const std::vector<FastControlStreamDefinition>& fastControlStreamDefinitions() const;

  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;
  //void econtAsicDefinitions(std::vector<EcontAsicDefinition> &v) const;

  uint64_t uniqueId() const;
  std::string uniqueIdString() const;
  
  std::string label() const;

  void print(const std::string &s="") const;

 private:  
  unsigned sectorNumber_;
  std::vector<unsigned> lpgbtNumberVector_;
  std::vector<FastControlStreamDefinition> streamVector_;
};

#endif
