#include <iostream>
#include <iomanip>
#include <cassert>

#include "GlobalConfiguration.h"

GlobalConfiguration GlobalConfiguration::configuration_;

GlobalConfiguration::GlobalConfiguration() :
  calibrationQueueSize_(500),
  preL1aQueueSize_(3),
  daqPacketFormat_(3),
  tpgTimeMplex_(18),
  slink25GbitPerSec_(false) {
}

GlobalConfiguration& GlobalConfiguration::configuration() {
  return configuration_;
}

unsigned GlobalConfiguration::calibrationQueueSize() const {
  return calibrationQueueSize_;
}

void GlobalConfiguration::setCalibrationQueueSizeTo(unsigned s) {
  calibrationQueueSize_=s;
}

unsigned GlobalConfiguration::preL1aQueueSize() const {
  return preL1aQueueSize_;
}

void GlobalConfiguration::setPreL1aQueueSizeTo(unsigned s) {
  preL1aQueueSize_=s;
}

unsigned GlobalConfiguration::beQueueSize() const {
  return beQueueSize_;
}

void GlobalConfiguration::setBeQueueSizeTo(unsigned s) {
  beQueueSize_=s;
}

unsigned GlobalConfiguration::daqPacketFormat() const {
  return daqPacketFormat_;
}

void GlobalConfiguration::setDaqPacketFormatTo(unsigned f) {
  daqPacketFormat_=f;
}

unsigned GlobalConfiguration::tpgTimeMplex() const {
  return tpgTimeMplex_;
}

void GlobalConfiguration::setTpgTimeMplexTo(unsigned t) {
  tpgTimeMplex_=t;
}

bool GlobalConfiguration::slink25GbitPerSec() const {
  return slink25GbitPerSec_;
}

void GlobalConfiguration::setSlink25GbitPerSecTo(bool b) {
  slink25GbitPerSec_=b;
}

Clocks::Frequency GlobalConfiguration::slinkClockFrequency() const {
  if(slink25GbitPerSec_) return Clocks::Clock0180;
  return Clocks::Clock0120;
}

Clocks::Frequency GlobalConfiguration::slinkBuilderClockFrequency() const {
  //if(slink25GbitPerSec_) return Clocks::Clock0400;
  if(slink25GbitPerSec_) return Clocks::Clock0360;
  return Clocks::Clock0280;
}

void GlobalConfiguration::print(const std::string &s) const {
  std::cout << s << "GlobalConfiguration::print()" << std::endl
	    << s << " Calibration queue size = " << calibrationQueueSize_ << std::endl
	    << s << " Pre-L1A queue size = " << preL1aQueueSize_ << std::endl
	    << s << " BE queue size = " << beQueueSize_ << std::endl
	    << s << " DAQ packet format = " << daqPacketFormat_;
  if(daqPacketFormat_==0) std::cout << " = Sep19" << std::endl;
  else if(daqPacketFormat_==1) std::cout << " = Nov19" << std::endl;
  else if(daqPacketFormat_==2) std::cout << " = Jun20" << std::endl;
  else std::cout << " = Dec20" << std::endl;
  std::cout << s << " TPG time multiplexing period = "
	    << tpgTimeMplex_ << std::endl;
  std::cout << s << " Slink speed 25 Gbit/s = "
	    << (slink25GbitPerSec_?"true":"false") << std::endl
	    << s << "  Slink blocks clock speed = "
	    << Clocks::frequencyName(slinkClockFrequency()) << std::endl
	    << s << "  SlinkBuilder clock speed = "
	    << Clocks::frequencyName(slinkBuilderClockFrequency()) << std::endl;
}
