#include <iostream>
#include <cassert>

#include "RandomSingleton.h"
#include "FastControlCounters.h"


FastControlCounters::FastControlCounters() {
  TRandom &r(RandomSingleton::random());
  word_[0]=(r.Integer(1<<20))|(r.Integer(MaximumBc)+1);
  word_[1]=r.Integer(1<<31);
}

void FastControlCounters::update(const FastControlCommands &c) {

  // Handle OC and BC counters
  if(c.orbitCounterReset()) word_[0]=0xfffff000;

  if(c.bunchCounterReset() || (word_[0]&0xfff)>=MaximumBc) word_[0]&=0xfffff000;

  if((word_[0]&0xfff)==0) word_[0]+=0x1000; // Increment OC
  word_[0]++; // Safe as BC guaranteed to be < MaximumBc (and so < 4096)
  
  // Handle EC counter
  if(c.eventCounterReset()) word_[1]=0;
  if(c.l1a()) word_[1]++;
}

uint16_t FastControlCounters::bc() const {
  return word_[0]&0xfff;
}

uint32_t FastControlCounters::oc() const {
  return word_[0]>>12;
}

uint32_t FastControlCounters::ec() const {
  return word_[1];
}

uint64_t FastControlCounters::totalBx() const {
  uint64_t t(oc());
  return MaximumBc*t+bc();
}

void FastControlCounters::setBcTo(uint16_t b) {
  word_[0]=(word_[0]&0xfffff000)|(b&0xfff);
}

void FastControlCounters::setOcTo(uint32_t o) {
  word_[0]=(word_[0]&0x00000fff)|(o<<12);
}

void FastControlCounters::setEcTo(uint32_t e) {
  word_[1]=e;
}

uint32_t FastControlCounters::word(unsigned i) const {
  assert(i<2);
  return word_[i];
}

void FastControlCounters::setWordTo(unsigned i, uint32_t w) {
  assert(i<2);
  word_[i]=w;
}

bool FastControlCounters::valid() const {
  if(bc()==0) return false;
  if(bc()>MaximumBc) return false;
  if(ec()==0) return false;

  return true;
}

bool FastControlCounters::operator==(const FastControlCounters &c) const {
  return word_[0]==c.word_[0] && word_[1]==c.word_[1];
}

void FastControlCounters::print(const std::string &s) const {
  std::cout << s << "FastControlCounters::print()"
	    << "  OC,BC,EC = " << oc() << ", " << bc() << ", " << ec()
	    << ", total BX = " << totalBx()
	    << std::endl;
}
