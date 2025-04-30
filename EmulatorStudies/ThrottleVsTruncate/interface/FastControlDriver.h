#ifndef Emulation_HGCalBufferModel_FastControlDriver_h
#define Emulation_HGCalBufferModel_FastControlDriver_h

#include <vector>

#include "RandomSingleton.h"
#include "Entity.h"
#include "FastControlCounters.h"
#include "FastControlRequests.h"
#include "FastControlCommands.h"
#include "Connection.h"
#include "HgcrocModel.h"
#include "FastControlDriverConfiguration.h"

class FastControlDriverAnalyser;

class FastControlDriver : public Entity {
 public:
  friend FastControlDriverAnalyser;
  
  FastControlDriver();

  void setConfigurationTo(const FastControlDriverConfiguration &c);
  void setArgumentsTo(const Arguments &a);
  void initialise();
  void initialiseCollisionBc();
  //void setMethodTo(unsigned m);
  
  //const FastControlCommands& fcCommands() const;

  // Inputs
  void connectRxPortTo(Connection<FastControlRequests> &r);
  void strobeRx();

  // Outputs
  void connectTxPortTo(Connection<FastControlCommands> &c);
  void connectDelayedTxPortTo(Connection<FastControlCommands> &c);
  void strobeTx();

  // Processing
  void process();

  // Request handling in HGCAL BE
  void subProcessBeRequests();

  // Command generation in TCDS
  void subProcessTcdsCommands();

  // Command handling in HGCAL BE
  void subProcessBeCommands();

  // Determine if triggering
  bool trigger();
  int categ_throttle();
  // Determine if there is a collision in a BC
  /*static*/ bool abortGap(uint16_t bc);
  /*static*/ bool collisionBc(uint16_t bc);
  
  void enableAnalyser();

  bool triggerRule() const;
  
  void print() const;
    
 private:

  // Input
  std::vector< Connection<FastControlRequests>* > rxRequestsPtrVector_;

  // Output
  std::vector< Connection<FastControlCommands>* > txCommandsPtrVector_;
  std::vector< Connection<FastControlCommands>* > txDelayedCommandsPtrVector_;

  // Internal
  FastControlRequests requests_;  

  std::queue<FastControlRequests> requestsQueue_;

  FastControlRequests tcdsRequests_;  
  FastControlCommands tcdsCommands_;
  FastControlCounters tcdsCounters_;

  std::queue<FastControlCommands> commandsQueue_;

  FastControlCommands commands_;
  FastControlCounters counters_;
  
  HgcrocModel hgcrocModel_;

  // Analyser
  FastControlDriverAnalyser *fastControlDriverAnalyserPtr_;

  // Statistics
  unsigned long commandCount_[33];
  unsigned long requestCount_[17];
  unsigned long numberOfHgcrocOverflows_;
  int Throttle_categ =0x0;
  //  std::vector<int> vec_categ_throttle;
  
  // Delays for calibration and pre-L1A
  std::queue<bool> calibrationDelayQueue_;
  std::queue<FastControlCommands> preL1aDelayQueue_;

  FastControlCommands delayedCommands_;
  std::queue<FastControlCommands> beDelayQueue_;

  // Vector and offset for trigger rule
  std::vector<bool> triggerRuleVector_;
  unsigned triggerRuleOffset_;
  
  unsigned long numberOfBx_;
  unsigned long numberOfLiveBx_;
  unsigned long numberOfLiveThrottles_;
  unsigned long numberOfLiveThrottles_Econd_;
  unsigned long numberOfLiveThrottles_Hgcroc_;
  unsigned long numberOfLiveThrottles_BE_;
  //counters for total L1As sent irrespective of throttle;
  unsigned long numberOfL1As_; //total number of L1As that should have been issued
  unsigned long numberOfL1As_wThrottle_; // total number of L1As that were not sent because of throttle
  unsigned long numberOfL1As_sent_; // total L1As actually sent with no throttling
  bool LiveThrottles_BE,LiveThrottles_Hgcroc,LiveThrottles_Econd;
  TRandom &random_;

  // Configuration data
  FastControlDriverConfiguration configuration_;

  // Local configuration variables
  double fL1aProbability; // Probability of trigger per collision BX
  bool collisionBc_[FastControlCounters::MaximumBc];
};

#endif
