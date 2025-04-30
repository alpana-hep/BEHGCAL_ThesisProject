#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cassert>

#include "Connection.h"
#include "GlobalConfiguration.h"
#include "FastControlDriver.h"
#include "FastControlDriverAnalyser.h"


FastControlDriver::FastControlDriver() :
  Entity("FastControlDriver",Clocks::Clock0040),
  fastControlDriverAnalyserPtr_(0),
  numberOfHgcrocOverflows_(0),
  numberOfBx_(0),
  numberOfLiveBx_(0),
  numberOfLiveThrottles_(0),
  random_(RandomSingleton::random()) {

  for(unsigned i(0);i<33;i++) {
    commandCount_[i]=0;
  }
  
  for(unsigned i(0);i<17;i++) {
    requestCount_[i]=0;
  }
}

void FastControlDriver::setConfigurationTo(const FastControlDriverConfiguration &c) {
  configuration_=c;
}

void FastControlDriver::setArgumentsTo(const Arguments &a) {
  configuration_.setTriggerMethodTo(a.triggerMethod());
  configuration_.setL1aRateKhzTo(a.triggerRate());
  configuration_.setCalOrbitPeriodTo(a.calOrbitPeriod());
  configuration_.setHllhcFilenameTo(a.config().at("tcdsHllhcFilename").get<std::string>());
  Entity::setArgumentsTo(a);
}

void FastControlDriver::initialiseCollisionBc() {
  
  // No HL-LHC file; use all bunches per orbit
  if(configuration_.hllhcFilename()=="Flat") {
    for(unsigned i(0);i<FastControlCounters::MaximumBc;i++) {
      collisionBc_[i]=true;
    }
    return;
  }

  // Open HL-LHC file
  std::ifstream fin((std::string("data/HL-LHC/")+configuration_.hllhcFilename()+".csv").c_str());
  assert(fin);

  // Set default for collision arrays
  bool bc[2][FastControlCounters::MaximumBc];
  for(unsigned i(0);i<FastControlCounters::MaximumBc;i++) {
    bc[0][i]=false;
    bc[1][i]=false;
  }
  
  std::string buffer;
  unsigned n;

  // Read and discard first line
  getline(fin,buffer);
  
  // Read each beam line and title
  for(unsigned i(0);i<2;i++) {
    getline(fin,buffer);
    
    std::istringstream sin(buffer);
    getline(sin,buffer,',');
    if(i==0) assert(buffer=="B1 filled buckets");
    else     assert(buffer=="B2 filled buckets");
    
    // Read rest of beam data and set array
    while(getline(sin,buffer,',')) {
      std::istringstream sBucket(buffer);
      sBucket >> n;
      
      // Change from 400 MHz to 40 MHz and check if in range
      n/=10;
      assert(n<FastControlCounters::MaximumBc);    
      bc[i][n]=true;
      // std::cout<<"Alpana- FastControlDriver.cc "<<"\t n "<<n<<"\t bc [i] [n] "<<bc[i][n]<<std::endl;
    }
  }

  // Collisions are the AND of both beams
  for(unsigned i(0);i<FastControlCounters::MaximumBc;i++) {
    collisionBc_[i]=bc[0][i] && bc[1][i];
    // std::cout<<"Alpana- FastControlDriver.cc "<<"\t collisionBc_[i] "<<collisionBc_[i]<<std::endl;

  }
}

void FastControlDriver::initialise() {
  std::cout << "FastControlDriver::initialise()" << std::endl;
  const GlobalConfiguration &gc(GlobalConfiguration::configuration());
  gc.print(" ");
  configuration_.print(" ");
  
  // Set up the requests queue
  requests_.setIdle();
  for(unsigned i(0);i<configuration_.requestsQueueSize();i++) requestsQueue_.push(requests_);

  // Set up the commands queue
  tcdsCommands_.setIdle();
  for(unsigned i(0);i<configuration_.commandsQueueSize();i++) commandsQueue_.push(tcdsCommands_);

  // Set up the calibration delay queue
  for(unsigned i(0);i<gc.calibrationQueueSize();i++) calibrationDelayQueue_.push(false);

  // Set up the pre-L1A delay queue;
  commands_.setIdle();
  for(unsigned i(0);i<gc.preL1aQueueSize();i++) preL1aDelayQueue_.push(commands_);

  // Set up the BE delay queue;
  for(unsigned i(0);i<gc.beQueueSize();i++) beDelayQueue_.push(commands_);

  // Set up the bunch structure
  initialiseCollisionBc();

  // Set up the trigger rules vector
  for(unsigned i(0);i<configuration_.triggerRulePeriod();i++) triggerRuleVector_.push_back(false);
  triggerRuleOffset_=0;
  
  // Find L1A probability per BX
  unsigned bcSum(0);
  for(unsigned i(1);i<=FastControlCounters::MaximumBc;i++) {
    if(collisionBc(i)) bcSum++;
  }
  fL1aProbability=configuration_.l1aRateKhz()*FastControlCounters::MaximumBc/(40000.0*bcSum);
  
  std::cout << " Number of collisions per orbit = " << bcSum << std::endl
	    << " Probability per BX of L1A for "
	    << configuration_.l1aRateKhz() << " kHz = " << fL1aProbability
    //<<"  "<< FastControlCounters::MaximumBc <<" FastControlCounters::MaximumBc"
	    << std::endl;
  /*
  std::cout << std::endl;
  bool activeBc(false);
  for(uint16_t b(1);b<=3564;b++) {
    bool cbc(collisionBc(b));
    if(cbc && !activeBc) {
      std::cout << " Collision period starting at bc = " << std::setw(4) << b;
      activeBc=true;
    } else if(!cbc && activeBc) {
      std::cout << ", ending at bc = " << std::setw(4) << b-1 << std::endl;
      activeBc=false;
    }
  }  
  */

  std::cout << " Number of TX command connections = "
	    << txCommandsPtrVector_.size() << std::endl
	    << " Number of RX command connections = "
	    << rxRequestsPtrVector_.size() << std::endl;

  fastControlDriverAnalyserPtr_=new FastControlDriverAnalyser(this);
  fastControlDriverAnalyserPtr_->initialise(doAnalysis_);
}

// Inputs
void FastControlDriver::connectRxPortTo(Connection<FastControlRequests> &r) {
  rxRequestsPtrVector_.push_back(&r);
  r.initialiseQueue(FastControlRequests()); // Idle
}

void FastControlDriver::strobeRx() {
  if(debugPrint_) {
    std::cout << "FastControlDriver::strobeRx()  entering" << std::endl;
  }

  for(unsigned i(0);i<rxRequestsPtrVector_.size();i++) {
    unsigned s(rxRequestsPtrVector_[i]->strobeOutput());
    if(debugPrint_) {
      std::cout << "FastControlDriver::strobeOutput() " << i << " returns " << s << std::endl;
    }
  }
  
  if(debugPrint_) {
    std::cout << "FastControlDriver::strobeRx()   exiting" << std::endl;
  }
}

// Outputs
void FastControlDriver::connectTxPortTo(Connection<FastControlCommands> &c) {
  txCommandsPtrVector_.push_back(&c);
}

void FastControlDriver::connectDelayedTxPortTo(Connection<FastControlCommands> &c) {
  txDelayedCommandsPtrVector_.push_back(&c);
}

void FastControlDriver::strobeTx() {
  if(debugPrint_) {
    std::cout << "FastControlDriver::strobeTx()  entering" << std::endl;
  }
  
  for(unsigned i(0);i<txCommandsPtrVector_.size();i++) {
    unsigned s(txCommandsPtrVector_[i]->strobeInput());
    if(debugPrint_) {
      std::cout << "FastControlDriver::strobeInput() " << i << " returns " << s << std::endl;
    }
  }
  
  for(unsigned i(0);i<txDelayedCommandsPtrVector_.size();i++) {
    unsigned s(txDelayedCommandsPtrVector_[i]->strobeInput());
    if(debugPrint_) {
      std::cout << "FastControlDriver::strobeInput() delayed " << i << " returns " << s << std::endl;
    }
  }
  
  if(debugPrint_) {
    std::cout << "FastControlDriver::strobeTx()   exiting" << std::endl;
  }
}

// Processing
void FastControlDriver::process() {
  if(debugPrint_) {
    std::cout << "FastControlDriver::process()  entering" << std::endl;
  }

  // Check connections are correct
  for(unsigned i(0);i<rxRequestsPtrVector_.size();i++) {
    //assert(rxRequestsPtrVector_[i]->size()==0);
  }
  
  for(unsigned i(0);i<txCommandsPtrVector_.size();i++) {
    if(txCommandsPtrVector_[i]->size()!=0) {
      if(debugPrint_) {
	std::cout << "FastControlDriver::process()  txCommandsPtrVector_["
		  << i << "]->size() = " << txCommandsPtrVector_[i]->size() << std::endl;
      }
      //assert(false);
    }
  }

  for(unsigned i(0);i<txDelayedCommandsPtrVector_.size();i++) {
    if(txDelayedCommandsPtrVector_[i]->size()!=0) {
      if(debugPrint_) {
	std::cout << "FastControlDriver::process()  txDelayedCommandsPtrVector_["
		  << i << "]->size() = " << txDelayedCommandsPtrVector_[i]->size() << std::endl;
      }
      //assert(false);
    }
  }

  // Request handling in HGCAL BE
  subProcessBeRequests();

  // Command handling in TCDS2
  subProcessTcdsCommands();

  // Command handling in HGCAL BE
  subProcessBeCommands();
  
  numberOfBx_++;

  if(debugPrint_) {
    std::cout << "FastControlDriver::process()  exiting" << std::endl;
  }
}

void FastControlDriver::subProcessBeRequests() {
  if(debugPrint_) {
    std::cout << " FastControlDriver::subProcessBeRequests()  entering" << std::endl;
  }

  // Reset the requests
  requests_.setIdle();

  // Add HGCROC buffer limiting
  if(configuration_.hgcrocBufferLimit()>0) {
    if(hgcrocModel_.occupancy()>configuration_.hgcrocBufferLimit()) {
      requests_.setHgcrocBeThrottleTo(true);
      if(debugPrint_) {
	std::cout << " Throttling due to HGCROC buffer limit, occupancy = " << hgcrocModel_.occupancy() << " ";
	requests_.print();
      }
    }
  }

  // OR throttle requests from all FastControlStreams
  FastControlRequests req;  
  for(unsigned i(0);i<rxRequestsPtrVector_.size();i++) {

    // Read in requests
    assert(rxRequestsPtrVector_[i]->get(req));

    if(debugPrint_) {
      std::cout << "   Request " << i << " ";
      req.print();
      
      if(req.hgcrocBeThrottle()) {
	std::cout << "   Throttle raised by HGCROC BE " << i << std::endl;
      }
      
      if(req.econdAsicThrottle()) {
	std::cout << "   Throttle raised by ECON-D ASIC " << i << std::endl;
      }
      
      if(req.econdBeThrottle()) {
	std::cout << "   Throttle raised by ECON-D BE " << i << std::endl;
      }
    }      
    
    requests_.merge(req);
  }

  if(debugPrint_) {std::cout << "  Merged request ";requests_.print();}
  
  // Keep some statistics
  uint16_t w(requests_.word());
  for(unsigned i(0);i<16;i++) {
    if((w&(1<<i))>0) requestCount_[i]++;
  }
  if(requests_.throttle()) requestCount_[16]++;
 
  requestsQueue_.push(requests_);
 
  if(debugPrint_) {
    std::cout << " FastControlDriver::subProcessRequests()  exiting" << std::endl;
  }
}

void FastControlDriver::subProcessTcdsCommands() {
  if(debugPrint_) {
    std::cout << " FastControlDriver::subProcessTcdsCommands()  entering" << std::endl;
  }

  // Reset the commands
  tcdsCommands_.setIdle();

  // Do orbit and event counter resets on first command to synch whole system
  if(numberOfBx_==0) {
    tcdsCommands_.setOrbitCounterResetTo(true);
    tcdsCommands_.setEventCounterResetTo(true);
    tcdsCounters_.update(tcdsCommands_);
    tcdsCounters_.setBcTo(0);
  }
  
  // Check for HGCROC buffer going full
  if(debugPrint_) {
    std::cout << "  HGCROC buffer occupancy = "
	      << hgcrocModel_.occupancy() << std::endl;
  }

  // Check on HGCROC model occupancy if not using explicit limit
  if(hgcrocModel_.fullBuffer() && configuration_.hgcrocBufferLimit()==0) {
    tcdsCommands_.setHgcrocTcdsThrottleTo(true);
    if(debugPrint_) std::cout << "  Throttle raised by HGCROC TCDS" << std::endl;
  }
  
  // Handle requests from BE
  tcdsRequests_=requestsQueue_.front();
  requestsQueue_.pop();
  if(debugPrint_) {std::cout << "  ";tcdsRequests_.print();}
  
  if(tcdsRequests_.hgcrocBeThrottle() ) tcdsCommands_.setHgcrocBeThrottleTo(true);
  if(tcdsRequests_.econdAsicThrottle()) tcdsCommands_.setEcondAsicThrottleTo(true);
  if(tcdsRequests_.econdBeThrottle()  ) tcdsCommands_.setEcondBeThrottleTo(true);

  // Handle trigger rule
  if(triggerRule()) tcdsCommands_.setTriggerRuleThrottleTo(true);
  
  // Get next bc value
  uint16_t bc(tcdsCounters_.bc()+1);
  if(bc>FastControlCounters::MaximumBc) bc=1;
  if(debugPrint_) {std::cout << "  Update bc = " << bc << " ";tcdsCounters_.print();}

  // Identify if a collision
  if(collisionBc(bc)) {
    tcdsCommands_.setCollisionBcTo(true);
    numberOfLiveBx_++;
  }

  // On start of each orbit, do bunch counter reset
  if(bc==1) tcdsCommands_.setBunchCounterResetTo(true);

  // On central BX in abort gap, do calibration
  if(configuration_.calOrbitPeriod()>0) {
    if(bc==3504-calibrationDelayQueue_.size()) {
      if((tcdsCounters_.oc()%configuration_.calOrbitPeriod())==0) tcdsCommands_.setCalPrepareTo(true);
    }
  }
  
  // Find out if there is a trigger (if not throttled)
  if(!tcdsCommands_.throttle()) {
    if(tcdsCommands_.collisionBc()) {
      if(trigger()) tcdsCommands_.setL1aTo(true);
    }
    
    // If a calibration was fired earlier, then do the corresponding L1A
    if(calibrationDelayQueue_.front()) {
      tcdsCommands_.setL1aTo(true);
    }
  }

  // Update calibration queue
  calibrationDelayQueue_.push(tcdsCommands_.calPrepare());

  if(calibrationDelayQueue_.front()) {
    tcdsCommands_.setCalL1aIntTo(true); // Use Int for now
  }
  
  calibrationDelayQueue_.pop();
  
  // Update the trigger rule vector
  triggerRuleVector_[triggerRuleOffset_]=tcdsCommands_.l1a();
  triggerRuleOffset_=(triggerRuleOffset_+1)%configuration_.triggerRulePeriod();
  
  // Update the local counters and HGCROC buffer
  tcdsCounters_.update(tcdsCommands_);
  if(debugPrint_) {std::cout << "  Update counters, bc = " << bc << " ";tcdsCounters_.print();}
  assert(bc==tcdsCounters_.bc());
  
  // Decide if fat event
  if(tcdsCommands_.l1a()) {
    if(random_.Uniform()<configuration_.fatEventProbability()) tcdsCommands_.setFatEventTo(true);
  }
  
  // Count HGCROC buffer overflows
  if(hgcrocModel_.fullBuffer() && tcdsCommands_.l1a()) {
    numberOfHgcrocOverflows_++;
    if(debugPrint_) {
      std::cout << " HGCROC buffer overflow, total seen = "
		<< numberOfHgcrocOverflows_ << std::endl;
    }
  }

  hgcrocModel_.update(tcdsCommands_);
  
  if(debugPrint_) {
    std::cout << "  TCDS commands = ";
    tcdsCommands_.print();
    std::cout << "  TCDS counters = ";
    tcdsCounters_.print();
  }
  
  // Insert into delay queue
  commandsQueue_.push(tcdsCommands_);

  if(debugPrint_) {
    std::cout << " FastControlDriver::subProcessTcdsCommands()  exiting" << std::endl;
  }
}

void FastControlDriver::subProcessBeCommands() {
  if(debugPrint_) {
    std::cout << " FastControlDriver::subProcessBeCommands()  entering" << std::endl;
  }

  // Get TCDS commands
  FastControlCommands tcdsComm(commandsQueue_.front());
  commandsQueue_.pop();
 
  // Put commands through pre-L1A delay
  preL1aDelayQueue_.push(tcdsComm);
  commands_=preL1aDelayQueue_.front();
  preL1aDelayQueue_.pop();

  // Add pre-L1A if needed
  if(tcdsComm.l1a()) commands_.setPreL1aTo(true);

  // Update counters
  counters_.update(commands_);

  if(debugPrint_) {
    std::cout << " Commands = ";
    commands_.print();
    std::cout << " Counters = ";
    counters_.print();
  }
  
  // Send out the commands
  for(unsigned i(0);i<txCommandsPtrVector_.size();i++) {
    assert(txCommandsPtrVector_[i]->set(commands_));
  }

  // Put commands through BE delay
  beDelayQueue_.push(tcdsComm);
  delayedCommands_=beDelayQueue_.front();
  beDelayQueue_.pop();

  // Send out the delayed commands
  for(unsigned i(0);i<txDelayedCommandsPtrVector_.size();i++) {
    assert(txDelayedCommandsPtrVector_[i]->set(delayedCommands_));
  }
  
  // Analysis
  if(doAnalysis_) fastControlDriverAnalyserPtr_->analyse();

  // Keep some statistics
  uint32_t w(commands_.word());
  for(unsigned i(0);i<32;i++) {
    if((w&(1<<i))>0) commandCount_[i]++;
  }
  if(commands_.throttle()) {
    commandCount_[32]++;
    if(commands_.collisionBc()) numberOfLiveThrottles_++;
  }
  
  if(debugPrint_) {
    std::cout << " FastControlDriver::subProcessBeCommands()  exiting" << std::endl;
  }
}

bool FastControlDriver::trigger() {
  bool l1a(false);
  
  if((configuration_.triggerMethod()/1000)==1) {
    // Burst at the beginning
    l1a=numberOfBx_>=5 && numberOfBx_<11;

  } else if((configuration_.triggerMethod()/1000)==2) {
    // Even spacing
    //l1a=(numberOfBx_%50)==49;
    unsigned period(configuration_.triggerMethod()%1000);
    l1a=(numberOfBx_%period)+1==period;

  } else if((configuration_.triggerMethod()/1000)==3) {
    // Continuous
    l1a=numberOfBx_>10;

  } else { // Default = physical; make realistic wrt orbit structure
    l1a=random_.Uniform()<fL1aProbability;
  }
  return l1a;
}

bool FastControlDriver::abortGap(uint16_t bc) {
  return bc>FastControlCounters::MaximumBc-120;
}

bool FastControlDriver::collisionBc(uint16_t bc) {

  // Abort gap
  //if(abortGap(bc)) return false; DISABLE FOR TEST

  assert(bc>0);
  assert(bc<=FastControlCounters::MaximumBc);

  return collisionBc_[bc-1];

  /* 2880 collisions
  // SPS injections; 4xPS + 32 empty
  if(bc>3360) return false; // Cludge to cut off upper end of injections

  unsigned bcSps((bc-1)%336);
  if(bcSps>=304) return false;

  // PS injections; 72 + 4 empty
  unsigned bcPs(bcSps%76);
  if(bcPs>=72) return false;
  */

  // 2748 collisions

  /*
    Train  1: BX   56 --   67 (12 bunches)
    Train  2: BX  109 --  180 (72 bunches)
    Train  3: BX  188 --  259 (72 bunches)
    Train  4: BX  267 --  338 (72 bunches)
    Train  5: BX  346 --  417 (72 bunches)
    Train  6: BX  449 --  520 (72 bunches)
    Train  7: BX  528 --  599 (72 bunches)
    Train  8: BX  607 --  678 (72 bunches)
    Train  9: BX  686 --  757 (72 bunches)
    Train 10: BX  809 --  880 (72 bunches)
    Train 11: BX  888 --  959 (72 bunches)
    Train 12: BX  991 -- 1062 (72 bunches)
    Train 13: BX 1070 -- 1141 (72 bunches)
    Train 14: BX 1149 -- 1220 (72 bunches)
    Train 15: BX 1228 -- 1299 (72 bunches)
    Train 16: BX 1331 -- 1402 (72 bunches)
    Train 17: BX 1410 -- 1481 (72 bunches)
    Train 18: BX 1489 -- 1560 (72 bunches)
    Train 19: BX 1568 -- 1639 (72 bunches)
    Train 20: BX 1703 -- 1774 (72 bunches)
    Train 21: BX 1782 -- 1853 (72 bunches)
    Train 22: BX 1885 -- 1956 (72 bunches)
    Train 23: BX 1964 -- 2035 (72 bunches)
    Train 24: BX 2043 -- 2114 (72 bunches)
    Train 25: BX 2122 -- 2193 (72 bunches)
    Train 26: BX 2225 -- 2296 (72 bunches)
    Train 27: BX 2304 -- 2375 (72 bunches)
    Train 28: BX 2383 -- 2454 (72 bunches)
    Train 29: BX 2462 -- 2533 (72 bunches)
    Train 30: BX 2597 -- 2668 (72 bunches)
    Train 31: BX 2676 -- 2747 (72 bunches)
    Train 32: BX 2779 -- 2850 (72 bunches)
    Train 33: BX 2858 -- 2929 (72 bunches)
    Train 34: BX 2937 -- 3008 (72 bunches)
    Train 35: BX 3016 -- 3087 (72 bunches)
    Train 36: BX 3119 -- 3190 (72 bunches)
    Train 37: BX 3198 -- 3269 (72 bunches)
    Train 38: BX 3277 -- 3348 (72 bunches)
    Train 39: BX 3356 -- 3427 (72 bunches)
  */

  // Catch exception of small batch at beginning of orbit
  if(bc<56) return false;
  if(bc<68) return true;
  
  uint16_t bcoMajor;
  uint16_t loCut;

  // Four major periods of variable length
  if(bc<765) {
    bcoMajor=bc+255;
    loCut=84;

  } else if(bc<1646) {
    bcoMajor=bc-627;
    loCut=809;

  } else if(bc<2540) {
    bcoMajor=bc-1521;
    loCut=1703;

  } else {
    bcoMajor=bc-2415;
    loCut=2597;
  }

  // Get rid of variable empty bunches at the start
  if(bc<loCut) return false;
  
  // Three minor periods, each 340 BX
  uint16_t bcMinor(bcoMajor%340);
  if(bcMinor<24) return false;
  
  // Four batchs, each 80 BX = 72 live + 7 empty
  uint16_t bcBatch((bcMinor-24)%79);
  return bcBatch<72;
}

bool FastControlDriver::triggerRule() const {
  unsigned n(0);
  for(unsigned i(0);i<triggerRuleVector_.size();i++) {
    if(triggerRuleVector_[i]) n++;
  }
  assert(n<=configuration_.triggerRuleLimit());
  return n==configuration_.triggerRuleLimit();
}

void FastControlDriver::print() const {
  std::cout << "FastControlDriver::print()"
	    << "  Number of BX = " << numberOfBx_
	    << ", HL-LHC run time = " << 0.000000025*numberOfBx_	
	    << " sec" << std::endl
	    << "  Final counters ";
  counters_.print();
  
  std::cout << " Counts of fast control commands" << std::endl;

  for(unsigned i(0);i<FastControlCommands::Range;i++) {
    std::cout << "  " << FastControlCommands::commandName((FastControlCommands::Command)i) << " = " << std::setw(10) << commandCount_[i] << std::endl;
  }
  
  std::cout << std::endl << " Counts of fast control checks/cheats" << std::endl;
  for(unsigned i(16);i<FastControlCommands::ExtendedRange;i++) {
    std::cout << "  " << FastControlCommands::commandName((FastControlCommands::Command)i) << " = "<< std::setw(10) << commandCount_[i] << std::endl;
  }
  std::cout << "  Overall throttle    = " << std::setw(10) << commandCount_[32] << std::endl;

  std::cout << std::endl << " Counts of fast control requests" << std::endl;
  for(unsigned i(0);i<FastControlRequests::Range;i++) {
    std::cout << "  " << FastControlRequests::requestName((FastControlRequests::Request)i) << "   = "<< std::setw(10) << requestCount_[i] << std::endl;
  }
  std::cout << "  Overall throttle    = "<< std::setw(10) << requestCount_[16] << std::endl;

  std::cout << std::endl
	    << " Number of live BX = " << numberOfLiveBx_ << std::endl
	    << " Number of throttles = " << commandCount_[32] << ", deadtime = "
	    << (1.0*commandCount_[32])/numberOfBx_ << std::endl;
  std::cout << " Number of live throttles = " << numberOfLiveThrottles_;
  if(numberOfLiveBx_>0) {
    std::cout << ", live deadtime = "
	      << (1.0*numberOfLiveThrottles_)/numberOfLiveBx_;
  }
  std::cout << std::endl
	    << " Number of HGCROC overflows = " << numberOfHgcrocOverflows_;
  if(commandCount_[FastControlCommands::L1a]>0) {
    std::cout << ", L1A fraction loss = "
	      << 1.0*numberOfHgcrocOverflows_/commandCount_[FastControlCommands::L1a];
  }
  std::cout << std::endl;
  std::cout << std::endl;

  fastControlDriverAnalyserPtr_->finalise();
}
