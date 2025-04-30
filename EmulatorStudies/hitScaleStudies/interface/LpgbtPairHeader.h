#ifndef Emulation_HGCalBufferModel_LpgbtPairHeader_h
#define Emulation_HGCalBufferModel_LpgbtPairHeader_h

#include <string>
#include <cstdint>

#include "RamWord.h"
#include "FastControlCounters.h"


class LpgbtPairHeader {
 public:
  enum {
    HeaderPattern=0x66
  };

  enum EcondStatus {
    Normal,
    PayloadCrcError,
    Unclear,
    TimestampError,
    Present=TimestampError,
    TimeoutError,
    BufferOverflow,
    Missing,
    Unconnected
  };
  
  LpgbtPairHeader();
  LpgbtPairHeader(const RamWord &w);

  uint32_t word(unsigned i) const;
  void setWordTo(unsigned i, uint32_t w);
  void setWordsTo(const RamWord &w);
  
  uint8_t headerPattern() const;
  
  unsigned numberOfEconds() const;
  unsigned numberOfConnectedEconds() const;
  unsigned numberOfPresentEcondPackets() const;
  unsigned numberOfAbsentEcondPackets() const;
  
  EcondStatus econdStatus(unsigned e) const;
  void setEcondStatusTo(unsigned e, EcondStatus s);
  bool packetPresent(unsigned e) const;
  
  // Counters
  FastControlCounters counters() const;
  void setCountersTo(const FastControlCounters &c);
  
  bool operator==(const LpgbtPairHeader &h) const;
  bool operator==(const FastControlCounters &c) const;
  
  void print(const std::string &s="") const;
  
 protected:
  uint32_t word_[2];
};

#endif
