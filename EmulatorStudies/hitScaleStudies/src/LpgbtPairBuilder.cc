#include <iostream>
#include <iomanip>

#include "Utils.h"
#include "PacketChecker.h"
#include "EcondAsicHitCounts.h"
#include "LpgbtPairHeader.h"
#include "LpgbtPairBuilder.h"
#include "LpgbtPairBuilderAnalyser.h"

std::string LpgbtPairBuilder::stateName_[NumberOfStates]={
  //"Wait",
  "AcquireEcond",
  //"FakeEcond",
  "GetHeaders",
  "FirstWord",
  "SendHeader"
};

LpgbtPairBuilder::LpgbtPairBuilder() :
  EntityFastControl("LpgbtPairBuilder",Clocks::Clock0280),
  txRamWordPtr_(0),
  state_(GetHeaders),
  outputEcond_(0),
  ebrSent_(0),
  fastControlDelay_(0),
  waitEcondCounter_(0),
  acquireEcondCounter_(0),
  lpgbtPairBuilderAnalyserPtr_(0),

  numberOfTicks_(0),

  numberOfInputBackpressureTicks_(0),
  numberOfInputNoL1aTicks_(0),
  numberOfInputNoHeaderTicks_(0),
  numberOfInputNoDataTicks_(0),
  numberOfInputHeaderTicks_(0),
  numberOfInputDataTicks_(0),
  
  numberOfInputRamWords_(0),
  numberOfInputPackets_(0),
  numberOfInputCheckedPackets_(0),
  numberOfDroppedRamWords_(0),
  numberOfDroppedPackets_(0),
  numberOfTimeoutRamWords_(0),
  numberOfTimeoutPackets_(0),
  numberOfOutputBackpressureTicks_(0),
  numberOfOutputNoDataTicks_(0),
  numberOfOutputDataTicks_(0),
  numberOfOutputRamWords_(0),
  numberOfOutputPackets_(0),
  numberOfOutputCheckedPackets_(0),
  sizeOfOutputBuffer_(0),
  maxOfOutputBuffer_(0) {
}

LpgbtPairBuilder::~LpgbtPairBuilder() {
}

void LpgbtPairBuilder::setDefinitionTo(const LpgbtPairDefinition &d) {
  definition_=d;
  d.econdAsicDefinitions(econdAsicDefinitionVector_);
  numberOfEcondAsics_=econdAsicDefinitionVector_.size();

  numberOfInputBackpressureTicksVector_.resize(numberOfEcondAsics_);
  numberOfInputNoL1aTicksVector_.resize(numberOfEcondAsics_);
  numberOfInputNoHeaderTicksVector_.resize(numberOfEcondAsics_);
  numberOfInputNoDataTicksVector_.resize(numberOfEcondAsics_);
  numberOfInputHeaderTicksVector_.resize(numberOfEcondAsics_);
  numberOfInputDataTicksVector_.resize(numberOfEcondAsics_);
  numberOfInputRamWordsVector_.resize(numberOfEcondAsics_);
  numberOfInputPacketsVector_.resize(numberOfEcondAsics_);
  numberOfInputCheckedPacketsVector_.resize(numberOfEcondAsics_);
  numberOfDroppedRamWordsVector_.resize(numberOfEcondAsics_);
  numberOfDroppedPacketsVector_.resize(numberOfEcondAsics_);
  numberOfTimeoutRamWordsVector_.resize(numberOfEcondAsics_);
  numberOfTimeoutPacketsVector_.resize(numberOfEcondAsics_);
  sizeOfInputBufferVector_.resize(numberOfEcondAsics_);
  maxOfInputBufferVector_.resize(numberOfEcondAsics_);

  for(unsigned i(0);i<numberOfEcondAsics_;i++) {
    numberOfInputBackpressureTicksVector_[i]=0;
    numberOfInputNoL1aTicksVector_[i]=0;
    numberOfInputNoHeaderTicksVector_[i]=0;
    numberOfInputNoDataTicksVector_[i]=0;
    numberOfInputNoHeaderTicksVector_[i]=0;
    numberOfInputNoDataTicksVector_[i]=0;
    numberOfInputRamWordsVector_[i]=0;
    numberOfInputPacketsVector_[i]=0;
    numberOfInputCheckedPacketsVector_[i]=0;
    numberOfDroppedRamWordsVector_[i]=0;
    numberOfDroppedPacketsVector_[i]=0;
    numberOfTimeoutRamWordsVector_[i]=0;
    numberOfTimeoutPacketsVector_[i]=0;
    sizeOfInputBufferVector_[i]=0;
    maxOfInputBufferVector_[i]=0;
  }
  
  econdAsicPacket72Vector_.resize(numberOfEcondAsics_);
}

void LpgbtPairBuilder::setArgumentsTo(const Arguments &a) {
  //configuration_.setL1aFakeProbabilityTo(a.config().at("econdL1aFakeProbability").get<double>());
  fastControlDelay_=a.config().at("fpgaFastControlDelay").get<unsigned>();

  Entity::setArgumentsTo(a);
  if(idNumber_==a.config().at("lpbIdDebugPrint").get<uint32_t>()) debugPrint_=true;
}

void LpgbtPairBuilder::initialise() {
  std::cout << name_ << "::initialise() " << definition_.label() << std::endl;
  definition_.print(" ");
  std::cout << std::endl;
  
  assert(rxCommandsPtr_!=0);

  assert(rxRamWordPtrVector_.size()               ==numberOfEcondAsics_);
  //assert(rxSideFifoFlagPtrVector_.size()          ==numberOfEcondAsics_);
  assert(numberOfInputRamWordsVector_.size()      ==numberOfEcondAsics_);
  assert(numberOfInputPacketsVector_.size()       ==numberOfEcondAsics_);
  assert(numberOfInputCheckedPacketsVector_.size()==numberOfEcondAsics_);
  assert(numberOfDroppedRamWordsVector_.size()    ==numberOfEcondAsics_);
  assert(numberOfDroppedPacketsVector_.size()     ==numberOfEcondAsics_);
  assert(numberOfTimeoutRamWordsVector_.size()    ==numberOfEcondAsics_);
  assert(numberOfTimeoutPacketsVector_.size()     ==numberOfEcondAsics_);
  assert(econdAsicPacket72Vector_.size()          ==numberOfEcondAsics_);

  for(unsigned i(0);i<rxRamWordPtrVector_.size();i++) {
    /* NOT CORRECT 
    std::cout << " Input buffer " << i << " maximum size = "
	      << rxRamWordPtrVector_[i]->maxSize()
      //<< ", side FIFO maximum size = "
      //      << rxSideFifoFlagPtrVector_[i]->maxSize()
	      << std::endl;
    */
    
    // Set up header storage
    boolHeaderVector_.push_back(std::pair<unsigned,RamWord>(0,RamWord()));
  }

  if(txRamWordPtr_==0) std::cout << " Output buffer disabled" << std::endl;
  else {
    std::cout << " Output buffer maximum size = "
	      << txRamWordPtr_->maxSize() << std::endl;
  }
  std::cout << std::endl;

  rxCommandsPtr_->initialiseQueue(FastControlCommands(),fastControlDelay_);
  
  lpgbtPairBuilderAnalyserPtr_=new LpgbtPairBuilderAnalyser(this);
  lpgbtPairBuilderAnalyserPtr_->initialise(doAnalysis_);
}

// Connections
void LpgbtPairBuilder::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  EntityFastControl::connectRxPortTo(*c);
  d.connectTxPortTo(*c);
}

void LpgbtPairBuilder::connect(LpgbtPairEcondBuilder &e) {
  Connection<RamWord> *w(new Connection<RamWord>);
  connectRxPortTo(*w);
  e.connectTxPortTo(*w);
  /*
  Connection<LpgbtPairEcondBuilder::SideFifoFlag> *f(new Connection<LpgbtPairEcondBuilder::SideFifoFlag>);
  connectRxPortTo(*f);
  e.connectTxPortTo(*f);
  */
}

// Inputs
void LpgbtPairBuilder::connectRxPortTo(Connection<RamWord> &w) {
  rxRamWordPtrVector_.push_back(&w);
}
/*
void LpgbtPairBuilder::connectRxPortTo(Connection<LpgbtPairEcondBuilder::SideFifoFlag> &f) {
  rxSideFifoFlagPtrVector_.push_back(&f);
}
*/
void LpgbtPairBuilder::strobeRx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeRx();
  
  for(unsigned i(0);i<rxRamWordPtrVector_.size();i++) {
    rxRamWordPtrVector_[i]->strobeOutput();
    //rxSideFifoFlagPtrVector_[i]->strobeOutput();
  }
}

// Outputs
void LpgbtPairBuilder::connectTxPortTo(Connection<RamWord> &w) {
  txRamWordPtr_=&w;
}

void LpgbtPairBuilder::strobeTx() {
  if(txRamWordPtr_!=0) txRamWordPtr_->strobeInput();
}

// Processing
void LpgbtPairBuilder::process() {
  //if(idNumber_==0x00800462 && Clocks::clocks().bxNumber()>=20000) debugPrint_=true;
  //  if(Clocks::clocks().bxNumber()== 536472) debugPrint_=true;
  
  if(debugPrint_) {
    std::cout << name_ << "::process()  entering" << std::endl;
  }

  if(Clocks::clocks().previousClock0040()) subProcess40Mhz(true);

  subProcessInput();
  subProcessOutput();

  // Check buffers
  for(unsigned i(0);i<numberOfEcondAsics_;i++) {
    unsigned s(rxRamWordPtrVector_[i]->size());
    sizeOfInputBufferVector_[i]+=s;
    if(maxOfInputBufferVector_[i]<s) maxOfInputBufferVector_[i]=s;
    
    if(debugPrint_) {
      std::cout << "  Input buffer " << i << " size = " << s << std::endl;
    }
  }

  if(txRamWordPtr_!=0) {
    unsigned s(txRamWordPtr_->size());
    sizeOfOutputBuffer_+=s;
    if(maxOfOutputBuffer_<s) maxOfOutputBuffer_=s;
    
    if(debugPrint_) {
      std::cout << "  Output buffer size = " << s << std::endl;
    }
  }

  // Analyse results
  if(lpgbtPairBuilderAnalyserPtr_!=0) lpgbtPairBuilderAnalyserPtr_->analyse();
  
  numberOfTicks_++;
  
  if(debugPrint_) {
    std::cout << name_ << "::process()  exiting" << std::endl;
  }
}

void LpgbtPairBuilder::subProcessInput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput() entering, state = "
	      << state_ << " = " << stateName_[state_] << std::endl;
  }

  // Check if room in output buffer
  if(txRamWordPtr_==0 || !txRamWordPtr_->full()) {
    //    std::cout<<"ALpana "<<"outputEcond_  "<<outputEcond_<<"\t"<<"econdAsicDefinitionVector_.size()   "<<econdAsicDefinitionVector_.size()<<"\t"<<std::endl;
    if(outputEcond_>=econdAsicDefinitionVector_.size()) {
      printBxTick();
      std::cout<<"Alpana"<<"\t"<<"checks for econd assertation in src/LpgbtPairBuilder.cc#L253"<<std::endl;
      assert(outputEcond_<econdAsicDefinitionVector_.size());
    }
    //    assert(outputEcond_<econdAsicDefinitionVector_.size());

    // Try to get next RAM word
    validRamOutputWord_=false;

    switch(state_) {

    case GetHeaders:
      {
	if(!l1aCountersQueue_.empty()) {
	  if(debugPrint_) {
	    std::cout << "  ECON-D " << outputEcond_
		      << ", L1A counters queue not empty, size = "
		      << l1aCountersQueue_.size() << std::endl;
	  }

	  if(boolHeaderVector_[outputEcond_].first==0) {
	    if(debugPrint_) {
	      std::cout << "  ECON-D " << outputEcond_
			<< ", no header ready" << std::endl;
	    }
	  
	    if(rxRamWordPtrVector_[outputEcond_]->get(ramOutputWord_)) {
	      if(debugPrint_) {
		std::cout << "  ECON-D " << outputEcond_ << ", main buffer output word ";
		ramOutputWord_.print();
	      }

	      numberOfInputHeaderTicks_++;
	      numberOfInputHeaderTicksVector_[outputEcond_]++;	      

	      assert(ramOutputWord_.startBit());
	      boolHeaderVector_[outputEcond_].first=4;
	      boolHeaderVector_[outputEcond_].second=ramOutputWord_;

	    } else { // No word in buffer to read
	      if(debugPrint_) {
		std::cout << " ECON-D " << outputEcond_
			  << ", no word in buffer" << std::endl;
	      }
	      
	      numberOfInputNoHeaderTicks_++;
	      numberOfInputNoHeaderTicksVector_[outputEcond_]++;	      

	      waitEcondCounter_++;
	      if(debugPrint_) {
		std::cout << " ECON-D " << outputEcond_
			  << ", wait counter incremented to "
			  << waitEcondCounter_ << std::endl;
	      }
	      
	      if(waitEcondCounter_>=50) {
		if(waitEcondCounter_>=15) {
		  std::cerr << name_ << " ECON-D " << outputEcond_
			    << ", WaitEcondCounter = " << waitEcondCounter_ << std::endl;
		  printBxTick();
		}
		
		// Give up; treat as timeout packet
		//outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
		outputEcond_++;
	      }
	    }
	  } else {
	    if(debugPrint_) {
	      std::cout << "  ECON-D " << outputEcond_
			<< ", header ready with value = "
			<< boolHeaderVector_[outputEcond_].first
			<< std::endl;
	    }	    
	  }
	
	  if(boolHeaderVector_[outputEcond_].first>0) {
	    if(debugPrint_) {
	      std::cout << "  ECON-D " << outputEcond_
			<< ", comparing header with value = "
			<< boolHeaderVector_[outputEcond_].first
			<< std::endl;
	    }
	    
	    EcondAsicHeader eah(boolHeaderVector_[outputEcond_].second);
	    if(eah.consistentTimeWith(l1aCountersQueue_.front())) {
	      boolHeaderVector_[outputEcond_].first=2;
	      if(eah==l1aCountersQueue_.front()) {
		boolHeaderVector_[outputEcond_].first=3;
	      }
	    } else {
	      /*
	      std::cout << "ERROR: Mismatch: expected counters = ";
	      l1aCountersQueue_.front().print();
	      std::cout << "ERROR: Mismatch: header found = ";
	      eah.print();
	      */
	      boolHeaderVector_[outputEcond_].first=1;
	    }
	    
	    if(debugPrint_) {
	      std::cout << "  ECON-D " << outputEcond_
			<< ", compared header has value = "
			<< boolHeaderVector_[outputEcond_].first
			<< std::endl;
	    }
	    
	    outputEcond_++;
	  }
	  
	  if(outputEcond_==econdAsicDefinitionVector_.size()) {
	    outputEcond_=0;
	    firstWord();
	    //state_=FirstWord;
	  }

	} else {
	  numberOfInputNoL1aTicks_++;
	  numberOfInputNoL1aTicksVector_[outputEcond_]++;	      
	}
	
	break;
      }
      /*      
    case FirstWord:
      {
	LpgbtPairHeader lh;
	lh.setCountersTo(l1aCountersQueue_.front());
	
	lastOutputEcond_=0;
	
	for(unsigned e(0);e<econdAsicDefinitionVector_.size();e++) {
	  assert(boolHeaderVector_[e].first<4);

	  if(boolHeaderVector_[e].first==3) {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::Normal);	      
	    lastOutputEcond_=e+1;
	    
	  } else if(boolHeaderVector_[e].first==2) {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::TimestampError);
	    lastOutputEcond_=e+1;

	  } else if(boolHeaderVector_[e].first==1) {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::Missing);
	    
	  } else {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::TimeoutError);

	  }
	}
	    
	if(debugPrint_) {
	  std::cout << "  Formed LpbgtPairHeader with last ECON-D = "
		    << lastOutputEcond_ << std::endl;
	  lh.print("   ");
	}
	      
	ramOutputWord_.setWordTo(0,lh.word(0));
	ramOutputWord_.setWordTo(1,lh.word(1));
	ramOutputWord_.clearBits();
	ramOutputWord_.setStartBitTo(true);
	validRamOutputWord_=true;	

	outputEcond_=0;

	if(lastOutputEcond_>0) {
	  state_=SendHeader;
	  
	} else {
	  ramOutputWord_.setEndBitTo(true);
	  state_=GetHeaders;
	}
	
	break;
      }
      */      
#ifdef NOT_ANY_MORE
    case Wait:
      {
	assert(false);
	// Looking for entry in side FIFO
	/*
	  if(rxSideFifoFlagPtrVector_[outputEcond_]->get(sideFifoFlag_)) {
	  if(debugPrint_) {
	  std::cout << " ECON-D " << outputEcond_
	  << ", side FIFO flag = " << sideFifoFlag_ << std::endl;
	  }
	*/
	
	if(!l1aCountersQueue_.empty()) {
	  if(debugPrint_) {
	    std::cout << " ECON-D " << outputEcond_
		      << ", L1A counters queue not empty, size = "
		      << l1aCountersQueue_.size() << std::endl;
	  }

	  if(!ebrSent_) {
	    if(!boolHeaderVector_[outputEcond_].first) {
	      if(debugPrint_) {
		std::cout << " ECON-D " << outputEcond_
			  << ", no header ready" << std::endl;
	      }

	      //assert(rxRamWordPtrVector_[outputEcond_]->get(ramOutputWord_));
	      if(rxRamWordPtrVector_[outputEcond_]->get(ramOutputWord_)) {
		if(debugPrint_) {
		  std::cout << " ECON-D " << outputEcond_ << ", main buffer output word ";
		  ramOutputWord_.print();
		}
		
		assert(ramOutputWord_.startBit());
		boolHeaderVector_[outputEcond_].first=true;
		boolHeaderVector_[outputEcond_].second.setWordTo(0,ramOutputWord_.word(0));
		boolHeaderVector_[outputEcond_].second.setWordTo(1,ramOutputWord_.word(1));
		
	      } else {
		if(debugPrint_) {
		  std::cout << " ECON-D " << outputEcond_
			    << ", no word in buffer" << std::endl;
		}
		
		waitEcondCounter_++;
		if(debugPrint_) {
		  std::cout << " ECON-D " << outputEcond_
			    << ", wait counter incremented to "
			    << waitEcondCounter_ << std::endl;
		}
	      
		if(waitEcondCounter_>=50) {
		  if(waitEcondCounter_>=15) {
		    std::cerr << name_ << " ECON-D " << outputEcond_
			      << ", WaitEcondCounter = " << waitEcondCounter_
			      << std::endl;
		    printBxTick();
		  }

		  // Give up; treat as missing packet
		  outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
		}
	      }

	    } else {
	      if(debugPrint_) {
		std::cout << " ECON-D " << outputEcond_
			  << ", previous header ready" << std::endl;
		EcondAsicHeader(boolHeaderVector_[outputEcond_].second).print("  ");
	      }	      
	    }
	    /*
	      std::cout << " ECON-D " << outputEcond_ << " about to compare headers" << std::endl;
	      boolHeaderVector_[outputEcond_].second.print();
	      l1aCountersQueue_.front().print(" ");
	    */
	    if(EcondAsicHeader(boolHeaderVector_[outputEcond_].second)==l1aCountersQueue_.front()) {
	      validRamOutputWord_=true;
	      if(debugCheck_) {
		assert(econdAsicPacket72Vector_[outputEcond_].size()==0);
		econdAsicPacket72Vector_[outputEcond_].push_back(ramOutputWord_);
	      } 
	      
	      numberOfInputRamWords_++;
	      numberOfInputRamWordsVector_[outputEcond_]++;
	      
	      if(outputEcond_!=0) ramOutputWord_.setStartBitTo(false);
	      
	      boolHeaderVector_[outputEcond_].first=false;
	      
	      if(ramOutputWord_.endBit()) {
		numberOfInputPackets_++;
		numberOfInputPacketsVector_[outputEcond_]++;
		
		if(debugCheck_) {
		  if(!PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[outputEcond_],l1aCountersQueue_.front(),econdAsicPacket72Vector_[outputEcond_],debugPrint_)) {
		    std::cout << name_ << "::subProcessInput()" << std::endl;
		    assert(PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[outputEcond_],l1aCountersQueue_.front(),econdAsicPacket72Vector_[outputEcond_],true));
		  }
		  econdAsicPacket72Vector_[outputEcond_].resize(0);
		  
		  numberOfInputCheckedPackets_++;
		  numberOfInputCheckedPacketsVector_[outputEcond_]++;
		}
		
		if((outputEcond_+1)!=econdAsicDefinitionVector_.size()) ramOutputWord_.setEndBitTo(false);
		outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
		
	      } else {
		acquireEcondCounter_=0;
		state_=AcquireEcond;
	      }
	      
	    } else {
	      
	      // Event dropped; fill dropped header
	      EcondAsicHeader eah;
	      eah.setPayloadLengthTo(0);
	      eah.setQualityFlagsTo(2);
	      eah.setCountersTo(l1aCountersQueue_.front());
	      eah.setResetRequestTo(EcondAsicHeader::None);
	      eah.setCrc();
	      eah.print(" BUILDER: ");
	      //debugPrint_=true;
	      
	      // Send first two words of dropped header
	      validRamOutputWord_=true;
	      ramOutputWord_.setWordTo(0,eah.word(0));
	      ramOutputWord_.setWordTo(0,eah.word(0));
	      ramOutputWord_.setWordTo(1,eah.word(1));

	      numberOfDroppedRamWords_++;
	      numberOfDroppedRamWordsVector_[outputEcond_]++;
	      
	      boolHeaderVector_[outputEcond_].first=false;
	      
	      // Clean up start/end bits
	      ramOutputWord_.setStartBitTo(outputEcond_==0);
	      ramOutputWord_.setEndBitTo((outputEcond_+1)==econdAsicDefinitionVector_.size());
	      outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();


	      waitEcondCounter_=0;


	      std::cout << "  BUILDER: ";ramOutputWord_.print();
	    }
	  }
	}
	
	break;
      }
#endif

    case SendHeader:
      {
	//EcondAsicHeader(boolHeaderVector_[outputEcond_].second).print(" SendH: ");
	if(boolHeaderVector_[outputEcond_].first<2) {
	  if(debugPrint_) {
	    std::cout << "  ECON-D " << outputEcond_
		      << ", no packet so no header sent" << std::endl;
	  }

	  outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
	  if(outputEcond_==0) state_=GetHeaders;

	} else {
	  ramOutputWord_=boolHeaderVector_[outputEcond_].second;
	  validRamOutputWord_=true;

	  if(debugPrint_) {
	    std::cout << "  ECON-D " << outputEcond_
		      << ", header sent" << std::endl;
	    ramOutputWord_.print("   ");
	  }
	  
	  if(debugCheck_) {
	    econdAsicPacket72Vector_[outputEcond_].push_back(ramOutputWord_);
	  }
	  
	  ramOutputWord_.setStartBitTo(false);
	  
	  if(ramOutputWord_.endBit()) {
	    if(debugCheck_) {
	      if(!PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[outputEcond_],l1aCountersQueue_.front(),econdAsicPacket72Vector_[outputEcond_],debugPrint_)) {

		std::cout << "  " << name_ << "::subProcessInput() ECON-D = " << outputEcond_ << ", calling PacketChecker::checkEcondAsicPacket72" << std::endl;
		printBxTick();
		assert(PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[outputEcond_],l1aCountersQueue_.front(),econdAsicPacket72Vector_[outputEcond_],true));
	      }
	      
	      econdAsicPacket72Vector_[outputEcond_].resize(0);
	      
	      numberOfInputCheckedPackets_++;
	      numberOfInputCheckedPacketsVector_[outputEcond_]++;
	    }

	    boolHeaderVector_[outputEcond_].first=0;
	    if((outputEcond_+1)<lastOutputEcond_) ramOutputWord_.setEndBitTo(false);
	    outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
	    if(outputEcond_==0) state_=GetHeaders;
	    
	  } else {
	    boolHeaderVector_[outputEcond_].first=0;
	    state_=AcquireEcond;
	  }
	}

	break;
      }

    case AcquireEcond:
      {
	// Send next words of normal event
	//printBxTick();
	//if(rxRamWordPtrVector_[outputEcond_]->empty()) {
	if(!rxRamWordPtrVector_[outputEcond_]->get(ramOutputWord_)) {
	  validRamOutputWord_=false;
	  acquireEcondCounter_++;
	  
	  numberOfInputNoDataTicks_++;
	  numberOfInputNoDataTicksVector_[outputEcond_]++;	      

	  if(debugPrint_) {
	    std::cout << " ECON-D " << outputEcond_
		      << ", acquire counter incremented to "
		      << acquireEcondCounter_ << std::endl;
	  }
	      
	  if(acquireEcondCounter_>=14) { // WHY NOT 14???
	    std::cerr  << name_ << " ECON-D " << outputEcond_
		       << "acquireEcondCounter = " << acquireEcondCounter_
		       << std::endl;
	    printBxTick();
	  }

	} else {
	  //if(rxRamWordPtrVector_[outputEcond_]->get(ramOutputWord_)) {
	  //assert(rxRamWordPtrVector_[outputEcond_]->get(ramOutputWord_));
	  if(debugPrint_) {
	    std::cout << "  " << name_ << "::subProcessInput() ECON-D = " << outputEcond_
		      << ", input word from main buffer" << std::endl;
	    ramOutputWord_.print("   ");
	  }
	  
	  numberOfInputDataTicks_++;
	  numberOfInputDataTicksVector_[outputEcond_]++;

	  validRamOutputWord_=true;

	  acquireEcondCounter_=0;
	  waitEcondCounter_=0;

	  if(debugCheck_) {
	    econdAsicPacket72Vector_[outputEcond_].push_back(ramOutputWord_);
	  }
	  
	  numberOfInputRamWords_++;
	  numberOfInputRamWordsVector_[outputEcond_]++;
	  
	  if(ramOutputWord_.endBit()) {
	    numberOfInputPackets_++;
	    numberOfInputPacketsVector_[outputEcond_]++;
	    
	    assert(!l1aCountersQueue_.empty());
	    
	    if(debugCheck_) {
	      if(!PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[outputEcond_],l1aCountersQueue_.front(),econdAsicPacket72Vector_[outputEcond_],debugPrint_)) {

		std::cout << "  " << name_ << "::subProcessInput() ECON-D = " << outputEcond_ << ", calling PacketChecker::checkEcondAsicPacket72" << std::endl;
		printBxTick();
		assert(PacketChecker::checkEcondAsicPacket72(econdAsicDefinitionVector_[outputEcond_],l1aCountersQueue_.front(),econdAsicPacket72Vector_[outputEcond_],true));
	      }

	      econdAsicPacket72Vector_[outputEcond_].resize(0);
	      
	      numberOfInputCheckedPackets_++;
	      numberOfInputCheckedPacketsVector_[outputEcond_]++;
	    }
	    
	    if((outputEcond_+1)<lastOutputEcond_) ramOutputWord_.setEndBitTo(false);
	    outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
	    
	    if(outputEcond_==0) state_=GetHeaders;
	    else state_=SendHeader;
	  }
	}
	break;
      }
      /*      
	      case FakeEcond:
	      {
	      // SHOULD NEVER GET HERE WITH TWO-WORD FAKE PACKETS
	      assert(false);

	      // Send remaining word of dropped header
	      ramOutputWord_.setPaddingBitTo(true);
	      //ramOutputWord_.setWordTo(0,econdAsicHeader_.word(2));
	      EcondAsicHitCounts eahc;
	      ramOutputWord_.setWordTo(0,eahc.word());
	      //ramOutputWord_.setWordTo(0,0xdddd0006||((l1aCountersQueue_.front().bc()&0xf)<<12)); // FIX HEADER FIRST
	      ramOutputWord_.setWordTo(1,0);

	      validRamOutputWord_=true;
	      if(debugCheck_) {
	      econdAsicPacket72Vector_[outputEcond_].push_back(ramOutputWord_);
	      }
	
	      if(sideFifoFlag_==LpgbtPairEcondBuilder::Dropped) {
	      numberOfDroppedRamWords_++;
	      numberOfDroppedRamWordsVector_[outputEcond_]++;
	  
	      numberOfDroppedPackets_++;
	      numberOfDroppedPacketsVector_[outputEcond_]++;
	      } else {
	      numberOfTimeoutRamWords_++;
	      numberOfTimeoutRamWordsVector_[outputEcond_]++;
	  
	      numberOfTimeoutPackets_++;
	      numberOfTimeoutPacketsVector_[outputEcond_]++;
	      }
	  
	      // CHECK FAKES?
	      if(debugCheck_) {
	      econdAsicPacket72Vector_[outputEcond_].resize(0);
	      }
	
	      if((outputEcond_+1)==econdAsicDefinitionVector_.size()) ramOutputWord_.setEndBitTo(true);
	      outputEcond_=(outputEcond_+1)%econdAsicDefinitionVector_.size();
	
	      waitEcondCounter_=0;
	      state_=Wait;
	
	      break;
	      }
      */	
    default:
      {
	// Should never happen
	assert(false);
      }
    }

  } else {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessInput() no room in output buffer"
		<< std::endl;
    }

    numberOfInputBackpressureTicks_++;
    numberOfInputBackpressureTicksVector_[outputEcond_]++;	      
  }
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()  exiting, state = "
	      << state_ << " = " << stateName_[state_] << std::endl;
  }
}


void LpgbtPairBuilder::firstWord() {

	LpgbtPairHeader lh;
	lh.setCountersTo(l1aCountersQueue_.front());
	
	lastOutputEcond_=0;
	
	for(unsigned e(0);e<econdAsicDefinitionVector_.size();e++) {
	  assert(boolHeaderVector_[e].first<4);

	  if(boolHeaderVector_[e].first==3) {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::Normal);	      
	    lastOutputEcond_=e+1;
	    
	  } else if(boolHeaderVector_[e].first==2) {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::TimestampError);
	    lastOutputEcond_=e+1;

	  } else if(boolHeaderVector_[e].first==1) {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::Missing);
	    
	  } else {
	    lh.setEcondStatusTo(e,LpgbtPairHeader::TimeoutError);

	  }
	}
	    
	if(debugPrint_) {
	  std::cout << "  Formed LpbgtPairHeader with last ECON-D = "
		    << lastOutputEcond_ << std::endl;
	  lh.print("   ");
	}
	      
	ramOutputWord_.setWordTo(0,lh.word(0));
	ramOutputWord_.setWordTo(1,lh.word(1));
	ramOutputWord_.clearBits();
	ramOutputWord_.setStartBitTo(true);
	validRamOutputWord_=true;	

	outputEcond_=0;

	if(lastOutputEcond_>0) {
	  state_=SendHeader;
	  
	} else {
	  ramOutputWord_.setEndBitTo(true);
	  state_=GetHeaders;
	}
}

void LpgbtPairBuilder::subProcessOutput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessOutput() entering" << std::endl;
  }
  
  // Check if room in output buffer
  if(txRamWordPtr_==0 || !txRamWordPtr_->full()) {
    
    // Write out if there is a valid word
    if(validRamOutputWord_) {
      if(debugPrint_) {
	std::cout << "  Valid output word to write to event buffer " << std::endl;
	ramOutputWord_.print("   ");
      }
      
      numberOfOutputDataTicks_++;

      if(txRamWordPtr_!=0) {
	if(!txRamWordPtr_->set(ramOutputWord_)) {
	  printBxTick();
	  assert(false);
	}
      }
      numberOfOutputRamWords_++;
    
      // Packet check
      if(debugCheck_) outputPacketCheck();
    
      // Catch end of packet
      if(ramOutputWord_.endBit()) {
	numberOfOutputPackets_++;
      
	assert(!l1aCountersQueue_.empty());
	/*      
	if(debugCheck_) {
	  if(!PacketChecker::checkLpgbtPairBuilderPacket(definition_,l1aCountersQueue_.front(),lpgbtPairBuilderPacket72Vector_,debugPrint_)) {

	    // If it fails, call again with debug turned on before asserting
	    std::cout << "  " << name_ << "::subProcessOutput() calling PacketChecker::checkLpgbtPairBuilderPacket" << std::endl;
	    printBxTick();
	    assert(PacketChecker::checkLpgbtPairBuilderPacket(definition_,l1aCountersQueue_.front(),lpgbtPairBuilderPacket72Vector_,true));
	  }
	
	  lpgbtPairBuilderPacket72Vector_.resize(0);
	
	  numberOfOutputCheckedPackets_++;
	}
	*/
	// Remove this L1A from the queue
	l1aCountersQueue_.pop();
      }

    } else {
      if(debugPrint_) {
	std::cout << "  No valid output RAM word to write to event buffer" << std::endl;
      }

      numberOfOutputNoDataTicks_++;
    }

  } else {
    numberOfOutputBackpressureTicks_++;
  }
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessOutput()  exiting" << std::endl;
  }
}

void LpgbtPairBuilder::outputPacketCheck() {
  lpgbtPairBuilderPacket72Vector_.push_back(ramOutputWord_);

  // Check when packet is finished
  if(ramOutputWord_.endBit()) {
    if(!PacketChecker::checkLpgbtPairBuilderPacket(definition_,l1aCountersQueue_.front(),lpgbtPairBuilderPacket72Vector_,debugPrint_)) {

      // If it fails, call again with debug turned on before asserting
      std::cout << "  " << name_ << " calling PacketChecker::checkLpgbtPairBuilderPacket" << std::endl;
      printBxTick();
      assert(PacketChecker::checkLpgbtPairBuilderPacket(definition_,l1aCountersQueue_.front(),lpgbtPairBuilderPacket72Vector_,true));
    }
	
    lpgbtPairBuilderPacket72Vector_.resize(0);
    numberOfOutputCheckedPackets_++;
  }
}

void LpgbtPairBuilder::print() const {
  if (numberOfTicks_ > 0) {
    std::cout << name_ << "::print() " << definition_.label() << std::endl
        << "  Number of ticks = " << numberOfTicks_ << std::endl
              << "  Number of input RAM words = " << numberOfInputRamWords_
              << ", duty cycle = " << 100.0*numberOfInputRamWords_/numberOfTicks_ << " %" << std::endl
              << "  Number of total input packets = " << numberOfInputPackets_;
    if(numberOfInputPackets_>0) {
      std::cout << ", average size = " << 1.0*numberOfInputRamWords_/numberOfInputPackets_ << " RAM words = "
                << 0.064*numberOfInputRamWords_/numberOfInputPackets_ << " kbit";
    }
    
    std::cout << std::endl;
    std::cout << "  Number of input backpressure ticks = " << numberOfInputBackpressureTicks_ << std::endl;
    std::cout << "  Number of input no L1A ticks       = " << numberOfInputNoL1aTicks_ << std::endl;
    std::cout << "  Number of input no header ticks    = " << numberOfInputNoHeaderTicks_ << std::endl;
    std::cout << "  Number of input no data ticks      = " << numberOfInputNoDataTicks_ << std::endl;
    std::cout << "  Number of input header ticks       = " << numberOfInputHeaderTicks_ << std::endl;
    std::cout << "  Number of input data ticks         = " << numberOfInputDataTicks_ << std::endl;
    std::cout << std::endl;
	      
    for(unsigned i(0);i<numberOfInputPacketsVector_.size();i++) {
      std::cout << "  Number of input " << i << " RAM words = " << numberOfInputRamWordsVector_[i]
          << ", packets = " << numberOfInputPacketsVector_[i];
      if(numberOfInputPacketsVector_[i]>0) {
        std::cout << ", average size = " << 1.0*numberOfInputRamWordsVector_[i]/numberOfInputPacketsVector_[i] << " RAM words = "
      << 0.064*numberOfInputRamWordsVector_[i]/numberOfInputPacketsVector_[i] << " kbit";

      }
      std::cout << std::endl;
    }

    std::cout << " Number of input checked packets = " << numberOfInputCheckedPackets_ << std::endl;
    for(unsigned i(0);i<numberOfInputCheckedPacketsVector_.size();i++) {
      std::cout << "  Number of input " << i << " checked packets = " << numberOfInputCheckedPacketsVector_[i] << std::endl;
    }

    std::cout << " Number of dropped RAM words = " << numberOfDroppedRamWords_ << std::endl
        << " Number of total dropped packets = " << numberOfDroppedPackets_;
    if(numberOfInputPackets_+numberOfDroppedPackets_>0) {
      std::cout << ", fraction = " << 100.0*numberOfDroppedPackets_/(numberOfInputPackets_+numberOfDroppedPackets_) << " %";
    }
    if(numberOfDroppedPackets_>0) {
      std::cout << ", average size = " << 1.0*numberOfDroppedRamWords_/numberOfDroppedPackets_ << " RAM words = "
                << 0.064*numberOfDroppedRamWords_/numberOfDroppedPackets_ << " kbit";
    }
    std::cout << std::endl;

    for(unsigned i(0);i<numberOfDroppedPacketsVector_.size();i++) {
      std::cout << "  Number of dropped " << i << " RAM words = " << numberOfDroppedRamWordsVector_[i]
          << " packets = " << numberOfDroppedPacketsVector_[i];
      if(numberOfInputPacketsVector_[i]+numberOfDroppedPacketsVector_[i]>0) {
        std::cout << ", fraction = " << 100.0*numberOfDroppedPacketsVector_[i]/(numberOfInputPacketsVector_[i]+numberOfDroppedPacketsVector_[i]) << " %";
      }
      if(numberOfDroppedPacketsVector_[i]>0) {
        std::cout << ", average size = " << 1.0*numberOfDroppedRamWordsVector_[i]/numberOfDroppedPacketsVector_[i] << " RAM words = "
      << 0.064*numberOfDroppedRamWordsVector_[i]/numberOfDroppedPacketsVector_[i] << " kbit";
      }
      std::cout << std::endl;
    }
  
    std::cout << " Number of timeout RAM words = " << numberOfTimeoutRamWords_ << std::endl
        << " Number of total timeout packets = " << numberOfTimeoutPackets_;
    if(numberOfInputPackets_+numberOfTimeoutPackets_>0) {
      std::cout << ", fraction = " << 100.0*numberOfTimeoutPackets_/(numberOfInputPackets_+numberOfTimeoutPackets_) << " %";
    }
    if(numberOfTimeoutPackets_>0) {
      std::cout << ", average size = " << 1.0*numberOfTimeoutRamWords_/numberOfTimeoutPackets_ << " RAM words = "
                << 0.064*numberOfTimeoutRamWords_/numberOfTimeoutPackets_ << " kbit";
    }
    std::cout << std::endl;

    for(unsigned i(0);i<numberOfTimeoutPacketsVector_.size();i++) {
      std::cout << "  Number of timeout " << i << " RAM words = " << numberOfTimeoutRamWordsVector_[i]
          << " packets = " << numberOfTimeoutPacketsVector_[i];
      if(numberOfInputPacketsVector_[i]+numberOfTimeoutPacketsVector_[i]>0) {
        std::cout << ", fraction = " << 100.0*numberOfTimeoutPacketsVector_[i]/(numberOfInputPacketsVector_[i]+numberOfTimeoutPacketsVector_[i]) << " %";
      }
      if(numberOfTimeoutPacketsVector_[i]>0) {
        std::cout << ", average size = " << 1.0*numberOfTimeoutRamWordsVector_[i]/numberOfTimeoutPacketsVector_[i] << " RAM words = "
      << 0.064*numberOfTimeoutRamWordsVector_[i]/numberOfTimeoutPacketsVector_[i] << " kbit";
      }
      std::cout << std::endl;
    }
  
    std::cout << " Input buffers" << std::endl;
    for(unsigned i(0);i<sizeOfInputBufferVector_.size();i++) {
      std::cout << "  Input buffer " << i << " average size = " << 1.0*sizeOfInputBufferVector_[i]/numberOfTicks_
          << ", maximum = " << maxOfInputBufferVector_[i] << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << " OUTPUT SECTION: Max possible data rate = " << 0.064*280.0 << " Gbit/s" << std::endl;
    std::cout << "  Number of output backpressure ticks = "
	      << std::setw(10) << numberOfOutputBackpressureTicks_ << " = "
	      << 100.0*countsFraction(numberOfOutputBackpressureTicks_,numberOfTicks_) << " %" << std::endl;
    std::cout << "  Number of output no data ticks      = "
	      << std::setw(10) << numberOfOutputNoDataTicks_ << " = "
	      << 100.0*countsFraction(numberOfOutputNoDataTicks_,numberOfTicks_) << " %" << std::endl;
    std::cout << "  Number of output data ticks         = "
	      << std::setw(10) << numberOfOutputDataTicks_ << " = "
	      << 100.0*countsFraction(numberOfOutputDataTicks_,numberOfTicks_) << " %" << std::endl;
    std::cout << "  Number of output RAM words = " << numberOfOutputRamWords_ << std::endl;
    std::cout << "  Data rate = " << 0.064*280.0*countsFraction(numberOfOutputRamWords_,numberOfTicks_)
	      << ", duty cycle = " << 100.0*countsFraction(numberOfOutputRamWords_,numberOfTicks_)
	      << " %" << std::endl
              << " Number of output packets = " << numberOfOutputPackets_;
    if(numberOfOutputPackets_>0) {
      std::cout << ", average size = " << 1.0*numberOfOutputRamWords_/numberOfOutputPackets_ << " RAM words = "
                << 0.064*numberOfOutputRamWords_/numberOfOutputPackets_ << " kbit";
    }
    std::cout << std::endl;
    std::cout << " Number of output checked packets = " << numberOfOutputCheckedPackets_ << std::endl;
    std::cout << " Output buffer average size = " << 1.0*sizeOfOutputBuffer_/numberOfTicks_
        << ", maximum = " << maxOfOutputBuffer_ << std::endl;
    
    std::cout << std::endl;
  }

  lpgbtPairBuilderAnalyserPtr_->finalise();
}
