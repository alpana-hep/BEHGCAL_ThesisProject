#include <iostream>
#include <sstream>

#include "GlobalConfiguration.h"
#include "EcondAsicDefinition.h"
#include "Initialisation.h"


void Initialisation(const Arguments &args) {

  // Print list of arguments
  std::cout << args.encodeArgs() << std::endl << std::endl;

  // Set up global configuration

  // Data format; V3 is Dec 20 which is the final version
  GlobalConfiguration::configuration().setDaqPacketFormatTo(3);

  // Switch between 16 and 25 Gbit/s Slinks
  GlobalConfiguration::configuration().setSlink25GbitPerSecTo(args.slink25GbitPerSec());

  // Set length of delayed fast control stream in FPGA used by BE
  GlobalConfiguration::configuration().setBeQueueSizeTo(args.config().at("fpgaFastControlDelay").get<unsigned>());

  // Print resulting parameters
  GlobalConfiguration::configuration().print();
  std::cout << std::endl;

  // Read in file of module properties
  EcondAsicDefinition::readDaqModules();
  std::cout << std::endl;
}
