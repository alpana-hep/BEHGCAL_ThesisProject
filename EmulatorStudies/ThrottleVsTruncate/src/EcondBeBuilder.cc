#include <iostream>
#include <iomanip>
#include <cassert>

#include "RandomSingleton.h"
#include "FastControlCommands.h"
#include "FastControlCounters.h"
#include "EcondBeBuilder.h"
#include "EcondBeHeader.h"
#include "EcondBeTrailer.h"
#include "EcondAsic.h"
#include "PacketChecker.h"

EcondBeBuilder::EcondBeBuilder() :
  Entity("EcondBeBuilder",Clocks::Clock0280),
  //txLpgbtWordPtr_(0),
  txRequestsPtr_(0) {

  rxCommandsPtr_[0]=0;
  rxCommandsPtr_[1]=0;

  state_=Idle;
  econdAsicBufferThrottle_=false;
  
  fPacketWords=0xffffffff;

  FastControlCommands fcc;
  fcc.setWordTo(0);

  unsigned n(10);
  for(unsigned i(0);i<n;i++) {
    vFastControlCommands[0].push(fcc);
    vFastControlCommands[1].push(fcc);
  }
}

EcondBeBuilder::~EcondBeBuilder() {
}

void EcondBeBuilder::setDefinitionTo(const EcondAsicDefinition &d) {
  fModuleDefinition=d;
  //vElink.resize(d.numberOfElinks());
}

void EcondBeBuilder::setReadOffsetTo(unsigned o) {
  elinkReadReset_=-(int)o;
  assert((elinkReadReset_+o)==0);
}

// Connections
void EcondBeBuilder::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  connectDriverRxPortTo(*c);
  d.connectTxPortTo(*c);
}

void EcondBeBuilder::connect(FastControlStream &s) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  connectStreamRxPortTo(*c);
  s.connectTxPortTo(*c);

  Connection<FastControlRequests> *r(new Connection<FastControlRequests>);
  //r->initialiseQueue(FastControlRequests());
  connectTxPortTo(*r);
  s.connectRxPortTo(*r);
}

void EcondBeBuilder::connect(EcondAsic &e) {
  for(unsigned i(0);i<fModuleDefinition.numberOfElinks();i++) {
    Connection<uint32_t> *w(new Connection<uint32_t>);
    connectRxPortTo(*w);
    e.connectTxPortTo(*w);
  }
}

// Inputs
void EcondBeBuilder::connectDriverRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_[0]=&c;
}

void EcondBeBuilder::connectStreamRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_[1]=&c;
}

void EcondBeBuilder::connectRxPortTo(Connection<uint32_t> &e) {
  rxElinkPtrVector_.push_back(&e);
}

void EcondBeBuilder::strobeRx() {
  rxCommandsPtr_[0]->strobeOutput();
  rxCommandsPtr_[1]->strobeOutput();
  
  for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
    rxElinkPtrVector_[i]->strobeOutput();
  }
}

// Outputs
void EcondBeBuilder::connectTxPortTo(Connection<FastControlRequests> &r) {
  txRequestsPtr_=&r;
}
/*
void EcondBeBuilder::connectTxPortTo(Connection<FastControlLpgbtWord> &c) {
  txLpgbtWordPtr_=&c;
}
*/
void EcondBeBuilder::connectTxPortTo(Connection<RamWord> &w) {
  assert(txConnectionPtrVector_.size()<2); // Single or double lpGBT only
  txConnectionPtrVector_.push_back(&w);
}

void EcondBeBuilder::strobeTx() {
  txRequestsPtr_->strobeInput();
  //txLpgbtWordPtr_->strobeInput();
  
  for(unsigned i(0);i<txConnectionPtrVector_.size();i++) {
    txConnectionPtrVector_[i]->strobeInput();
  }
}


unsigned EcondBeBuilder::numberOfElinks() const {
  return 0;//vElinks.size();
}

void EcondBeBuilder::process() {
  if(debugPrint_) {
    std::cout << "EcondBeBuilder::process()  entering" << std::endl;
  }

  // Check fast control connections
  assert(rxCommandsPtr_[0]!=0);
  assert(rxCommandsPtr_[1]!=0);
  assert(rxCommandsPtr_[0]->size()==0);
  assert(rxCommandsPtr_[1]->size()==0);

  //assert(txLpgbtWordPtr_!=0);
  assert(txRequestsPtr_!=0);

  // 40 MHz processing  
  ///if(txLpgbtWordPtr_->size()==0) { // Only after a 40 MHz tick
  if(txRequestsPtr_->size()==0) { // Only after a 40 MHz tick
    subProcess40MHz();    

    // Reset the elink counter
    elinkRead_=elinkReadReset_;

    if(debugPrint_) {
      std::cout << " Elink read number reset to " << elinkRead_ << std::endl;
    }

  } else {
    //assert(txLpgbtWordPtr_->size()==1);
    assert(txRequestsPtr_->size()==1);
  }
  
  // 280 MHz processing
  unsigned activeNumber(0);
  
  for(unsigned i(0);i<txConnectionPtrVector_.size();i++) {
    if(elinkRead_>=0 && elinkRead_<(int)rxElinkPtrVector_.size()) {
      activeNumber++;

      if(debugPrint_) {
	std::cout << " Elink read number " << elinkRead_
		  << " in range 0 to " << rxElinkPtrVector_.size()-1
		  << std::endl;
      }
      
      subProcess280MHzA(elinkRead_);
    
    } else {
      if(debugPrint_) {
	std::cout << " Elink read number " << elinkRead_
		  << " not in range 0 to " << rxElinkPtrVector_.size()-1
		  << std::endl;
      }
    }
      
    // Increment the elink counter
    elinkRead_++;
  }

  // Requires care with the offsets (always even for double lpGBT cases)
  if(activeNumber>0) subProcess280MHzB();

  if(debugPrint_) {
    std::cout << "EcondBeBuilder::process()  exiting" << std::endl;
  }
}

void EcondBeBuilder::subProcess40MHz() {
  if(debugPrint_) {
    std::cout << " EcondBeBuilder::subProcess40MHz()  entering" << std::endl;
  }

  //assert(txLpgbtWordPtr_->size()==0);
  assert(txRequestsPtr_->size()==0);

  FastControlCommands comm;
  for(unsigned i(0);i<2;i++) {      
    assert(rxCommandsPtr_[i]->get(comm));
      
    if(debugPrint_) {
      std::cout << " Input commands "<< i << ", ";comm.print();
    }

    // Send local commands to ECON-D ASIC
    /*
    if(i==1) {
      assert(txLpgbtWordPtr_->set(lpgbtWord_));
      comm=lpgbtWord_.commands();
    }
    */
    vFastControlCommands[i].push(comm);
      
    comm=vFastControlCommands[i].front();
    vFastControlCommands[i].pop();
      
    if(debugPrint_) {
      std::cout << " Queue commands "<< i << ", ";comm.print();
    }
    
    fastControlCounters_[i].update(comm);

    if(comm.l1a()) {
      EcondBeHeader ebh;
      ebh.setCountersTo(fastControlCounters_[i]);
      ebh.setErrorBitsTo(0);
      ebh.setPacketLengthTo(0x3ff);
      econdBeHeaderQueue_[i].push(ebh);

      vFastControlCounters[i].push(fastControlCounters_[i]);
	
      if(debugPrint_) {
	std::cout << " L1A added to counters " << i << " queue, ";
	fastControlCounters_[i].print();
	ebh.print();
      }
    }
  }
  
  // This allows requests for global L1A throttle and local resets
  FastControlRequests req;
  req.setEcondAsicThrottleTo(econdAsicBufferThrottle_);

  if(debugPrint_) {
    std::cout << " Request sent ";
    req.print();
  }

  assert(txRequestsPtr_->set(req)); // Selay after doing all channels?

  if(debugPrint_) {
    std::cout << " Elink data received =" << std::hex << std::setfill('0');
  }

  for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
    assert(rxElinkPtrVector_[i]->get(elinkArray[i]));

    if(debugPrint_) {
      std::cout << " 0x" << std::setw(8) << elinkArray[i];
    }
  }

  if(debugPrint_) {
    std::cout << std::dec << std::setfill(' ') << std::endl;
  }

  
  if(debugPrint_) {
    std::cout << " EcondBeBuilder::subProcess40MHz()  exiting" << std::endl;
  }
}

void EcondBeBuilder::subProcess280MHzA(const unsigned elink) {
  if(debugPrint_) {
    std::cout << " EcondBeBuilder::subProcess280MHzA()  entering" << std::endl;
  }
  
  if(debugPrint_) {
    std::cout << " Elink " << elink << " value = 0x"
	      << std::hex << std::setfill('0')
	      << std::setw(8) << elinkArray[elink]
	      << std::dec << std::setfill(' ') << std::endl;
  }

  // Input state machine
  if(state_==Idle) {
    if(debugPrint_) {
      std::cout << " Initial state = Idle" << std::endl;
    }
  
    if((elinkArray[elink]&0xffff)!=EcondAsicDefinition::IdlePattern) {
      assert((elinkArray[elink]&0xffff)==EcondAsicHeader::HeaderPattern);

      econAsicPacket32_.resize(0);
      econBuilderPacket32_.resize(0);

      assert(!econdBeHeaderQueue_[0].empty());
      EcondBeHeader ebh(econdBeHeaderQueue_[0].front());

      EcondAsicHeader eahHalf;
      eahHalf.setWordTo(0,elinkArray[elink]); // Enough to get length
      ebh.setPacketLengthTo(eahHalf.packetWords());
      if(debugPrint_) ebh.print();
      
      // Should compare with fast control queue
      
      nonIdleWords_.push(ebh.word(0));
      nonIdleFlags_.push(1);
      econBuilderPacket32_.push_back(nonIdleWords_.back());
      nonIdleWords_.push(ebh.word(1));
      nonIdleFlags_.push(0);
      econBuilderPacket32_.push_back(nonIdleWords_.back());

      nonIdleWords_.push(elinkArray[elink]);
      nonIdleFlags_.push(0);
      econAsicPacket32_.push_back(nonIdleWords_.back());
      econBuilderPacket32_.push_back(nonIdleWords_.back());

      econdAsicHeader_.setWordTo(0,nonIdleWords_.back());
	
      state_=Header;
      if(debugPrint_) std::cout << " State changed to Header" << std::endl;
    }

  } else if(state_==Header) {
    if(debugPrint_) {
      std::cout << " Initial state = Header" << std::endl;
    }

    nonIdleWords_.push(elinkArray[elink]);
    nonIdleFlags_.push(0);
    econAsicPacket32_.push_back(nonIdleWords_.back());
    econBuilderPacket32_.push_back(nonIdleWords_.back());
    econdAsicHeader_.setWordTo(1,nonIdleWords_.back());
    
    if(debugPrint_) {std::cout << " ECON-D ASIC header ";econdAsicHeader_.print();}
    packetLength_=2*econdAsicHeader_.packetWords();

    /*
    EcondAsicHeader::BufferStatus bs(econdAsicHeader_.bufferStatus());
    if(econdAsicBufferThrottle_) {
      if(bs==EcondAsicHeader::Low) {
	econdAsicBufferThrottle_=false;
	if(debugPrint_) std::cout << " ECON-D ASIC buffer throttle changed to false" << std::endl;
      }
      
    } else {
      if(bs==EcondAsicHeader::High || bs==EcondAsicHeader::Full) {
	econdAsicBufferThrottle_=true;
	if(debugPrint_) std::cout << " ECON-D ASIC buffer throttle changed to true" << std::endl;
      }
    }
    */
    state_=Body;
    if(debugPrint_) std::cout << " State changed to Body" << std::endl;
      
  } else if(state_==Body) {
    if(debugPrint_) {
      std::cout << " Initial state = Body" << std::endl;
    }

    nonIdleWords_.push(elinkArray[elink]);
    nonIdleFlags_.push(0);
    econAsicPacket32_.push_back(nonIdleWords_.back());
    econBuilderPacket32_.push_back(nonIdleWords_.back());

    if(econAsicPacket32_.size()>=((packetLength_+1)/2)) {
      EcondBeTrailer ebt;
      ebt.setPacketLengthTo(econAsicPacket32_.size());
      nonIdleWords_.push(ebt.word());
      nonIdleFlags_.push(2);
      econBuilderPacket32_.push_back(nonIdleWords_.back());

      assert(PacketChecker::checkEcondAsicPacket32(fModuleDefinition,vFastControlCounters[1].front(),econAsicPacket32_,debugPrint_));
      //assert(PacketChecker::checkEcondBeBuilderPacket32(econBuilderPacket32_));

      vFastControlCounters[0].pop();
      vFastControlCounters[1].pop();
      
      econdBeHeaderQueue_[0].pop();
      econdBeHeaderQueue_[1].pop();
      state_=Idle;
      if(debugPrint_) std::cout << " State changed to Idle" << std::endl;
    }
  }
  if(debugPrint_) {
    std::cout << " EcondBeBuilder::subProcess280MHzA()  exiting" << std::endl;
  }
}

void EcondBeBuilder::subProcess280MHzB() {
  if(debugPrint_) {
    std::cout << " EcondBeBuilder::subProcess280MHzB()  entering" << std::endl;
  }

  // Output; first, merge 32-bit words to RAM words
  while(nonIdleWords_.size()>=2) {
    if(debugPrint_) {
      std::cout << " Number of 32-bit output words in queue " << nonIdleWords_.size() << ", merging" << std::endl;
    }
      
    RamWord rw;
    rw.clearBits();
      
    rw.setWordTo(0,nonIdleWords_.front());
    if(nonIdleFlags_.front()==1) rw.setStartBit();
    if(nonIdleFlags_.front()==2) rw.setEndBit();
    nonIdleWords_.pop();
    nonIdleFlags_.pop();
      
    if(rw.endBit()) {
      rw.setWordTo(1,0);
      rw.setPaddingBit();
    } else {
      rw.setWordTo(1,nonIdleWords_.front());
      if(nonIdleFlags_.front()==2) rw.setEndBit();
      nonIdleWords_.pop();
      nonIdleFlags_.pop();
    }

    if(debugPrint_) {
      std::cout << " Non-idle RAM word, ";rw.print();
    }

    ramWordQueue_.push(rw);
    
  }

  if(debugPrint_) {
    std::cout << " Number of 32-bit output words in queue " << nonIdleWords_.size() << ", waiting" << std::endl;
  }

  // Second, send one set of output RAM words matching number of output connections
  if(ramWordQueue_.size()>=txConnectionPtrVector_.size()) {
    if(debugPrint_) {
      std::cout << " Number of RAM output words in queue " << txConnectionPtrVector_.size() << ", sending" << std::endl;
    }
    
    for(unsigned i(0);i<txConnectionPtrVector_.size();i++) {
      if(debugPrint_) ramWordQueue_.front().print();
      assert(txConnectionPtrVector_[i]->set(ramWordQueue_.front()));
      ramWordQueue_.pop();
    }
  
  } else {
    if(debugPrint_) {
      std::cout << " Number of RAM output words in queue " << txConnectionPtrVector_.size() << ", waiting" << std::endl;
    }
  }

  if(debugPrint_) {
    std::cout << " EcondBeBuilder::subProcess280MHzB()  exiting" << std::endl;
  }
}

void EcondBeBuilder::print() const {
  std::cout << "EcondBeBuilder::print()" << std::endl;
}
