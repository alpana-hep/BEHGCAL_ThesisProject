#ifndef Emulation_HGCalBufferModel_FastControlDriverConfiguration_h
#define Emulation_HGCalBufferModel_FastControlDriverConfiguration_h

#include <string>

class FastControlDriverConfiguration {
 public:

  FastControlDriverConfiguration();

  unsigned requestsQueueSize() const;
  void setRequestsQueueSizeTo(unsigned s);

  unsigned commandsQueueSize() const;
  void setCommandsQueueSizeTo(unsigned s);

  unsigned triggerMethod() const;
  void setTriggerMethodTo(unsigned m);

  unsigned l1aRateKhz() const;
  void setL1aRateKhzTo(unsigned r);
  
  unsigned calOrbitPeriod() const;
  void setCalOrbitPeriodTo(unsigned p);
  
  unsigned calRateHz() const;
  
  unsigned fatEventRateHz() const;
  void setFatEventRateHzTo(unsigned f);

  double fatEventProbability() const;

  unsigned hgcrocBufferLimit() const;
  void setHgcrocBufferLimitTo(unsigned l);

  unsigned triggerRulePeriod() const;
  void setTriggerRulePeriodTo(unsigned p);

  unsigned triggerRuleLimit() const;
  void setTriggerRuleLimitTo(unsigned l);

  const std::string& hllhcFilename() const;
  void setHllhcFilenameTo(const std::string &f);

  void print(const std::string &s="") const;

 protected:
  unsigned requestsQueueSize_;
  unsigned commandsQueueSize_;
  unsigned triggerMethod_;
  unsigned l1aRateKhz_;
  unsigned calOrbitPeriod_;
  unsigned fatEventRateHz_;
  unsigned hgcrocBufferLimit_;
  unsigned triggerRulePeriod_;
  unsigned triggerRuleLimit_;
  std::string hllhcFilename_;
};

#endif
