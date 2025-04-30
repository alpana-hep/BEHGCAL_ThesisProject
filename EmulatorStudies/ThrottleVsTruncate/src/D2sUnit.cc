#include <iostream>
#include <iomanip>
#include <cassert>

#include "PacketChecker.h"
#include "D2sUnit.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"

D2sUnit::D2sUnit() :
  EntityFastControl("D2sUnit",Clocks::Clock0180),
  //rxSlinkWordPtr_(0),
  txDataPtr_(0),
  //dthCheckerData_.active_(false),
  //dthCheckerData_.numberOfTicks_(0),
  active_(false),
  inputSlink_(0),
  //orbitNumber_(0xffffffff),
  orbitWordCount_(0),
  d2sWordCount_(0),
  wordCountOut_(0),
  numberOfWords_(0),
  numberOfPackets_(0) {
}

D2sUnit::~D2sUnit() {
}

// Initialisation
void D2sUnit::setDefinitionTo(const SlinkDefinition &d) {
  assert(slinkDefinitionVector_.size()<5);
  slinkDefinitionVector_.push_back(d);
}

void D2sUnit::setArgumentsTo(const Arguments &a) {
  d2sBufferSize_=a.config().at("d2sBufferSize").get<unsigned>();
  Entity::setArgumentsTo(a);
  if(idNumber_==a.config().at("d2sUnitIdDebugPrint").get<uint32_t>()) debugPrint_=true;
}

void D2sUnit::initialise() {
  std::cout << "D2sUnit::initialise() "
	    << " Number of Slinks = " << slinkDefinitionVector_.size()
	    << ", labels:" << std::endl;
  for(unsigned s(0);s<slinkDefinitionVector_.size();s++) {
    std::cout << slinkDefinitionVector_[s].label() << std::endl;

    slinkQueueVector_.push_back(std::queue< std::pair<bool,SlinkWord> >());
  }
}

// Connections
void D2sUnit::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  EntityFastControl::connectRxPortTo(*c);
  d.connectTxPortTo(*c);
}

void D2sUnit::connect(Slink &s) {
  Connection< std::pair<bool,SlinkWord> > *w(new Connection< std::pair<bool,SlinkWord> >);
  connectRxPortTo(*w);
  s.connectTxPortTo(*w);
}

// Inputs
/*
  void D2sUnit::connectRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtrVector_&c;
  }
*/
void D2sUnit::connectRxPortTo(Connection< std::pair<bool,SlinkWord> > &w) {
  rxSlinkWordPtrVector_.push_back(&w);
}

void D2sUnit::strobeRx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeRx();
  for(unsigned r(0);r<rxSlinkWordPtrVector_.size();r++) {
    rxSlinkWordPtrVector_[r]->strobeOutput();
  }
}

void D2sUnit::connectTxPortTo(Connection<D2sWord> &d) {
  txDataPtr_=&d;
}

void D2sUnit::strobeTx() {
  if(Clocks::clocks().previousClock0040()) EntityFastControl::strobeTx();
  if(txDataPtr_!=0) txDataPtr_->strobeInput();
}

// Processing
void D2sUnit::process() {
  if(debugPrint_) {
    std::cout << "D2sUnit::process()  entering" << std::endl;
  }
  
  // Check connections are correct

  //assert(rxCommandsPtr_!=0);
  //assert(rxCommandsPtr_->size()==0);
  for(unsigned r(0);r<rxSlinkWordPtrVector_.size();r++) {
    assert(rxSlinkWordPtrVector_[r]!=0);
    //assert(rxSlinkWordPtrVector_[r]->size()==0);
  }
  
  // 40 MHz section
  if(Clocks::clocks().previousClock0040()) subProcess40Mhz(true);
  /*
    FastControlCommands fcc;
    if(rxCommandsPtr_->get(fcc)) { // Only after a 40 MHz tick

    // Update counters
    dthCheckerData_.counters_.update(fcc);

    // Check for L1A
    if(fcc.l1a()) {

    // Add counters to the queue
    l1aCountersQueue_.push(dthCheckerData_.counters_);

    if(debugPrint_) {
    std::cout << " New L1A added, queue size = " << l1aCountersQueue_.size() << " ";
    dthCheckerData_.counters_.print();
    }
    }
    }
  */
  
  // 180 MHz section
  /*  
      std::pair<bool,SlinkWord> bsw;
      if(rxSlinkWordPtrVector_[0]->get(bsw)) {
      //bsw.second.print("D2sUnit ");
      bsw.second.print();

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

      if(debugCheck_) assert(PacketChecker::checkSlinkPacket(slinkDefinitionVector_[0],l1aCountersQueue_.front(),slinkWordVector_,debugPrint_));
      l1aCountersQueue_.pop();
      }
      }
    
      numberOfWords_++;
      }
  */

  subProcessInput();
  subProcessBuilder();
  subProcessD2sOutput();
  
  dthCheckerData_.numberOfTicks_++;

  //if(txDataPtr_!=0) assert(txDataPtr_->set(dthCheckerData_));
  
  if(debugPrint_) {
    std::cout << "D2sUnit::process()  exiting" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////

void D2sUnit::subProcessInput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput() entering" << std::endl;
  }

  std::pair<bool,SlinkWord> bsw;

  for(unsigned s(0);s<slinkQueueVector_.size();s++) {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessInput()"
		<< " Slink " << s << " has queue size = "
		<< slinkQueueVector_[s].size() << std::endl;
    }
      
    if(slinkQueueVector_[s].size()<d2sBufferSize_) {
      if(rxSlinkWordPtrVector_[s]->get(bsw)) {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessInput()"
		    << " Slink " << s << " word read" << std::endl;
	  bsw.second.print();
	}
	
	slinkQueueVector_[s].push(bsw);

      } else {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessInput()"
		    << " Slink " << s << " no word read" << std::endl;
	}
      }
    }
  }  
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()  exiting" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////

void D2sUnit::subProcessBuilder() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilder() entering" << std::endl;
  }

  
  bool done(false);
  d2sWordCount_=0;
  
  for(unsigned i(0);i<4 && !done;i++) {
    if(slinkQueueVector_[inputSlink_].empty()) {
      done=true;
      
      
    } else {
      std::pair<bool,SlinkWord> bsw(slinkQueueVector_[inputSlink_].front());
      slinkQueueVector_[inputSlink_].pop();
      
      d2sWord_.setBoolSlinkWordTo(d2sWordCount_,bsw);
      d2sWordCount_++;

      if(bsw.first) {
	if(active_) inputSlink_++;
	if(inputSlink_==slinkQueueVector_.size()) inputSlink_=0;
	active_=!active_;
      }      
    }
  }

  orbitWordCount_=d2sWordCount_;

  if(d2sWordCount_>0) {
    d2sWord_.setPaddingCountTo(4-d2sWordCount_);
    if(txDataPtr_!=0) assert(txDataPtr_->set(d2sWord_));
  }

#ifdef JUNK

  /*
	  if(orbitNumber_!=sb.oc()) {
	  if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
	  << ", new orbit = " << sb.oc() << ", adding " << orbitWordCount_
	  << " words to queue" << std::endl;
	  }
	  assert(inputSlink_==0);

	  if(orbitWordCount_!=0) wordCountQueue_.push(orbitWordCount_);
	
	  orbitNumber_=sb.oc();
	  orbitWordCount_=8;
	
	  } else {
	  if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
	  << ", continuing orbit = " << sb.oc() << ", words = "
	  << orbitWordCount_ << std::endl;
	  }

	  orbitWordCount_+=8;
	  }
	*/      
  /*
} else {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
		    << ", no data in input queue" << std::endl;
	}

	done=true;
      }

    } else {
      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
		  << ", active" << std::endl;
      }

      if(!slinkQueueVector_[inputSlink_].empty()) {
	if(slinkQueueVector_[inputSlink_].front().first) {
	  SlinkEoe se(slinkQueueVector_[inputSlink_].front().second);

	  if(debugPrint_) {
	    std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
		      << ", EOE found" << std::endl;
	    //se.print("  ");
	    se.print();
	  }

	  if(debugCheck_) {
	    slinkPacket_.push_back(slinkQueueVector_[inputSlink_].front().second);
	    if(debugPrint_) {
	      std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
			<< ", calling PacketCheckZZZ" << std::endl;
	    
	      // CHECK!
	      for(unsigned i(0);i<slinkPacket_.size();i++) {
		slinkPacket_[i].print();
	      }
	    }

	    slinkPacket_.resize(0);
	  }
  */
	  
	/*	  
	  d2sPacketLevel_=0;
	  BasicWord bw;
	  for(unsigned i(0);i<4;i++) {
	    bw.setWordTo(slinkQueueVector_[inputSlink_].front().second.word(i));
	    bw.setEndBitTo(i==3 && inputSlink_==0);
	    cdQueue_.push(bw);
	  }
	*/

	  orbitWordCount_+=8;
	
	  active_=false;

	  if((inputSlink_+1)==slinkQueueVector_.size()) {
	    done=true;
	    d2sWord_.setEndBitTo(true);
	  }
	  /*
	  for(unsigned i(0);i<4;i++) {
	    d2sWord_.setWordTo(4*d2sWordCount_+i,slinkQueueVector_[inputSlink_].front().second.word(i));
	  }
	  */
	  d2sWord_.setBoolSlinkWordTo(d2sWordCount_,slinkQueueVector_[inputSlink_].front());
	  d2sWordCount_++;
	  
	  if(inputSlink_==0 || d2sWordCount_==4) {
	    done=true;
	    if(debugPrint_) d2sWord_.print("  ");
	    if(txDataPtr_!=0) assert(txDataPtr_->set(d2sWord_));
	    
	    d2sWordCount_=0;
	    d2sWord_.clear();
	  }

	  slinkQueueVector_[inputSlink_].pop();
	  inputSlink_=(inputSlink_+1)%slinkQueueVector_.size();
	  
	} else {
	  if(debugPrint_) {
	    std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
		      << ", packet word" << std::endl;
	    //slinkQueueVector_[inputSlink_].front().second.print("  ");
	    slinkQueueVector_[inputSlink_].front().second.print();
	  }
	
	  if(debugCheck_) {
	    slinkPacket_.push_back(slinkQueueVector_[inputSlink_].front().second);
	  }
	  /*
	  if(d2sPacketLevel_==1) {
	    BasicWord bw;
	    for(unsigned i(0);i<4;i++) {
	      bw.setWordTo(slinkQueueVector_[inputSlink_].front().second.word(i));
	      cdQueue_.push(bw);
	    }
	    d2sPacketLevel_=2;
	  }
	  */
	  /*
	    for(unsigned i(0);i<4;i++) {
	    d2sWord_.setWordTo(4*d2sWordCount_+i,slinkQueueVector_[inputSlink_].front().second.word(i));
	    }
	  */
	  d2sWord_.setBoolSlinkWordTo(d2sWordCount_,slinkQueueVector_[inputSlink_].front());

	  d2sWordCount_++;
	  
	  if(d2sWordCount_==4) {
	    done=true;
	    if(debugPrint_) d2sWord_.print("  ");
	    if(txDataPtr_!=0) assert(txDataPtr_->set(d2sWord_));
	    
	    d2sWordCount_=0;
	    d2sWord_.clear();
	  }

	  slinkQueueVector_[inputSlink_].pop();

	  orbitWordCount_+=8;
	}

      } else {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessBuilder()  Slink " << inputSlink_
		    << ", no data in input queue" << std::endl;
	}
	done=true;
      }
    }
  }
  
#endif

  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilder()  exiting" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////

void D2sUnit::subProcessCentralDaqOutput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessCentralDaqOutput() entering" << std::endl;
  }
  /*
    while(!cdQueue_.empty()) {
    if(debugPrint_) {
    std::cout << "CD QUEUE: ";
    cdQueue_.front().print();
    }
    cdQueue_.pop();
    }
  */
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessCentralDaqOutput()  exiting" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////

void D2sUnit::subProcessD2sOutput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessD2sOutput() entering" << std::endl;
  }

  if(wordCountOut_>0) {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessDs2Output()"
		<< " writing words to D2S links, initial count = "
		<< wordCountOut_;
    }
    
    if(wordCountOut_<D2sWordsPerTick) wordCountOut_=0;
    else wordCountOut_-=D2sWordsPerTick;

    if(debugPrint_) {
      std::cout << ", final count = "
		<< wordCountOut_ << std::endl;
    }

  } else {
    if(!wordCountQueue_.empty()) {
      wordCountOut_=wordCountQueue_.front();
      wordCountQueue_.pop();

      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcessD2sOutput()"
		  << " new orbit found with " << wordCountOut_
		  << " words" << std::endl;
      }

    } else {
      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcessD2sOutput()"
		  << " no words to write and queue empty"
		  << std::endl;
      }      
    }
  }
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessD2sOutput()  exiting" << std::endl;
  }
}

void D2sUnit::print() const {
  std::cout << "D2sUnit::print() " << slinkDefinitionVector_[0].label() << std::endl;
  std::cout << " Number of ticks = " << dthCheckerData_.numberOfTicks_
	    << ", total possible rate = " << 128.0*0.120 << " Gbit/s"
	    << ", total possible data = " << 0.000000128*dthCheckerData_.numberOfTicks_
	    << " Gbit" << std::endl;
  if (dthCheckerData_.numberOfTicks_) {
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
    
    std::cout << std::endl;
  }
}
