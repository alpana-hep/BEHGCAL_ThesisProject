#ifndef Emulation_HGCalBufferModel_EcondAsicConfiguration_h
#define Emulation_HGCalBufferModel_EcondAsicConfiguration_h

#include <string>

#include "EcondAsicIdle.h"

class EcondAsicConfiguration {
 public:

  EcondAsicConfiguration();

  unsigned mediumBufferLevel() const;
  void setMediumBufferLevelTo(unsigned l);

  unsigned highBufferLevel() const;
  void setHighBufferLevelTo(unsigned l);

  unsigned truncationBufferLevel() const;
  void setTruncationBufferLevelTo(unsigned l);

  static unsigned fullBufferLevel();
  //void setFullBufferLevelTo(unsigned l);

  EcondAsicIdle::BufferStatus bufferStatus(unsigned n) const;
void setHitScalingArgTo(bool scale);  
  unsigned lpgbtDelayLength() const;
  void setLpgbtDelayLengthTo(unsigned l);

  double lpgbtBer() const;
  void setLpgbtBerTo(double r);
  unsigned readMode() const;
  bool hitScaling() const;
  double hitScaleFactor() const;
  void setHitScalingTo(unsigned s);
  double hitAdditiveArg() const; //added by Alpana                                                                                                                         
  double hitlayerArg() const;
  double ScintiHitScaling() const;
  void setHitAdditiveArgTo(unsigned add);
  void setHitlayerArgTo(unsigned Layer);
  void setScintHitScalingArgTo(unsigned scale);
  void setfileReadModeTo(unsigned f);
  double l1aFakeProbability() const;
  void setL1aFakeProbabilityTo(double p);
  
  bool updateIdleStatus() const;
  void setUpdateIdleStatusTo(bool s);
  
  void print(const std::string &s="") const;

  static const unsigned fullBufferLevel_;
  static unsigned defaultMediumBufferLevel_;
  static unsigned defaultHighBufferLevel_;
  static unsigned defaultTruncationBufferLevel_;
  static unsigned defaultLpgbtDelayLength_;
  static double   defaultLpgbtBer_;
  static unsigned defaultHitScaling_;
  static unsigned defaultfileReadMode_;
  static double   defaultL1aFakeProbability_;
  static bool     defaultUpdateIdleStatus_;
  static unsigned defaultHitAdditiveArg_; //added by Alpana     
  static unsigned defaultHitlayerArg_;
  static bool defaultHitScalingArg_;
  static unsigned  defaultScintHitScalingArg_;

 protected:
  unsigned mediumBufferLevel_;
  unsigned highBufferLevel_;
  unsigned truncationBufferLevel_;
  unsigned lpgbtDelayLength_;
  double   lpgbtBer_;
  unsigned hitScaling_;
  unsigned hitAdditiveArg_;
  unsigned hitLayerArg_;
  bool hitScalingArg_;
  unsigned ScintHitScalingArg_;
  unsigned fileReadMode_;
  double   l1aFakeProbability_;
  bool     updateIdleStatus_;
};

#endif
