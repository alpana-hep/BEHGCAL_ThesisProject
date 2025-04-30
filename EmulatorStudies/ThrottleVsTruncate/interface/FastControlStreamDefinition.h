#ifndef Emulation_HGCalBufferModel_FastControlStreamDefinition_h
#define Emulation_HGCalBufferModel_FastControlStreamDefinition_h

#include <vector>
#include <string>

#include "EcondAsicDefinition.h"
//#include "EcontAsicDefinition.h"


class FastControlStreamDefinition {
 public:
  FastControlStreamDefinition();

  void setSectorNumberTo(unsigned s);

  unsigned lpgbtNumber() const;
  void setLpgbtNumberTo(unsigned l);

  unsigned streamNumber() const;
  void setStreamNumberTo(unsigned s);

  void setNumbersTo(unsigned l, unsigned s);
  
  void addModule(const EcondAsicDefinition &d); // REMOVE
  void addEcondAsic(const EcondAsicDefinition &d);
  //void addEcontAsic(const EcontAsicDefinition &d);

  const std::vector<EcondAsicDefinition>& moduleDefinition() const; // REMOVE
  const std::vector<EcondAsicDefinition>& econdAsicDefinition() const;
  //const std::vector<EcontAsicDefinition>& econtAsicDefinition() const;

  void econdAsicDefinitions(std::vector<EcondAsicDefinition> &v) const;
  //void econtAsicDefinitions(std::vector<EcontAsicDefinition> &v) const;

  std::string label() const;

  void print(const std::string &s="") const;

 private:
  unsigned sectorNumber_;
  unsigned lpgbtNumber_;
  unsigned streamNumber_;
  std::vector<EcondAsicDefinition> econdAsicDefinitionVector_;
  //std::vector<EcontAsicDefinition> econtAsicDefinitionVector_;
};

#endif
