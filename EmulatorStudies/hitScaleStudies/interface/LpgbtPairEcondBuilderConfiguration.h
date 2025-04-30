#ifndef Emulation_HGCalBufferModel_LpgbtPairEcondBuilderConfiguration_h
#define Emulation_HGCalBufferModel_LpgbtPairEcondBuilderConfiguration_h

#include <iostream>
#include <string>

class LpgbtPairEcondBuilderConfiguration {
 public:
  LpgbtPairEcondBuilderConfiguration();

  unsigned phaseOf40Mhz() const;
  void setPhaseOf40MHzTo(unsigned p);

  unsigned l1aTimeout() const;
  void setL1aTimeoutTo(unsigned t);

  unsigned mediumBufferFactor() const;
  void setMediumBufferFactorTo(unsigned l);

  unsigned highBufferFactor() const;
  void setHighBufferFactorTo(unsigned l);

  //unsigned fullBufferFactor() const;
  //void setFullBufferFactorTo(unsigned l);

  unsigned almostFullBufferSpace() const;
  void setAlmostFullBufferSpaceTo(unsigned l);

  unsigned ebrWait() const;
  void setEbrWaitTo(unsigned l);

  void print(const std::string &s="") const;

  const static unsigned totalBufferLevel_;
  const static unsigned fullBufferFactor_;

  static unsigned defaultPhaseOf40Mhz_;
  static unsigned defaultL1aTimeout_;
  static unsigned defaultMediumBufferFactor_;
  static unsigned defaultHighBufferFactor_;
  static unsigned defaultAlmostFullBufferSpace_;
  static unsigned defaultEbrWait_;
  
 protected:
  unsigned phaseOf40Mhz_;
  unsigned l1aTimeout_;
  unsigned mediumBufferFactor_;
  unsigned highBufferFactor_;
  unsigned almostFullBufferSpace_;
  unsigned ebrWait_;
};

#endif
