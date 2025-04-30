#ifndef Emulation_HGCalBufferModel_BEMappingStruct_h
#define Emulation_HGCalBufferModel_BEMappingStruct_h
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"

#include "Arguments.h"
#include "Connection.h"
#include "Definer.h"
#include "FastControlCommands.h"
#include "FastControlRequests.h"
#include "FastControlCounters.h"
#include "FastControlDriver.h"
#include "EcondAsic.h"
#include "HgcrocAsics.h"
#include "SlinkDefinition.h"
#include "LpgbtDefinition.h"
#include "SectorDefinition.h"
class BEMappingStruct {
public:
  BEMappingStruct();
  std::vector<SectorDefinition> const& SectorDefinitionVector() const;
  //  const std::vector<SlinkDefinition>& SlinkDefinitionVector() const;

  //const std::pair<std::vector<float>, float>& Datarates() const;
  //a function to read the Slink to LpGBT mapping file
  //void defineSlinkMapping(const std::string &filename);
  /* std::vector<std::pair< std::vector<std::pair<float,float>>, float>> defineSlinkMapping(const std::string &filename); */
  /* std::vector<std::pair< std::vector<std::pair<float,float>>, float>>Datarates; */
  void readBEFile(const std::string &filename, const std::string &FE_filename, SectorDefinition &sd);
  //function to create objects after reading the above file
  //void CreateBEMappingObjects(const std::vector<unsigned> &SlinkTOlpgbts);
  //std::vector<LpgbtDefinition> lpgbtDefinitionVector_;

private:
  std::vector<SectorDefinition> SectorDefinitionVector_;
  //std::vector<SlinkDefinition> SlinkDefinitionVector_;
  //std::pair< std::vector<float>, float> Datarates;
};
#endif
