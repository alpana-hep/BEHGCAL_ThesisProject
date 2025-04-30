#include <iostream>
#include <iomanip>

#include "Backtrace.h"
#include "EcondAsicHeader.h"


EcondAsicHeader::EcondAsicHeader() {
  word_[0]=(HeaderPattern<<23);
  word_[1]=0;
}

EcondAsicHeader::EcondAsicHeader(const RamWord &w) {
  setWordsTo(w);
}

uint32_t EcondAsicHeader::word(unsigned i) const {
  backtraceAssert(i<2);
  return word_[i];
}

void EcondAsicHeader::setWordTo(unsigned i, uint32_t w) {
  backtraceAssert(i<2);
  word_[i]=w;
}

void EcondAsicHeader::setWordsTo(const RamWord &w) {
  word_[0]=w.word(0);
  word_[1]=w.word(1);
}

uint16_t EcondAsicHeader::headerPattern() const {
  return word_[0]>>23;
}

uint16_t EcondAsicHeader::payloadLength() const {
  return (word_[0]>>14)&0x1ff;
}

void EcondAsicHeader::setPayloadLengthTo(uint16_t l) {
  backtraceAssert(l<512);
  word_[0]=(word_[0]&0xff803fff)|(l<<14);
  if(l==0) word_[0]|=0x40; // Set truncation bit
}


uint16_t EcondAsicHeader::packetWords() const {
  return payloadLength();
}

uint16_t EcondAsicHeader::totalPacketLength() const {
  return payloadLength()+2;
}

void EcondAsicHeader::setPacketWordsTo(uint16_t w) {
  setPayloadLengthTo(w);
}

uint8_t EcondAsicHeader::qualityFlags() const {
  return ((word_[1]&0x00000400)>>3)|((word_[0]&0x00003f80)>>7);
}

void EcondAsicHeader::setQualityFlagsTo(uint8_t f) {
  word_[0]=(word_[0]&0xffffc07f)|((f&0x7f)<<7);
  word_[1]=(word_[1]&0xfffffbff)|((f&0x80)<<3);
}

bool EcondAsicHeader::truncated() const {
  return (word_[0]&0x40)!=0;
}

uint8_t EcondAsicHeader::hamming() const {
  return word_[0]&0x3f;
}

EcondAsicHeader::DataType EcondAsicHeader::dataType() const {
  return (DataType)((word_[0]>>6)&0x1);
}

void EcondAsicHeader::setDataTypeTo(EcondAsicHeader::DataType t) {
  word_[0]=(word_[0]&0xffffffbf)|((t&0x1)<<6);
}

FastControlCounters EcondAsicHeader::counters() const {
  FastControlCounters fcc;
  fcc.setOcTo((word_[1]>>11)&0x7);
  fcc.setEcTo((word_[1]>>14)&0x3f);
  fcc.setBcTo((word_[1]>>20)&0xfff);
  return fcc;
}

void EcondAsicHeader::setCountersTo(const FastControlCounters &c) {
  word_[1]=(word_[1]&0x000007ff)|(c.oc()&0x7)<<11|(c.ec()&0x3f)<<14|(c.bc()&0xfff)<<20;
}

EcondAsicHeader::ResetRequest EcondAsicHeader::resetRequest() const {
  return (EcondAsicHeader::ResetRequest)((word_[1]>>8)&0x3); // WRONG!!!
}

uint32_t EcondAsicHeader::timeDelayFrom(uint64_t bx) const {
  FastControlCounters fcc(counters());
  uint32_t bxEah(FastControlCounters::MaximumBc*fcc.oc()+fcc.bc());
  uint32_t bxMod((bx-bxEah)%(FastControlCounters::MaximumBc*8));
  return bxMod;
}

void EcondAsicHeader::setResetRequestTo(EcondAsicHeader::ResetRequest r) {
  word_[1]=(word_[1]&0xfffffcff)|((r&0x3)<<8);
}

uint8_t EcondAsicHeader::crc() const {
  return (word_[1]&0xff);
}

uint8_t EcondAsicHeader::calculateCrc() const {
  const uint16_t key(0x1a7); // x^8+x^7+x^5+x^2+x^1+x^0 = 110100111

  uint64_t d(word_[0]);
  d=(d<<32)|(word_[1]&0xffffff00);

  uint16_t tmp(d>>55);

  for(unsigned i(9);i<=64;i++) {
    if((tmp&0xff00)!=0) tmp=key^tmp;
    else                tmp=  0^tmp;
    if(i<64) tmp=(tmp<<1)|((d>>(63-i))&0x1);
  }

  backtraceAssert((tmp&0xff00)==0);
  return tmp&0xff;
}

bool EcondAsicHeader::validCrc() const {
  /*
  uint8_t sum(word_[0]&0xc0);
  for(unsigned i(0);i<2;i++) {
    sum+=(word_[i]>> 8)&0xff;
    sum+=(word_[i]>>16)&0xff;
    sum+=(word_[i]>>24)&0xff;
  }
  sum+=(word_[1]    )&0xff;
  return sum==0;
  */
  return crc()==calculateCrc();
}

void EcondAsicHeader::setCrc() {
  /*
  uint8_t sum(word_[0]&0xc0);
  for(unsigned i(0);i<2;i++) {
    sum+=(word_[i]>> 8)&0xff;
    sum+=(word_[i]>>16)&0xff;
    sum+=(word_[i]>>24)&0xff;
  }
  sum=(~sum)+1;
  word_[1]=(word_[1]&0xffffff00)|sum;
  */
  word_[1]=(word_[1]&0xffffff00)|calculateCrc();

  assert(validCrc()); // TEMP
}

bool EcondAsicHeader::operator==(const EcondAsicHeader &h) const {
  return ((word_[0]&0xfff90001)==(h.word_[0]&0xfff90001))
      && ((word_[1]&0xfc01ffff)==(h.word_[1]&0xfc01ffff));
}

bool EcondAsicHeader::operator==(const FastControlCounters &c) const {
  return (word_[1]&0xfffff800)==((c.ec()&0x3f)<<14|(c.bc()&0xfff)<<20|(c.oc()&0x7)<<11);
}

bool EcondAsicHeader::consistentTimeWith(const FastControlCounters &c) const {
  return (word_[1]&0xfff03800)==((c.bc()&0xfff)<<20|(c.oc()&0x7)<<11);
}

bool EcondAsicHeader::valid() const {
  return true; // ???
}

void EcondAsicHeader::print(const std::string &s) const {
  std::cout << s << "EcondAsicHeader::print()"
	    << "  Words =" << std::hex << std::setfill('0');
  for(unsigned i(0);i<2;i++) {
    std::cout << " 0x" << std::setw(8) << word_[i];
  }
  std::cout << std::dec << std::setfill(' ') << std::endl;

  std::cout << s << " Header pattern = 0x" << std::hex << std::setfill('0');
  std::cout << std::setw(4) << headerPattern();
  std::cout << std::dec << std::setfill(' ') << std::endl;
  
  std::cout << s << " Packet words = " << packetWords();
  //	    << ", data type = ";
  //if     (dataType()==   Normal) std::cout << "Normal";
  //else if(dataType()==Truncated) std::cout << "Truncated";
  if(truncated()) std::cout << ", truncated";
  std::cout << std::endl;
  
  std::cout << s << " Quality flags = "
	    << unsigned(qualityFlags()) << std::endl;

  counters().print(s+" ");

  std::cout << s << " Reset request = " << resetRequest() << " = ";
  if     (resetRequest()==    None) std::cout << "None";
  else if(resetRequest()==RequestA) std::cout << "RequestA";
  else if(resetRequest()==RequestB) std::cout << "RequestB";
  else if(resetRequest()==RequestC) std::cout << "RequestC";
  else std::cout << "Unknown";
  std::cout << std::endl;

  std::cout << s << " CRC = 0x" << std::hex << std::setfill('0')
	    << std::setw(2) << unsigned(crc())
	    << std::dec << std::setfill(' ') << " = "
	    << (validCrc()?"valid":"invalid") << std::endl;
}
