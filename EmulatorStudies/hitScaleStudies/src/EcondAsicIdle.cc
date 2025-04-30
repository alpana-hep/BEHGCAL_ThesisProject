#include <iostream>
#include <iomanip>
#include <cassert>

#include "Backtrace.h"
#include "EcondAsicIdle.h"


EcondAsicIdle::EcondAsicIdle() {
  word_=IdlePattern;
}

EcondAsicIdle::EcondAsicIdle(uint32_t w) {
  word_=w;
}

uint32_t EcondAsicIdle::word() const {
  return word_;
}

void EcondAsicIdle::setWordTo(uint32_t w) {
  word_=w;
}

uint32_t EcondAsicIdle::idlePattern() const {
  return word_&0xffffffc0;
}

EcondAsicIdle::BufferStatus EcondAsicIdle::bufferStatus() const {
  return (BufferStatus)(word_&0x7);
}

void EcondAsicIdle::setBufferStatusTo(EcondAsicIdle::BufferStatus s) {
  word_=(word_&0xfffffff8)|(s&0x7);
}

uint8_t EcondAsicIdle::errorStatus() const {
  return (word_>>3)&0x7;
}

void EcondAsicIdle::setErrorStatusTo(uint8_t e) {
  backtraceAssert(e<8);
  word_=(word_&0xffffffc7)|(e<<3);
}

EcondAsicHeader::ResetRequest EcondAsicIdle::resetRequest() const {
  return (EcondAsicHeader::ResetRequest)((word_>>6)&0x3);
}

void EcondAsicIdle::setResetRequestTo(EcondAsicHeader::ResetRequest r) {
  word_=(word_&0xffffff3f)|((r&0x3)<<6);
}

bool EcondAsicIdle::operator==(const EcondAsicIdle &h) const {
  return word_==h.word_;
}

bool EcondAsicIdle::valid() const {
  return idlePattern()==IdlePattern &&
    errorStatus()==0 && bufferStatus()<=High;
}

void EcondAsicIdle::print(const std::string &s) const {
  std::cout << s << "EcondAsicIdle::print()"
	    << "  Word =" << std::hex << std::setfill('0')
	    << " 0x" << std::setw(8) << word_
	    << std::dec << std::setfill(' ') << ", valid = "
	    << (valid()?"True":"False") << std::endl;

  std::cout << s << " Idle pattern = 0x" << std::hex << std::setfill('0')
	    << std::setw(8) << idlePattern() << ", expected = 0x"
	    << std::setw(8) << IdlePattern
	    << std::dec << std::setfill(' ') << std::endl;

  std::cout << s << " Reset request = " << resetRequest() << " = ";
  if     (resetRequest()==EcondAsicHeader::None    ) std::cout << "None";
  else if(resetRequest()==EcondAsicHeader::RequestA) std::cout << "RequestA";
  else if(resetRequest()==EcondAsicHeader::RequestB) std::cout << "RequestB";
  else if(resetRequest()==EcondAsicHeader::RequestC) std::cout << "RequestC";
  else std::cout << "Unknown";
  std::cout << std::endl;
  
  std::cout << s << " Error status = " << unsigned(errorStatus()) << std::endl;

  std::cout << s << " Buffer status = " << bufferStatus() << " = ";
  if     (bufferStatus()== Empty) std::cout << "Empty";
  else if(bufferStatus()==   Low) std::cout << "Low";
  else if(bufferStatus()==Medium) std::cout << "Medium";
  else if(bufferStatus()==  High) std::cout << "High";
  else std::cout << "Unknown";
  std::cout << std::endl;
}
