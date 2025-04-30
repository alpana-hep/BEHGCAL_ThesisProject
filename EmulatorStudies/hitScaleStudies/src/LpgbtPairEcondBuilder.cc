/*
  Takes stream of 2x32-bit elink pairs and extracts ECON-D packets.

  The ECON-D packets without idle words are packed into 72-bit RAM words
  and sent on to the main buffer. 

  This class uses a state machine to handle the idle followed by header
  requirement on either of the two elink words.

  The idle words are be used to monitor the occupancy of the ECON-D
  buffers so this entity will request throttling if this buffer gets
  close to full.

  The entity can also throttle due to the main buffer going too full. It
  also cannot write into the buffer if it is too full, hence discarding the
  packet. It always writes or discards full packets.
*/

#include <iostream>
#include <iomanip>
#include <cassert>

#include "LpgbtPairEcondBuilder.h"
#include "PacketChecker.h"
#include "LpgbtPairEcondBuilderAnalyser.h"

std::string LpgbtPairEcondBuilder::stateName_[NumberOfStates]={
  "WaitForIdle",
  "WaitForHeader",
  "PayloadA",
  "PayloadB",
  "EndAA",
  "EndAB",
  "EndBA",
  "EndBB"
};

LpgbtPairEcondBuilder::LpgbtPairEcondBuilder() :
  EntityFastControl("LpgbtPairEcondBuilder",Clocks::Clock0280),
  rxElinkPairPtr_(0),
  txRamWordPtr_(0),
  lpgbtPairEcondBuilderAnalyserPtr_(0),
  state_(State::WaitForIdle),
  waitForIdleCounter_(0),
  error_(true),
  arraySaveValid_(false),
  econdBufferStatus_(EcondAsicIdle::Empty),
  discardingPacket_(false),
  econdAsicThrottle_(false),
  econdBeThrottle_(false),
  packetOutMinDelay_(0xffffffff),

  numberOfTicks_(0),
  numberOf40MhzTicks_(0),

  numberOfInputWords_(0),
  numberOfInputIdleWords_(0),
  numberOfInputPacketWords_(0),
  numberOfInputErrorWords_(0),

  numberOfStartedPackets_(0),
  numberOfFinishedPackets_(0),
  numberOfCheckedPackets_(0),
  numberOfStoredPackets_(0),
  numberOfDiscardedPackets_(0),

  numberOfOutputRamWords_(0),

  maxMainBufferLevel_(0),
  numberOfEcondAsicThrottles_(0),
  numberOfEcondBeThrottles_(0) {
  
  for(unsigned i(0);i<4;i++) numberOfEcondBufferStatus_[i]=0;
  for(unsigned i(0);i<5;i++) numberOfMainBufferLevel_[i]=0;
}

LpgbtPairEcondBuilder::~LpgbtPairEcondBuilder() {
}

// Initialisation
void LpgbtPairEcondBuilder::setDefinitionTo(const EcondAsicDefinition &ead) {
  definition_=ead;
}

void LpgbtPairEcondBuilder::setConfigurationTo(const LpgbtPairEcondBuilderConfiguration &c) {
  configuration_=c;
}

LpgbtPairEcondBuilderConfiguration& LpgbtPairEcondBuilder::configuration() {
  return configuration_;
}

void LpgbtPairEcondBuilder::setArgumentsTo(const Arguments &a) {
  /*
    configuration_.setPhaseOf40MHzTo(a.config().at("lpgbtPairEcondBuilderPhaseOf40MHz").get<unsigned>());
    configuration_.setL1aTimeoutTo(a.config().at("lpgbtPairEcondBuilderL1aTimeout").get<unsigned>());
  */
  configuration_.setMediumBufferFactorTo(a.config().at("lpgbtPairEcondBuilderMediumBufferFactor").get<unsigned>());
  configuration_.setHighBufferFactorTo(a.config().at("lpgbtPairEcondBuilderHighBufferFactor").get<unsigned>());
  configuration_.setAlmostFullBufferSpaceTo(a.config().at("lpgbtPairEcondBuilderAlmostFullBufferSpace").get<unsigned>());
  configuration_.setEbrWaitTo(a.config().at("lpgbtPairEcondBuilderEbrWait").get<unsigned>());
  
  Entity::setArgumentsTo(a);
  if(idNumber_==a.config().at("lpebIdDebugPrint").get<uint32_t>()) debugPrint_=true;
}

void LpgbtPairEcondBuilder::initialise() {
  std::cout << name_ << "::initialise()" << std::endl;
  definition_.print(" ");
  idleWord_=definition_.idleWord();

  configuration_.print(" ");
  
  mediumBufferLevel_=definition_.numberOfElinks()*configuration_.mediumBufferFactor();
  highBufferLevel_=definition_.numberOfElinks()*configuration_.highBufferFactor();
  fullBufferLevel_=definition_.numberOfElinks()*configuration_.fullBufferFactor_;
  almostFullBufferLevel_=fullBufferLevel_-configuration_.almostFullBufferSpace();
  
  std::cout << "      Medium buffer level = " << mediumBufferLevel_ << std::endl
	    << "        High buffer level = " << highBufferLevel_ << std::endl
	    << " Almost full buffer level = " << almostFullBufferLevel_ << std::endl
	    << "        Full buffer level = " << fullBufferLevel_ << std::endl;

  txRamWordPtr_->setMaxSizeTo(fullBufferLevel_);
  
  lpgbtPairEcondBuilderAnalyserPtr_=new LpgbtPairEcondBuilderAnalyser(this);
  lpgbtPairEcondBuilderAnalyserPtr_->initialise(doAnalysis_);

  assert(rxCommandsPtr_!=0);
  assert(txRequestsPtr_!=0);
  assert(rxElinkPairPtr_!=0);
}

// Connections

void LpgbtPairEcondBuilder::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  EntityFastControl::connectRxPortTo(*c);
  d.connectTxPortTo(*c);
  c->setMaxSizeTo(100);
  c->initialiseQueue(FastControlCommands(),54+(definition_.numberOfElinks()==1?1:0)); // Idles, should be 54
  
  Connection<FastControlRequests> *r(new Connection<FastControlRequests>);
  EntityFastControl::connectTxPortTo(*r);
  r->initialiseQueue(FastControlRequests()); // Idle
  d.connectRxPortTo(*r);
}

void LpgbtPairEcondBuilder::connect(FastControlStream &) {
}

void LpgbtPairEcondBuilder::connectRxPortTo(Connection< std::array<uint32_t,2> > &a) {
  rxElinkPairPtr_=&a;
}

void LpgbtPairEcondBuilder::strobeRx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeRx();

  rxElinkPairPtr_->strobeOutput();
}

void LpgbtPairEcondBuilder::connectTxPortTo(Connection<RamWord> &w) {  
  txRamWordPtr_=&w;
}

void LpgbtPairEcondBuilder::strobeTx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeTx();
  
  if(txRamWordPtr_!=0) txRamWordPtr_->strobeInput();
}

bool LpgbtPairEcondBuilder::consistentWithIdle(uint32_t w) {
  if((w&0xffffff00)!=idleWord_) return false;

  EcondAsicIdle eai(w);
  if(debugPrint_) eai.print("  ");
  numberOfInputIdleWords_++;

  setEcondAsicThrottle(eai.bufferStatus());
  numberOfEcondBufferStatus_[eai.bufferStatus()]++;

  return true;
}

bool LpgbtPairEcondBuilder::consistentWithHeader(uint32_t w) {
  return (w>>23)==EcondAsicHeader::HeaderPattern;
}

// Processing
void LpgbtPairEcondBuilder::process() {
  //if(idNumber_==0x004008ff && Clocks::clocks().bxNumber()>=4000) debugPrint_=true;
  
  if(debugPrint_) {
    std::cout << name_ << "::process()  entering" << std::endl;
  }
  
  // Check for 40 MHz tick since last processing
  if(Clocks::clocks().previousClock0040()) {

    // Check requests which were just sent
    if(requests_.econdAsicThrottle()) numberOfEcondAsicThrottles_++;
    if(requests_.econdBeThrottle()) numberOfEcondBeThrottles_++;
    
    // 40 MHz processing
    subProcess40Mhz(debugCheck_);
    numberOf40MhzTicks_++;
  }
  
  subProcessInput();
  if(validArray_) {    
    subProcessBuilder();
    if(validRamWord_) {
      subProcessOutput();
    }
  }

  ////////////////////////////////////////////////////
  
  // Set throttle due to main buffer occupancy
  unsigned l(txRamWordPtr_==0?0:txRamWordPtr_->size());
  setEcondBeThrottle(l);
  
  // Keep some statistics
  if(maxMainBufferLevel_<l) maxMainBufferLevel_=l;
  
  if(l>=highBufferLevel_) numberOfMainBufferLevel_[2]++;
  else if(l>=mediumBufferLevel_) numberOfMainBufferLevel_[1]++;
  else numberOfMainBufferLevel_[0]++;
  
  if(l>=fullBufferLevel_) numberOfMainBufferLevel_[3]++;
  if(l>=almostFullBufferLevel_) numberOfMainBufferLevel_[4]++;
  
  // Set throttle requests to go to TCDS2
  // These are updated every tick but only sent at 40 MHz
  requests_.setEcondAsicThrottleTo(econdAsicThrottle_);
  requests_.setEcondBeThrottleTo(econdBeThrottle_);
  
  // Analyse results
  if(lpgbtPairEcondBuilderAnalyserPtr_!=0) lpgbtPairEcondBuilderAnalyserPtr_->analyse();
  
  numberOfTicks_++;
  
  if(debugPrint_) {
    std::cout << name_ << "::process()  exiting" << std::endl;
  }
}

// Subprocessing
void LpgbtPairEcondBuilder::subProcessInput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()  entering" << std::endl;
  }
  
  // Check link is OK
  if(rxElinkPairPtr_->size()>0) {
    printBxTick();
    assert(false);
  }

  // Get the next pair of elink words (if any)
  if(rxElinkPairPtr_->get(array_)) {
    if(debugPrint_) {
      std::cout << "  Processing ECON-D, array words = 0x"
		<< std::hex << std::setfill('0')
		<< std::setw(8) << array_[0] << ", 0x"
		<< std::setw(8) << array_[1]
		<< std::dec << std::setfill(' ')
		<< std::endl;
    }

    validArray_=true;
    numberOfInputWords_+=2;

  } else {
    if(debugPrint_) {
      std::cout << "  Processing ECON-D, no array words"
		<< std::endl;
    }
    
    validArray_=false;
  }

  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()   exiting" << std::endl;
  }
}

void LpgbtPairEcondBuilder::subProcessBuilder() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilder()  entering" << std::endl;
  }

  assert(validArray_);
  ramWord_.clearBits();
  validRamWord_=true;
    
  // Catch EBR
  if(commands_.eventBufferReset()) {
    waitForIdleCounter_=configuration_.ebrWait();
    state_=WaitForIdle;

    if(debugPrint_) {
      std::cout << " Event buffer reset: state_ = " << state_
		<< " = " << stateName_[state_]
		<< ", wait for idle counter = "
		<< waitForIdleCounter_
		<< std::endl;
    }
  }

  if(debugPrint_) {
    std::cout << " State machine start: state_ = " << state_
	      << " = " << stateName_[state_]
	      << std::endl;
  }

  // Process state machine
  switch(state_) {

  case WaitForIdle:
    {	
      validRamWord_=false;

      // Allow delay before starting wait
      if(waitForIdleCounter_>0) {
	waitForIdleCounter_--;

	numberOfInputErrorWords_+=2;
	
	if(debugCheck_) {
	  printBxTick();
	  assert(false);
	}
	
      } else {
	  
	if(consistentWithIdle(array_[0])) {

	  // First word is idle
	  if(consistentWithIdle(array_[1])) {

	    // Second word is idle as well
	    state_=WaitForHeader;

	    if(debugPrint_) {
	      std::cout << " Two idles seen in WaitForIdle" << std::endl;
	    }
	      
	  } else if(consistentWithHeader(array_[1])) {	    

	    // Second word is header
	    startPacketB();
	    
	    if(debugPrint_) {
	      std::cout << " Idle and header seen in WaitForIdle" << std::endl;
	    }
	      
	  } else {	    

	    // Second word is unknown
	    if(debugPrint_) {
	      std::cout << " Idle and unknown seen in WaitForIdle" << std::endl;
	    }
	      
	    numberOfInputErrorWords_++;
	      
	    if(debugCheck_) {
	      printBxTick();
	      assert(false);
	    }
	  }
	    
	} else {

	  // First word is non-idle
	  if(consistentWithIdle(array_[1])) {

	    // Second word is idle
	    if(debugPrint_) {
	      std::cout << " Non-idle and idle seen in WaitForIdle" << std::endl;
	    }
	    
	    numberOfInputErrorWords_++;
	    
	    if(debugCheck_) {
	      printBxTick();
	      assert(false);
	    }
	    
	    state_=WaitForHeader;
	    
	  } else {
	    
	    // Second word is non-idle
	    if(debugPrint_) {
	      std::cout << " Two non-idles seen in WaitForIdle" << std::endl;
	    }
	    
	    numberOfInputErrorWords_+=2;
	    
	    if(debugCheck_) {
	      printBxTick();
	      assert(false);
	    }
	  }
	}
      }
	
      break;
    }
    
  case WaitForHeader:
    {	
      validRamWord_=false;
      
      if(consistentWithIdle(array_[0])) {

	// First word is idle
	if(consistentWithIdle(array_[1])) {

	  // Second word is also idle
	  if(debugPrint_) {
	    std::cout << " Two idles seen in WaitForHeader" << std::endl;
	  }

	} else if(consistentWithHeader(array_[1])) {

	  // Second word is header
	  startPacketB();
	  
	  if(debugPrint_) {
	    std::cout << " Idle and header seen in WaitForHeader" << std::endl;
	  }

	} else {

	  // Second word is unknown
	  if(debugPrint_) {
	    std::cout << " Idle and unknown seen in WaitForHeader" << std::endl;
	  }
	    
	  numberOfInputErrorWords_++;
	    
	  if(debugCheck_) {
	    printBxTick();
	    assert(false);
	  }
	    
	  state_=WaitForIdle;
	}
	  
      } else if(consistentWithHeader(array_[0])) {

	// First word is header
	econdAsicHeader_.setWordTo(0,array_[0]);
	econdAsicHeader_.setWordTo(1,array_[1]);
	  
	if(!econdAsicHeader_.validCrc()) {
	  if(debugPrint_) {
	    std::cout << " Header and any seen in WaitForHeader" << std::endl;
	    econdAsicHeader_.print("  ");
	    std::cout << "Invalid CRC so dropping packet" << std::endl;
	  }

	  numberOfInputErrorWords_++;
	    
	  if(debugCheck_) {
	    printBxTick();
	    assert(false);
	  }
	    
	  state_=WaitForIdle;
	  break;
	}
	  
	ramWord_.setStartBit();
	ramWord_.setWordTo(0,array_[0]);
	ramWord_.setWordTo(1,array_[1]);
	arraySaveValid_=false;
	validRamWord_=true;
	  
	numberOfInputPacketWords_+=2;

	wordCount_=econdAsicHeader_.payloadLength();

	if(wordCount_==0) {
	  ramWord_.setEndBit();
	  numberOfFinishedPackets_++;

	  state_=WaitForIdle;
	    
	} else {

	  state_=PayloadA;	  
	  if(wordCount_==1) state_=EndAA;
	  if(wordCount_==2) state_=EndAB;
	}
	  
	if(debugPrint_) {
	  std::cout << " Header and any seen in WaitForHeader" << std::endl;
	  econdAsicHeader_.print("  ");
	}
	  
      } else {

	// First word is unknown
	numberOfInputErrorWords_++;

	if(debugCheck_) {
	  printBxTick();
	  assert(false);
	}

	if(consistentWithIdle(array_[1])) {

	  // Second word is idle
	  if(debugPrint_) {
	    std::cout << " Unknown and idle seen in WaitForHeader" << std::endl;
	  }

	} else {
	  
	  // Second word is non-idle
	  if(debugPrint_) {
	    std::cout << " Unknown and non-idle in WaitForHeader" << std::endl;
	  }
	  
	  state_=WaitForIdle;
	}
      }

      break;
    }

  case PayloadA:
    {    
      ramWord_.setWordTo(0,array_[0]);
      ramWord_.setWordTo(1,array_[1]);
      arraySaveValid_=false;
	
      wordCount_-=2;	
      numberOfInputPacketWords_+=2;
	
      if(debugPrint_) {
	std::cout << " PayloadA: wordCount = " << wordCount_ << std::endl;
      }
	
      if(wordCount_==0) {
	printBxTick();
	assert(false);
      }
	
      if(wordCount_==1) state_=EndAA;
      if(wordCount_==2) state_=EndAB;

      break;
    }
      
  case EndAA:
    {
      ramWord_.setEndBit();
      ramWord_.setPaddingBit();
      ramWord_.setWordTo(0,array_[0]);
      ramWord_.setWordTo(1,0);
      arraySaveValid_=false;
	
      numberOfInputPacketWords_++;
      numberOfFinishedPackets_++;

      // Check if remaining word is idle
      if(consistentWithIdle(array_[1])) {	    
	state_=WaitForHeader;
	    
	if(debugPrint_) {
	  std::cout << " Idle seen in EndAA" << std::endl;
	}
	  
      } else {
	if(debugPrint_) {
	  std::cout << " Non-idle seen in EndAA" << std::endl;
	}
	  
	numberOfInputErrorWords_++;
	  
	if(debugCheck_) {
	  printBxTick();
	  assert(false);
	}
	
	state_=WaitForIdle;
      }
      
      break;
    }

  case EndAB:
    {
      ramWord_.setEndBit();
      ramWord_.setWordTo(0,array_[0]);
      ramWord_.setWordTo(1,array_[1]);
      arraySaveValid_=false;
	
      numberOfInputPacketWords_+=2;
      numberOfFinishedPackets_++;
	
      state_=WaitForIdle;

      break;
    }

  case PayloadB:
    {
      if(wordCount_==econdAsicHeader_.payloadLength()+2) {
	econdAsicHeader_.setWordTo(1,array_[0]);
	ramWord_.setStartBit();
	  
	if(debugPrint_) {
	  std::cout << " Completion of header in PayloadB" << std::endl;
	  econdAsicHeader_.print("  ");
	}

	if(!econdAsicHeader_.validCrc()) {
	  if(debugPrint_) {
	    std::cout << "Invalid CRC so dropping packet" << std::endl;
	  }

	  state_=WaitForIdle;
	  break;
	}
      }

      ramWord_.setWordTo(0,arraySave_);
      ramWord_.setWordTo(1,array_[0]);
      arraySave_=array_[1];
      arraySaveValid_=true;
	
      wordCount_-=2;	
      numberOfInputPacketWords_+=2;
	
      if(debugPrint_) {
	std::cout << " PayloadB: wordCount = " << wordCount_ << std::endl;
      }
	
      if(wordCount_==0) {
	printBxTick();
	assert(false);
      }

      if(wordCount_==1) state_=EndBA;
      if(wordCount_==2) state_=EndBB;

      break;
    }


  case EndBA:
    {
      ramWord_.setEndBit();
      ramWord_.setPaddingBit();
      ramWord_.setWordTo(0,arraySave_);
      ramWord_.setWordTo(1,0);
      arraySaveValid_=false;

      numberOfFinishedPackets_++;

      if(consistentWithIdle(array_[0])) {
	    
	if(consistentWithIdle(array_[1])) {
	    
	  state_=WaitForHeader;
	    
	  if(debugPrint_) {
	    std::cout << " Two idles seen in EndBA" << std::endl;
	  }
	  
	} else if(consistentWithHeader(array_[1])) {

	  // Second word is header
	  startPacketB();

	  if(debugPrint_) {
	    std::cout << " Idle and header seen in EndBA" << std::endl;
	  }

	} else {
	  if(debugPrint_) {
	    std::cout << " Idle and unknown seen in EndBA" << std::endl;
	  }

	  numberOfInputErrorWords_++;
	    
	  if(debugCheck_) {
	    printBxTick();
	    assert(false);
	  }
	    
	  state_=WaitForIdle;
	}
	  
      } else if(consistentWithHeader(array_[0])) {
	// SHOULD NEVER HAPPEN
	printBxTick();
	assert(false);

	econdAsicHeader_.setWordTo(0,array_[0]);
	econdAsicHeader_.setWordTo(1,array_[1]);

	uint32_t d(econdAsicHeader_.timeDelayFrom(Clocks::clocks().bxNumber()));
	if(packetOutMinDelay_>d) packetOutMinDelay_=d;
	  
	numberOfInputPacketWords_+=2;

	wordCount_=econdAsicHeader_.payloadLength();

	if(debugPrint_) {
	  std::cout << " Header and any seen in EndBA" << std::endl;
	}
	  
	if(wordCount_==0) { //???
	  ramWord_.setEndBit();
	  numberOfFinishedPackets_++;

	  state_=WaitForIdle;
	    
	} else {
	  state_=PayloadA;	  
	}
	  
      } else {
	if(debugPrint_) {
	  std::cout << " Unknown and any seen in EndBA" << std::endl;
	}
	  
	numberOfInputErrorWords_++;
	  
	if(debugCheck_) {
	  printBxTick();
	  assert(false);
	}
	  
	state_=WaitForIdle;
      }

      break;
    }
      
  case EndBB:
    {
      if(wordCount_==econdAsicHeader_.payloadLength()+2) {
	econdAsicHeader_.setWordTo(1,array_[0]);
	ramWord_.setStartBit();
	  
	uint32_t d(econdAsicHeader_.timeDelayFrom(Clocks::clocks().bxNumber()));
	if(packetOutMinDelay_>d) packetOutMinDelay_=d;
	  
	if(debugPrint_) {
	  std::cout << " Completion of header in EndBB" << std::endl;
	  econdAsicHeader_.print("  ");
	  std::cout << "  Delay = " << d << std::endl;
	}
	  
	if(!econdAsicHeader_.validCrc()) {
	  if(debugPrint_) {	
	    std::cout << "Invalid CRC so dropping packet" << std::endl;
	  }

	  state_=WaitForIdle;
	  break;
	}
      }

      ramWord_.setEndBit();
      ramWord_.setWordTo(0,arraySave_);
      ramWord_.setWordTo(1,array_[0]);
      arraySaveValid_=false;
	
      numberOfInputPacketWords_++;
      numberOfFinishedPackets_++;

      if(consistentWithIdle(array_[1])) {
	    
	state_=WaitForHeader;
	    
	if(debugPrint_) {
	  std::cout << " Any and idle seen in EndBB" << std::endl;
	}
	  
      } else {
	if(debugPrint_) {
	  std::cout << " Any and non-idle seen in EndBB" << std::endl;
	}

	numberOfInputErrorWords_++;

	if(debugCheck_) {
	  printBxTick();
	  assert(false);
	}
	  
	state_=WaitForIdle;
      }
	
      break;
    }


  default:
    {
      if(debugPrint_) {
	std::cout << " Unknown state" << std::endl;
      }

      // Should never happen
      printBxTick();
      assert(false);
    }
  }

  if(debugPrint_) {
    std::cout << " State machine end: state_ = " << state_
	      << " = " << stateName_[state_]
	      << std::endl;
  }
    
  ////////////////////////////////////////////////////


  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilder()   exiting" << std::endl;
  }
}

void LpgbtPairEcondBuilder::subProcessOutput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessOutput()  entering" << std::endl;
  }

  assert(validRamWord_);
  numberOfOutputRamWords_++;
  
  // Start of packet
  if(ramWord_.startBit()) {
    numberOfStartedPackets_++;
    
    // Check if main buffer too full
    unsigned s(txRamWordPtr_==0?0:txRamWordPtr_->size());
    discardingPacket_=(s>=almostFullBufferLevel_);
    
    if(debugPrint_) {
      std::cout << " Start of packet: size of TX RAM = " << s;
      if(discardingPacket_) std::cout << ", discarding packet";
      std::cout << std::endl;
    }
  }
  
  // All packets
  if(discardingPacket_) {
    if(debugPrint_) {
      std::cout << " Discarding ";
      ramWord_.print();
    }
    
  } else {
    if(debugPrint_) {
      std::cout << " Sending ";
      ramWord_.print();
    }
    
    if(txRamWordPtr_!=0) {
      if(!txRamWordPtr_->set(ramWord_)) {
	printBxTick();
	assert(false);
      }
    }
  }
  
  // Packet check
  if(debugCheck_) {
    econdPacket72Vector_.push_back(ramWord_);
  }
  
  // End of packet
  if(ramWord_.endBit()) {
    if(discardingPacket_) numberOfDiscardedPackets_++;
    else                  numberOfStoredPackets_++;
    
    if(debugCheck_) {
      if(l1aCountersQueue_.size()==0) {
	printBxTick();
	assert(false);
      }
	
      if(!PacketChecker::checkEcondAsicPacket72(definition_,l1aCountersQueue_.front(),econdPacket72Vector_,debugPrint_)) {
	PacketChecker::checkEcondAsicPacket72(definition_,l1aCountersQueue_.front(),econdPacket72Vector_,true);
	printBxTick();
	assert(false);
      }
      
      numberOfCheckedPackets_++;
      l1aCountersQueue_.pop();	  
      econdPacket72Vector_.resize(0);
      
    } else {
      if(l1aCountersQueue_.size()>0) {
	printBxTick();
	assert(false);
      }
    }
  }

  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessOutput()   exiting" << std::endl;
  }
}


void LpgbtPairEcondBuilder::startPacketB() {
  econdAsicHeader_.setWordTo(0,array_[1]);
  econdAsicHeader_.setWordTo(1,0xffffffff);
  
  arraySave_=array_[1];
  arraySaveValid_=true;
  
  numberOfInputPacketWords_++;
  
  wordCount_=econdAsicHeader_.payloadLength()+2;
  
  state_=PayloadB;
  if(wordCount_==1) state_=EndBA;
  if(wordCount_==2) state_=EndBB;
}

void LpgbtPairEcondBuilder::setEcondAsicThrottle(EcondAsicIdle::BufferStatus s) {
  if(econdAsicThrottle_) econdAsicThrottle_=(s>=EcondAsicIdle::Medium);
  else                   econdAsicThrottle_=(s>=EcondAsicIdle::High  );
}

void LpgbtPairEcondBuilder::setEcondBeThrottle(unsigned b) {
  if(econdBeThrottle_) econdBeThrottle_=(b>=mediumBufferLevel_);
  else                 econdBeThrottle_=(b>=  highBufferLevel_);
}

///////////////////////////////////////////////////////////////////////////////

double LpgbtPairEcondBuilder::fractionOfIdleWords() const {
  if(numberOfInputWords_==0) return 0.0;
  return 1.0*numberOfInputIdleWords_/numberOfInputWords_;
}

double LpgbtPairEcondBuilder::fractionOfPacketWords() const {
  if(numberOfInputWords_==0) return 0.0;
  return 1.0*numberOfInputPacketWords_/numberOfInputWords_;
}

double LpgbtPairEcondBuilder::fractionOfErrorWords() const {
  if(numberOfInputWords_==0) return 0.0;
  return 1.0*numberOfInputErrorWords_/numberOfInputWords_;
}

double LpgbtPairEcondBuilder::fractionOfEcondBufferStatus(EcondAsicIdle::BufferStatus s) const {
  if(numberOfInputIdleWords_==0) return 0.0;
  return 1.0*numberOfEcondBufferStatus_[s]/numberOfInputIdleWords_;
}

double LpgbtPairEcondBuilder::fractionOfCheckedPackets() const {
  if(numberOfFinishedPackets_==0) return 0.0;
  return 1.0*numberOfCheckedPackets_/numberOfFinishedPackets_;
}

double LpgbtPairEcondBuilder::fractionOfStoredPackets() const {
  if(numberOfFinishedPackets_==0) return 0.0;
  return 1.0*numberOfStoredPackets_/numberOfFinishedPackets_;
}

double LpgbtPairEcondBuilder::fractionOfDiscardedPackets() const {
  if(numberOfFinishedPackets_==0) return 0.0;
  return 1.0*numberOfDiscardedPackets_/numberOfFinishedPackets_;
}

double LpgbtPairEcondBuilder::fractionOfMainBufferLevel(unsigned l) const {
  if(numberOfTicks_==0) return 0.0;
  return 1.0*numberOfMainBufferLevel_[l]/numberOfTicks_;
}

void LpgbtPairEcondBuilder::print() const {
  std::cout << name_ << "::print()" << std::endl;
  std::cout << " Number of ticks = " << numberOfTicks_ << std::endl;
  std::cout << " Number of 40 MHz ticks = " << numberOf40MhzTicks_ << std::endl;

  std::cout << " Number of input        words = " << std::setw(10)
	    << numberOfInputWords_ << std::endl;
  std::cout << " Number of input   idle words = " << std::setw(10)
	    << numberOfInputIdleWords_
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfIdleWords() << " %"
	    << std::endl;
  std::cout << " Number of input packet words = " << std::setw(10)
	    << numberOfInputPacketWords_
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfPacketWords() << " %"
	    << std::endl;
  std::cout << " Number of input  error words = " << std::setw(10)
	    << numberOfInputErrorWords_
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfErrorWords() << " %"
	    << std::endl;
  std::cout << " Number of idles with ECON-D buffer  empty = " << std::setw(10)
	    <<  numberOfEcondBufferStatus_[0]
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfEcondBufferStatus(EcondAsicIdle::Empty) << " %"
	    << std::endl
	    << " Number of idles with ECON-D buffer    low = " << std::setw(10)
	    <<  numberOfEcondBufferStatus_[1]
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfEcondBufferStatus(EcondAsicIdle::Low) << " %"
	    << std::endl
	    << " Number of idles with ECON-D buffer medium = " << std::setw(10)
	    <<  numberOfEcondBufferStatus_[2]
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfEcondBufferStatus(EcondAsicIdle::Medium) << " %"
	    << std::endl
	    << " Number of idles with ECON-D buffer   high = " << std::setw(10)
	    <<  numberOfEcondBufferStatus_[3]
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfEcondBufferStatus(EcondAsicIdle::High) << " %"
	    << std::endl;
  
  std::cout << " Number of   started packets = "
	    << std::setw(10) << numberOfStartedPackets_ << std::endl;
  std::cout << " Number of  finished packets = "
	    << std::setw(10) << numberOfFinishedPackets_ << std::endl;
  std::cout << " Number of   checked packets = "
	    << std::setw(10) << numberOfCheckedPackets_
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfCheckedPackets() << " %"
	    << std::endl;
  std::cout << " Number of    stored packets = "
	    << std::setw(10) << numberOfStoredPackets_
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfStoredPackets() << " %"
	    << std::endl;
  std::cout << " Number of discarded packets = "
	    << std::setw(10) << numberOfDiscardedPackets_
	    << ", fraction = " << std::setw(6)
	    << 100.0*fractionOfDiscardedPackets() << " %"
	    << std::endl;
  
  if(numberOfFinishedPackets_>0) {
    std::cout << " Packet average words = " << 1.0*numberOfInputPacketWords_/numberOfFinishedPackets_
	      << " elink words = " << 0.032*numberOfInputPacketWords_/numberOfFinishedPackets_
	      << " kbit";
    std::cout << std::endl;
  }

  //std::cout << " Maximum side FIFO level = " << maxSideFifoLevel_ << std::endl;
  std::cout << " Maximum main buffer level = " << maxMainBufferLevel_ << std::endl;
  std::cout << " Number of ticks with main buffer      low = "
	    << std::setw(10) << numberOfMainBufferLevel_[0] << ", fraction = "
	    << std::setw(6) << 100.0*fractionOfMainBufferLevel(0) << " %"
	    << std::endl
	    << " Number of ticks with main buffer   medium = "
	    << std::setw(10) <<  numberOfMainBufferLevel_[1] << ", fraction = "
	    << std::setw(6) << 100.0*fractionOfMainBufferLevel(1) << " %"
	    << std::endl
	    << " Number of ticks with main buffer     high = "
	    << std::setw(10) <<  numberOfMainBufferLevel_[2] << ", fraction = "
	    << std::setw(6) << 100.0*fractionOfMainBufferLevel(2) << " %"
	    << std::endl
	    << " Number of ticks with main buffer     full = "
	    << std::setw(10) <<  numberOfMainBufferLevel_[3] << ", fraction = "
	    << std::setw(6) << 100.0*fractionOfMainBufferLevel(3) << " %"
	    << std::endl
	    << " Number of ticks with main buffer unusable = "
	    << std::setw(10) <<  numberOfMainBufferLevel_[4] << ", fraction = "
	    << std::setw(6) << 100.0*fractionOfMainBufferLevel(4) << " %"
	    << std::endl;

  std::cout << " Number of output RAM words = "
	    << std::setw(10) << numberOfOutputRamWords_ << std::endl;
  std::cout << " Number of requests with ECON-D ASIC throttle = "
	    << std::setw(6) << numberOfEcondAsicThrottles_ << std::endl
	    << " Number of requests with ECON-D   BE throttle = "
	    << std::setw(6) << numberOfEcondBeThrottles_ << std::endl;
  
  std::cout << std::endl;

  lpgbtPairEcondBuilderAnalyserPtr_->finalise();
}
