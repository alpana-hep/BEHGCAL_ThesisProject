#include <iostream>
#include <iomanip>
#include <cassert>

#include "EcondBeHeader.h"


EcondBeHeader::EcondBeHeader() {
  word_[0]|=HeaderPattern;
}

void EcondBeHeader::setErrorBitsTo(uint8_t c) {
  word_[1]=(word_[1]&0xfc03ffff)|(c<<18);
}

uint8_t EcondBeHeader::errorBits() const {
  return (word_[1]>>18)&0xff;
}

bool EcondBeHeader::operator==(const EcondBeHeader &h) const {
  return EcondHeader::operator==(h) &&
    pattern()==h.pattern() && errorBits()==h.errorBits();
}

bool EcondBeHeader::operator==(const FastControlCounters &c) const {
  return EcondHeader::operator==(c);
}

bool EcondBeHeader::valid() const {
  return EcondHeader::valid() && pattern()==HeaderPattern;
}

void EcondBeHeader::print() const {
  std::cout << "EcondBeHeader::print() ";
  EcondHeader::print();

  std::cout << " Error bits = 0x" << std::hex  << std::setfill('0')
	    << std::setw(2) << (unsigned)errorBits()
	    << std::dec << std::setfill(' ') << std::endl;

  std::cout << " Valid = " << (valid()?"true":"false") << std::endl;
}
