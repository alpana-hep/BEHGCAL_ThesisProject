#ifndef Emulation_HGCalBufferModel_FastControlStreamConfiguration_h
#define Emulation_HGCalBufferModel_FastControlStreamConfiguration_h

#include <string>

class FastControlStreamConfiguration {
 public:

  FastControlStreamConfiguration();

  unsigned noZeroSuppressionPeriod() const;
  void setNoZeroSuppressionPeriodTo(unsigned p);

  double nzsProbability() const;
  void setNzsProbabilityTo(double p);

  void print(const std::string &s="") const;

 protected:
  unsigned noZeroSuppressionPeriod_;
  double nzsProbability_;
};

#endif
