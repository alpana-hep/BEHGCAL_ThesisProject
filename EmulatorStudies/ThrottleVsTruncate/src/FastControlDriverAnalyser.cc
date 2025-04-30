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
  a =true;
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
  h1Counts_liveL1As_= new TH1I((labelStub+"Counts_liveL1As").c_str(),
                      ";Live throttle bit;Number",20,0,20);

  h1Fractions_= new TH1F((labelStub+"Fractions").c_str(),
		      ";Fast control bits;Fraction",128,0,128);
  h1Fractions_liveThrottle_= new TH1F((labelStub+"LiveBX_Fractions").c_str(),
			 ";Live throttle bit;Fraction",20,0,20);  

  h1Throttle_bits_ = new TH1I((labelStub+"Throttle_bits").c_str(),
			     "; Type of Throttling; BX Counts",10,0,10);
  h1LiveThrottle_bits_ = new TH1I((labelStub+"LiveThrottle_bits").c_str(),
                             "; Type of Throttling; BX Counts",10,0,10);
  
  h2Throttle_bits_HGCROCvsECOND_ = new TH2I((labelStub+"Throttle_bits_HGCROCvsECOND").c_str(),"; HGCROC throttling; ECON-D throttling",2,0,2,2,0,2);
  h2Throttle_bits_HGCROCvsBE_ = new TH2I((labelStub+"Throttle_bits_HGCROCvsBE").c_str(),"; HGCROC throttling; BE throttling",2,0,2,2,0,2);
  h2Throttle_bits_ECONDvsBE_ = new TH2I((labelStub+"Throttle_bits_ECONDvsBE").c_str(),"; ECON-D throttling; BE throttling",2,0,2,2,0,2);   
  h2LiveThrottle_bits_HGCROCvsECOND_ = new TH2I((labelStub+"LiveThrottle_bits_HGCROCvsECOND").c_str(),"; HGCROC throttling; ECON-D throttling",2,0,2,2,0,2);
  h2LiveThrottle_bits_HGCROCvsBE_ = new TH2I((labelStub+"LiveThrottle_bits_HGCROCvsBE").c_str(),"; HGCROC throttling; BE throttling",2,0,2,2,0,2);
  h2LiveThrottle_bits_ECONDvsBE_ = new TH2I((labelStub+"LiveThrottle_bits_ECONDvsBE").c_str(),"; ECON-D throttling; BE throttling",2,0,2,2,0,2);
  h1_deadTimewithTime_ = new TH1D((labelStub+"deadTimewithTime").c_str(),";Busy time (in Bx); counts",1000,0,500000);
  h2d_deadTimeVsTime_ = new TH2D((labelStub+"deadTimeVsTime").c_str(),";Bx number;Busy time (in Bx)",1000,0,500000,1000,0,500000);    				  
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

  h1_deadTimewithTime_->Fill(fastControlDriverPtr_->numberOfLiveThrottles_);
  h2d_deadTimeVsTime_->Fill(fastControlDriverPtr_->numberOfLiveBx_,fastControlDriverPtr_->numberOfLiveThrottles_);
  int throttle_bit = 0, liveThrottle_bit=0;
  // if(comm.hgcrocTcdsThrottle()) throttle_bit = 1;//0xC1;
  if(comm.hgcrocBeThrottle()) throttle_bit=1;
  if(comm.econdAsicThrottle()) throttle_bit=2;
  if(comm.econdAsicThrottle() && comm.hgcrocBeThrottle()) throttle_bit=3;
  if(comm.econdBeThrottle()) throttle_bit=4;
  if(comm.econdBeThrottle() && comm.hgcrocBeThrottle()) throttle_bit=5;
  if(comm.econdBeThrottle() && comm.econdAsicThrottle()) throttle_bit=6;
  if(comm.econdAsicThrottle() && comm.econdBeThrottle() && comm.hgcrocBeThrottle()) throttle_bit=7;
  h1Throttle_bits_->Fill(throttle_bit);
  numberOfTicks_++;
  h2Throttle_bits_HGCROCvsECOND_->Fill(comm.hgcrocBeThrottle(),comm.econdAsicThrottle()); // 
  h2Throttle_bits_HGCROCvsBE_->Fill(comm.hgcrocBeThrottle(),comm.econdBeThrottle());
  h2Throttle_bits_ECONDvsBE_->Fill(comm.econdAsicThrottle(),comm.econdBeThrottle());

  h2LiveThrottle_bits_HGCROCvsECOND_->Fill(fastControlDriverPtr_->LiveThrottles_Hgcroc,fastControlDriverPtr_->LiveThrottles_Econd);
  h2LiveThrottle_bits_HGCROCvsBE_->Fill(fastControlDriverPtr_->LiveThrottles_Hgcroc,fastControlDriverPtr_->LiveThrottles_BE);
  h2LiveThrottle_bits_ECONDvsBE_->Fill(fastControlDriverPtr_->LiveThrottles_Econd,fastControlDriverPtr_->LiveThrottles_BE);
  if(fastControlDriverPtr_->LiveThrottles_Hgcroc) liveThrottle_bit=1;
  if(fastControlDriverPtr_->LiveThrottles_Econd) liveThrottle_bit=2;
  if(fastControlDriverPtr_->LiveThrottles_Hgcroc && fastControlDriverPtr_->LiveThrottles_Econd) liveThrottle_bit=3;
  if(fastControlDriverPtr_->LiveThrottles_BE) liveThrottle_bit=4;
  if(fastControlDriverPtr_->LiveThrottles_BE && fastControlDriverPtr_->LiveThrottles_Hgcroc) liveThrottle_bit=5;
  if(fastControlDriverPtr_->LiveThrottles_BE && fastControlDriverPtr_->LiveThrottles_Econd) liveThrottle_bit=6;
  if(fastControlDriverPtr_->LiveThrottles_BE && fastControlDriverPtr_->LiveThrottles_Econd && fastControlDriverPtr_->LiveThrottles_Hgcroc) liveThrottle_bit=7;
  
  h1LiveThrottle_bits_->Fill(liveThrottle_bit);
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
  h1Fractions_liveThrottle_->SetBinContent(1,1.0*(fastControlDriverPtr_->numberOfLiveBx_/fastControlDriverPtr_->numberOfBx_));
  h1Fractions_liveThrottle_->SetBinContent(2,1.0*(fastControlDriverPtr_->numberOfLiveThrottles_/fastControlDriverPtr_->numberOfLiveBx_));
  h1Fractions_liveThrottle_->SetBinContent(3,1.0*fastControlDriverPtr_->numberOfLiveThrottles_Econd_/fastControlDriverPtr_->numberOfLiveBx_);
  h1Fractions_liveThrottle_->SetBinContent(4,1.0*fastControlDriverPtr_->numberOfLiveThrottles_Hgcroc_/fastControlDriverPtr_->numberOfLiveBx_);
  h1Fractions_liveThrottle_->SetBinContent(5,1.0*fastControlDriverPtr_->numberOfLiveThrottles_BE_/fastControlDriverPtr_->numberOfLiveBx_);
  h1Fractions_liveThrottle_->SetBinContent(7,1.0*(fastControlDriverPtr_->numberOfL1As_/fastControlDriverPtr_->numberOfLiveBx_));
  h1Fractions_liveThrottle_->SetBinContent(8,1.0*(fastControlDriverPtr_->numberOfL1As_wThrottle_/fastControlDriverPtr_->numberOfL1As_));
  h1Fractions_liveThrottle_->SetBinContent(9,1.0*(fastControlDriverPtr_->numberOfL1As_sent_/fastControlDriverPtr_->numberOfL1As_));

  h1Counts_liveL1As_->SetBinContent(1,fastControlDriverPtr_->numberOfLiveBx_);
  h1Counts_liveL1As_->SetBinContent(2,fastControlDriverPtr_->numberOfLiveThrottles_);
  h1Counts_liveL1As_->SetBinContent(3,fastControlDriverPtr_->numberOfLiveThrottles_Econd_);
  h1Counts_liveL1As_->SetBinContent(4,fastControlDriverPtr_->numberOfLiveThrottles_Hgcroc_);
  h1Counts_liveL1As_->SetBinContent(5,fastControlDriverPtr_->numberOfLiveThrottles_BE_);
  h1Counts_liveL1As_->SetBinContent(7,fastControlDriverPtr_->numberOfL1As_);
  h1Counts_liveL1As_->SetBinContent(8,fastControlDriverPtr_->numberOfL1As_wThrottle_);
  h1Counts_liveL1As_->SetBinContent(9,fastControlDriverPtr_->numberOfL1As_sent_);
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
