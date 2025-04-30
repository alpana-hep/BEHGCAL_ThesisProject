#include <iostream>
#include <cassert>

#include "PacketChecker.h"
#include "LpgbtBuilder.h"
#include "LpgbtBuilderAnalyser.h"


LpgbtBuilderMiddleState::LpgbtBuilderMiddleState() :
  writeBuffer_(true),
  maxRamWordQueueSize_(0) {
}

LpgbtBuilder::LpgbtBuilder() :
  Entity("LpgbtBuilder",Clocks::Clock0280),
  rxCommandsPtr_(0),
  txRamWordPtr_(0),
  inputEcond_(0),
  middleEcond_(0),
  outputEcond_(0),
  singleLpgbtBuilderAnalyserPtr_(0),
  numberOfTicks_(0),
  numberOfInputRamWords_(0),
  numberOfInputPackets_(0),
  numberOfInputCheckedPackets_(0),
  numberOfOutputRamWords_(0),
  numberOfOutputPackets_(0),
  numberOfOutputCheckedPackets_(0) {

  nEcond_=0;
}

LpgbtBuilder::~LpgbtBuilder() {
}

void LpgbtBuilder::setDefinitionTo(const LpgbtPairDefinition &d) {
  definition_=d;
  d.econdAsicDefinitions(econdAsicDefinitionVector_);
  econdBePacket72Vector_.resize(econdAsicDefinitionVector_.size());
}

void LpgbtBuilder::enableAnalyser() {
  singleLpgbtBuilderAnalyserPtr_=new LpgbtBuilderAnalyser(this);
}

// Connections

void LpgbtBuilder::connect(FastControlDriver &d) {
  rxCommandsPtr_=new Connection<FastControlCommands>;
  d.connectTxPortTo(*rxCommandsPtr_);
}

void LpgbtBuilder::connect(EcondBeBuilder &) {
/*
  Connection<RamWord> *w(new Connection<RamWord>);
  connectRxPortTo(*w);
  e.connectTxPortTo(*w);

  // Create a main buffer also
  ramWordQueueVector_.push_back(std::queue<RamWord>());
  //basicWordQueueVector_.push_back(std::queue<BasicWord>());
*/
}

void LpgbtBuilder::connect(EcondBePreBuffer &e) {
  for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
    Connection<RamWord> *w(new Connection<RamWord>);
    connectRxPortTo(*w);
    e.connectTxPortTo(*w);
  }
}

// Inputs
void LpgbtBuilder::connectRxPortTo(Connection<RamWord> &w) {
  rxRamWordPtrVector_.push_back(&w);

  middleStateVector_.push_back(LpgbtBuilderMiddleState());
}

void LpgbtBuilder::strobeRx() {
  rxCommandsPtr_->strobeOutput();
  for(unsigned i(0);i<rxRamWordPtrVector_.size();i++) {
    rxRamWordPtrVector_[i]->strobeOutput();
  }
}

// Outputs
void LpgbtBuilder::connectTxPortTo(Connection<RamWord> &w) {
  txRamWordPtr_=&w;
}

void LpgbtBuilder::strobeTx() {
  if(txRamWordPtr_!=0) txRamWordPtr_->strobeInput();
}

// Processing
void LpgbtBuilder::process() {
  if(debugPrint_) {
    std::cout << "LpgbtBuilder::process()  entering" << std::endl;
  }

  assert(middleStateVector_.size()==rxRamWordPtrVector_.size());
  /*
  if(definition_.singleLpgbt()) {
    assert(rxSingleRamWordPtrVector_.size()==ramWordQueueVector_.size());
    assert(rxDoubleRamWordPtrVector_[0].size()==0);
    assert(rxDoubleRamWordPtrVector_[1].size()==0);

  } else {
    assert(rxSingleRamWordPtrVector_.size()==0);
    assert(rxDoubleRamWordPtrVector_[0].size()==ramWordQueueVector_.size());
    assert(rxDoubleRamWordPtrVector_[1].size()==ramWordQueueVector_.size());
  }
  */
  
  // 40 MHz commands section
  subProcess40Mhz();

  subProcessOutput();
  subProcessMiddle();
  subProcessInput();

  if(singleLpgbtBuilderAnalyserPtr_!=0) singleLpgbtBuilderAnalyserPtr_->analyse();

  numberOfTicks_++;
  
  if(debugPrint_) {
    std::cout << "LpgbtBuilder::process()  exiting" << std::endl;
  }
}

void LpgbtBuilder::subProcessInput() {
  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcessInput()  entering" << std::endl;
  }

  validInputs_=0;
  unsigned nValid(0);
  RamWord rw;

  bool done(false);
  for(unsigned i(0);i<rxRamWordPtrVector_.size() && !done;i++) {
    inputEcond_=(inputEcond_+1)%rxRamWordPtrVector_.size();
    if(rxRamWordPtrVector_[inputEcond_]->get(rw)) {
      done=true;

      numberOfInputRamWords_++;
      
      // PacketChecker
      if(true) {
	if(rw.startBit()) econdBePacket72Vector_[inputEcond_].resize(0);
	econdBePacket72Vector_[inputEcond_].push_back(rw);

	if(rw.endBit()) {
	  if(true) {
	    assert(PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[inputEcond_],middleStateVector_[inputEcond_].inputCountersQueue_.front(),econdBePacket72Vector_[inputEcond_],debugPrint_));
	    numberOfInputCheckedPackets_++;
	  }

	  middleStateVector_[inputEcond_].inputCountersQueue_.pop();
	  numberOfInputPackets_++;
	}
      }

      
      nValid++;
      validInputs_|=(1<<inputEcond_);

      
      middleStateVector_[inputEcond_].ramWordQueue_.push(rw);

      if(middleStateVector_[inputEcond_].maxRamWordQueueSize_<middleStateVector_[inputEcond_].ramWordQueue_.size()) {
	middleStateVector_[inputEcond_].maxRamWordQueueSize_=middleStateVector_[inputEcond_].ramWordQueue_.size();
      }
      
      if(debugPrint_) {
	std::cout << " ECON-D " << inputEcond_ << " input word = ";rw.print();
      }
    } else {
      if(debugPrint_) {
	std::cout << " ECON-D " << inputEcond_ << " no input word" << std::endl;
      }
    }
  }

  if(!done) {
    if(debugPrint_) {
      std::cout << " No ECON-D input words" << std::endl;
    }
  }

  // At most one ECON-D BE builder should send a word
  //assert(nValid<=1);

  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcessInput()  exiting" << std::endl;
  }
}

void LpgbtBuilder::subProcessMiddle() {
  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcessMiddle()  entering" << std::endl;
  }

  assert(middleEcond_<middleStateVector_.size());
  
  // Check if word to read from main buffer
  if(middleStateVector_[middleEcond_].writeBuffer_) {

    std::queue<RamWord> &rwq(middleStateVector_[middleEcond_].ramWordQueue_);
    
    if(!rwq.empty()) {
    RamWord rw(rwq.front());
    rwq.pop();
    
    if(debugPrint_) {
      std::cout << " ECON-D " << middleEcond_ << ", main buffer middle word ";
      rw.print();
    }

    std::queue<RamWord> &eb(middleStateVector_[middleEcond_].eventBuffer_);
    
    if(rw.startBit()) {
      EcondBeHeader ebh;
      ebh.setCountersTo(middleStateVector_[middleEcond_].middleCountersQueue_.front());
      middleStateVector_[middleEcond_].middleCountersQueue_.pop();
 
      //econdBeTrailerVector_[middleEcond_].setPacketLengthTo(0);
      //econdBeTrailerVector_[middleEcond_].setErrorBitsTo(0);
      middleStateVector_[middleEcond_].econdBeTrailer_.setPacketLengthTo(0);
      middleStateVector_[middleEcond_].econdBeTrailer_.setErrorBitsTo(0);

      //assert(eventBufferVector_[middleEcond_].size()==0);
      //eventBufferVector_[middleEcond_].push(RamWord());
      //eventBufferVector_[middleEcond_].front().clearBits();
      //eventBufferVector_[middleEcond_].front().setStartBitTo(true);
      //eventBufferVector_[middleEcond_].front().setWordTo(0,ebh.word(0));
      //eventBufferVector_[middleEcond_].front().setWordTo(1,ebh.word(1));
      
      assert(eb.size()==0);
      eb.push(RamWord());
      eb.front().clearBits();
      eb.front().setStartBitTo(true);
      eb.front().setWordTo(0,ebh.word(0));
      eb.front().setWordTo(1,ebh.word(1));
      
      rw.setStartBitTo(false);
      //eb.push(rw);
      eb.push(rw);
      
    } else if(rw.endBit()) {
      
      if(rw.paddingBit()) {      
	rw.setPaddingBitTo(false);
	//rw.setWordTo(1,econdBeTrailerVector_[middleEcond_].word());
	rw.setWordTo(1,middleStateVector_[middleEcond_].econdBeTrailer_.word());
	eb.push(rw);

	EcondBeHeader ebh;
	ebh.setWordTo(0,eb.front().word(0));
	ebh.setPacketLengthTo(4*eb.size()-6);
	eb.front().setWordTo(0,ebh.word(0));
	
      } else {
	rw.setEndBitTo(false);
	eb.push(rw);

	rw.clearBits();
	rw.setEndBitTo(true);
	rw.setPaddingBitTo(true);
	//rw.setWordTo(0,econdBeTrailerVector_[middleEcond_].word());
	rw.setWordTo(0,middleStateVector_[middleEcond_].econdBeTrailer_.word());
	rw.setWordTo(1,0);
	eb.push(rw);
      
	EcondBeHeader ebh;
	ebh.setWordTo(0,eb.front().word(0));
	ebh.setPacketLengthTo(4*eb.size()-8);
	eb.front().setWordTo(0,ebh.word(0));
      }
      
      if(debugPrint_) {
	std::cout << " Middle ECON-D " << middleEcond_ << ", completed event" << std::endl;
	//for(unsigned i(0);i<eb.size();i++) {
	// std::cout << "  ";
	// eb[i].print();
	//}
	//std::cout << std::endl;
      }

      middleStateVector_[middleEcond_].writeBuffer_=false;
      middleEcond_=(middleEcond_+1)%middleStateVector_.size();
      
    } else {
      eb.push(rw);
      
      //econdBeTrailerVector_[middleEcond_].setErrorBitsTo(0xff); // TEST!!!
      middleStateVector_[middleEcond_].econdBeTrailer_.setErrorBitsTo(0xff); // TEST!!!
    }
  }
  }

  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcessMiddle()  exiting" << std::endl;
  }
}

void LpgbtBuilder::subProcessOutput() {
  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcessOutput()  entering" << std::endl;
  }

  // Check if room in output buffer
  if(txRamWordPtr_==0 || txRamWordPtr_->size()<1024) { // BRAM size
    
    assert(outputEcond_<middleStateVector_.size());
    
    // Check if word to read from event buffer
    if(!middleStateVector_[outputEcond_].writeBuffer_) {
      
      std::queue<RamWord> &eb(middleStateVector_[outputEcond_].eventBuffer_);
      
      if(!eb.empty()) {
	RamWord rw(eb.front());
	eb.pop();
	
	if(debugPrint_) {
	  std::cout << " ECON-D " << outputEcond_ << ", main buffer output word ";
	  rw.print();
	}

	if(outputEcond_!=0 && rw.startBit()) rw.setStartBitTo(false);

	if(rw.endBit()) {
	  middleStateVector_[outputEcond_].writeBuffer_=true;
	  outputEcond_=(outputEcond_+1)%middleStateVector_.size();
	}

	if(outputEcond_!=0 && rw.endBit()) rw.setEndBitTo(false);

	if(debugCheck_ || true) lpgbtPacket_.push_back(rw);

	// Checking
	if(rw.endBit()) {
	  if(debugCheck_ || true) {
	    assert(PacketChecker::checkLpgbtPairPacket(definition_,countersQueue_.front(),lpgbtPacket_,debugPrint_));
	    lpgbtPacket_.resize(0);
	    numberOfOutputCheckedPackets_++;
	  }

	  countersQueue_.pop();
	  numberOfOutputPackets_++;
	}
	
	/*      
		if(basicWordQueue_.size()>=2) {
		RamWord rw;
		BasicWord bw[2];
		
		bw[0]=basicWordQueue_.front();
		basicWordQueue_.pop();      
		
		if(bw[0].endBit()) {
		rw.setWordTo(0,bw[0].word());
		rw.setWordTo(1,0);
		rw.setEndBit();
		rw.setPaddingBit();
		
		} else {
		bw[1]=basicWordQueue_.front();
		basicWordQueue_.pop();      
		
		rw.setWordTo(0,bw[0].word());
		rw.setWordTo(1,bw[1].word());
		if(bw[0].startBit()) rw.setStartBit();
		if(bw[1].endBit()) rw.setEndBit();
		}
		
		// Packet checker
		if(rw.startBit()) {
		lpgbtBuilderPacket72_.resize(0);
		}
		
		
		if(rw.endBit()) {
		//assert(PacketChecker::checkLpgbtBuilderPacket72(lpgbtBuilderPacket72_));
		}
		}
	*/
	
	// Write to secondary buffer
	//if(txRamWordPtr_!=0)
	assert(txRamWordPtr_->set(rw));
	numberOfOutputRamWords_++;		
      }
    }
  }
  
  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcessOutput()  exiting" << std::endl;
  }
}

void LpgbtBuilder::subProcess40Mhz() {
  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcess40Mhz()  entering" << std::endl;
  }

  if(rxCommandsPtr_->get(commands_)) { // Only after a 40 MHz tick

    // Update counters
    counters_.update(commands_);

    // Check for L1A
    if(commands_.l1a()) {

      for(unsigned i(0);i<middleStateVector_.size();i++) {
	middleStateVector_[i].inputCountersQueue_.push(counters_);
	middleStateVector_[i].middleCountersQueue_.push(counters_);

	if(debugPrint_) {
	  std::cout << " New LpgbtBuilder middle L1A added, queue size = "
		    << middleStateVector_[i].inputCountersQueue_.size() << ", ";
	  middleStateVector_[i].inputCountersQueue_.back().print();
	}
      }
      
      countersQueue_.push(counters_);
      
      if(debugPrint_) {
        std::cout << " New LpgbtBuilder L1A added, queue size = "
		  << countersQueue_.size() << ", ";
        countersQueue_.back().print();
      }
    }
  }
  
  if(debugPrint_) {
    std::cout << " LpgbtBuilder::subProcess40Mhz()  exiting" << std::endl;
  }
}

void LpgbtBuilder::print() const {
  std::cout << "LpgbtBuilder::print() " << definition_.label() << std::endl
              << " Number of ticks = " << numberOfTicks_ << std::endl
            << " Number of input RAM words = " << numberOfInputRamWords_
            << ", duty cycle = " << 100*numberOfInputRamWords_/numberOfTicks_ << " %" << std::endl
            << " Number of input packets = " << numberOfInputPackets_;
  if(numberOfInputPackets_>0) {
    std::cout << ", average size = " << 1.0*numberOfInputRamWords_/numberOfInputPackets_ << " RAM words = "
              << 0.064*numberOfInputRamWords_/numberOfInputPackets_ << " kbit";
  }
  std::cout << std::endl
            << " Number of input checked packets = " << numberOfInputCheckedPackets_ << std::endl
            << " Number of output RAM words = " << numberOfOutputRamWords_
            << ", duty cycle = " << 100*numberOfOutputRamWords_/numberOfTicks_ << " %" << std::endl
            << " Number of output packets = " << numberOfOutputPackets_;
  if(numberOfOutputPackets_>0) {
    std::cout << ", average size = " << 1.0*numberOfOutputRamWords_/numberOfOutputPackets_ << " RAM words = "
              << 0.064*numberOfOutputRamWords_/numberOfOutputPackets_ << " kbit";
  }
  std::cout << std::endl
            << " Number of output checked packets = " << numberOfOutputCheckedPackets_ << std::endl;
  
  for(unsigned i(0);i<middleStateVector_.size();i++) {
    std::cout << " ECON-D " << i << ", max size of main buffer = " << middleStateVector_[i].maxRamWordQueueSize_ << std::endl;
  }
  std::cout << std::endl;

}
