#ifndef Emulation_HGCalBufferModel_RamWord_h
#define Emulation_HGCalBufferModel_RamWord_h

#include <cstdint>

#include "WordFlags.h"


class RamWord : public WordFlags {
public:
  RamWord();

  uint32_t word(unsigned i) const;

  // BAD!
  //void setWordTo(uint64_t w);
  //void setWordTo(uint32_t w0, uint32_t w1);
  void setWordTo(unsigned i, uint32_t w);

  void increment();
  
  void print(const std::string &s="") const;
  
private:
  uint32_t word_[2];
};

#endif
