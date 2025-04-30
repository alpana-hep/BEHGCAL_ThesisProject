#include <iostream>
#include <iomanip>
#include <cassert>

#include "Backtrace.h"
#include "D2sWord.h"


D2sWord::D2sWord() {
  for(unsigned i(0);i<4;i++) slinkWord_[i].second.clear();  
}

void D2sWord::setWordTo(unsigned i, uint32_t w) {
  backtraceAssert(i<16);
  slinkWord_[i/4].second.setWordTo(i%4,w);
}

uint32_t D2sWord::word(unsigned i) const {
  backtraceAssert(i<16);
  return slinkWord_[i/4].second.word(i%4);
}

void D2sWord::setSlinkWordTo(unsigned i, const SlinkWord &w) {
  backtraceAssert(i<4);
  slinkWord_[i].second=w;
}

const SlinkWord& D2sWord::slinkWord(unsigned i) const {
  backtraceAssert(i<4);
  return slinkWord_[i].second;
}

void D2sWord::setBoolSlinkWordTo(unsigned i, const std::pair<bool,SlinkWord> &w) {
  backtraceAssert(i<4);
  slinkWord_[i]=w;
}

const std::pair<bool,SlinkWord>& D2sWord::boolSlinkWord(unsigned i) const {
  backtraceAssert(i<4);
  return slinkWord_[i];
}

void D2sWord::clear() {
  WordFlags::clearBits();
  for(unsigned i(0);i<4;i++) slinkWord_[i].second.clear();  
}

void D2sWord::print(const std::string &s) const {
  std::cout << s << "D2sWord::print()  ";
  WordFlags::print();

  std::cout << std::endl;// << std::hex << std::setfill('0');

  for(unsigned i(0);i<4;i++) {
    /*
    if(i==0) std::cout << s << " Words =";
    else     std::cout << s << "        ";
    for(unsigned j(4*i);j<4*(i+1);j++) {
      std::cout << " 0x" << std::setw(8) << word_[j];
    }
    std::cout << std::endl;
    */
    std::cout << s << " Flag = "
	      << (slinkWord_[i].first?"true  ":"false ");
    slinkWord_[i].second.print();
  }

  //std::cout << std::dec << std::setfill(' ');
}
