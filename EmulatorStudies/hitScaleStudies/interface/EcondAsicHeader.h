#ifndef Emulation_HGCalBufferModel_EcondAsicHeader_h
#define Emulation_HGCalBufferModel_EcondAsicHeader_h

#include <string>
#include <cstdint>

#include "RamWord.h"
#include "FastControlCounters.h"


class EcondAsicHeader {
 public:
  enum {
    HeaderPattern=0x154
  };

  enum DataType {
    Normal,
    Truncated
  };
  
  // Also used in idle
  enum ResetRequest {
    None,
    RequestA, //???
    RequestB, //???
    RequestC  //???
  };

  EcondAsicHeader();
  EcondAsicHeader(const RamWord &w);

  uint32_t word(unsigned i) const;
  void setWordTo(unsigned i, uint32_t w);
  void setWordsTo(const RamWord &w);
  
  uint16_t headerPattern() const;
  
  // Historic: length in 16-bit words
  //uint16_t packetLength() const;

  // Length in 32-bit words
  uint16_t payloadLength() const;
  void setPayloadLengthTo(uint16_t l);
  uint16_t totalPacketLength() const;
  
  uint16_t packetWords() const;
  void setPacketWordsTo(uint16_t w);

  // P,E,H/T,E/B/O,M,S
  uint8_t qualityFlags() const;
  void setQualityFlagsTo(uint8_t f);
  
  // Truncation
  bool truncated() const;
  
  // Hamming
  uint8_t hamming() const;
  void setHamming();
  
  // Set normal or truncated
  DataType dataType() const;
  void setDataTypeTo(DataType t);

  // Counters
  FastControlCounters counters() const;
  void setCountersTo(const FastControlCounters &c);
  uint32_t timeDelayFrom(uint64_t bx) const;

  // Reset request
  ResetRequest resetRequest() const;
  void setResetRequestTo(ResetRequest r);
  
  // CRC
  uint8_t crc() const;
  uint8_t calculateCrc() const;
  bool validCrc() const;
  void setCrc();
  
  bool operator==(const EcondAsicHeader &h) const;
  bool operator==(const FastControlCounters &c) const;
  bool consistentTimeWith(const FastControlCounters &c) const;
  
  bool valid() const;
  
  void print(const std::string &s="") const;
  
 protected:
  uint32_t word_[2];
};

#endif
