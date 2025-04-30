#ifndef Emulation_HGCalBufferModel_GlobalConfiguration_h
#define Emulation_HGCalBufferModel_GlobalConfiguration_h

#include <string>

#include "Clocks.h"

class GlobalConfiguration {
 public:
  static GlobalConfiguration& configuration();

  unsigned calibrationQueueSize() const;
  void setCalibrationQueueSizeTo(unsigned s);

  unsigned preL1aQueueSize() const;
  void setPreL1aQueueSizeTo(unsigned s);

  unsigned beQueueSize() const;
  void setBeQueueSizeTo(unsigned s);

  unsigned daqPacketFormat() const;
  void setDaqPacketFormatTo(unsigned f);
  
  unsigned tpgTimeMplex() const;
  void setTpgTimeMplexTo(unsigned t);

  bool slink25GbitPerSec() const;
  void setSlink25GbitPerSecTo(bool b);
  Clocks::Frequency slinkClockFrequency() const;
  Clocks::Frequency slinkBuilderClockFrequency() const;
  
  void print(const std::string &s="") const;

 protected:
  GlobalConfiguration();
  
  unsigned calibrationQueueSize_;
  unsigned preL1aQueueSize_;
  unsigned beQueueSize_;
  unsigned daqPacketFormat_;
  unsigned tpgTimeMplex_;
  bool slink25GbitPerSec_;
  
  static GlobalConfiguration configuration_;
};

#endif
