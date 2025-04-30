#ifndef Emulation_HGCalBufferModel_D2sWord_h
#define Emulation_HGCalBufferModel_D2sWord_h

#include <cstdint>

#include "WordFlags.h"
#include "SlinkWord.h"


class D2sWord : public WordFlags {
public:
  D2sWord();

  uint32_t word(unsigned i) const;
  void setWordTo(unsigned i, uint32_t w);

  const SlinkWord& slinkWord(unsigned i) const;
  void setSlinkWordTo(unsigned i, const SlinkWord &w);

  const std::pair<bool,SlinkWord>& boolSlinkWord(unsigned i) const;
  void setBoolSlinkWordTo(unsigned i, const std::pair<bool,SlinkWord> &w);

  void clear();
  
  void print(const std::string &s="") const;
  
private:
  std::pair<bool,SlinkWord> slinkWord_[4];
};

#endif
