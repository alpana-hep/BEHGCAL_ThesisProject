#include <iostream>
#include <iomanip>
#include <cassert>

#include "EcondHeader.h"


EcondHeader::EcondHeader() {
  // Due to valid bit in EcondAsicHeader
  word_[0]=0x00010000;
  word_[1]=0x00010001;
}

uint32_t EcondHeader::word(unsigned i) const {
  assert(i<sizeof(*this));
  return word_[i];
}

void EcondHeader::setWordTo(unsigned i, uint32_t w) {
  assert(i<sizeof(*this));
  word_[i]=w;
}

uint16_t EcondHeader::pattern() const {
  return word_[0]&0xffff;
}

uint16_t EcondHeader::packetLength() const {
  return word_[0]>>22;
}

void EcondHeader::setPacketLengthTo(uint16_t l) {
  assert(l<1024);
  word_[0]=(word_[0]&0x003fffff)|(l<<22);
}

uint16_t EcondHeader::packetWords() const {
  return (packetLength()+1)/2;
}

bool EcondHeader::padding() const {
  return (packetLength()%2)==1;
}

bool EcondHeader::hdm() const {
  return (word_[0]&0x00020000)!=0;
}

void EcondHeader::setHdmTo(bool b) {
  if(b) word_[0]|= 0x00020000;
  else  word_[0]&=~0x00020000;
}

EcondHeader::DataType EcondHeader::dataType() const {
  return (DataType)((word_[0]>>18)&0x1); // Only use 1 bit of 2 available
}

void EcondHeader::setDataTypeTo(EcondHeader::DataType t) {
  word_[0]=(word_[0]&0xfffbffff)|(t&0x1)<<18; // Only use 1 bit of 2 available
}

EcondHeader::BufferStatus EcondHeader::bufferStatus() const {
  return (BufferStatus)((word_[0]>>19)&0x3); // Only use 2 bits of 3 available
}

void EcondHeader::setBufferStatusTo(EcondHeader::BufferStatus s) {
  word_[0]=(word_[0]&0xffc7ffff)|(s&0x3)<<19; // Only use 2 bits of 3 available
}

FastControlCounters EcondHeader::counters() const {
  FastControlCounters fcc;
  fcc.setBcTo((word_[1]>>4)&0xfff);
  fcc.setOcTo((word_[1]>>1)&0x7);
  fcc.setEcTo(word_[1]>>26);
  return fcc;
}

void EcondHeader::setCountersTo(const FastControlCounters &c) {
  word_[1]=(c.ec()&0x3f)<<26|(c.bc()&0xfff)<<4|(c.oc()&0x7)<<1|0x00010001;
}

bool EcondHeader::operator==(const EcondHeader &h) const {
  return ((word_[0]&0xfff90001)==(h.word_[0]&0xfff90001))
      && ((word_[1]&0xfc01ffff)==(h.word_[1]&0xfc01ffff));
}

bool EcondHeader::operator==(const FastControlCounters &c) const {
  return word_[1]==((c.ec()&0x3f)<<26|(c.bc()&0xfff)<<4|(c.oc()&0x7)<<1|0x00010001);
  return (word_[1]&0xfffefffe)==((c.ec()&0x3f)<<26|(c.bc()&0xfff)<<4|(c.oc()&0x7)<<1);
}

bool EcondHeader::valid() const {
  return (word_[0]&0x00210001)==0x00010000 &&
    (word_[1]&0x01ff0001)==0x00010001;
}

void EcondHeader::print(const std::string &s) const {
  std::cout << s << "EcondHeader::print()"
	    << "  Words =" << std::hex << std::setfill('0');
  for(unsigned i(0);i<2;i++) {
    std::cout << " 0x" << std::setw(8) << word_[i];
  }
  std::cout << std::dec << std::setfill(' ') << std::endl;

  std::cout << s << " Pattern = 0x" << std::hex << std::setfill('0');
  std::cout << std::setw(4) << pattern();
  std::cout << std::dec << std::setfill(' ') << std::endl;
  
  std::cout << s << " Packet length = " << packetLength()
	    << " half-words (16-bit), " << packetWords()
	    << " words (32-bit)" << (padding()?" with padding":"")
	    << std::endl
	    << s << " Type = " << (hdm()?"HDM":"LDM")
	    << std::endl;

  std::cout << s << " Buffer status = ";
  if     (bufferStatus()==   Low) std::cout << "Low";
  else if(bufferStatus()==Medium) std::cout << "Medium";
  else if(bufferStatus()==  High) std::cout << "High";
  else if(bufferStatus()==  Full) std::cout << "Full";
  else                            std::cout << "Unknown";
  std::cout << std::endl;

  std::cout << s << " Data type = ";
  if     (dataType()==   Normal) std::cout << "Normal";
  else if(dataType()==Truncated) std::cout << "Truncated";
  std::cout << std::endl << " ";

  counters().print();
}
