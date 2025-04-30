#ifndef Emulation_HGCalBufferModel_SlinkBuilderConfiguration_h
#define Emulation_HGCalBufferModel_SlinkBuilderConfiguration_h

#include <string>

class SlinkBuilderConfiguration {
 public:

  SlinkBuilderConfiguration();

  unsigned bufferSize() const;
  void setBufferSizeTo(unsigned s);

  void print(const std::string &s="") const;

  static unsigned defaultBufferSize_;
  
 protected:
  unsigned bufferSize_;
};

#endif
