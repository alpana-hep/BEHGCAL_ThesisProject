#ifndef Emulation_HGCalBufferModel_EcondAsicIdle_h
#define Emulation_HGCalBufferModel_EcondAsicIdle_h

#include <string>
#include <cstdint>

#include "EcondAsicHeader.h"


class EcondAsicIdle {
 public:
  enum {
    //IdlePattern=0xaaaaaa00
    // Avoid similarity to header pattern
    IdlePattern=0x55555500
  };

  enum BufferStatus {
    Empty,
    Low,
    Medium,
    High
    //Full
  };
  
  EcondAsicIdle();
  EcondAsicIdle(uint32_t w);

  uint32_t word() const;
  void setWordTo(uint32_t w);
  
  uint32_t idlePattern() const;

  // Buffer occupancy flags
  BufferStatus bufferStatus() const;
  void setBufferStatusTo(BufferStatus s);

  // Error status
  uint8_t errorStatus() const;
  void setErrorStatusTo(uint8_t e);
  
  // Reset request
  EcondAsicHeader::ResetRequest resetRequest() const;
  void setResetRequestTo(EcondAsicHeader::ResetRequest r);

  bool operator==(const EcondAsicIdle &h) const;
  
  bool valid() const;
  
  void print(const std::string &s="") const;
  
 protected:
  uint32_t word_;
};

#endif
