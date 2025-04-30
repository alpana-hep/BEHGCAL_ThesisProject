#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

#include "CentralDaq.h"
#include "CentralDaq.h"


CentralDaq::CentralDaq() :
  Entity("CentralDaq",Clocks::Clock0180),
  builderActive_(false),
  d2sActive_(0),
  iSlink_(0),
  //slinkNumber_(0),
  completeEvent_(false),
  outputTree_(nullptr) {
    outputTree_ = new TTree("CentralDaq", "CentralDaq");
    outputTree_->Branch("EventData", &eventData_);
}

CentralDaq::~CentralDaq() {
}

void CentralDaq::setDefinitionTo(const SectorDefinition &sd) {
  definition_=&sd;

  std::vector<SlinkDefinition> vSd;
  nSlinks_=vSd.size();

  sd.slinkDefinitions(vSd);
  for(unsigned s(0);s<vSd.size();s++) {
    numberOfLpgbtPairsVector_.push_back(vSd[s].numberOfLpgbtPairs());
  }
  /*
    assert(fpgaPtrVector_.size()<2);
    fpgaPtrVector_.push_back(&f);

    const std::vector<SlinkDefinition>& vSd(f.definition().slinkDefinitions());
    for(unsigned s(0);s<vSd.size();s++) {
    dthCheckerPtrVector_.push_back(new DthChecker());
    dthCheckerPtrVector_.back()->setDefinitionTo(vSd[s]);
    //slinkPtrVector_[s]->connect(*(dthCheckerPtrVector_[s]));
    //f.connect(*(dthCheckerPtrVector_[s]));
    }
  */
}

// Arguments
void CentralDaq::setArgumentsTo(const Arguments &a) {
  /*
    for(unsigned f(0);f<fpgaVector_.size();f++) {
    // for(unsigned f(0);f<fpgaPtrVector_.size();f++) {
    //fpgaPtrVector_[f]->setArgumentsTo(a);
    fpgaVector_[f].setArgumentsTo(a);
    }
  */
  Entity::setArgumentsTo(a);
}

// Connections
/*
  void CentralDaq::connect(FastControlDriver &fcd) {
  for(unsigned d(0);d<dthCheckerPtrVector_.size();d++) {
  dthCheckerPtrVector_[d]->connect(fcd);
  }
  }
*/

void CentralDaq::connect(Sector &s) {  
  const std::vector<DaqUnit*> &vA(s.daqUnit());
  for(unsigned u(0);u<vA.size();u++) {    
    const std::vector<D2sUnit*> &vB(vA[u]->d2sUnit());
    for(unsigned d(0);d<vB.size();d++) {
      connect(*(vB[d]));
    }
  }
}

void CentralDaq::connect(D2sUnit &d) {  
  Connection<D2sWord> *w(new Connection<D2sWord>());
  connectRxPortTo(*w);
  d.connectTxPortTo(*w);
}

void CentralDaq::connectRxPortTo(Connection<D2sWord> &w) {
  rxD2sWordPtrVector_.push_back(&w);
}

void CentralDaq::strobeRx() {
  for(unsigned d(0);d<rxD2sWordPtrVector_.size();d++) {
    rxD2sWordPtrVector_[d]->strobeOutput();
  }
}

void CentralDaq::initialise() {
  std::cout << name_ << "::initialise()" << std::endl
	    << " Size of rxD2sWordPtrVector = "
	    << rxD2sWordPtrVector_.size() << std::endl;

  for(unsigned i(0);i<rxD2sWordPtrVector_.size();i++) {
    boolSlinkWordQueueVector_.push_back(std::queue< std::pair<bool,SlinkWord> >());
    numberOfSlinksVector_.push_back((i%5)==4?4:5);
  }
  nSlinks_=numberOfSlinksVector_[0];

  hEventSize_=new TH1I("EventSize",
		       ";Event size (kbytes);Number of events",
		       100,0,1000);
  hDroppedPackets_=new TH1I("DroppedPackets",
		       ";Number of dropped packets;Number of events",
		       100,0,1000);
  hDroppedPacketsVsEventSize_=new TH2I("DroppedPacketsVsEventSize",
				       ";Event size (kbytes);Number of dropped packets;Number of events",
				       100,0,1000,100,0,1000);
  hnumberOfEcondAsicTruncations_ = new TH1I("numberOfEcondAsicTruncations",
					  ";Number of EcondAsic truncations;Number of events",
					  100,0,1000);
  hnumberOfEcondBeTruncations_ = new TH1I("numberOfEcondBeTruncations",
					    ";Number of EcondBe truncations;Number of events",
					    100,0,1000);


}

void CentralDaq::process() {
  subProcessInput();
  /*
    D2sWord dw;
    if(rxD2sWordPtrVector_[0]->get(dw)) {
    if(debugPrint_) dw.print("CENTRAL_DAQ: ");

    if(dw.startBit()) {
    SlinkBoe boe;
    for(unsigned i(0);i<4;i++) boe.setWordTo(i,dw.word(i));
    eventData_.addSlinkBoe(boe);

    LpgbtPairHeader lph;
    for(unsigned i(0);i<2;i++) lph.setWordTo(i,dw.word(i+4));
    eventData_.addLpgbtPairData(lph);

    EcondAsicHeader eah;
    for(unsigned i(0);i<2;i++) eah.setWordTo(i,dw.word(i+6));
    eventData_.addEcondData(eah);
    if(debugPrint_) eventData_.print("CENTRAL_DAQ PART: ");
    }

    if(dw.endBit()) {
    SlinkEoe eoe;
    for(unsigned i(0);i<4;i++) eoe.setWordTo(i,dw.word(i+12));
    eventData_.addSlinkEoe(eoe);
    if(debugPrint_) eventData_.print("CENTRAL_DAQ: ");
    eventData_.clear();
    }
    }
  */
  subProcessBuilder();
  subProcessOutput();
}

/////////////////////////////////////////////////////////////////////////////

void CentralDaq::subProcessInput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput() entering" << std::endl;
  }

  for(unsigned s(0);s<rxD2sWordPtrVector_.size();s++) {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessInput()"
                << " D2S " << s << " has queue size = "
                << boolSlinkWordQueueVector_[s].size() << std::endl;
    }
    
    while(rxD2sWordPtrVector_[s]->get(d2sWord_)) {
      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcessInput()"
		  << " D2S " << s << " word read" << std::endl;
	d2sWord_.print("   ");
      }
      
      unsigned nSlinkWords(4-d2sWord_.paddingCount());
      if(nSlinkWords>4 || nSlinkWords<1) {
	printBxTick();
	assert(false);
      }

      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcessInput()"
		  << " D2S " << s << " number of Slink words = "
		  << nSlinkWords << std::endl;
      }

      for(unsigned i(0);i<nSlinkWords;i++) {
		
	bool flag((i  ==0           && d2sWord_.startBit()) ||
		  (i+1==nSlinkWords && d2sWord_.endBit()  ));	
	

	//bool flag(d2sWord_.startBit() || d2sWord_.endBit());
	//std::pair<bool,SlinkWord> bsw(flag,d2sWord_.slinkWord(i));
	std::pair<bool,SlinkWord> bsw(d2sWord_.boolSlinkWord(i));
	boolSlinkWordQueueVector_[s].push(bsw);

	if(debugPrint_) {
	  std::cout << "   " << name_ << "::subProcessInput()"
		    << " D2S " << s << " wrote flag = "
		    << (flag?"true  ":"false ")
		    << (bsw.first?"true  ":"false ");
	  bsw.second.print();
	  std::cout << std::endl;
	}
      }
    }
  }
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessInput()  exiting" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////

void CentralDaq::subProcessBuilderHalf(uint32_t w0, uint32_t) {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilderHalf() entering" << std::endl;
  }

  if(iPayloadWord_<nPayloadWords_) {
    if(debugPrint_) {
      std::cout << "  Payload word = 0x" << std::hex << std::setfill('0')
		<< std::setw(8) << w0 
		<< std::dec << std::setfill(' ') << std::endl;
    }
    
    //iPayloadWord_+=2;
    iPayloadWord_++;

  } else if((iEcondAsic_/2)<nEcondAsics_) {
      econdAsicHeader_.setWordTo(iEcondAsic_%2,w0);
      //econdAsicHeader_.setWordTo(1,w1);
      if((iEcondAsic_%2)==1) {
	if(debugPrint_) econdAsicHeader_.print("   ");
	eventData_.addEcondData(econdAsicHeader_);
	
	iPayloadWord_=0;
	nPayloadWords_=econdAsicHeader_.payloadLength();
	//if((nPayloadWords_%2)==1) nPayloadWords_++;
      }
      iEcondAsic_++;
      
    } else if((iLpgbtPair_/2)<nLpgbtPairs_) {
      lpgbtPairHeader_.setWordTo(iLpgbtPair_%2,w0);
      //lpgbtPairHeader_.setWordTo(1,w1);
      if((iLpgbtPair_%2)==1) {
	if(debugPrint_) lpgbtPairHeader_.print("  ");
	eventData_.addLpgbtPairData(lpgbtPairHeader_);

	nPayloadWords_=0;
	iEcondAsic_=0;
	nEcondAsics_=lpgbtPairHeader_.numberOfPresentEcondPackets();
      }
      iLpgbtPair_++;
    
  } else {
    if(debugPrint_) {
      std::cout << "  Unknown word = 0x" << std::hex << std::setfill('0')
		<< std::setw(8) << w0 
		<< std::dec << std::setfill(' ') << std::endl;
    }
    
    if(w0!=0) {
      printBxTick();
      assert(false);    
    }
  }
    
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessBuilderHalf()  exiting" << std::endl;
  }
}

  /////////////////////////////////////////////////////////////////////////////

  void CentralDaq::subProcessBuilder() {
    if(debugPrint_) {
      std::cout << " " << name_ << "::subProcessBuilder() entering" << std::endl;
    }

    if(completeEvent_) {
      printBxTick();
      assert(false);
    }

    while(!boolSlinkWordQueueVector_[d2sActive_].empty() && !completeEvent_) {
      if(debugPrint_) {
	std::cout << "  " << name_ << "::subProcessBuilder()"
		  << " D2S " << d2sActive_ << " has queue size = "
		  << boolSlinkWordQueueVector_[d2sActive_].size() << std::endl;
      }

      /*      
	      if(d2sActive_>0) {
	      printBxTick();
	      assert(false);
	      }
      */

      // Check for Slink BOE or EOE
      if(boolSlinkWordQueueVector_[d2sActive_].front().first) {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessBuilder()"
		    << " next word flag = true" << std::endl;
	  boolSlinkWordQueueVector_[d2sActive_].front().second.print();
	}
	
	if(!builderActive_) {
	  SlinkBoe boe((boolSlinkWordQueueVector_[d2sActive_].front().second));
	  boolSlinkWordQueueVector_[d2sActive_].pop();
	  if(debugPrint_) boe.print("  ");
	  eventData_.addSlinkBoe(boe);
	  builderActive_=true;
	
	  nPayloadWords_=0;
	  nEcondAsics_=0;
	  iLpgbtPair_=0;
	  nLpgbtPairs_=boe.numberOfLpgbtPairs();
	  
	  
	} else {
	  SlinkEoe eoe((boolSlinkWordQueueVector_[d2sActive_].front().second));
	  boolSlinkWordQueueVector_[d2sActive_].pop();

	  if(debugPrint_) {
	    std::ostringstream sout;
	    sout << iSlink_ << "/" << nSlinks_ << ": ";
	    eoe.print(sout.str());
	  }
	  
	  eventData_.addSlinkEoe(eoe);

	  builderActive_=false;

	  nPayloadWords_=0;
	  nEcondAsics_=0;
	  nLpgbtPairs_=0;
	  iSlink_++;
	
	  if(iSlink_==nSlinks_) {
	    d2sActive_++;

	    if(d2sActive_==boolSlinkWordQueueVector_.size()) {
	      completeEvent_=true;
	      d2sActive_=0;
	    }
	    
	    iSlink_=0;
	    nSlinks_=numberOfSlinksVector_[d2sActive_];
	  }
	}

      } else {
	if(debugPrint_) {
	  std::cout << "  " << name_ << "::subProcessBuilder()"
		    << " next word flag = false" << std::endl;
	  boolSlinkWordQueueVector_[d2sActive_].front().second.print();
	}
	  /*
	  subProcessBuilderHalf(boolSlinkWordQueueVector_[d2sActive_].front().second.word(0),
	  boolSlinkWordQueueVector_[d2sActive_].front().second.word(1));
	  subProcessBuilderHalf(boolSlinkWordQueueVector_[d2sActive_].front().second.word(2),
	  boolSlinkWordQueueVector_[d2sActive_].front().second.word(3));
	*/
	subProcessBuilderHalf(boolSlinkWordQueueVector_[d2sActive_].front().second.word(0),0);
	subProcessBuilderHalf(boolSlinkWordQueueVector_[d2sActive_].front().second.word(1),0);
	subProcessBuilderHalf(boolSlinkWordQueueVector_[d2sActive_].front().second.word(2),0);
	subProcessBuilderHalf(boolSlinkWordQueueVector_[d2sActive_].front().second.word(3),0);
    
	boolSlinkWordQueueVector_[d2sActive_].pop();
      }
    }

    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessBuilder()"
		<< " D2S " << d2sActive_ << " has queue size = "
		<< boolSlinkWordQueueVector_[d2sActive_].size()
		<< ", stopping builder" << std::endl;
    }
  
    if(debugPrint_) {
      std::cout << " " << name_ << "::subProcessBuilder()  exiting" << std::endl;
    }
  }

/////////////////////////////////////////////////////////////////////////////

void CentralDaq::subProcessOutput() {
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessOutput() entering" << std::endl;
  }

  if(completeEvent_) {
    if(true || debugPrint_) {
      std::cout << "  " << name_ << "::subProcessOutput()"
                << " Event is complete"  << std::endl;
      eventData_.print();

      unsigned s(eventData_.sizeInBytes());
      unsigned t(eventData_.numberOfEcondAsicTruncations()+
		 eventData_.numberOfEcondBeTruncations());
      hEventSize_->Fill(s/1024.0);
      hDroppedPackets_->Fill(t);
      hDroppedPacketsVsEventSize_->Fill(s/1024.0,t);
      unsigned b(eventData_.numberOfEcondAsicTruncations());
      unsigned c (eventData_.numberOfEcondBeTruncations());
      hnumberOfEcondAsicTruncations_->Fill(b);
      hnumberOfEcondBeTruncations_->Fill(c);
	
    }

    outputTree_->Fill();

    //Show event has been processed
    eventData_.clear();
    completeEvent_=false;

  } else {
    if(debugPrint_) {
      std::cout << "  " << name_ << "::subProcessOutput()"
		<< " Event is not yet complete"  << std::endl;
    }
  }
  
  if(debugPrint_) {
    std::cout << " " << name_ << "::subProcessOutput()  exiting" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////

void CentralDaq::print() const {
  std::cout << name_ << "::print()" << std::endl;
}
