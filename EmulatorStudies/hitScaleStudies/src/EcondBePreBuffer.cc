#include <iostream>
#include <iomanip>
#include <cassert>

#include "EcondBePreBuffer.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"

EcondBePreBuffer::EcondBePreBuffer() :
  Entity("EcondBePreBuffer",Clocks::Clock0280),
  rxArrayPtr_(0),
  //txRamWordPtr_(0),
  numberOfWords_(0),
  numberOfPackets_(0) {
}

EcondBePreBuffer::~EcondBePreBuffer() {
}

EcondBePreBuffer::StateMachine::StateMachine() :
  state_(State::Idle),
  bufferWrite_(0),
  error_(true) {
}


// Initialisation
void EcondBePreBuffer::setDefinitionTo(const LpgbtPairDefinition &d) {
  lpgbtPairDefinition_=d;
}

void EcondBePreBuffer::initialise() {
  std::cout << "EcondBePreBuffer::initialise()" << std::endl
	    << " Number of outputs " << txRamWordPtrVector_.size()
	    << std::endl;
  std::cout << std::endl;
}

// Connections
/*
void EcondBePreBuffer::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  connectRxPortTo(*c);
  d.connectTxPortTo(*c);
}
*/
void EcondBePreBuffer::connect(LpgbtPairSerialiser &l) {
  Connection< std::array<uint32_t,3> > *a(new Connection< std::array<uint32_t,3> >);
  connectRxPortTo(*a);
  l.connectTxPortTo(*a);
}

// Inputs
/*
void EcondBePreBuffer::connectRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_=&c;
}
*/
void EcondBePreBuffer::connectRxPortTo(Connection< std::array<uint32_t,3> > &a) {
  rxArrayPtr_=&a;
  array_[0]=14;
  a.initialiseQueue(array_);
}

void EcondBePreBuffer::strobeRx() {
  //rxCommandsPtr_->strobeOutput();
  rxArrayPtr_->strobeOutput();
}
/*
void EcondBePreBuffer::connectTxPortTo(Connection< std::pair<uint32_t,RamWord> > &w) {  
  txRamWordPtr_=&w;
}
*/
void EcondBePreBuffer::connectTxPortTo(Connection<RamWord> &w) {  
  txRamWordPtrVector_.push_back(&w);
}

void EcondBePreBuffer::strobeTx() {
  for(unsigned i(0);i<txRamWordPtrVector_.size();i++) {
    txRamWordPtrVector_[i]->strobeInput();
  }
}

// Processing
void EcondBePreBuffer::process() {
  if(debugPrint_) {
    std::cout << "EcondBePreBuffer::process()  entering" << std::endl;
  }
  
  // Check connections are correct

  //assert(rxCommandsPtr_!=0);
  //assert(rxCommandsPtr_->size()==0);
  assert(rxArrayPtr_!=0);
  assert(rxArrayPtr_->size()==0);

  // Get the next pair of elink words
  assert(rxArrayPtr_->get(array_));
  assert(array_[0]<15);

  if(array_[0]<14) {
    if(debugPrint_) {
      std::cout << " Processing ECON-D " << array_[0] << std::endl;
    }
  
    // Transfer to buffers
    StateMachine &sm(stateMachine_[array_[0]]);
    sm.buffer_[(sm.bufferWrite_  )%4]=array_[1];
    sm.buffer_[(sm.bufferWrite_+1)%4]=array_[2];
    
    if(debugPrint_) {
      std::cout << " Buffer = " << std::hex
		<< sm.buffer_[0] << " " << sm.buffer_[1] << " "
		<< sm.buffer_[2] << " " << sm.buffer_[3]
		<< ", write = " << sm.bufferWrite_%4 << std::dec << std::endl;
    }
    
    // Process state machine
    numberPlusRamWord_.first=array_[0];
    numberPlusRamWord_.second.clearBits();

    if(debugPrint_) {
      std::cout << " State machine start: state_ = " << sm.state_ << std::endl;
    }
    
    if(sm.state_==StateMachine::HeaderB1) {
      sm.econdAsicHeader_.setWordTo(1,sm.buffer_[(sm.bufferWrite_  )%4]);
      sm.econdAsicHeader_.setWordTo(2,sm.buffer_[(sm.bufferWrite_+1)%4]);
      if(debugPrint_) sm.econdAsicHeader_.print("HeaderB1: ");
 
      numberPlusRamWord_.second.setStartBit();
      sm.bufferRead_=(sm.bufferWrite_+3)%4;

      sm.wordCount_=sm.econdAsicHeader_.packetWords()-2;

      if(debugPrint_) {
	std::cout << "HeaderB1: wordCount = " << sm.wordCount_ << std::endl;
      }
      
      sm.state_=StateMachine::PayloadB;

    } else if(sm.state_==StateMachine::HeaderA2) {
      sm.econdAsicHeader_.setWordTo(2,sm.buffer_[(sm.bufferWrite_  )%4]);

      if(debugPrint_) sm.econdAsicHeader_.print("HeaderA2: ");

      sm.bufferRead_=sm.bufferWrite_%4;

      sm.wordCount_=sm.econdAsicHeader_.packetWords()-4;

      if(debugPrint_) {
	std::cout << "HeaderA2: wordCount = " << sm.wordCount_ << std::endl;
      }
      
      sm.state_=StateMachine::PayloadA;

    } else if(sm.state_==StateMachine::PayloadA) {    
      sm.bufferRead_=(sm.bufferRead_+2)%4;

      sm.wordCount_-=2;
      if(debugPrint_) {
	std::cout << "PayloadA: wordCount = " << sm.wordCount_ << std::endl;
      }
      
      if(sm.wordCount_==1) sm.state_=StateMachine::EndAA;
      if(sm.wordCount_==2) sm.state_=StateMachine::EndAB;

    } else if(sm.state_==StateMachine::PayloadB) {    
      sm.bufferRead_=(sm.bufferRead_+2)%4;

      sm.wordCount_-=2;
      if(debugPrint_) {
	std::cout << "PayloadB: wordCount = " << sm.wordCount_ << std::endl;
      }
      
      if(sm.wordCount_==1) sm.state_=StateMachine::EndBA;
      if(sm.wordCount_==2) sm.state_=StateMachine::EndBB;

    } else if(sm.state_==StateMachine::EndAA) {
      sm.bufferRead_=(sm.bufferRead_+2)%4;

      numberPlusRamWord_.second.setEndBit();
      numberPlusRamWord_.second.setPaddingBit();

      if(debugPrint_) {
	std::cout << "EndAA: write, read = " << sm.bufferWrite_%4 << " " << sm.bufferRead_%4 << std::endl;
      }
      
      assert((sm.bufferWrite_%4)==(sm.bufferRead_%4));
      
      unsigned offset((sm.bufferWrite_%4)==(sm.bufferRead_%4)?1:2);
      if((sm.buffer_[(sm.bufferRead_+offset)%4]&0xffff)==EcondAsicHeader::HeaderPattern) {
	sm.econdAsicHeader_.setWordTo(0,sm.buffer_[(sm.bufferRead_+1)%4]);
	sm.state_=StateMachine::HeaderB1;
      }
      else sm.state_=StateMachine::Idle;

    } else if(sm.state_==StateMachine::EndAB) {
      sm.bufferRead_=(sm.bufferRead_+2)%4;

      numberPlusRamWord_.second.setEndBit();
    
      sm.state_=StateMachine::Idle;

    } else if(sm.state_==StateMachine::EndBA) {
      sm.bufferRead_=(sm.bufferRead_+2)%4;

      numberPlusRamWord_.second.setEndBit();
      numberPlusRamWord_.second.setPaddingBit();

      if(debugPrint_) {
	std::cout << "EndBA: write, read = " << sm.bufferWrite_%4 << " " << sm.bufferRead_%4 << std::endl;
      }
      
      assert((sm.bufferWrite_%4)!=(sm.bufferRead_%4));
      unsigned offset((sm.bufferWrite_%4)==(sm.bufferRead_%4)?1:2);
      
      if((sm.buffer_[(sm.bufferRead_+offset)%4]&0xffff)==EcondAsicHeader::HeaderPattern) {
	sm.econdAsicHeader_.setWordTo(0,sm.buffer_[(sm.bufferRead_+2)%4]);
	sm.state_=StateMachine::HeaderB1;
      }
      else sm.state_=StateMachine::Idle;

    } else if(sm.state_==StateMachine::EndBB) {
      sm.bufferRead_=(sm.bufferRead_+2)%4;

      numberPlusRamWord_.second.setEndBit();
    
      sm.state_=StateMachine::Idle;

    } else {
      assert(sm.state_==StateMachine::Idle);

      sm.bufferRead_=4;

      if((sm.buffer_[(sm.bufferWrite_  )%4]&0xffff)==EcondAsicHeader::HeaderPattern) {
	sm.econdAsicHeader_.setWordTo(0,sm.buffer_[(sm.bufferWrite_  )%4]);
	sm.econdAsicHeader_.setWordTo(1,sm.buffer_[(sm.bufferWrite_+1)%4]);
	
	numberPlusRamWord_.second.setStartBit();
	sm.bufferRead_=sm.bufferWrite_%4;

	sm.state_=StateMachine::HeaderA2;

      } else if((sm.buffer_[(sm.bufferWrite_+1)%4]&0xffff)==EcondAsicHeader::HeaderPattern) {
	sm.econdAsicHeader_.setWordTo(0,sm.buffer_[(sm.bufferWrite_+1)%4]);

	sm.state_=StateMachine::HeaderB1;
      }
    }

    if(debugPrint_) {
      std::cout << " Buffer read = " << sm.bufferRead_%8 << std::endl;
    }
    
    if(sm.bufferRead_<4) {
      if(numberPlusRamWord_.second.paddingBit()) { 
	numberPlusRamWord_.second.setWordTo(0,sm.buffer_[sm.bufferRead_]);
	numberPlusRamWord_.second.setWordTo(1,0);
      } else {
	numberPlusRamWord_.second.setWordTo(0,sm.buffer_[sm.bufferRead_]);
	numberPlusRamWord_.second.setWordTo(1,sm.buffer_[(sm.bufferRead_+1)%4]);
      }

      if(debugPrint_) {
	std::cout << " Sending, number = " << numberPlusRamWord_.first << " ";
	numberPlusRamWord_.second.print();
      }
      
      RamWord temp(numberPlusRamWord_.second);
      assert(txRamWordPtrVector_[numberPlusRamWord_.first]->set(temp));
    }
    
    sm.bufferWrite_+=2;
    if(debugPrint_) {
      std::cout << " State machine end: state_ = " << sm.state_ << std::endl;
    }
    
  } else {
    if(debugPrint_) {
      std::cout << " Processing bypassed" << std::endl;
    }
  }
  
  numberOfWords_++;
  numberOfPackets_++;

  if(debugPrint_) {
    std::cout << "EcondBePreBuffer::process()  exiting" << std::endl;
  }
}

void EcondBePreBuffer::print() const {
  std::cout << "EcondBePreBuffer::print()" << std::endl;
  
  std::cout << std::endl;
}
