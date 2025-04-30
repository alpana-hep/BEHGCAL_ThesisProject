#include <iostream>
#include <iomanip>
#include <cassert>

#include "Backtrace.h"
#include "WordFlags.h"


WordFlags::WordFlags() : fBits(0) {
}

WordFlags::WordFlags(uint8_t b) : fBits(b) {
}

////////////////////////////////////////////////////////
  
void WordFlags::clearBits() {
  fBits=0;
}

void WordFlags::setBitsTo(uint8_t b) {
  fBits=b;
}

void WordFlags::setStartBit() {
  fBits|=1;
}

void WordFlags::setEndBit() {
  fBits|=2;
}

void WordFlags::setPaddingBit() {
  fBits|=4;
}

void WordFlags::setTruncationBit() {
  fBits|=16;
}

void WordFlags::setErrorBit() {
  fBits|=32;
}

void WordFlags::setInvalidBit() {
  fBits|=128;
}

void WordFlags::setStartBitTo(bool b) {
  if(b) fBits|=0x01;
  else  fBits&=0xfe;
}

void WordFlags::setEndBitTo(bool b) {
  if(b) fBits|=0x02;
  else  fBits&=0xfd;
}

void WordFlags::setPaddingBitTo(bool b) {
  if(b) fBits|=0x04;
  else  fBits&=0xfb;
}

void WordFlags::setPaddingCountTo(unsigned c) {
  backtraceAssert(c<4);
  fBits&=0xf3;
  fBits|=(c<<2);
}

void WordFlags::setTruncationBitTo(bool b) {
  if(b) fBits|=0x10;
  else  fBits&=0xe0;
}

void WordFlags::setErrorBitTo(bool b) {
  if(b) fBits|=0x20;
  else  fBits&=0xdf;
}

void WordFlags::setInvalidBitTo(bool b) {
  if(b) fBits|=0x80;
  else  fBits&=0x7f;
}

////////////////////////////////////////////////////////

uint8_t WordFlags::bits() const {
  return fBits;
}
  
bool WordFlags::noBits() const {
  return fBits==0;
}
  
bool WordFlags::startBit() const {
  return (fBits&1)!=0;
}
  
bool WordFlags::endBit() const {
  return (fBits&2)!=0;
}
  
bool WordFlags::paddingBit() const {
  return (fBits&4)!=0;
}
  
unsigned WordFlags::paddingCount() const {
  return (fBits>>2)&0x3;
}
  
bool WordFlags::truncationBit() const {
  return (fBits&16)!=0;
}
  
bool WordFlags::errorBit() const {
  return (fBits&32)!=0;
}
  
bool WordFlags::invalidBit() const {
  return (fBits&128)!=0;
}

void WordFlags::print() const {
  std::cout << "WordFlags::print()  Bits = 0x"
	    << std::hex << std::setfill('0')
	    << std::setw(2) << (unsigned)fBits
	    << std::dec << std::setfill(' ');
}
