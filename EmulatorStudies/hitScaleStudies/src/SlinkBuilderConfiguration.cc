#include <iostream>
#include <iomanip>
#include <cassert>

#include "SlinkBuilderConfiguration.h"

// 2 x URAM = 8k x 64 bits = 4k x 128 bits = 4k SlinkWords
unsigned SlinkBuilderConfiguration::defaultBufferSize_(4*1024);

SlinkBuilderConfiguration::SlinkBuilderConfiguration() :
  bufferSize_(defaultBufferSize_) {
}

unsigned SlinkBuilderConfiguration::bufferSize() const {
  return bufferSize_;
}

void SlinkBuilderConfiguration::setBufferSizeTo(unsigned s) {
  bufferSize_=s;
}

void SlinkBuilderConfiguration::print(const std::string &s) const {
  std::cout << s << "SlinkBuilderConfiguration::print()" << std::endl
	    << s << " Buffer size = "
	    << bufferSize_ << std::endl;
}
