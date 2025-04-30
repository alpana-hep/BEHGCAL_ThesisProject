#include <iostream>

#include "FastControlRequests.h"

std::string FastControlRequests::requestName_[Range]={
  "HgcrocBeThrottle ",
  "EcondAsicThrottle",
  "EcondBeThrottle  ",
  "LinkResetEcond   ",
  "EventBufferReset "
};

FastControlRequests::FastControlRequests() : word_(0) {
}

uint16_t FastControlRequests::word() const {
  return word_;
}

void FastControlRequests::setWordTo(uint16_t c) {
  word_=c;
}

bool FastControlRequests::linkResetEcond() const {
  return (word_&(1<<LinkResetEcond))!=0;
}
/*
bool FastControlRequests::linkResetEcont() const {
  return (word_&(1<<LinkResetEcont))!=0;
}
*/
bool FastControlRequests::eventBufferReset() const {
  return (word_&(1<<EventBufferReset))!=0;
}

bool FastControlRequests::throttle() const {
  return (word_&((1<<HgcrocBeThrottle)|(1<<EcondAsicThrottle)|(1<<EcondBeThrottle)))!=0;
}

bool FastControlRequests::hgcrocBeThrottle() const {
  return (word_&(1<<HgcrocBeThrottle))!=0;
}

bool FastControlRequests::econdAsicThrottle() const {
  return (word_&(1<<EcondAsicThrottle))!=0;
}

bool FastControlRequests::econdBeThrottle() const {
  return (word_&(1<<EcondBeThrottle))!=0;
}

/*
bool FastControlRequests::throttle() const {
  return (word_&thrBit)!=0;
}

bool FastControlRequests::linkReset() const {
  return (word_&lkrBit)!=0;
}

bool FastControlRequests::eventBufferReset() const {
  return (word_&ebrBit)!=0;
}
*/
bool FastControlRequests::idle() const {
  return word_==0;
}
/*
void FastControlRequests::setThrottle() {
  word_|= thrBit;
}

void FastControlRequests::setLinkReset() {
  word_|= lkrBit;
}

void FastControlRequests::setEventBufferReset() {
  word_|= ebrBit;
}

void FastControlRequests::setThrottleTo(bool b) {
  if(b) word_|= (1<<Throttle);
  else  word_&=~(1<<Throttle);
}
*/
void FastControlRequests::setLinkResetEcondTo(bool b) {
  if(b) word_|= (1<<LinkResetEcond);
  else  word_&=~(1<<LinkResetEcond);
}
/*
void FastControlRequests::setLinkResetEconTTo(bool b) {
  if(b) word_|= (1<<LinkResetEconT);
  else  word_&=~(1<<LinkResetEconT);
}
*/
void FastControlRequests::setEventBufferResetTo(bool b) {
  if(b) word_|= (1<<EventBufferReset);
  else  word_&=~(1<<EventBufferReset);
}

void FastControlRequests::setHgcrocBeThrottleTo(bool b) {
  if(b) word_|= (1<<HgcrocBeThrottle);
  else  word_&=~(1<<HgcrocBeThrottle);
}

void FastControlRequests::setEcondAsicThrottleTo(bool b) {
  if(b) word_|= (1<<EcondAsicThrottle);
  else  word_&=~(1<<EcondAsicThrottle);
}

void FastControlRequests::setEcondBeThrottleTo(bool b) {
  if(b) word_|= (1<<EcondBeThrottle);
  else  word_&=~(1<<EcondBeThrottle);
}
/*
void FastControlRequests::setThrottleTo(bool b) {
  if(b) word_|= thrBit;
  else  word_&=~thrBit;
}

void FastControlRequests::setLinkResetTo(bool b) {
  if(b) word_|= lkrBit;
  else  word_&=~lkrBit;
}

void FastControlRequests::setEventBufferResetTo(bool b) {
  if(b) word_|= ebrBit;
  else  word_&=~ebrBit;
}
*/
void FastControlRequests::setIdle() {
  word_=0;
}

void FastControlRequests::merge(const FastControlRequests &r) {
  // Only OR throttles
  //if(r.hgcrocThrottle())    setHgcrocThrottleTo(true);
  //if(r.econdAsicThrottle()) setEcondAsicThrottleTo(true);
  //if(r.econdBeThrottle())   setEcondBeThrottleTo(true);

  // Everything
  word_|=r.word_;
}

const std::string& FastControlRequests::requestName(FastControlRequests::Request r) {
  return requestName_[r];
}

void FastControlRequests::print(const std::string &s) const {
  std::cout << s << "FastControlRequests::print()  word = 0x"
	    << std::hex << word_ << std::dec
    	    << "  THR = " << (throttle()?" true":"false")
	    << "  HGT = " << (hgcrocBeThrottle()?" true":"false")
    	    << "  EAT = " << (econdAsicThrottle()?" true":"false")
    	    << "  EBT = " << (econdBeThrottle()?" true":"false")
	    << ", LKR = " << (linkResetEcond()?" true":"false")
	    << ", EBR = " << (eventBufferReset()?" true":"false")
	    << std::endl;
}
