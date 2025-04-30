#include <iostream>
#include <iomanip>
#include <cassert>

#include "Utils.h"
#include "GlobalConfiguration.h"
#include "PacketChecker.h"
#include "SlinkBuilder.h"
#include "SlinkBuilderAnalyser.h"

// 8 BRAMs, each with 512 72-bit words
const unsigned SlinkBuilder::eventBufferSize_(8*512);
//const unsigned SlinkBuilder::eventBufferSize_(16*8*512); // TEMP!!!

SlinkBuilder::SlinkBuilder() :
  //Entity("SlinkBuilder",Clocks::Clock0280),
  //EntityFastControl("SlinkBuilder",GlobalConfiguration::configuration().slinkBuilderClockFrequency()),
  EntityFastControl("SlinkBuilder",Clocks::Clock0180),
  txSlinkWordPtr_(0),

  activeEvent_(false),
  activeOutput_(false),
  slinkWordReady_(0),
  slinkFlag_(false),
  slinkBuilderAnalyserPtr_(0),

  numberOfTicks_(0),
  numberOf40MhzTicks_(0),
  numberOf360MhzTicks_(0),
  
  numberOfInputRamWords_(0),
  numberOfInputBasicWords_(0),
  numberOfInputPaddingWords_(0),
  numberOfInputNoDataTicks_(0),
  numberOfInputBackpressureTicks_(0),
  numberOfInputStartedPackets_(0),
  numberOfInputFinishedPackets_(0),
  numberOfInputPackets_(0),
  numberOfInputCheckedPackets_(0),

  numberOfBuilderBackpressureTicks_(0),
  
  maxOfOutputSlinkWordQueueSize_(0),
  numberOfOutputBackpressureTicks_(0),
  numberOfOutputDataTicks_(0),
  numberOfOutputNoDataTicks_(0),
  numberOfOutputSlinkWords_(0),
  numberOfOutputPackets_(0),
  numberOfOutputStartedPackets_(0),
  numberOfOutputFinishedPackets_(0),
  numberOfOutputCheckedPackets_(0),
  sizeOfOutputBuffer_(0),
  maxOfOutputBuffer_(0) {
}

SlinkBuilder::~SlinkBuilder() {
}

// Initialisation
void SlinkBuilder::setDefinitionTo(const SlinkDefinition &d) {
  definition_=d;
  //lpgbtPairDefinitionVector_=d.lpgbtPairDefinitions();
  numberOfLpgbtPairs_=d.lpgbtPairDefinitions().size();

  activeInputVector_.resize(numberOfLpgbtPairs_);
  inputL1aCountersQueueVector_.resize(numberOfLpgbtPairs_);

  numberOfInputRamWordsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputRamWordsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputBasicWordsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputPaddingWordsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputNoDataTicksVector_.resize(numberOfLpgbtPairs_);
  numberOfInputBackpressureTicksVector_.resize(numberOfLpgbtPairs_);

  numberOfInputStartedPacketsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputFinishedPacketsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputPacketsVector_.resize(numberOfLpgbtPairs_);
  numberOfInputCheckedPacketsVector_.resize(numberOfLpgbtPairs_);
  sizeOfInputBufferVector_.resize(numberOfLpgbtPairs_);
  maxOfInputBufferVector_.resize(numberOfLpgbtPairs_);

  inputRamWordQueueVector_.resize(numberOfLpgbtPairs_);
  
  for(unsigned i(0);i<numberOfLpgbtPairs_;i++) {
    activeInputVector_[i]=false;
    
    numberOfInputRamWordsVector_[i]=0;
    numberOfInputBasicWordsVector_[i]=0;
    numberOfInputPaddingWordsVector_[i]=0;
    numberOfInputNoDataTicksVector_[i]=0;
    numberOfInputBackpressureTicksVector_[i]=0;
    numberOfInputStartedPacketsVector_[i]=0;
    numberOfInputFinishedPacketsVector_[i]=0;
    numberOfInputPacketsVector_[i]=0;
    numberOfInputCheckedPacketsVector_[i]=0;
    
    sizeOfInputBufferVector_[i]=0;
    maxOfInputBufferVector_[i]=0;
  }
  
  lpgbtPairPacketVector_.resize(numberOfLpgbtPairs_);
}

void SlinkBuilder::setArgumentsTo(const Arguments &a) {
  configuration_.setBufferSizeTo(a.config().at("slinkBufferSize").get<unsigned>());
  Entity::setArgumentsTo(a);
  if(idNumber_==a.config().at("entityIdDebugPrint").get<uint32_t>()) debugPrint_=true;
}

void SlinkBuilder::initialise() {
  std::cout << name_ << "::initialise() " << definition_.label() << std::endl;
  definition_.print(" ");
  configuration_.print(" ");
  std::cout << std::endl;
  
  //assert(rxCommandsPtr_!=0);

  assert(rxRamWordPtrVector_.size()               ==numberOfLpgbtPairs_);
  assert(numberOfInputRamWordsVector_.size()      ==numberOfLpgbtPairs_);
  assert(numberOfInputPacketsVector_.size()       ==numberOfLpgbtPairs_);
  assert(numberOfInputCheckedPacketsVector_.size()==numberOfLpgbtPairs_);
  assert(lpgbtPairPacketVector_.size()            ==numberOfLpgbtPairs_);

  for(unsigned i(0);i<rxRamWordPtrVector_.size();i++) {
    std::cout << " Input buffer " << i << " maximum size = "
	      << rxRamWordPtrVector_[i]->maxSize() << std::endl;
  }

  if(txSlinkWordPtr_==0) std::cout << " Output buffer disabled" << std::endl;
  else {
    std::cout << " Output buffer maximum size = "
	      << txSlinkWordPtr_->maxSize() << std::endl;
  }
  std::cout << std::endl;
  
  slinkBuilderAnalyserPtr_=new SlinkBuilderAnalyser(this);
  slinkBuilderAnalyserPtr_->initialise(doAnalysis_);
}

// Connections
void SlinkBuilder::connect(FastControlDriver &d) {

  // No initial value needed as FastControlDriver runs first
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  EntityFastControl::connectRxPortTo(*c);
  d.connectTxPortTo(*c);
}

void SlinkBuilder::connect(LpgbtPair &l) {
  EventBuffer *w(new EventBuffer());
  w->setMaxSizeTo(eventBufferSize_);
  connectRxPortTo(*w);
  l.connectTxPortTo(*w);
}

// Inputs
/*
  void SlinkBuilder::connectRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_=&c;
  }
*/
void SlinkBuilder::connectRxPortTo(Connection<RamWord> &w) {
  rxRamWordPtrVector_.push_back(&w);
}

void SlinkBuilder::strobeRx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeRx();

  //if(rxCommandsPtr_!=0) rxCommandsPtr_->strobeOutput();

  for(unsigned i(0);i<rxRamWordPtrVector_.size();i++) {
    rxRamWordPtrVector_[i]->strobeOutput();
  }
}

// Output
void SlinkBuilder::connectTxPortTo(Connection< std::pair<bool,SlinkWord> > &w) {
  txSlinkWordPtr_=&w;
  w.setMaxSizeTo(1);
}

void SlinkBuilder::strobeTx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeTx();

  if(txSlinkWordPtr_!=0) txSlinkWordPtr_->strobeInput();
}

// Processing
void SlinkBuilder::process() {
  //if(Clocks::clocks().bxNumber()>=101586) debugPrint_=true;

  if(debugPrint_) {
    std::cout << name_ << "::process()  entering" << std::endl;
  }

  // Check input connections
  //assert(rxCommandsPtr_->size()==0);

  // Check buffers
  for(unsigned i(0);i<numberOfLpgbtPairs_;i++) {
    unsigned s(rxRamWordPtrVector_[i]->size());
    sizeOfInputBufferVector_[i]+=s;
    if(maxOfInputBufferVector_[i]<s) maxOfInputBufferVector_[i]=s;
    
    if(debugPrint_) {
      std::cout << "  Input buffer " << i << " size = " << s << std::endl;
    }
  }

  if(txSlinkWordPtr_!=0) {
    unsigned s(txSlinkWordPtr_->size());
    sizeOfOutputBuffer_+=s;
    if(maxOfOutputBuffer_<s) maxOfOutputBuffer_=s;
    
    if(debugPrint_) {
      std::cout << "  Output buffer size = " << s << std::endl;
    }
  }

  // Check for 40 MHz tick since last processing
  if(Clocks::clocks().previousClock0040()) {
    
    // 40 MHz processing
    subProcess40Mhz(debugCheck_);
    numberOf40MhzTicks_++;

    // Check for L1A
    if(commands_.l1a()) {

      // Create BOE and add to the queue
      uint16_t et(0x0001); // Physics

      boeQueue_.push(SlinkBoe(definition_.sourceId(),counters_,et));
      if(debugPrint_) {
	std::cout << " New Slink L1A added, queue size = " << boeQueue_.size() << " ";
	boeQueue_.back().print();
	counters_.print();
      }
			
      if(debugCheck_) {
	for(unsigned i(0);i<numberOfLpgbtPairs_;i++) {
	  inputL1aCountersQueueVector_[i].push(counters_);
	}
      }
    }
  }

  // 360 MHz input read "gearbox"; two reads per tick
  subProcessInput();
  for(unsigned i(0);i<rxRamWordPtrVector_.size();i++) {
    rxRamWordPtrVector_[i]->strobeOutput();
  }
  subProcessInput();

  // 180 MHz builder and output
  subProcessBuilder();
  subProcessOutput();

  // Analyse results
  if(slinkBuilderAnalyserPtr_!=0) slinkBuilderAnalyserPtr_->analyse();
  
  numberOfTicks_++;

  if(debugPrint_) {
    std::cout << name_ << "::process()  exiting" << std::endl;
  }
}
  
void SlinkBuilder::subProcessInput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()  entering" << std::endl;
  }

  numberOf360MhzTicks_++;
  
  RamWord rw;
  BasicWord bw;
  
  for(unsigned i(0);i<numberOfLpgbtPairs_;i++) {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessInput()"
		<< " Input " << i << " initial queue size = "
		<< inputRamWordQueueVector_[i].size()
		<< std::endl;
    }
    
    if(inputRamWordQueueVector_[i].size()<4) {
      if(rxRamWordPtrVector_[i]->get(rw)) {

	// Packet checking
	if(debugCheck_) inputPacketCheck(i,rw);
	  
	numberOfInputRamWords_++;
	numberOfInputRamWordsVector_[i]++;

	if(!activeInputVector_[i]) {
	  if(!rw.startBit()) {
	    printBxTick();
	    assert(false);
	  }
	  activeInputVector_[i]=true;
	  
	  numberOfInputStartedPackets_++;
	  numberOfInputStartedPacketsVector_[i]++;
	}
	
	if(rw.endBit()) {
	  if(!activeInputVector_[i]) {
	    printBxTick();
	    assert(false);
	  }
	  activeInputVector_[i]=false;
	  
	  numberOfInputFinishedPackets_++;
	  numberOfInputFinishedPacketsVector_[i]++;
	}

	// Put word into buffer
	//inputRamWordQueueVector_[i].push(rw);

	// Put first word into buffer
	bw.clearBits();
	bw.setStartBitTo(rw.startBit());
	if(rw.paddingBit()) bw.setEndBitTo(rw.endBit());
	bw.setWordTo(rw.word(0));
	inputRamWordQueueVector_[i].push(bw);
	
	numberOfInputBasicWords_++;
	numberOfInputBasicWordsVector_[i]++;

	// Put second word into buffer unless it is padding
	if(!rw.paddingBit()) {
	  bw.clearBits();
	  bw.setEndBitTo(rw.endBit());
	  bw.setWordTo(rw.word(1));
	  inputRamWordQueueVector_[i].push(bw);

	  numberOfInputBasicWords_++;
	  numberOfInputBasicWordsVector_[i]++;

	} else {
	  numberOfInputPaddingWords_++;
	  numberOfInputPaddingWordsVector_[i]++;
	}

	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessInput()"
		    << " Input " << i << " data"
		    << std::endl;
	}

      } else {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessInput()"
		    << " Input " << i << " no data"
		    << std::endl;
	}

	numberOfInputNoDataTicks_++;
	numberOfInputNoDataTicksVector_[i]++;
      }

    } else {
      numberOfInputBackpressureTicks_++;
      numberOfInputBackpressureTicksVector_[i]++;
    }

    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessInput()"
		<< " Input " << i << "  final queue size = "
		<< inputRamWordQueueVector_[i].size()
		<< std::endl;
    }
  }
    
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()  exiting" << std::endl;
  }
}

void SlinkBuilder::subProcessBuilder() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilder()  entering" << std::endl;
  }

  // Check if output buffer is full
  if(outputSlinkWordQueue_.size()<configuration_.bufferSize()) {

    if(!activeEvent_) {
      if(debugPrint_) std::cout << " Inactive event" << std::endl;
      
      // Wait until current word is sent
      //if(slinkWordReady_==0) {
      assert(slinkWordReady_==0);
	
      // Check for an L1A waiting in queue and if so, start new packet with BOE
      if(!boeQueue_.empty()) {
	boe_=boeQueue_.front();
	boeQueue_.pop();
	
	// Reset the EOE to accumulate the packet length
	eoe_.initialise();
	eoe_.setFedUserIdTo(definition_.sourceId());
	eoe_.setStatusTo(0);
	eoe_.setCrcTo(0);
	eoe_.setEventLengthTo(2); // BOE and EOE

	eoeCrc_=boe_.crcIncrement();
	
	activeEvent_=true;
	nLpgbtPair_=0;
	slinkFlag_=true;
	slinkWord_[0]=boe_;
	slinkWordReady_=4;
	
	if(debugPrint_) {
	  std::cout << " Inactive and L1A in queue" << std::endl;
	  boe_.print("  ");
	}
	
      } else {
	if(debugPrint_) {
	  std::cout << " Inactive and no L1A in queue" << std::endl;
	}
      }
      /*
	} else {
	if(debugPrint_) {
	std::cout << " Inactive but Slink word ready = " << slinkWordReady_ << std::endl;
	}
      */
    
    } else {
      if(debugPrint_) std::cout << " Active event" << std::endl;

      // Check if all lpGBTs have been read and if so, end packet with EOE
      if(nLpgbtPair_>=rxRamWordPtrVector_.size()) {
	
	// Wait until current word is sent
	//if(slinkWordReady_==0) {
	assert(slinkWordReady_==0);
	activeEvent_=false;
	
	eoeCrc_+=eoe_.crcIncrement();
	eoe_.setCrcTo(eoeCrc_);
	
	slinkWord_[0]=eoe_;
	slinkFlag_=true;
	slinkWordReady_=4;

	if(debugPrint_) {
	  std::cout << " Finished L1A, ";eoe_.print();
	}
	numberOfOutputPackets_++;
	/*	
		} else if(slinkWordReady_<4) {
		if(debugPrint_) std::cout << " Adding padding" << std::endl;
	
		while(slinkWordReady_<4) {
		slinkWord_[slinkWordReady_/4].setWordTo(slinkWordReady_%4,0);
		slinkWordReady_++;
		}
		slinkFlag_=false;
	
		} else {
		assert(slinkWordReady_==4);
		}
	*/
	
      } else {
	if(debugPrint_) std::cout << " Getting packet body" << std::endl;

	slinkFlag_=false;
	
	bool stalled(false);
	//if(slinkWordReady_<4) {
	while(slinkWordReady_<4 && !stalled) {
	  if(debugPrint_) std::cout << " Input RAM word queue "
				    << nLpgbtPair_ << " = "
				    << inputRamWordQueueVector_[nLpgbtPair_].size()
				    << std::endl;

	  BasicWord bw;
	  if(inputRamWordQueueVector_[nLpgbtPair_].size()>0) {
	    bw=inputRamWordQueueVector_[nLpgbtPair_].front();
	    inputRamWordQueueVector_[nLpgbtPair_].pop();
	    
	    if(debugPrint_) {
	      std::cout << " Lpgbt " << nLpgbtPair_ << ", new word ";bw.print();
	    }

	    // Input packet checking
	    //if(debugCheck_) inputPacketCheck(nLpgbtPair_,rw);
	  
	    //numberOfInputRamWords_++;
	    //numberOfInputRamWordsVector_[nLpgbtPair_]++;

	    slinkWord_[slinkWordReady_/4].setWordTo(slinkWordReady_%4,bw.word());
	    slinkWordReady_++;
	  
	    // If end of lpGBT packet, move to next lpGBT
	    if(bw.endBit()) {
	      //numberOfInputPackets_++;
	      //numberOfInputPacketsVector_[nLpgbtPair_]++;
	      nLpgbtPair_++;
	    }

	    // If end of last packet, add padding
	    if(nLpgbtPair_>=rxRamWordPtrVector_.size()) {
	      while(slinkWordReady_<4) {
		slinkWord_[slinkWordReady_/4].setWordTo(slinkWordReady_%4,0);
		slinkWordReady_++;
	      }
	    }
	    
	  } else {
	    if(debugPrint_) {
	      std::cout << " Lpgbt " << nLpgbtPair_ << ", no word received" << std::endl;
	    }
	    stalled=true;
	  }
	}
      }
    }

    // Send only on odd ticks
    //assert(slinkWordReady_<8);
    assert(slinkWordReady_<=4);

    // Check word is ready
    if(slinkWordReady_==4) {
    
      // Send the word and reset the ready flag
      std::pair<bool,SlinkWord> bsw(slinkFlag_,slinkWord_[0]);
      /*
      //if(txSlinkWordPtr_!=0 && !(txSlinkWordPtr_->set(bsw))) {
      if(outputSlinkWordQueue_.size()>=1024) {
	if(debugPrint_) {
	  std::cout << " Output slink set failed, buffer size = "
	    //<< txSlinkWordPtr_->size() << std::endl;
		    << outputSlinkWordQueue_.size() << std::endl;
	}
      } else {
      */
	outputSlinkWordQueue_.push(bsw);
      
	//numberOfOutputSlinkWords_++;
      
	if(!slinkFlag_) {
	  eoeCrc_+=slinkWord_[0].crcIncrement();
	  eoe_.incrementEventLength();
	}
      
	slinkWordReady_=0;
	//slinkWord_[0]=slinkWord_[1];
      
	if(debugPrint_) {
	  std::cout << " Slink sent, start/end flag " << (bsw.first?" true":"false") << " ";
	  bsw.second.print();
	}
	//}
    }

  } else {
    if(debugPrint_) {
      std::cout << " " << name_ << "::subProcessBuilder()"
		<< " Output buffer full = "
		<< outputSlinkWordQueue_.size()	
		<< "; no processing" << std::endl;
    }
    
    numberOfBuilderBackpressureTicks_++;
  }

  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilder()  exiting" << std::endl;
  }
}


void SlinkBuilder::subProcessBuilderOld() {
  if(debugPrint_) {
    std::cout << " SlinkBuilder::subProcessBuilder()  entering" << std::endl;
  }
  
  // 180 MHz output section
  
  SlinkWord sw;
  if(!activeEvent_) {
    if(debugPrint_) std::cout << " Inactive event" << std::endl;

    // Wait until current word is sent
    if(slinkWordReady_==0) {
    
      // Check for an L1A waiting in queue and if so, start new packet with BOE
      if(!boeQueue_.empty()) {
	boe_=boeQueue_.front();
	boeQueue_.pop();
	
	// Reset the EOE to accumulate the packet length
	eoe_.initialise();
	eoe_.setFedUserIdTo(definition_.sourceId());
	eoe_.setStatusTo(0);
	eoe_.setCrcTo(0);
	eoe_.setEventLengthTo(2); // BOE and EOE

	eoeCrc_=boe_.crcIncrement();
	
	activeEvent_=true;
	nLpgbtPair_=0;
	slinkFlag_=true;
	slinkWord_[0]=boe_;
	slinkWordReady_=4;
	
	if(debugPrint_) {
	  std::cout << " Starting to look for new L1A, ";boe_.print();      
	}
	
      } else {
	if(debugPrint_) {
	  std::cout << " Inactive and no L1A in queue" << std::endl;
	}
      }
    } else {
      if(debugPrint_) {
	std::cout << " Inactive but Slink word ready = " << slinkWordReady_ << std::endl;
      }
    }
    
  } else {
    if(debugPrint_) std::cout << " Active event" << std::endl;

    // Check if all lpGBTs have been read and if so, end packet with EOE
    if(nLpgbtPair_>=rxRamWordPtrVector_.size()) {

      // Wait until current word is sent
      if(slinkWordReady_==0) {
	activeEvent_=false;

	eoeCrc_+=eoe_.crcIncrement();
	eoe_.setCrcTo(eoeCrc_);
	
	slinkWord_[0]=eoe_;
	slinkFlag_=true;
	slinkWordReady_=4;

	if(debugPrint_) {
	  std::cout << " Finished L1A, ";eoe_.print();
	}
	numberOfOutputPackets_++;
	
      } else if(slinkWordReady_<4) {
	if(debugPrint_) std::cout << " Adding padding" << std::endl;
	
	while(slinkWordReady_<4) {
	  slinkWord_[slinkWordReady_/4].setWordTo(slinkWordReady_%4,0);
	  slinkWordReady_++;
	}
	slinkFlag_=false;
	
      } else {
	assert(slinkWordReady_==4);
      }
      
    } else {
      if(debugPrint_) std::cout << " Getting packet body" << std::endl;

      if(slinkWordReady_<4) {

	// Read from current lpGBT
	//assert(rxRamWordPtrVector_[nLpgbtPair_]->size()<=eventBufferSize_); // Size of BRAM (using Connection to implement it)

	RamWord rw;
	//if(rxRamWordPtrVector_[nLpgbtPair_]->get(rw)) {
	if(inputRamWordQueueVector_[nLpgbtPair_].size()>0) {
	  //rw=inputRamWordQueueVector_[nLpgbtPair_].front(); // TEMP TO COMPILE!
	  inputRamWordQueueVector_[nLpgbtPair_].pop();
	  
	  if(debugPrint_) {
	    std::cout << " Lpgbt " << nLpgbtPair_ << ", new word ";rw.print();
	  }

	  // Input packet checking
	  //if(debugCheck_) inputPacketCheck(nLpgbtPair_,rw);
	  
	  //numberOfInputRamWords_++;
	  //numberOfInputRamWordsVector_[nLpgbtPair_]++;

	  slinkWord_[slinkWordReady_/4].setWordTo(slinkWordReady_%4,rw.word(0));
	  slinkWordReady_++;
	  if(!rw.paddingBit()) {
	    slinkWord_[slinkWordReady_/4].setWordTo(slinkWordReady_%4,rw.word(1));
	    slinkWordReady_++;
	  }
	  slinkFlag_=false;
	  
	  // If end of lpGBT packet, move to next lpGBT
	  if(rw.endBit()) {
	    numberOfInputPackets_++;
	    numberOfInputPacketsVector_[nLpgbtPair_]++;
	    nLpgbtPair_++;
	  }
	  
	} else {
	  if(debugPrint_) {
	    std::cout << " Lpgbt " << nLpgbtPair_ << ", no word received" << std::endl;
	  }
	}
      }
    }
  }

  // Send only on odd ticks
  assert(slinkWordReady_<8);

  // Check word is ready
  if(slinkWordReady_>=4) {
    
    // Send the word and reset the ready flag
    std::pair<bool,SlinkWord> bsw(slinkFlag_,slinkWord_[0]);
    //if(txSlinkWordPtr_!=0 && !(txSlinkWordPtr_->set(bsw))) {
    if(outputSlinkWordQueue_.size()>=1024) {
      if(debugPrint_) {
	std::cout << " Output slink set failed, buffer size = "
	  //<< txSlinkWordPtr_->size() << std::endl;
		  << outputSlinkWordQueue_.size() << std::endl;
      }
    } else {
      outputSlinkWordQueue_.push(bsw);
      
      //numberOfOutputSlinkWords_++;
      
      if(!slinkFlag_) {
	eoeCrc_+=slinkWord_[0].crcIncrement();
	eoe_.incrementEventLength();
      }
      
      slinkWordReady_-=4;
      slinkWord_[0]=slinkWord_[1];
      
      if(debugPrint_) {
	std::cout << " Slink sent, start/end flag " << (bsw.first?" true":"false") << " ";
	bsw.second.print();
      }
    }
  }

  if(debugPrint_) {
    std::cout << name_ << "::subProcessBuilder()  exiting" << std::endl;
  }
}


void SlinkBuilder::subProcessOutput() {
  if(debugPrint_) {
    std::cout << " SlinkBuilder::subProcessOutput()  entering" << std::endl;
  }

  if(maxOfOutputSlinkWordQueueSize_<outputSlinkWordQueue_.size())
    maxOfOutputSlinkWordQueueSize_=outputSlinkWordQueue_.size();
  
  if(outputSlinkWordQueue_.size()>0) {

    bool sent(false);
    if(txSlinkWordPtr_==0) {
      sent=true;
      
    } else {
      if(txSlinkWordPtr_->full()) {
	numberOfOutputBackpressureTicks_++;
	
      } else {
	sent=true;

	if(!txSlinkWordPtr_->set(outputSlinkWordQueue_.front())) {
	  printBxTick();
	  assert(false);
	}
      }
    }

    if(sent) {
      numberOfOutputDataTicks_++;
	
      // Output packet checking
      if(debugCheck_) outputPacketCheck(outputSlinkWordQueue_.front());      
    
      // Statistics
      numberOfOutputSlinkWords_++;

      // Check for start or end of packet
      if(outputSlinkWordQueue_.front().first) {
	activeOutput_=!activeOutput_;

	if(activeOutput_) numberOfOutputStartedPackets_++;
	else              numberOfOutputFinishedPackets_++;
      }
      
      // Remove word from queue
      outputSlinkWordQueue_.pop();
    }

  } else {
    numberOfOutputNoDataTicks_++;
  }
    
  if(debugPrint_) {
    std::cout << name_ << "::subProcessOutput()  exiting" << std::endl;
  }
}
/*  
    void SlinkBuilder::subProcess40MHz() {
    if(debugPrint_) {
    std::cout << " SlinkBuilder::subProcess40MHz()  entering" << std::endl;
    }

    if(rxCommandsPtr_->get(commands_)) { // Only after a 40 MHz tick

    // Update counters
    counters_.update(commands_);

    // Check for L1A
    if(commands_.l1a()) {

    // Create BOE and add to the queue
    uint16_t et(0x0001); // Physics

    boeQueue_.push(SlinkBoe(definition_.sourceId(),counters_,et));
    if(debugPrint_) {
    std::cout << " New Slink L1A added, queue size = " << boeQueue_.size() << " ";
    boeQueue_.back().print();
    counters_.print();
    }
    }
    }
  
    if(debugPrint_) {
    std::cout << " SlinkBuilder::subProcess40MHz()  exiting" << std::endl;
    }
    }
*/
void SlinkBuilder::inputPacketCheck(unsigned n, const RamWord &w) {
  assert(n<lpgbtPairPacketVector_.size());
  lpgbtPairPacketVector_[n].push_back(w);
  
  // Check when packet is finished
  if(w.endBit()) {
    assert(inputL1aCountersQueueVector_[n].size()>0);

    if(!PacketChecker::checkLpgbtPairBuilderPacket(definition_.lpgbtPairDefinitions()[n],
						   inputL1aCountersQueueVector_[n].front(),
						   lpgbtPairPacketVector_[n],debugPrint_)) {

      printBxTick();
      assert(PacketChecker::checkLpgbtPairBuilderPacket(definition_.lpgbtPairDefinitions()[n],
							inputL1aCountersQueueVector_[n].front(),
							lpgbtPairPacketVector_[n],debugPrint_));
    }
    
    numberOfInputCheckedPackets_++;
    numberOfInputCheckedPacketsVector_[n]++;

    inputL1aCountersQueueVector_[n].pop();
    lpgbtPairPacketVector_[n].resize(0);
  }
}

void SlinkBuilder::outputPacketCheck(const std::pair<bool,SlinkWord> &w) {
  slinkPacket_.push_back(w.second);

  // Check when packet is finished (indicated by non-zero size)
  if(w.first && slinkPacket_.size()>1) {
    assert(l1aCountersQueue_.size()>0);

    if(!PacketChecker::checkSlinkBuilderPacket(definition_,
					       l1aCountersQueue_.front(),
					       slinkPacket_,debugPrint_)) {
      printBxTick();
      assert(PacketChecker::checkSlinkBuilderPacket(definition_,
						    l1aCountersQueue_.front(),
						    slinkPacket_,true));
    }

    numberOfOutputCheckedPackets_++;
    
    l1aCountersQueue_.pop();
    slinkPacket_.resize(0);
  }
}

void SlinkBuilder::print() const {
  std::cout << name_ << "::print() " << definition_.label() << std::endl
	    << " Number of ticks = " << numberOfTicks_ << std::endl << std::endl;
  std::cout << " INPUT SECTION: Max possible data rate = " << 0.064*360 << " Gbit/s" << std::endl;
  std::cout << "  Number of total 360 MHz ticks            = " << std::setw(10)
	    << numberOfInputRamWordsVector_.size()*numberOf360MhzTicks_
	    << std::endl;
  std::cout << "  Number of total input no data ticks      = "
	    << std::setw(10) << numberOfInputNoDataTicks_ << " = "
	    << std::setw(6) << 100.0*countsFraction(numberOfInputNoDataTicks_,numberOfInputRamWordsVector_.size()*numberOf360MhzTicks_)
	    << " %" << std::endl;
  std::cout << "  Number of total input backpressure ticks = "
	    << std::setw(10) << numberOfInputBackpressureTicks_ << " = "
	    << std::setw(6) << 100.0*countsFraction(numberOfInputBackpressureTicks_,numberOfInputRamWordsVector_.size()*numberOf360MhzTicks_)
	    << " %" << std::endl;
  std::cout << "  Number of total input read ticks         = "
	    << std::setw(10) << numberOfInputRamWords_ << " = "
	    << std::setw(6) << 100.0*countsFraction(numberOfInputRamWords_,numberOfInputRamWordsVector_.size()*numberOf360MhzTicks_)
	    << " %" << std::endl;
  //std::cout << "  Number of total input RAM words      = "
  //	    << std::setw(10) << numberOfInputRamWords_ << std::endl;
  std::cout << "  Number of total input possible words = "
	    << std::setw(10) << 2*numberOfInputRamWords_ << std::endl;
  std::cout << "  Number of total input basic words    = "
	    << std::setw(10) << numberOfInputBasicWords_ << std::endl;
  std::cout << "  Number of total input padding words  = "
	    << std::setw(10) << numberOfInputPaddingWords_ << std::endl;
  std::cout << "  Total Slink data input rate = " << 0.064*360.0*countsFraction(numberOfInputRamWords_,numberOf360MhzTicks_)
	    << " Gbit/s, duty cycle = " << 100.0*countsFraction(numberOfInputRamWords_,numberOf360MhzTicks_) << " %" << std::endl;

  //<< ", duty cycle = " << 100.0*countsFraction(numberOfInputRamWords_,numberOf360MhzTicks_) << " %" << std::endl;

  std::cout << "  Number of 360 MHz ticks               = "
	    << std::setw(10) << numberOf360MhzTicks_ << std::endl;
  for(unsigned i(0);i<numberOfInputRamWordsVector_.size();i++) {
    std::cout << "   Number of input " << i << " no data ticks      = "
	      << std::setw(10) << numberOfInputNoDataTicksVector_[i] << std::endl;
    std::cout << "   Number of input " << i << " backpressure ticks = "
	      << std::setw(10) << numberOfInputBackpressureTicksVector_[i] << std::endl;
    std::cout << "   Number of input " << i << " read ticks         = "
	      << std::setw(10) << numberOfInputRamWordsVector_[i] << std::endl;
    std::cout << "   Number of input " << i << " possible words = "
	      << std::setw(10) << 2*numberOfInputRamWordsVector_[i] << std::endl;
    std::cout << "   Number of input " << i << " basic words    = "
	      << std::setw(10) << numberOfInputBasicWordsVector_[i] << std::endl;
    std::cout << "   Number of input " << i << " padding words  = "
	      << std::setw(10) << numberOfInputPaddingWordsVector_[i] << std::endl;
    //std::cout << "  Number of input " << i << " RAM words = "
    //<< numberOfInputRamWordsVector_[i]
    //	      << ", duty cycle = " << 100*numberOfInputRamWordsVector_[i]/numberOfTicks_ << " %" << std::endl;
    std::cout << std::endl;
  }
  
  if(numberOfTicks_>0) {
    std::cout << "  Number of input  started, finished packets = " << numberOfInputStartedPackets_ << ", " << numberOfInputFinishedPackets_;
    if(numberOfInputFinishedPackets_>0) {
      std::cout << ", average size = " << 1.0*numberOfInputRamWords_/numberOfInputPackets_ << " RAM words = "
		<< 0.064*numberOfInputRamWords_/numberOfInputPackets_ << " kbit";
    }
    std::cout << std::endl;

    for(unsigned i(0);i<numberOfInputStartedPacketsVector_.size();i++) {
      std::cout << "   Number of input " << i << "  started, finished packets = " << numberOfInputStartedPacketsVector_[i] << ", " << numberOfInputFinishedPacketsVector_[i];
      if(numberOfInputPacketsVector_[i]>0) {
        std::cout << ", average size = " << 1.0*numberOfInputRamWordsVector_[i]/numberOfInputPacketsVector_[i] << " RAM words = "
		  << 0.064*numberOfInputRamWordsVector_[i]/numberOfInputPacketsVector_[i] << " kbit";
      }
      std::cout << std::endl;
    }
    
    std::cout << "  Number of input checked packets = " << numberOfInputCheckedPackets_ << std::endl;
    for(unsigned i(0);i<numberOfInputCheckedPacketsVector_.size();i++) {
      std::cout << "   Number of input " << i << " checked packets = " << numberOfInputCheckedPacketsVector_[i] << std::endl;
    }
    std::cout << "  Input buffers" << std::endl;
    for(unsigned i(0);i<sizeOfInputBufferVector_.size();i++) {
      std::cout << "   Input buffer " << i << " average size = " << 1.0*sizeOfInputBufferVector_[i]/numberOfTicks_
		<< ", maximum = " << maxOfInputBufferVector_[i] << std::endl;
    }
    std::cout << std::endl;

  std::cout << " BUILDER SECTION: Max possible data rate = " << 0 << " Gbit/s" << std::endl;
  std::cout << "  Number of builder backpressure ticks = " << std::setw(10) << numberOfBuilderBackpressureTicks_ << std::endl;
  std::cout << std::endl;
    
  std::cout << " OUTPUT SECTION: Max possible data rate = " << 0.128*180 << " Gbit/s" << std::endl;
  std::cout << " Output slink word queue average size = " << 0.0
	    << ", maximum = " << maxOfOutputSlinkWordQueueSize_ << std::endl;

std::cout << "  Number of total output ticks        = " << std::setw(10) << numberOfTicks_ << std::endl;
    std::cout << "  Number of output backpressure ticks = " << std::setw(10) << numberOfOutputBackpressureTicks_ << std::endl;
    std::cout << "  Number of output no data ticks      = " << std::setw(10) << numberOfOutputNoDataTicks_ << std::endl;
    std::cout << "  Number of output data ticks         = " << std::setw(10) << numberOfOutputDataTicks_ << std::endl;
    std::cout << "  Number of output Slink words = " << numberOfOutputSlinkWords_ << std::endl;
    std::cout << "  Data rate = " << 0.128*180.0*countsFraction(numberOfOutputSlinkWords_,numberOfTicks_)
	      << " Gbit/s, duty cycle = " << 100.0*countsFraction(numberOfOutputSlinkWords_,numberOfTicks_) << " %" << std::endl;
    std::cout << " Number of output started, finished packets = " << numberOfOutputStartedPackets_ << ", " << numberOfOutputFinishedPackets_ << std::endl;
    if(numberOfOutputPackets_>0) {
      std::cout << ", average size = " << 1.0*numberOfOutputSlinkWords_/numberOfOutputPackets_ << " Slink words = "
		<< 0.128*numberOfOutputSlinkWords_/numberOfOutputPackets_ << " kbit";
    }
    std::cout << std::endl;
    std::cout << " Number of output checked packets = " << numberOfOutputCheckedPackets_ << std::endl;
    std::cout << " Output buffer average size = " << 1.0*sizeOfOutputBuffer_/numberOfTicks_
	      << ", maximum = " << maxOfOutputBuffer_ << std::endl;

    std::cout << std::endl;
  }

  slinkBuilderAnalyserPtr_->finalise();
}
