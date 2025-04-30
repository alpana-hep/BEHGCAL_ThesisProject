#include <iostream>
#include <iomanip>

#include "EcondBeTrailer.h"

EcondBeTrailer::EcondBeTrailer() {
  word_=Pattern<<24;
}

bool EcondBeTrailer::valid() const {
  return (word_>>24)==Pattern;
}

uint32_t EcondBeTrailer::word() const {
  return word_;
}

void EcondBeTrailer::setWordTo(uint32_t w) {
  word_=w;
}

uint8_t EcondBeTrailer::errorBits() const {
  return (word_>>16)&0xff;
}

void EcondBeTrailer::setErrorBitsTo(uint8_t b) {
  word_=(word_&0xff00ffff)|(b<<16);
}

uint16_t EcondBeTrailer::packetLength() const {
  return word_&0x0000ffff;
}

void EcondBeTrailer::setPacketLengthTo(uint16_t l) {
  word_=(word_&0xffff0000)|l;
}

void EcondBeTrailer::print() const {
  std::cout << "EcondBeTrailer::print()  Value = 0x"
	    << std::hex << std::setw(8) << std::setfill('0') << word_
	    << std::dec << " = " << std::setw(10) << std::setfill(' ')
	    << word_ << std::endl << " Pattern = 0x"
	    << std::hex << std::setw(2) << std::setfill('0') << (word_>>24)
	    << ", error bits = 0x" << std::setw(2) 
	    << (unsigned)errorBits()
	    << ", packet length = " << std::dec << packetLength()
	    << std::setfill(' ') << std::endl;
}
