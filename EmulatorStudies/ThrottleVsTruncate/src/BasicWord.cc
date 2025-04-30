#include <iostream>
#include <iomanip>

#include "BasicWord.h"


BasicWord::BasicWord() {
}

BasicWord::BasicWord(uint8_t b, uint32_t w) : WordFlags(b), word_(w) {
}

void BasicWord::setWordTo(uint32_t w) {
  word_=w;
}
  
uint32_t BasicWord::word() const {
  return word_;
}

void BasicWord::print() const {
  std::cout << "BasicWord::print()  ";
  WordFlags::print();
  std::cout << " Word = 0x" << std::hex << std::setfill('0')
	    << std::setw(8) << word_
	    << std::dec << std::setfill(' ') << std::endl;
}
