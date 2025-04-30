#include <iostream>
#include <iomanip>
#include <cassert>

#include "FastControlCounters.h"
#include "FastControlDriverConfiguration.h"

FastControlDriverConfiguration::FastControlDriverConfiguration() :
  // TCDS-to-BE delays: 20m FO link (= 100ns) + two x SERDES (= 300ns), total 400 ns = 16 BX
  requestsQueueSize_(16),
  commandsQueueSize_(16),
  triggerMethod_(0),
  l1aRateKhz_(750),
  calOrbitPeriod_(0),
  //fatEventRateHz_(l1aRateKhz_),
  fatEventRateHz_(0),
  hgcrocBufferLimit_(0),
  triggerRulePeriod_(130),
  triggerRuleLimit_(triggerRulePeriod_),
  hllhcFilename_("25ns_2760b_2748_2492_2574_288bpi_13inj_800ns_bs200ns") {
}

unsigned FastControlDriverConfiguration::requestsQueueSize() const {
  return requestsQueueSize_;
}

void FastControlDriverConfiguration::setRequestsQueueSizeTo(unsigned s) {
  requestsQueueSize_=s;
}

unsigned FastControlDriverConfiguration::commandsQueueSize() const {
  return commandsQueueSize_;
}

void FastControlDriverConfiguration::setCommandsQueueSizeTo(unsigned s) {
  commandsQueueSize_=s;
}

unsigned FastControlDriverConfiguration::triggerMethod() const {
  return triggerMethod_;
}

void FastControlDriverConfiguration::setTriggerMethodTo(unsigned m) {
  triggerMethod_=m;
}

unsigned FastControlDriverConfiguration::l1aRateKhz() const {
  return l1aRateKhz_;
}

void FastControlDriverConfiguration::setL1aRateKhzTo(unsigned r) {
  l1aRateKhz_=r;
}

unsigned FastControlDriverConfiguration::calOrbitPeriod() const {
  return calOrbitPeriod_;
}

void FastControlDriverConfiguration::setCalOrbitPeriodTo(unsigned p) {
  calOrbitPeriod_=p;
}

unsigned FastControlDriverConfiguration::calRateHz() const {
  if(calOrbitPeriod_==0) return 0;
  return 40000000/(FastControlCounters::MaximumBc*calOrbitPeriod_);
}

unsigned FastControlDriverConfiguration::fatEventRateHz() const {
  return fatEventRateHz_;
}

void FastControlDriverConfiguration::setFatEventRateHzTo(unsigned r) {
  fatEventRateHz_=r;
}

double FastControlDriverConfiguration::fatEventProbability() const {
  return 0.001*fatEventRateHz_/l1aRateKhz_;
}

unsigned FastControlDriverConfiguration::hgcrocBufferLimit() const {
  return hgcrocBufferLimit_;
}

void FastControlDriverConfiguration::setHgcrocBufferLimitTo(unsigned l) {
  hgcrocBufferLimit_=l;
}

unsigned FastControlDriverConfiguration::triggerRulePeriod() const {
  return triggerRulePeriod_;
}

void FastControlDriverConfiguration::setTriggerRulePeriodTo(unsigned p) {
  triggerRulePeriod_=p;
}

unsigned FastControlDriverConfiguration::triggerRuleLimit() const {
  return triggerRuleLimit_;
}

void FastControlDriverConfiguration::setTriggerRuleLimitTo(unsigned l) {
  triggerRuleLimit_=l;
}

const std::string&  FastControlDriverConfiguration::hllhcFilename() const {
  return hllhcFilename_;
}

void FastControlDriverConfiguration::setHllhcFilenameTo(const std::string &f) {
  hllhcFilename_=f;
}

void FastControlDriverConfiguration::print(const std::string &s) const {
  std::cout << s << "FastControlDriverConfiguration::print()" << std::endl
	    << s << " Requests queue size = " << requestsQueueSize_
	    << ", delay = " << 0.025*requestsQueueSize_ << " us" << std::endl
	    << s << " Commands queue size = " << commandsQueueSize_
	    << ", delay = " << 0.025*commandsQueueSize_ << " us" << std::endl
	    << s << " Trigger method = " << triggerMethod_  << std::endl
	    << s << " L1A rate = " << l1aRateKhz_ << " kHz" << std::endl
	    << s << " Calibration orbit period = " << calOrbitPeriod_
	    << " orbits, rate = " << calRateHz() << " Hz, " << std::endl
	    << s << " Fat event rate = " << fatEventRateHz_
	    << " Hz, probability = " << fatEventProbability()
	    << " per L1A" << std::endl
	    << s << " HGCROC buffer limit = " << hgcrocBufferLimit_
	    << (hgcrocBufferLimit_==0?" = TCDS limit":"") << std::endl
	    << s << " Trigger rule limit of " << triggerRuleLimit_
	    << " L1As in any " << triggerRulePeriod_ << " BXs" << std::endl
	    << s << " HL-LHC filename = " << hllhcFilename_ << std::endl;
}
