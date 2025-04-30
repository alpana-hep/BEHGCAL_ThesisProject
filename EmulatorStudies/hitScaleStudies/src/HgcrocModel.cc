#include <iostream>
#include <iomanip>
#include <cassert>

#include "HgcrocModel.h"


HgcrocModel::HgcrocModel() : newL1a_(false), bx_(0), activeBx_(0) {
}

void HgcrocModel::update(const FastControlCommands &c) {
  fFastControlCounters.update(c);

  // Check working OK; remove when trying to handle errors later
  //if(c.l1a()) assert(!fullBuffer());
  
  // Add L1A to queue; drop it completely if buffer is full
  if(c.l1a() && !fullBuffer()) counterQueue_.push(fFastControlCounters);

  // Drain the event data; 40 BX for data, 1 BX for an idle word
  if(counterQueue_.empty()) bx_=0;
  else {
    bx_++;

    // Flag as end of event data
    newL1a_=(bx_==PacketTransmitTime);

    // Ready for next event; discard from buffer
    if(bx_==PacketTransmitTime+1) {
      counterQueue_.pop();
      bx_=0;
    }
  }

  // Keep track of continuous activity
  if(activeBx_==0) {
    if(counterQueue_.size()>0) activeBx_=1;
  } else {
    if(counterQueue_.size()>0) activeBx_++;
    else activeBx_=0;
  }
}

void HgcrocModel::eventBufferReset() {
  newL1a_=false;
  bx_=0;
  activeBx_=0;
  
  while(counterQueue_.size()>0) counterQueue_.pop();
  assert(counterQueue_.size()==0);
}

bool HgcrocModel::l1a(FastControlCounters &c) const {
  if(!newL1a_) return false;

  // Return counters of event which has just finished being sent
  c=counterQueue_.front();
  return true;
}

unsigned HgcrocModel::occupancy() const {
  return counterQueue_.size();
}

bool HgcrocModel::fullBuffer() const {
  return counterQueue_.size()>=BufferSize;
}

unsigned HgcrocModel::activeBx() const {
  return activeBx_;
}

void HgcrocModel::print() const {
  std::cout << "HgcrocModel::print()" << std::endl
	    << " Current counter status" << std::endl;
  fFastControlCounters.print();
  std::cout << " Number of L1As in buffer = "
	    << std::setw(2) << counterQueue_.size();
  if(fullBuffer()) std::cout << " = full";
  std::cout << std::endl;
}
