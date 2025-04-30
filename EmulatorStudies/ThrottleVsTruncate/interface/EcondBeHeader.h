#ifndef Emulation_HGCalBufferModel_EcondBeHeader_h
#define Emulation_HGCalBufferModel_EcondBeHeader_h

#include <cstdint>

#include "EcondHeader.h"
#include "FastControlCounters.h"


class EcondBeHeader : public EcondHeader {
 public:
  enum {
    HeaderPattern=0xefac
  };
  
  EcondBeHeader();

  uint8_t errorBits() const;
  void setErrorBitsTo(uint8_t c);

  bool operator==(const EcondBeHeader &h) const;
  bool operator==(const FastControlCounters &c) const;

  bool valid() const;
  
  void print() const;
};

#endif
