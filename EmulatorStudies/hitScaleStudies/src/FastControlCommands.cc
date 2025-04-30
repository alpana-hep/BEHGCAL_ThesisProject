#include <iostream>
#include <cassert>

#include "FastControlCommands.h"

std::string FastControlCommands::commandName_[ExtendedRange]={
  "OrbitCounterReset  ",
  "BunchCounterReset  ",
  "EventCounterReset  ",
  "EventBufferReset   ",
  "L1A                ", 
  "CalPrepare         ",
  
  "PreL1A             ",
  "NoZeroSuppression  ",
  "CalPulseInt        ",
  "CalPulseExt        ",
  "LinkResetEcond     ",
  "LinkResetEcont     ",
  "ChipSync           ",
  "FatEvent           ",
  "Spare              ",
  "Spare              ",
  
  "CollisionBc        ",
  "CalL1aInt          ",
  "CalL1aExt          ",
  "HgcrocTcdsThrottle ",
  "HgcrocBeThrottle   ",
  "EcondAsicThrottle  ",
  "EcondBeThrottle    ",
  "TriggerRuleThrottle"
};

FastControlCommands::FastControlCommands() : word_(0) {
}

uint32_t FastControlCommands::word() const {
  return word_;
}

void FastControlCommands::setWordTo(uint32_t c) {
  word_=c;
}

bool FastControlCommands::command(FastControlCommands::Command c) const {
  assert(c<ExtendedRange);
  return (word_&(1<<c))!=0;
}

bool FastControlCommands::orbitCounterReset() const {
  return (word_&(1<<OrbitCounterReset))!=0;
}

bool FastControlCommands::bunchCounterReset() const {
  return (word_&(1<<BunchCounterReset))!=0;
}

bool FastControlCommands::eventCounterReset() const {
  return (word_&(1<<EventCounterReset))!=0;
}

bool FastControlCommands::eventBufferReset() const {
  return (word_&(1<<EventBufferReset))!=0;
}

bool FastControlCommands::l1a() const {
  return (word_&(1<<L1a))!=0;
}

bool FastControlCommands::calPrepare() const {
  return (word_&(1<<CalPrepare))!=0;
}

bool FastControlCommands::preL1a() const {
  return (word_&(1<<PreL1a))!=0;
}

bool FastControlCommands::noZeroSuppression() const {
  return (word_&(1<<NoZeroSuppression))!=0;
}

bool FastControlCommands::calPulseInt() const {
  return (word_&(1<<CalPulseInt))!=0;
}

bool FastControlCommands::calPulseExt() const {
  return (word_&(1<<CalPulseExt))!=0;
}

bool FastControlCommands::linkResetEcond() const {
  return (word_&(1<<LinkResetEcond))!=0;
}

bool FastControlCommands::linkResetEcont() const {
  return (word_&(1<<LinkResetEcont))!=0;
}

bool FastControlCommands::chipSync() const {
  return (word_&(1<<ChipSync))!=0;
}

bool FastControlCommands::fatEvent() const {
  return (word_&(1<<FatEvent))!=0;
}

bool FastControlCommands::idle() const {
  return word_==0;
}

bool FastControlCommands::collisionBc() const {
  return (word_&(1<<CollisionBc))!=0;
}

bool FastControlCommands::calL1aInt() const {
  return (word_&(1<<CalL1aInt))!=0;
}

bool FastControlCommands::calL1aExt() const {
  return (word_&(1<<CalL1aExt))!=0;
}

bool FastControlCommands::hgcrocTcdsThrottle() const {
  return (word_&(1<<HgcrocTcdsThrottle))!=0;
}

bool FastControlCommands::hgcrocBeThrottle() const {
  return (word_&(1<<HgcrocBeThrottle))!=0;
}

bool FastControlCommands::econdAsicThrottle() const {
  return (word_&(1<<EcondAsicThrottle))!=0;
}

bool FastControlCommands::econdBeThrottle() const {
  return (word_&(1<<EcondBeThrottle))!=0;
}

bool FastControlCommands::triggerRuleThrottle() const {
  return (word_&(1<<TriggerRuleThrottle))!=0;
}

bool FastControlCommands::throttle() const {
  return (word_&((1<<HgcrocTcdsThrottle)|(1<<HgcrocBeThrottle)|(1<<EcondAsicThrottle)|(1<<EcondBeThrottle)|(1<<TriggerRuleThrottle)))!=0;
}

bool FastControlCommands::operator==(const FastControlCommands &c) const {
  return word_==c.word_;
}

void FastControlCommands::setCommandTo(FastControlCommands::Command c, bool b) {
  assert(c<ExtendedRange);
  if(b) word_|= (1<<c);
  else  word_&=~(1<<c);
}

void FastControlCommands::setOrbitCounterResetTo(bool b) {
  if(b) word_|= (1<<OrbitCounterReset);
  else  word_&=~(1<<OrbitCounterReset);
}

void FastControlCommands::setBunchCounterResetTo(bool b) {
  if(b) word_|= (1<<BunchCounterReset);
  else  word_&=~(1<<BunchCounterReset);
}

void FastControlCommands::setEventCounterResetTo(bool b) {
  if(b) word_|= (1<<EventCounterReset);
  else  word_&=~(1<<EventCounterReset);
}

void FastControlCommands::setEventBufferResetTo(bool b) {
  if(b) word_|= (1<<EventBufferReset);
  else  word_&=~(1<<EventBufferReset);
}

void FastControlCommands::setL1aTo(bool b) {
  if(b) word_|= (1<<L1a);
  else  word_&=~(1<<L1a);
}

void FastControlCommands::setCalPrepareTo(bool b) {
  if(b) word_|= (1<<CalPrepare);
  else  word_&=~(1<<CalPrepare);
}

void FastControlCommands::setPreL1aTo(bool b) {
  if(b) word_|= (1<<PreL1a);
  else  word_&=~(1<<PreL1a);
}

void FastControlCommands::setNoZeroSuppressionTo(bool b) {
  if(b) word_|= (1<<NoZeroSuppression);
  else  word_&=~(1<<NoZeroSuppression);
}

void FastControlCommands::setCalPulseIntTo(bool b) {
  if(b) word_|= (1<<CalPulseInt);
  else  word_&=~(1<<CalPulseInt);
}

void FastControlCommands::setCalPulseExtTo(bool b) {
  if(b) word_|= (1<<CalPulseExt);
  else  word_&=~(1<<CalPulseExt);
}

void FastControlCommands::setLinkResetEcondTo(bool b) {
  if(b) word_|= (1<<LinkResetEcond);
  else  word_&=~(1<<LinkResetEcond);
}

void FastControlCommands::setLinkResetEcontTo(bool b) {
  if(b) word_|= (1<<LinkResetEcont);
  else  word_&=~(1<<LinkResetEcont);
}

void FastControlCommands::setChipSyncTo(bool b) {
  if(b) word_|= (1<<ChipSync);
  else  word_&=~(1<<ChipSync);
}

void FastControlCommands::setFatEventTo(bool b) {
  if(b) word_|= (1<<FatEvent);
  else  word_&=~(1<<FatEvent);
}

void FastControlCommands::setIdle() {
  word_=0;
}

void FastControlCommands::setCollisionBcTo(bool b) {
  if(b) word_|= (1<<CollisionBc);
  else  word_&=~(1<<CollisionBc);
}

void FastControlCommands::setCalL1aIntTo(bool b) {
  if(b) word_|= (1<<CalL1aInt);
  else  word_&=~(1<<CalL1aInt);
}

void FastControlCommands::setCalL1aExtTo(bool b) {
  if(b) word_|= (1<<CalL1aExt);
  else  word_&=~(1<<CalL1aExt);
}

void FastControlCommands::setHgcrocTcdsThrottleTo(bool b) {
  if(b) word_|= (1<<HgcrocTcdsThrottle);
  else  word_&=~(1<<HgcrocTcdsThrottle);
}

void FastControlCommands::setHgcrocBeThrottleTo(bool b) {
  if(b) word_|= (1<<HgcrocBeThrottle);
  else  word_&=~(1<<HgcrocBeThrottle);
}

void FastControlCommands::setEcondAsicThrottleTo(bool b) {
  if(b) word_|= (1<<EcondAsicThrottle);
  else  word_&=~(1<<EcondAsicThrottle);
}

void FastControlCommands::setEcondBeThrottleTo(bool b) {
  if(b) word_|= (1<<EcondBeThrottle);
  else  word_&=~(1<<EcondBeThrottle);
}

void FastControlCommands::setTriggerRuleThrottleTo(bool b) {
  if(b) word_|= (1<<TriggerRuleThrottle);
  else  word_&=~(1<<TriggerRuleThrottle);
}

void FastControlCommands::merge(const FastControlCommands &c) {
  word_|=c.word_;
}

const std::string& FastControlCommands::commandName(FastControlCommands::Command c) {
  return commandName_[c];
}

void FastControlCommands::print(const std::string &s) const {
  std::cout << s << "FastControlCommands::print()  word = 0x"
	    << std::hex << word_ << std::dec;
  
  if(idle()) std::cout << "  Idle" << std::endl;
  else {
    std::cout << "  OCR = " << (orbitCounterReset()?" true":"false")
	      << ", BCR = " << (bunchCounterReset()?" true":"false")
	      << ", ECR = " << (eventCounterReset()?" true":"false")
	      << ", EBR = " << (eventBufferReset()?" true":"false")
	      << ", L1A = " << (l1a()?" true":"false")
	      << ", PRE = " << (preL1a()?" true":"false")
	      << ", NZS = " << (noZeroSuppression()?" true":"false")
	      << ", CLI = " << (calPulseInt()?" true":"false")
	      << ", CLE = " << (calPulseExt()?" true":"false")
	      << ", LKR = " << (linkResetEcond()?" true":"false")
	      << ", LKR = " << (linkResetEcont()?" true":"false")
	      << ", SYN = " << (chipSync()?" true":"false")
	      << ", FAT = " << (fatEvent()?" true":"false")
	      << std::endl;
  }
}
