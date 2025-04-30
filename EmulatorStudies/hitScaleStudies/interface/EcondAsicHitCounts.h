#ifndef Emulation_HGCalBufferModel_EcondAsicHitCounts_h
#define Emulation_HGCalBufferModel_EcondAsicHitCounts_h

#include <cstdint>
#include <string>

#include "FastControlCounters.h"
#include "EcondAsicDefinition.h"

class EcondAsicHitCounts {
 public:
  EcondAsicHitCounts();

  uint32_t word() const;
  void setWordTo(uint32_t w);

  uint16_t n10() const;
  void setN10To(uint16_t n);

  uint16_t n20() const;
  void setN20To(uint16_t n);

  uint16_t n30() const;
  void setN30To(uint16_t n);
  
  uint16_t packetWords(const EcondAsicDefinition &e) const;

  bool operator==(const EcondAsicHitCounts &h) const;
  bool operator==(const FastControlCounters &c) const;

  bool valid() const;

  // Conversion froms number of hits to number of 16-bit words
  static unsigned daqRawDataPacketWords(const EcondAsicDefinition &e, unsigned n10, unsigned n20, unsigned n30);
      
  void print(const std::string &s="") const;

  static unsigned daqRawDataPacketWordsSep19(const EcondAsicDefinition &e, unsigned n20, unsigned n30);  
  static unsigned daqRawDataPacketWordsNov19(const EcondAsicDefinition &e, unsigned n20, unsigned n30);
  static unsigned daqRawDataPacketWordsJun20(const EcondAsicDefinition &e, unsigned n20, unsigned n30);
  static unsigned daqRawDataPacketWordsDec20(const EcondAsicDefinition &e, unsigned n10, unsigned n20, unsigned n30);

 private:
  uint32_t word_;

};

#endif
