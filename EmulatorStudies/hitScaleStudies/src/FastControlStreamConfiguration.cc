#include <iostream>
#include <iomanip>
#include <cassert>

#include "FastControlStreamConfiguration.h"

FastControlStreamConfiguration::FastControlStreamConfiguration() :
  noZeroSuppressionPeriod_(0),
  nzsProbability_(0.0) {
}

unsigned FastControlStreamConfiguration::noZeroSuppressionPeriod() const {
  return noZeroSuppressionPeriod_;
}

void FastControlStreamConfiguration::setNoZeroSuppressionPeriodTo(unsigned p) {
  noZeroSuppressionPeriod_=p;
}

double FastControlStreamConfiguration::nzsProbability() const {
  if(nzsProbability_<0.0) return 0.0;
  if(nzsProbability_>1.0) return 1.0;
  return nzsProbability_;
}

void FastControlStreamConfiguration::setNzsProbabilityTo(double p) {
  nzsProbability_=p;
}

void FastControlStreamConfiguration::print(const std::string &s) const {
  std::cout << s << "FastControlStreamConfiguration::print()" << std::endl
	    << s << " No zero suppression period = "
	    << noZeroSuppressionPeriod_ << std::endl
	    << s << " Local NZS probability = "
	    << nzsProbability_ << std::endl;
}
