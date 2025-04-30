#include <unistd.h>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "TFile.h"

#include "Arguments.h"
#include "Connection.h"
#include "GlobalConfiguration.h"
#include "FastControlDriver.h"
#include "FastControlDriverConfiguration.h"


int main(int argc, char *argv[]) {
  Arguments args(argc,argv);
  args.print();

  FastControlDriverConfiguration fcdc;
  
  // Set overall L1A rate
  fcdc.setL1aRateKhzTo(args.triggerRate());

  // Set HGCROC buffer limit
  //fcdc.setHgcrocBufferLimitTo(999999);

  // Choose HL-LHC bunch structire
  if(args.filenameArg()) fcdc.setHllhcFilenameTo(args.filename());
  
  // Turn off calibration L1As
  //fcdc.setCalRateHzTo(0);
  
  // Change trigger rule limit
  //fcdc.setTriggerRulePeriodTo(640);
  if(args.numberArg()) fcdc.setTriggerRuleLimitTo(args.number());
  
  FastControlDriver fcd;
  fcd.setConfigurationTo(fcdc);
  
  TFile rootFile((args.summaryString()+".root").c_str(),"RECREATE");

  Entity::printTable();
  Entity::doTick(args);
  Entity::printAll();

  rootFile.Write();
  rootFile.Close();

  return 0;
};
