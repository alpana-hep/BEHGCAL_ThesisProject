#ifndef Emulation_HGCalBufferModel_EcondHeader_h
#define Emulation_HGCalBufferModel_EcondHeader_h

#include <string>
#include <cstdint>

#include "FastControlCounters.h"


class EcondHeader {
 public:
  enum BufferStatus {
    Low,
    Medium,
    High,
    Full
  };
  
  enum DataType {
    Normal,
    Truncated
  };
  
  EcondHeader();

  uint32_t word(unsigned i) const;
  void setWordTo(unsigned i, uint32_t w);
  
  uint16_t pattern() const;

  // Native length in 16-bit words
  uint16_t packetLength() const;
  void setPacketLengthTo(uint16_t l);

  // Length in 32-bit words
  uint16_t packetWords() const;
  bool padding() const;

  // Set normal or truncated
  DataType dataType() const;
  void setDataTypeTo(DataType t);

  // High or low density
  bool hdm() const;
  void setHdmTo(bool b);

  // Buffer occupancy flags
  BufferStatus bufferStatus() const;
  void setBufferStatusTo(BufferStatus s);

  // Counters
  FastControlCounters counters() const;
  void setCountersTo(const FastControlCounters &c);

  bool operator==(const EcondHeader &h) const;
  bool operator==(const FastControlCounters &c) const;
  
  bool valid() const;
  
  void print(const std::string &s="") const;
  
 protected:
  uint32_t word_[2];
};

#endif
