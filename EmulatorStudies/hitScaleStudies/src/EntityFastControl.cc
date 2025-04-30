#include <iostream>
#include <cassert>

#include "EntityFastControl.h"


EntityFastControl::EntityFastControl(const std::string &n, Clocks::Frequency f) :
  Entity(n,f),
  rxCommandsPtr_(0),
  txRequestsPtr_(0) {
}

EntityFastControl::~EntityFastControl() {
}

// Inputs
void EntityFastControl::connectRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_=&c;
}

void EntityFastControl::strobeRx() {
  assert(rxCommandsPtr_!=0);
  rxCommandsPtr_->strobeOutput();
}

// Outputs
void EntityFastControl::connectTxPortTo(Connection<FastControlRequests> &r) {
  txRequestsPtr_=&r;
}

void EntityFastControl::strobeTx() {
  if(txRequestsPtr_!=0) txRequestsPtr_->strobeInput();
}

// Processing
void EntityFastControl::subProcess40Mhz(bool qL1a) {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcess40Mhz()  entering"
	      << std::endl;
  }

  rxCommandsPtr_->get(commands_);
  if(debugPrint_) {
    std::cout << "  " << name_ << "::subProcess40Mhz()  Got commands"
	      << std::endl;
    commands_.print("   ");
  }

  // Update counters
  counters_.update(commands_);
  if(debugPrint_) {
    std::cout << "  " << name_ << "::subProcess40Mhz()  Updated counters"
	      << std::endl;
    counters_.print("   ");
  }

  // Keep counters for L1As
  if(qL1a) {
    
    // No HGCROC modelling
    if(false) {
      if(commands_.l1a()) {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcess40Mhz()  Found L1A"
		    << std::endl;
	  counters_.print("   ");
	}
	
	l1aCountersQueue_.push(counters_);
      }
      
      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcess40Mhz()  L1A counter queue size = "
		  << l1aCountersQueue_.size() << std::endl;
      }

    } else {

      // With HGCROC modelling
      hgcrocModel_.update(commands_);

      FastControlCounters c;
      if(hgcrocModel_.l1a(c)) {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcess40Mhz()  Found L1A"
		    << std::endl;
	  c.print("   ");
	  
	  std::cout << "  Current time = " << Clocks::clocks().bxNumber()
		    << ", L1A time = " << c.totalBx()
		    << ", delay = " << Clocks::clocks().bxNumber()-c.totalBx()
                << std::endl;
	}
	
	l1aCountersQueue_.push(c);
      }
    }

    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcess40Mhz()  L1A counter queue size = "
		<< l1aCountersQueue_.size() << std::endl;
    }
  }

  // Send requests 
  if(txRequestsPtr_!=0) {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcess40Mhz()  Sending requests"
		<< std::endl;
      requests_.print("   ");
    }
    txRequestsPtr_->set(requests_);
  }
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcess40Mhz()   exiting" << std::endl;
  }
}

void EntityFastControl::print() const {
  std::cout << "::print()" << std::endl;
  std::cout << std::endl;
}
