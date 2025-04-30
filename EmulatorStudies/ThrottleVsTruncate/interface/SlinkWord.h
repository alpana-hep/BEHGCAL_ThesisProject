#ifndef Emulation_HGCalBufferModel_SlinkWord_h
#define Emulation_HGCalBufferModel_SlinkWord_h

#include <cstdint>

#include "EcondAsicHeader.h"
#include "RamWord.h"
#include "FastControlCounters.h"

class SlinkWord {
 public:
  SlinkWord();

  void clear();

  const uint32_t& word(unsigned i) const;
  void setWordTo(unsigned i, uint32_t w);

  void setTo(const FastControlCounters &c, const EcondAsicHeader &h);
  FastControlCounters fcCounters() const;
  const EcondAsicHeader& econdHeader() const;

  void setTo(unsigned i, uint16_t w);
  void setTo(unsigned i, const RamWord &w);

  uint16_t crcIncrement() const;
  
  void print(const std::string &s="") const;

 protected:
  uint32_t word_[4];
};

#endif
