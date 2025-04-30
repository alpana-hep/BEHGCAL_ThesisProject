#include <iomanip>
#include <sstream>
#include <string>
#include <cassert>

#include "FastControlDriverAnalyser.h"

FastControlDriverAnalyser::FastControlDriverAnalyser(const FastControlDriver *d) :
  fastControlDriverPtr_(d),
  throttleActive_(0xffffffff),
  activeBxOld_(0),
  numberOfTicks_(0) {
}

void FastControlDriverAnalyser::initialise(bool a) {  
  std::string labelStub("FastControlDriver_");

  if(a) {
    h1HgcrocBuffer_=new TH1D((labelStub+"HgcrocBuffer").c_str(),
			     ";Buffer occupancy",50,0,50);
    h1L1aBcs_=new TH1I((labelStub+"L1aBcs").c_str(),
		       ";BCs of L1As",FastControlCounters::MaximumBc,
		       1,FastControlCounters::MaximumBc+1);
    h1AllBcs_=new TH1I((labelStub+"AllBcs").c_str(),
		       ";BCs",FastControlCounters::MaximumBc,
		       1,FastControlCounters::MaximumBc+1);
    h1CollisionBcs_=new TH1I((labelStub+"CollisionBcs").c_str(),
			     ";BX",FastControlCounters::MaximumBc,
			     1,FastControlCounters::MaximumBc+1);
    
    th1iThrottleLength_= new TH1I((labelStub+"ThrottleLength").c_str(),
				  ";BCs",200,0,200);
    th1iActiveL1a_= new TH1I((labelStub+"ActiveL1a").c_str(),
			     ";Sequential L1As",200,0,200);
    
    for(unsigned i(0);i<FastControlCounters::MaximumBc;i++) {
      if(fastControlDriverPtr_->collisionBc_[i]) h1CollisionBcs_->Fill(i+1);
    }
  }
  
  h1Counts_= new TH1I((labelStub+"Counts").c_str(),
		      ";Fast control bits;Number",128,0,128);
  h1Fractions_= new TH1F((labelStub+"Fractions").c_str(),
		      ";Fast control bits;Fraction",128,0,128);
}

void FastControlDriverAnalyser::analyse() {
  h1HgcrocBuffer_->Fill(fastControlDriverPtr_->hgcrocModel_.occupancy());

  if(activeBxOld_>0) {
    if(fastControlDriverPtr_->hgcrocModel_.activeBx()==0) {
      //std::cout << "Active BX old = " << activeBxOld_ << std::endl;
      assert((activeBxOld_%41)==40);
      th1iActiveL1a_->Fill((activeBxOld_+1)/41);
    }
  }
  activeBxOld_=fastControlDriverPtr_->hgcrocModel_.activeBx();
  
  const FastControlCounters &count(fastControlDriverPtr_->counters_);
  const FastControlRequests &tcdsRequ(fastControlDriverPtr_->tcdsRequests_);
  const FastControlCommands &tcdsComm(fastControlDriverPtr_->tcdsCommands_);
  const FastControlCommands &comm(fastControlDriverPtr_->commands_);
  const FastControlRequests &requ(fastControlDriverPtr_->requests_);

  h1AllBcs_->Fill(count.bc());
  if(comm.l1a()) h1L1aBcs_->Fill(count.bc());

  if(count.oc()<10) {
    if(count.oc()==orbitTh1fL1aVector_.size()) defineOrbitHists(count.oc());
    
    unsigned n(orbitTh1fL1aVector_.size()-1);
    orbitTh1fBcrVector_[n]->SetBinContent(count.bc(),comm.bunchCounterReset()?0.9:0.1);
    orbitTh1fEcrVector_[n]->SetBinContent(count.bc(),comm.eventCounterReset()?1.9:1.1);
    orbitTh1fPreVector_[n]->SetBinContent(count.bc(),comm.preL1a()?2.9:2.1);
    orbitTh1fL1aVector_[n]->SetBinContent(count.bc(),comm.l1a()?3.9:3.1);
    orbitTh1fCliVector_[n]->SetBinContent(count.bc(),comm.calPulseInt()?4.9:4.1);
    orbitTh1fCleVector_[n]->SetBinContent(count.bc(),comm.calPulseExt()?5.9:5.1);
    orbitTh1fThrVector_[n]->SetBinContent(count.bc(),comm.throttle()?6.9:6.1);
    //orbitTh1iHboVector_[n]->SetBinContent(count.bc(),fastControlDriverPtr_->hgcrocModel_.occupancy());
    orbitTh1iHboVector_[n]->SetBinContent(count.bc(),comm.hgcrocTcdsThrottle()?7.9:7.1);
    
    orbitTh1fTcdsL1aVector_[n]->SetBinContent((numberOfTicks_%FastControlCounters::MaximumBc)+1,tcdsComm.l1a()?1:0);
    orbitTh1fTcdsThrVector_[n]->SetBinContent((numberOfTicks_%FastControlCounters::MaximumBc)+1,tcdsRequ.throttle()?1:0);
    orbitTh1fBeL1aVector_[n]->SetBinContent((numberOfTicks_%FastControlCounters::MaximumBc)+1,comm.l1a()?1:0);
    orbitTh1fBeThrVector_[n]->SetBinContent((numberOfTicks_%FastControlCounters::MaximumBc)+1,requ.throttle()?1:0);
  }

  if(throttleActive_==0xffffffff && comm.throttle()) {
    throttleActive_=1;
  } else if(throttleActive_<0xffffffff && comm.throttle()) {
    throttleActive_++;
  } else if(throttleActive_<0xffffffff && !comm.throttle()) {
    th1iThrottleLength_->Fill(throttleActive_);
    throttleActive_=0xffffffff;
  }

  numberOfTicks_++;
}

void FastControlDriverAnalyser::finalise() {
  for(unsigned i(0);i<33;i++) {
    h1Counts_->SetBinContent(i+1,fastControlDriverPtr_->commandCount_[i]);
    if(fastControlDriverPtr_->numberOfBx_>0) {
      h1Fractions_->SetBinContent(i+1,1.0*fastControlDriverPtr_->commandCount_[i]/fastControlDriverPtr_->numberOfBx_);
    }
  }

  for(unsigned i(0);i<17;i++) {
    h1Counts_->SetBinContent(i+65,fastControlDriverPtr_->requestCount_[i]);
    if(fastControlDriverPtr_->numberOfBx_>0) {
      h1Fractions_->SetBinContent(i+65,1.0*fastControlDriverPtr_->requestCount_[i]/fastControlDriverPtr_->numberOfBx_);
    }
  }
}

void FastControlDriverAnalyser::defineOrbitHists(unsigned oc) {
  std::ostringstream sout;
  sout << "FastControlDriver_Orbit" << std::setw(2) << std::setfill('0')
       << oc << "_";
  std::string labelStub(sout.str());
  
  orbitTh1fBcrVector_.push_back(new TH1F((labelStub+"Bcr").c_str(),
					 ";Bunch crossing;Bunch counter reset",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fEcrVector_.push_back(new TH1F((labelStub+"Ecr").c_str(),
					 ";Bunch crossing;Event counter reset",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fPreVector_.push_back(new TH1F((labelStub+"Pre").c_str(),
					 ";Bunch crossing;L1Accept",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fL1aVector_.push_back(new TH1F((labelStub+"L1a").c_str(),
					 ";Bunch crossing;L1Accept",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fCliVector_.push_back(new TH1F((labelStub+"Cli").c_str(),
					 ";Bunch crossing;L1Accept",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fCleVector_.push_back(new TH1F((labelStub+"Cle").c_str(),
					 ";Bunch crossing;L1Accept",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fThrVector_.push_back(new TH1F((labelStub+"Thr").c_str(),
					 ";Bunch crossing;Throttle",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1iHboVector_.push_back(new TH1I((labelStub+"Hbo").c_str(),
					 ";Bunch crossing;HGCROC occupancy",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fTcdsL1aVector_.push_back(new TH1F((labelStub+"TcdsL1a").c_str(),
					 ";Bunch crossing;L1Accept",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fTcdsThrVector_.push_back(new TH1F((labelStub+"TcdsThr").c_str(),
					 ";Bunch crossing;Throttle",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fBeL1aVector_.push_back(new TH1F((labelStub+"BeL1a").c_str(),
					 ";Bunch crossing;L1Accept",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
  orbitTh1fBeThrVector_.push_back(new TH1F((labelStub+"BeThr").c_str(),
					 ";Bunch crossing;Throttle",
					 FastControlCounters::MaximumBc,
					 1,FastControlCounters::MaximumBc+1));
}
