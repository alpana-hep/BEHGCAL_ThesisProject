#ifndef Emulation_HGCalBufferModel_FastControlCommands_h
#define Emulation_HGCalBufferModel_FastControlCommands_h

#include <cstdint>

#include <string>


class FastControlCommands {
   private:
 public:
  enum Command {
    // TCDS commands
    OrbitCounterReset,
    BunchCounterReset,
    EventCounterReset,
    EventBufferReset,
    L1a,
    CalPrepare,

    // HGCAL commands
    PreL1a,
    NoZeroSuppression,
    CalPulseInt,
    CalPulseExt,
    LinkResetEcond,
    LinkResetEcont,
    ChipSync,
    FatEvent,
    Range,

    // Truth = cheats
    CollisionBc=16,
    CalL1aInt,
    CalL1aExt,
    HgcrocTcdsThrottle,
    HgcrocBeThrottle,
    EcondAsicThrottle,
    EcondBeThrottle,
    TriggerRuleThrottle,
    ExtendedRange
  };

 public:
  FastControlCommands();

  // Directly handle all commands
  uint32_t word() const;
  void setWordTo(uint32_t c);

  // Get individual commands
  bool command(Command c) const;
  
  bool orbitCounterReset() const;
  bool bunchCounterReset() const;
  bool eventCounterReset() const;
  bool eventBufferReset() const;
  bool l1a() const;
  bool calPrepare() const;

  bool preL1a() const;
  bool noZeroSuppression() const;
  bool calPulseInt() const;
  bool calPulseExt() const;
  bool linkResetEcond() const;
  bool linkResetEcont() const;
  bool chipSync() const;
  bool fatEvent() const;

  bool idle() const;

  // Emulation-only truth/cheat flags for checking/statistics
  bool collisionBc() const;
  bool calL1aInt() const;
  bool calL1aExt() const;
  bool hgcrocTcdsThrottle() const;
  bool hgcrocBeThrottle() const;
  bool econdAsicThrottle() const;
  bool econdBeThrottle() const;
  bool triggerRuleThrottle() const;
  
  bool throttle() const;
  
  bool operator==(const FastControlCommands &c) const;

  // Set individual commands
  void setCommandTo(Command c, bool b);

  void setOrbitCounterResetTo(bool b);
  void setBunchCounterResetTo(bool b);
  void setEventCounterResetTo(bool b);  
  void setEventBufferResetTo(bool b);
  void setL1aTo(bool b);
  void setCalPrepareTo(bool b);
  
  void setPreL1aTo(bool b);
  void setNoZeroSuppressionTo(bool b);
  void setCalPulseIntTo(bool b);
  void setCalPulseExtTo(bool b);
  void setLinkResetEcondTo(bool b);
  void setLinkResetEcontTo(bool b);
  void setChipSyncTo(bool b);
  void setFatEventTo(bool b);

  void setIdle();

  // Emulation-only truth/cheat flags for checking/statistics
  void setCollisionBcTo(bool b);  
  void setCalL1aIntTo(bool b);  
  void setCalL1aExtTo(bool b);  
  void setHgcrocTcdsThrottleTo(bool b);  
  void setHgcrocBeThrottleTo(bool b);  
  void setEcondAsicThrottleTo(bool b);  
  void setEcondBeThrottleTo(bool b);  
  void setTriggerRuleThrottleTo(bool b);  

  void merge(const FastControlCommands &c);
  
  void print(const std::string &s="") const;
    
  static const std::string& commandName(Command c);

 private:  
  uint32_t word_;

  static std::string commandName_[ExtendedRange];
};

#endif
