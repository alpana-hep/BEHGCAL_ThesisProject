#include <iostream>
#include <iomanip>

#include "Backtrace.h"
#include "SlinkBoe.h"

SlinkBoe::SlinkBoe() {
  word_[0]=BoePattern<<24;
}

SlinkBoe::SlinkBoe(const SlinkWord &w) {
  *((SlinkWord*)this)=w;
}

SlinkBoe::SlinkBoe(uint32_t si, uint32_t bi, uint32_t ei, uint16_t et) {
  word_[0]=BoePattern<<24;
  setSourceIdTo(si);
  setBxIdTo(bi);
  setEventIdTo(ei);
  setEventTypeTo(et);
}

SlinkBoe::SlinkBoe(uint32_t si, const FastControlCounters &c, uint16_t et) {
  word_[0]=BoePattern<<24;
  setSourceIdTo(si);
  setCountersTo(c);
  setEventTypeTo(et);
}

void SlinkBoe::setSourceIdTo(uint32_t s) {
  word_[3]=s;
}
  
void SlinkBoe::setBxIdTo(uint32_t b) {
  word_[2]=b;
}
  
void SlinkBoe::setEventIdTo(uint32_t e) {
  word_[1]=e;
}
  
void SlinkBoe::setEventTypeTo(uint16_t t) {
  word_[0]=(word_[0]&0xffff0000)|t;
}
  
uint32_t SlinkBoe::sourceId() const {
  return word_[3];
}

uint32_t SlinkBoe::bxId() const {
  return word_[2];
}

uint32_t SlinkBoe::eventId() const {
  return word_[1];
}

uint16_t SlinkBoe::bc() const {
  return counters().bc();
}

uint32_t SlinkBoe::oc() const {
  return counters().oc();
}

uint32_t SlinkBoe::ec() const {
  return counters().ec();
}

uint16_t SlinkBoe::eventType() const {
  return word_[0]&0xffff;
}

uint8_t SlinkBoe::boe() const {
  return (word_[0]>>24);
}

bool SlinkBoe::valid() const {
  return boe()==BoePattern;
}

unsigned SlinkBoe::sectorNumber() const {
  return (word_[3]&0x0000e000)>>13;
}

unsigned SlinkBoe::fpgaNumber() const {
  return (word_[3]&0x00001f00)>>8;
}

unsigned SlinkBoe::slinkNumber() const {
  return (word_[3]&0x000000f0)>>4;
}

unsigned SlinkBoe::numberOfLpgbtPairs() const {
  return word_[3]&0x0000000f;
}

void SlinkBoe::setFpgaNumberTo(unsigned f) {
  backtraceAssert(f<32);
  word_[3]=(word_[3]&0xffffe0ff)|(f<<8);
}

void SlinkBoe::setSlinkNumberTo(unsigned s) {
  backtraceAssert(s<16);
  word_[3]=(word_[3]&0xffffff0f)|(s<<4);
}

void SlinkBoe::setNumberOfLpgbtPairsTo(unsigned n) {
  backtraceAssert(n<256);
  word_[3]=(word_[3]&0xfffffff0)|n;
}

const FastControlCounters& SlinkBoe::counters() const {
  /*
  FastControlCounters fcc;
  fcc.setBcTo(bc());
  fcc.setOcTo(oc());
  fcc.setEcTo(ec());
  return fcc;
  */
  return (const FastControlCounters&)word_[1];
}

void SlinkBoe::setCountersTo(const FastControlCounters &c) {
  (FastControlCounters&)word_[1]=c;
  //word_[1]=c.word(0);
  //word_[2]=c.word(1);
}

bool SlinkBoe::operator==(const FastControlCounters &c) const {
  return (FastControlCounters&)word_[1]==c;
  //return word_[1]==c.word(0) && word_[2]==c.word(1);
}

void SlinkBoe::print(const std::string &s) const {
  std::cout << s << "SlinkBoe::print() ";
  SlinkWord::print();
  std::cout << s << " BOE = " << (unsigned)boe() << " = 0x" << std::hex << (unsigned)boe() << std::dec
	    << (valid()?", valid":", not valid") << std::endl;
  std::cout << s << " Event type = " << eventType() << " = 0x" << std::hex << eventType() << std::dec << std::endl;

  std::cout << s << " Event ID = EC = " << eventId() << " = 0x" << std::hex << eventId() << std::dec << std::endl;

  std::cout << s << " BX ID = " << bxId() << " = 0x" << std::hex << bxId() << std::dec << ", OC = " << oc() << ", BC = " << bc() << std::endl;
  std::cout << s << " "; counters().print();

  std::cout << s << " Source ID = " << sourceId() << " = 0x"
	    << std::hex << std::setfill('0')
	    << std::setw(8) << sourceId()
	    << std::dec << std::setfill(' ')
	    << ", F/S/LP = " << fpgaNumber() << "/" << slinkNumber()
	    << "/" << numberOfLpgbtPairs() << std::endl;
}
