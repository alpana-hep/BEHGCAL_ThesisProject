#ifndef Emulation_HGCalBufferModel_BasicWord_h
#define Emulation_HGCalBufferModel_BasicWord_h

#include <cstdint>

#include "WordFlags.h"


class BasicWord : public WordFlags {
public:
  BasicWord();
  BasicWord(uint8_t b, uint32_t w);

  void setWordTo(uint32_t w);

  uint32_t word() const;

  void print() const;
  
private:
  uint32_t word_;
};

#endif
