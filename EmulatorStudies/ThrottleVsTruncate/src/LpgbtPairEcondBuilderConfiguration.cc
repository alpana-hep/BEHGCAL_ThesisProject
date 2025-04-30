#include <iostream>
#include <iomanip>
#include <cassert>

#include "LpgbtPairEcondBuilderConfiguration.h"

// Total buffer for lpGBT pair; 4 URAMs, each 4k deep
const unsigned LpgbtPairEcondBuilderConfiguration::totalBufferLevel_(4*4*1024);

// Buffer per elink given 14 elinks per lpGBT pair
const unsigned LpgbtPairEcondBuilderConfiguration::fullBufferFactor_(LpgbtPairEcondBuilderConfiguration::totalBufferLevel_/14);

unsigned LpgbtPairEcondBuilderConfiguration::defaultPhaseOf40Mhz_(1);
unsigned LpgbtPairEcondBuilderConfiguration::defaultL1aTimeout_(2);

// Throttling levels
unsigned LpgbtPairEcondBuilderConfiguration::defaultMediumBufferFactor_(0.5*LpgbtPairEcondBuilderConfiguration::fullBufferFactor_);
unsigned LpgbtPairEcondBuilderConfiguration::defaultHighBufferFactor_(0.75*LpgbtPairEcondBuilderConfiguration::fullBufferFactor_);

// Max packet size is 417 words (32-bit) = 236 URAM words (72-bit); allow overhead
unsigned LpgbtPairEcondBuilderConfiguration::defaultAlmostFullBufferSpace_(256);

unsigned LpgbtPairEcondBuilderConfiguration::defaultEbrWait_(40);


LpgbtPairEcondBuilderConfiguration::LpgbtPairEcondBuilderConfiguration() :
  phaseOf40Mhz_(defaultPhaseOf40Mhz_),
  l1aTimeout_(defaultL1aTimeout_),
  mediumBufferFactor_(defaultMediumBufferFactor_),
  highBufferFactor_(defaultHighBufferFactor_),
  almostFullBufferSpace_(defaultAlmostFullBufferSpace_) {
}

unsigned LpgbtPairEcondBuilderConfiguration::phaseOf40Mhz() const {
  return phaseOf40Mhz_;
}

void LpgbtPairEcondBuilderConfiguration::setPhaseOf40MHzTo(unsigned p) {
  phaseOf40Mhz_=p;
}

unsigned LpgbtPairEcondBuilderConfiguration::l1aTimeout() const {
  return l1aTimeout_;
}

void LpgbtPairEcondBuilderConfiguration::setL1aTimeoutTo(unsigned l) {
  l1aTimeout_=l;
}

unsigned LpgbtPairEcondBuilderConfiguration::mediumBufferFactor() const {
  return mediumBufferFactor_;
}

void LpgbtPairEcondBuilderConfiguration::setMediumBufferFactorTo(unsigned l) {
  mediumBufferFactor_=l;
}

unsigned LpgbtPairEcondBuilderConfiguration::highBufferFactor() const {
  return highBufferFactor_;
}

void LpgbtPairEcondBuilderConfiguration::setHighBufferFactorTo(unsigned l) {
  highBufferFactor_=l;
}

unsigned LpgbtPairEcondBuilderConfiguration::almostFullBufferSpace() const {
  return almostFullBufferSpace_;
}

void LpgbtPairEcondBuilderConfiguration::setAlmostFullBufferSpaceTo(unsigned l) {
  almostFullBufferSpace_=l;
}

unsigned LpgbtPairEcondBuilderConfiguration::ebrWait() const {
  return ebrWait_;
}

void LpgbtPairEcondBuilderConfiguration::setEbrWaitTo(unsigned l) {
  ebrWait_=l;
}

void LpgbtPairEcondBuilderConfiguration::print(const std::string &s) const {
  std::cout << s << "LpgbtPairEcondBuilderConfiguration::print()" << std::endl
	    << s << " Phase offset of 40 MHz = " << phaseOf40Mhz_ << " clock ticks" << std::endl
	    << s << " L1A timeout = " << l1aTimeout_ << " BX" << std::endl
	    << s << " Total buffer level = " << totalBufferLevel_ << std::endl
	    << s << " Full buffer factor = " << fullBufferFactor_ << std::endl
	    << s << " Medium buffer factor = " << mediumBufferFactor_ << std::endl
	    << s << "   High buffer factor = " << highBufferFactor_ << std::endl
	    << s << " Almost full buffer space = " << almostFullBufferSpace_ << std::endl
	    << s << " EBR wait = " << ebrWait_ << std::endl;
}
