#include <iostream>
#include <iomanip>

#include "Backtrace.h"
#include "LpgbtPairHeader.h"


LpgbtPairHeader::LpgbtPairHeader() {
  word_[0]=(HeaderPattern<<25);
  word_[1]=0;
  for(unsigned e(0);e<12;e++) setEcondStatusTo(e,Unconnected);
}

LpgbtPairHeader::LpgbtPairHeader(const RamWord &w) {
  setWordsTo(w);
}

uint32_t LpgbtPairHeader::word(unsigned i) const {
  backtraceAssert(i<2);
  return word_[i];
}

void LpgbtPairHeader::setWordTo(unsigned i, uint32_t w) {
  backtraceAssert(i<2);
  word_[i]=w;
}

void LpgbtPairHeader::setWordsTo(const RamWord &w) {
  word_[0]=w.word(0);
  word_[1]=w.word(1);
}

uint8_t LpgbtPairHeader::headerPattern() const {
  return word_[0]>>25;
}

unsigned LpgbtPairHeader::numberOfEconds() const {
  return 12;
}

unsigned LpgbtPairHeader::numberOfConnectedEconds() const {
  unsigned n(0);

  for(unsigned i(0);i<12;i++) {
    if(econdStatus(i)!=Unconnected) n++;
  }
  
  return n;  
}

unsigned LpgbtPairHeader::numberOfPresentEcondPackets() const {
  unsigned n(0);

  for(unsigned i(0);i<12;i++) {
    if(packetPresent(i)) n++;
  }
  
  return n;  
}

unsigned LpgbtPairHeader::numberOfAbsentEcondPackets() const {
  unsigned n(0);

  for(unsigned i(0);i<12;i++) {
    if(econdStatus(i)!=Unconnected && !packetPresent(i)) n++;
  }
  
  return n;  
}

LpgbtPairHeader::EcondStatus LpgbtPairHeader::econdStatus(unsigned e) const {
  backtraceAssert(e<12);
  if(e<10) return (EcondStatus)((word_[1]>>(3*e))&0x7);
  else if(e>10) return (EcondStatus)((word_[0]>>1)&0x7);
  else return (EcondStatus)(((word_[1]>>30)&0x3)|((word_[0]<<2)&0x4));
}

void LpgbtPairHeader::setEcondStatusTo(unsigned e, LpgbtPairHeader::EcondStatus s) {
  backtraceAssert(e<12);
  backtraceAssert(s<8);
  if(e<10) word_[1]=(word_[1]&(~(7<<(3*e))))|(s<<(3*e));
  else if(e>10) word_[0]=(word_[0]&(~(7<<1)))|(s<<1);
  else {
    word_[1]=(word_[1]&(~(3<<30)))|((s&0x3)<<30);
    word_[0]=(word_[0]&(~(1)))|((s&0x4)>>2);
  }
}

bool LpgbtPairHeader::packetPresent(unsigned e) const {
  backtraceAssert(e<12);
  return econdStatus(e)<=Present;
}

FastControlCounters LpgbtPairHeader::counters() const {
  FastControlCounters fcc;
  fcc.setOcTo((word_[0]>> 4)&0x7);
  fcc.setEcTo((word_[0]>> 7)&0x3f);
  fcc.setBcTo((word_[0]>>13)&0xfff);
  return fcc;
}

void LpgbtPairHeader::setCountersTo(const FastControlCounters &c) {
  word_[0]=(word_[0]&0xfe00000f)|(c.oc()&0x7)<<4|(c.ec()&0x3f)<<7|(c.bc()&0xfff)<<13;
}

bool LpgbtPairHeader::operator==(const LpgbtPairHeader &h) const {
  return word_[0]==h.word_[0] && word_[1]==h.word_[1];
}

bool LpgbtPairHeader::operator==(const FastControlCounters &c) const {
  return (word_[0]&0x01fffff0)==((c.oc()&0x7)<<4|(c.ec()&0x3f)<<7|(c.bc()&0xfff)<<13);
}

void LpgbtPairHeader::print(const std::string &s) const {
  std::cout << s << "LpgbtPairHeader::print()"
	    << "  Words =" << std::hex << std::setfill('0');
  for(unsigned i(0);i<2;i++) {
    std::cout << " 0x" << std::setw(8) << word_[i];
  }
  std::cout << std::dec << std::setfill(' ') << std::endl;

  std::cout << s << " Header pattern = 0x" << std::hex << std::setfill('0');
  std::cout << std::setw(2) << (unsigned)headerPattern();
  std::cout << std::dec << std::setfill(' ') << std::endl;

  counters().print(s+" ");
  
  std::cout << s << " Number of ECON-Ds = " << numberOfEconds()
	    << ", number connected = " << numberOfConnectedEconds()
	    << ", number with/without packets = "
	    << numberOfPresentEcondPackets() << "/"
	    << numberOfAbsentEcondPackets()
	    << std::endl;
  
  for(unsigned e(0);e<12;e++) {
    EcondStatus x(econdStatus(e));
    std::cout << s << "  ECON-D " << std::setw(2) << e << ", packet "
	      << (packetPresent(e)?"present,":"absent, ")
	      << " status = " << x << " = ";
    
    if(x==Normal         ) std::cout << "Normal";
    if(x==PayloadCrcError) std::cout << "PayloadCrcError";
    if(x==Unclear        ) std::cout << "Unclear";
    if(x==TimeoutError   ) std::cout << "TimeoutError";
    if(x==TimestampError ) std::cout << "TimestampError";
    if(x==BufferOverflow ) std::cout << "BufferOverflow";
    if(x==Missing        ) std::cout << "Missing";
    if(x==Unconnected    ) std::cout << "Unconnected";
    std::cout << std::endl;
  }
}
