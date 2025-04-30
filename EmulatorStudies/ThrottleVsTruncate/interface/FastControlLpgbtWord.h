#ifndef Emulation_HGCalBufferModel_FastControlLpgbtWord_h
#define Emulation_HGCalBufferModel_FastControlLpgbtWord_h

#include "FastControlCommands.h"

class FastControlLpgbtWord {
 public:
  FastControlLpgbtWord();

  FastControlCommands commands();
  FastControlCommands setTo(const FastControlCommands &c);

  void print() const;
  
 private:
  uint16_t word_[2];
};

#endif
