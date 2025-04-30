#include <iostream>
#include <iomanip>
#include <cassert>

#include "Backtrace.h"
#include "SlinkWord.h"

SlinkWord::SlinkWord() {
}

void SlinkWord::clear() {
  for(unsigned i(0);i<4;i++) word_[i]=0;
}

const uint32_t& SlinkWord::word(unsigned i) const {
  backtraceAssert(i<4);
  return word_[i];
}

void SlinkWord::setWordTo(unsigned i, uint32_t w) {
  backtraceAssert(i<4);
  word_[i]=w;
}

void SlinkWord::setTo(const FastControlCounters &, const EcondAsicHeader &) {
  /*
  uint64_t hWord0(h.word32(0));
  uint64_t hWord2(h.word32(2));
  word_[0]=(hWord0<<32)|c.ec()<<24|c.oc()<<16|c.bc();
  word_[1]=(hWord2<<32)|h.word32(1);
  */
}

void SlinkWord::setTo(unsigned i, const RamWord &w) {
  backtraceAssert(i<2);
  word_[2*i]=w.word(0);
  word_[2*i+1]=w.word(1);
}

void SlinkWord::setTo(unsigned i, uint16_t w) {
  backtraceAssert(i<8);
  uint32_t w32(w);
  if((i%2)==0) {
    word_[i/2]&=0xffff0000;
    word_[i/2]|=w32;
  } else {
    word_[i/2]&=0x0000ffff;
    word_[i/2]|=w32<<16;
  }
}

FastControlCounters SlinkWord::fcCounters() const {
  FastControlCounters c;
  c.setBcTo(word_[0]&0xffff);
  c.setOcTo((word_[0]>>16)&0xff);
  c.setEcTo((word_[1]>>8)&0xff);
  return c;
}

const EcondAsicHeader& SlinkWord::econdHeader() const {
  return (const EcondAsicHeader&)word_[2];
}

uint16_t SlinkWord::crcIncrement() const {
  uint16_t crc(0);
  for(unsigned i(0);i<4;i++) {
    crc+=word_[i]&0xffff;
    crc+=word_[i]>>16;
  }
  return crc;
}

void SlinkWord::print(const std::string &s) const {
  std::cout << s << " SlinkWord::print()  Value ="
	    << std::hex << std::setfill('0')
	    << " 0x" << std::setw(8) << word_[0]
	    << " 0x" << std::setw(8) << word_[1]
	    << " 0x" << std::setw(8) << word_[2]
	    << " 0x" << std::setw(8) << word_[3]
	    << std::dec << std::setfill(' ') << std::endl;
}
