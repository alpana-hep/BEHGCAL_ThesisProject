#include <unistd.h>

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
#include "EcondAsicHeader.h"
#include "HgcrocAsics.h"
#include "BEMappingStruct.h"
#include "HGCalDefinition.h"
#include "SectorDefinition.h"

int main()
{
  BEMappingStruct be;
  SectorDefinition sd;
  be.readBEFile("data/BE/BEDAQ_224SlinksToLpGBT_Mapping.json","FeMappingV8.txt", sd);
  std::cout<<sd.label()<<std::endl;
  return 0;
}
