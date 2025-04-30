#ifndef Emulation_HGCalBufferModel_Definer_h
#define Emulation_HGCalBufferModel_Definer_h

#include <vector>
#include<fstream>
#include<iostream>
#include "SlinkDefinition.h"

class Definer {
 public:
  Definer();
  
  const std::vector<LpgbtDefinition>& lpgbtDefinitionVector() const;
  
  void defineSlink(const std::vector<unsigned> &lpgbts, SlinkDefinition &d);
  void readFile(const std::string &filename); // added by Alpana. Date: 08-May-2020
 private:
  std::vector<LpgbtDefinition> lpgbtDefinitionVector_;
};

#endif
