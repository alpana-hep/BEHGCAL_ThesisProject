#ifndef Emulation_HGCalBufferModel_FastControlCounters_h
#define Emulation_HGCalBufferModel_FastControlCounters_h

#include <cstdint>

#include "FastControlCommands.h"

class FastControlCounters {
 public:
  enum {
    MaximumBc=3564 // Number of BX per orbit
  };

  FastControlCounters();

  // Call every 40 MHz tick to update counters
  void update(const FastControlCommands &c);

  // Return counters
  uint16_t bc() const;
  uint32_t oc() const;
  uint32_t ec() const;
  uint64_t totalBx() const;

  // Set counters
  void setBcTo(uint16_t b);
  void setOcTo(uint32_t o);
  void setEcTo(uint32_t e);

  // Set whole word
  uint32_t word(unsigned i) const;
  void setWordTo(unsigned i, uint32_t w);

  // Check counters are in valid ranges
  bool valid() const;

  // Comparison
  bool operator==(const FastControlCounters &c) const;
  
  void print(const std::string &s="") const;
    
 private:
  uint32_t word_[2];
};

#endif
