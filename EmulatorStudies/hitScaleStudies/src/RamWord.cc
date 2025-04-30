#include <iostream>
#include <iomanip>
#include <cassert>

#include "RamWord.h"


RamWord::RamWord() {
  word_[0]=0;
  word_[1]=0;
}
/*
void RamWord::setWordTo(uint64_t w) {
  word_=w;
}

void RamWord::setWordTo(uint32_t w0, uint32_t w1) {
  word_=(uint64_t(w0)<<32)|w1;
}
*/
void RamWord::setWordTo(unsigned i, uint32_t w) {
  assert(i<2);
  word_[i]=w;
  //if(i==0) word_=(word_&0xffffffff00000000)|uint64_t(w);
  //else     word_=(word_&0x00000000ffffffff)|(uint64_t(w)<<32);
}

void RamWord::increment() {
  word_[0]++; // Only need first word for counter
}

uint32_t RamWord::word(unsigned i) const {
  assert(i<2);
  return word_[i];
}

void RamWord::print(const std::string &s) const {
  std::cout << s << "RamWord::print()  ";
  WordFlags::print();
  std::cout << " Words =" << std::hex << std::setfill('0')
	    << " 0x" << std::setw(8) << word_[0]
	    << ", 0x" << std::setw(8) << word_[1]
	    << std::dec << std::setfill(' ') << std::endl;
}
