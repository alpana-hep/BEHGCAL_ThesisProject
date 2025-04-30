#include <iostream>
#include <iomanip>

#include "SlinkEoe.h"

SlinkEoe::SlinkEoe() {
  initialise();
}

SlinkEoe::SlinkEoe(const SlinkWord &w) {
  *((SlinkWord*)this)=w;
}

SlinkEoe::SlinkEoe(uint32_t f, uint16_t s, uint16_t c, uint32_t l) {
  initialise();

  setFedUserIdTo(f);
  setStatusTo(s);
  setCrcTo(c);
  setEventLengthTo(l);
}

void SlinkEoe::initialise() {
  uint64_t eoe64(EoePattern);
  word_[0]=eoe64<<24;  
  word_[1]=0;
  word_[2]=0;
  word_[3]=0;
}

void SlinkEoe::setFedUserIdTo(uint32_t f) {
  word_[3]=f;
}
  
void SlinkEoe::setSubSystemUserTo(uint32_t f) {
  setFedUserIdTo(f);
}
  
void SlinkEoe::setStatusTo(uint16_t s) {
  word_[2]=(word_[2]&0xffff0000)|s;
}
  
void SlinkEoe::setCrcTo(uint16_t c) {
  uint32_t c32(c);
  word_[2]=(word_[2]&0x0000ffff)|(c32<<16);
}
  
void SlinkEoe::setEventLengthTo(uint32_t l) {
  word_[1]=l;
}
  
void SlinkEoe::incrementEventLength() {
  setEventLengthTo(eventLength()+1);
}
  
uint32_t SlinkEoe::fedUserId() const {
  return word_[3];
}

uint32_t SlinkEoe::subSystemUser() const {
  return fedUserId();
}

uint16_t SlinkEoe::status() const {
  return word_[2]&0xffff;
}

uint16_t SlinkEoe::crc() const {
  return word_[2]>>16;
}

uint32_t SlinkEoe::eventLength() const {
  return word_[1];
}

uint8_t SlinkEoe::eoe() const {
  return (word_[0]>>24);
}


bool SlinkEoe::valid() const {
  return eoe()==EoePattern;
}

void SlinkEoe::print(const std::string &s) const {
  std::cout << s << "SlinkEoe::print() ";
  SlinkWord::print();
  std::cout << s << " FED user ID = " << fedUserId() << " = 0x" << std::hex << fedUserId() << std::dec << std::endl;
  std::cout << s << " Status = " << status() << " = 0x" << std::hex << status() << std::dec << std::endl;
  std::cout << s << " CRC = " << crc() << " = 0x" << std::hex << crc() << std::dec << std::endl;
  std::cout << s << " Event length = " << eventLength() << " = 0x" << std::hex << eventLength() << std::dec << std::endl;
  std::cout << s << " EOE = " << (unsigned)eoe() << " = 0x" << std::hex << (unsigned)eoe() << std::dec
	    << (valid()?", valid":", not valid") << std::endl;
}
