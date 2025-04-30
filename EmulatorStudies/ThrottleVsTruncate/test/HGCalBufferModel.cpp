#include <unistd.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TObjString.h"

#include "Arguments.h"
#include "Initialisation.h"
#include "Connection.h"
#include "GlobalConfiguration.h"
#include "Definer.h"
#include "FastControlCommands.h"
#include "FastControlCounters.h"
#include "FastControlDriver.h"
#include "EcondAsic.h"
#include "LpgbtPairSerialiser.h"
#include "HgcrocAsics.h"
#include "PacketChecker.h"
#include "Sector.h"
#include "CentralDaq.h"
#include "BEMappingStruct.h"
#include "Utils.h"


int main(int argc, char *argv[]) {
  Arguments args(argc,argv);
  args.print();

  Initialisation(args);
  /*
  std::cout << args.encodeArgs() << "\n";

  GlobalConfiguration::configuration().setDaqPacketFormatTo(3);
  // If -x is set, returns true
  GlobalConfiguration::configuration().setSlink25GbitPerSecTo(args.slink25GbitPerSec());
  GlobalConfiguration::configuration().setBeQueueSizeTo(args.config().at("fpgaFastControlDelay").get<unsigned>());
  GlobalConfiguration::configuration().print();
  std::cout << std::endl;

  EcondAsicDefinition::readDaqModules();
  std::cout << std::endl;
  */
  
  SectorDefinition sd;

  BEMappingStruct be;
  be.readBEFile(std::string("data/BE/")+args.beMappingFile(),args.feMappingFile(),sd);
  sd.print();

  FastControlDriver fcd;
  HgcrocAsics has;

  TFile rootFile(args.fullOutputFilename().c_str(),"RECREATE");

  if(args.config().at("fpgaNumberArg").get<bool>()) {
    unsigned f(args.config().at("fpgaNumber").get<unsigned>());
    
    if(args.config().at("slinkNumberArg").get<bool>()) {
      unsigned s(args.config().at("slinkNumber").get<unsigned>());
      
      if(args.config().at("lpgbtPairNumberArg").get<bool>()) {
	unsigned lp(args.config().at("lpgbtPairNumber").get<unsigned>());
	
	if(args.config().at("econdNumberArg").get<bool>()) {
	  //unsigned e(args.config().at("econdNumber").get<unsigned>());
	  
	  FastControlStream *fcs(new FastControlStream);
	  fcs->connect(fcd);
	  EcondAsic econdAsic;
	  //econdAsic.definition(sd.lpgbtPairDefinition(f,s,lp,e));
	  econdAsic.connect(*fcs);
	  has.connect(econdAsic);
	} else {
	  LpgbtPair lpgbtPair;
	  lpgbtPair.setDefinitionTo(sd.lpgbtPairDefinition(f,s,lp));
	  lpgbtPair.connect(fcd);
	  lpgbtPair.connect(has);
	}
      } else {
	Slink slink;
	slink.setDefinitionTo(sd.slinkDefinition(f,s));
	slink.connect(fcd);
	slink.connect(has);
      }
    } else {
      Fpga fpga;
      fpga.setDefinitionTo(sd.fpgaDefinition(f));
      fpga.connect(fcd);
      fpga.connect(has);
    }
    
  } else {
    Sector sector;
    sector.setDefinitionTo(sd);
    sector.connect(fcd);
    sector.connect(has);

    // Add on central DAQ if requested
    if(args.config().at("centralDaq").get<bool>()) {      
      CentralDaq *cd(new CentralDaq());
      cd->setDefinitionTo(sd);
      cd->connect(sector);
    }
  }
  
  TObjString(args.encodeArgs().c_str()).Write("config");

  Entity::printTable();
  Entity::doTick(args);
  Entity::printAll();

  rootFile.Write();
  rootFile.Close();

  return 0;
};
