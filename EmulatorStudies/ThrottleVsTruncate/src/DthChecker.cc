#include <iostream>
#include <iomanip>
#include <cassert>

#include "GlobalConfiguration.h"
#include "PacketChecker.h"
#include "DthChecker.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"

DthChecker::DthChecker() :
  Entity("DthChecker",GlobalConfiguration::configuration().slinkClockFrequency()),
  rxCommandsPtr_(0),
  rxSlinkWordPtr_(0),
  txDataPtr_(0),
  //dthCheckerData_.active_(false),
  //dthCheckerData_.numberOfTicks_(0),
  numberOfWords_(0),
  numberOfPackets_(0),
  numberOfCheckedPackets_(0) {
}

DthChecker::~DthChecker() {
}

// Initialisation
void DthChecker::setDefinitionTo(const SlinkDefinition &d) {
  slinkDefinition_=d;
}

void DthChecker::initialise() {
  std::cout << "DthChecker::initialise() " << slinkDefinition_.label() << std::endl;
  std::cout << std::endl;
}

// Connections
void DthChecker::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  connectRxPortTo(*c);
  d.connectTxPortTo(*c);
}

void DthChecker::connect(SlinkBuilder &s) {
  Connection< std::pair<bool,SlinkWord> > *w(new Connection< std::pair<bool,SlinkWord> >);
  connectRxPortTo(*w);
  s.connectTxPortTo(*w);
}

// Inputs
void DthChecker::connectRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_=&c;
}

void DthChecker::connectRxPortTo(Connection< std::pair<bool,SlinkWord> > &w) {
  rxSlinkWordPtr_=&w;
}

void DthChecker::strobeRx() {
  rxCommandsPtr_->strobeOutput();
  rxSlinkWordPtr_->strobeOutput();
}

void DthChecker::connectTxPortTo(Connection<DthCheckerData> &d) {
  txDataPtr_=&d;
}

void DthChecker::strobeTx() {
  if(txDataPtr_!=0) txDataPtr_->strobeInput();
}

// Processing
void DthChecker::process() {
  if(debugPrint_) {
    std::cout << "DthChecker::process()  entering" << std::endl;
  }
  
  // Check connections are correct

  assert(rxCommandsPtr_!=0);
  assert(rxCommandsPtr_->size()==0);
  assert(rxSlinkWordPtr_!=0);
  assert(rxSlinkWordPtr_->size()==0);

  // 40 MHz section

  FastControlCommands fcc;
  if(rxCommandsPtr_->get(fcc)) { // Only after a 40 MHz tick

    // Update counters
    dthCheckerData_.counters_.update(fcc);

    // Check for L1A
    if(fcc.l1a()) {

      // Add counters to the queue
      countersQueue_.push(dthCheckerData_.counters_);

      if(debugPrint_) {
        std::cout << " New L1A added, queue size = " << countersQueue_.size() << " ";
	dthCheckerData_.counters_.print();
      }
    }
  }

  // 120 MHz section
  
  std::pair<bool,SlinkWord> bsw;
  if(rxSlinkWordPtr_->get(bsw)) {
    if(!dthCheckerData_.active_) {
      assert(bsw.first);

      slinkWordVector_.resize(1);
      slinkWordVector_[0]=bsw.second;

      if(debugPrint_) {
	std::cout << " Inactive, starting Slink word 0, ";bsw.second.print();
      }

      dthCheckerData_.active_=true;

    } else {
      slinkWordVector_.push_back(bsw.second);

      if(!bsw.first) {
	if(debugPrint_) {
	  std::cout << " Active, new Slink word "
		    << slinkWordVector_.size()-1 << ", ";bsw.second.print();
	}

      } else {
	dthCheckerData_.active_=false;	
	if(debugPrint_) {
	  std::cout << " Active, ending Slink word "
		    << slinkWordVector_.size()-1 << ", ";bsw.second.print();
	}
	numberOfPackets_++;

	assert(!countersQueue_.empty());
	if(debugCheck_) {
	  assert(PacketChecker::checkSlinkBuilderPacket(slinkDefinition_,countersQueue_.front(),slinkWordVector_,debugPrint_));
	  numberOfCheckedPackets_++;
	}
	countersQueue_.pop();
      }
    }
    
    numberOfWords_++;
  }

  dthCheckerData_.numberOfTicks_++;

  if(txDataPtr_!=0) assert(txDataPtr_->set(dthCheckerData_));
  
  if(debugPrint_) {
    std::cout << "DthChecker::process()  exiting" << std::endl;
  }
}

void DthChecker::print() const {
  std::cout << "DthChecker::print() " << slinkDefinition_.label() << std::endl;
  std::cout << " Number of ticks = " << dthCheckerData_.numberOfTicks_
	    << ", total possible rate = " << 128.0*0.120 << " Gbit/s"
	    << ", total possible data = " << 0.000000128*dthCheckerData_.numberOfTicks_
	    << " Gbit" << std::endl;
  std::cout << " Number of words received = " << numberOfWords_
	    << ", average data rate = "
	    << 128.0*numberOfWords_*0.120/dthCheckerData_.numberOfTicks_
	    << " Gbit/s, actual data volume = "
	    << 0.000000128*numberOfWords_ << " Gbit" << std::endl;
  std::cout << " Slink efficiency = " << (100*numberOfWords_)/dthCheckerData_.numberOfTicks_
	    << " %" << std::endl;
  std::cout << " Number of complete event packets received = "
	    << numberOfPackets_;
  if(numberOfPackets_>0) {
    std::cout << ", average packet size = "
	      << 0.128*numberOfWords_/numberOfPackets_
	      << " kbit";
  }
  std::cout << std::endl;
  std::cout << " Number of event packets checked = " << numberOfCheckedPackets_ << std::endl;

  std::cout << std::endl;
}
