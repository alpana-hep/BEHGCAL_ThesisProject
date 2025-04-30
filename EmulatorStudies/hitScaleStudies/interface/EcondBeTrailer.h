#ifndef Emulation_HGCalBufferModel_EcondBeTrailer_h
#define Emulation_HGCalBufferModel_EcondBeTrailer_h

#include <cstdint>

class EcondBeTrailer {
 public:
  enum {
    Pattern=0xae
  };

  EcondBeTrailer();

  bool valid() const;
  
  uint32_t word() const;
  void setWordTo(uint32_t w);

  uint8_t errorBits() const;
  void setErrorBitsTo(uint8_t e);

  uint16_t packetLength() const;
  void setPacketLengthTo(uint16_t l);
  
  void print() const;

 protected:
  uint32_t word_;
};

#endif
