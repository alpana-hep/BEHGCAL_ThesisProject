#include <string>

#include "LpgbtPairEcondBuilderAnalyser.h"

bool LpgbtPairEcondBuilderAnalyser::staticsInitialised_(false);

TH1I *LpgbtPairEcondBuilderAnalyser::h1EcondOrThr_(0);
TH1I *LpgbtPairEcondBuilderAnalyser::h1EcondBufferLuv_(0);
TH1F *LpgbtPairEcondBuilderAnalyser::h1EcondErrorFractionLuv_(0);
TH2F *LpgbtPairEcondBuilderAnalyser::h1L1aMatchTime_[8]={0,0,0,0,0,0,0,0};
TH1I *LpgbtPairEcondBuilderAnalyser::h1PacketOutMinDelayLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1PacketWordsFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1ErrorWordsFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1DiscardedPacketsFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1MainBufferLowFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1MainBufferMediumFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1MainBufferHighFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1MainBufferAlmostFullFractionLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1EcondAsicThrottleNumberLuv_;
TH1F *LpgbtPairEcondBuilderAnalyser::h1EcondBeThrottleNumberLuv_;

LpgbtPairEcondBuilderAnalyser::LpgbtPairEcondBuilderAnalyser(const LpgbtPairEcondBuilder *e) :
  lpgbtPairEcondBuilderPtr_(e),
  econdAsicDefinitionPtr_(&(e->definition_)) {
  //stateOld_(LpgbtPairEcondBuilder::WaitForIdle),
  //stateOlder_(LpgbtPairEcondBuilder::WaitForIdle),
  //l1aStateOld_(LpgbtPairEcondBuilder::Waiting),
}

void LpgbtPairEcondBuilderAnalyser::initialise(bool a) {
  std::string labelStub("LpgbtPairEcondBuilder_");

  if(!staticsInitialised_) {
    staticsInitialised_=true;

    h1EcondOrThr_=new TH1I((labelStub+"EcondOrThr").c_str(),
			   ";ECON-D OR throttle",
			   3564,0,3564);
    
    h1EcondBufferLuv_=new TH1I((labelStub+"BufferLuv").c_str(),
			       ";LUV;Max main buffer occupancy (72-bit words)",
			       256*52,0,256*52);
    
    h1EcondErrorFractionLuv_=new TH1F((labelStub+"ErrorFractionLuv").c_str(),
			       ";LUV;Fraction of error input words",
			       256*52,0,256*52);
    
    h1L1aMatchTime_[0]=new TH2F((labelStub+"L1aMatchTime0").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[1]=new TH2F((labelStub+"L1aMatchTime1").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[2]=new TH2F((labelStub+"L1aMatchTime2").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[3]=new TH2F((labelStub+"L1aMatchTime3").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[4]=new TH2F((labelStub+"L1aMatchTime4").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[5]=new TH2F((labelStub+"L1aMatchTime5").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[6]=new TH2F((labelStub+"L1aMatchTime6").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    h1L1aMatchTime_[7]=new TH2F((labelStub+"L1aMatchTime7").c_str(),
				";LUV;Match time (280 MHz ticks)",
				256*52,0,256*52,15,0,15);
    
    h1PacketOutMinDelayLuv_=new TH1I((labelStub+"PacketOutMinDelayLuv").c_str(),
                                  ";LUV;Packet out minimum delay (BX)",
                                  256*52,0,256*52);


    h1PacketWordsFractionLuv_=new TH1F((labelStub+"PacketWordsFractionLuv").c_str(),
				       ";LUV;Fraction of packet words",
				       256*52,0,256*52);
    h1ErrorWordsFractionLuv_=new TH1F((labelStub+"ErrorWordsFractionLuv").c_str(),
				      ";LUV;Fraction of error words",
				       256*52,0,256*52);
    h1DiscardedPacketsFractionLuv_=new TH1F((labelStub+"DiscardedPacketsFractionLuv").c_str(),
					    ";LUV;Fraction of discarded packets",
				       256*52,0,256*52);
    h1MainBufferLowFractionLuv_=new TH1F((labelStub+"MainBufferLowFractionLuv").c_str(),
					 ";LUV;Fraction of ticks with main buffer low",
				       256*52,0,256*52);
    h1MainBufferMediumFractionLuv_=new TH1F((labelStub+"MainBufferMediumFractionLuv").c_str(),
					    ";LUV;Fraction of ticks with main buffer medium",
				       256*52,0,256*52);
    h1MainBufferHighFractionLuv_=new TH1F((labelStub+"MainBufferHighFractionLuv").c_str(),
					  ";LUV;Fraction of ticks with main buffer high",
					  256*52,0,256*52);
    h1MainBufferAlmostFullFractionLuv_=new TH1F((labelStub+"MainBufferAlmostFullFractionLuv").c_str(),
						";LUV;Fraction of ticks with main buffer almost full",
				       256*52,0,256*52);
    h1EcondAsicThrottleNumberLuv_=new TH1F((labelStub+"EcondAsicThrottleNumberLuv").c_str(),
					   ";LUV;Number of ECON-D ASIC throttles",
				       256*52,0,256*52);
    h1EcondBeThrottleNumberLuv_=new TH1F((labelStub+"EcondBeThrottleNumberLuv").c_str(),
					 ";LUV;Number of ECON-D BE throttles",
				       256*52,0,256*52);
  }

  nonStaticsInitialised_=a;

  if(a) {
    labelStub+=econdAsicDefinitionPtr_->label()+"_";

    h1L1aQueueSize_=new TH1I((labelStub+"L1aQueueSize").c_str(),
			     ";L1A queue size",100,0,100);
    h1EcondL1a_=new TH1I((labelStub+"EcondL1a").c_str(),
			 ";ECON-D header",3564,0,3564);
    h1EcondThr_=new TH1I((labelStub+"EcondThr").c_str(),
			 ";ECON-D throttle",3564,0,3564);
    h1EcondBufferStatus_=new TH1I((labelStub+"EcondBufferStatus").c_str(),
				  ";ECON-D buffer status",16,0,16);
    h1EcondBuffer_[0]=new TH1F((labelStub+"LpgbtPairEcondBuffer0").c_str(),
			       ";Buffer occupancy (72-bit words)",100,0,500);
    h1EcondBuffer_[1]=new TH1F((labelStub+"LpgbtPairEcondBuffer1").c_str(),
			       ";Buffer occupancy (72-bit words)",100,0,5000);
  }
}

void LpgbtPairEcondBuilderAnalyser::analyse() {
  unsigned s(lpgbtPairEcondBuilderPtr_->txRamWordPtr_->size());
  
  if(nonStaticsInitialised_) {
    if((lpgbtPairEcondBuilderPtr_->numberOfTicks_%7)==0) {
      h1L1aQueueSize_->Fill(lpgbtPairEcondBuilderPtr_->l1aCountersQueue_.size());
    }
    
    if((lpgbtPairEcondBuilderPtr_->numberOfTicks_/7)<3564) {
      /*
      if(lpgbtPairEcondBuilderPtr_->state_==LpgbtPairEcondBuilder::HeaderA2 ||
	 lpgbtPairEcondBuilderPtr_->state_==LpgbtPairEcondBuilder::HeaderB1)
	h1EcondL1a_->SetBinContent((lpgbtPairEcondBuilderPtr_->numberOfTicks_/7)+1,1);
      
      if(lpgbtPairEcondBuilderPtr_->econdBufferStatus_!=EcondAsicIdle::Low)
	h1EcondThr_->SetBinContent((lpgbtPairEcondBuilderPtr_->numberOfTicks_/7)+1,1);
      */
    }
    
    h1EcondBufferStatus_->Fill(lpgbtPairEcondBuilderPtr_->econdBufferStatus_);
    
    h1EcondBuffer_[0]->Fill(s);
    h1EcondBuffer_[1]->Fill(s);
  }

  if((lpgbtPairEcondBuilderPtr_->numberOfTicks_/7)<3564) {
      if(lpgbtPairEcondBuilderPtr_->econdBufferStatus_!=EcondAsicIdle::Low)
	h1EcondOrThr_->SetBinContent((lpgbtPairEcondBuilderPtr_->numberOfTicks_/7)+1,1);
  }

  unsigned luv(lpgbtPairEcondBuilderPtr_->definition_.luv());
  if(h1EcondBufferLuv_->GetBinContent(luv+1)<s) {
    h1EcondBufferLuv_->SetBinContent(luv+1,s);
  }
  
  /*
  if(lpgbtPairEcondBuilderPtr_->l1aState_==LpgbtPairEcondBuilder::Matched &&
     l1aStateOld_!=LpgbtPairEcondBuilder::Matched) {
     //     (l1aStateOld_==LpgbtPairEcondBuilder::Waiting ||
     // l1aStateOld_==LpgbtPairEcondBuilder::Timing)) {
    
    if(lpgbtPairEcondBuilderPtr_->debugPrint_)
       std::cout << "Analyser LUV " << luv << ", number of elinks = "
		  << econdAsicDefinitionPtr_->numberOfElinks()
		  << ", l1aTime_ = " << lpgbtPairEcondBuilderPtr_->l1aTimer_
		  << std::endl;

    //if(stateOlder_==LpgbtPairEcondBuilder::Idle &&
    // stateOld_==LpgbtPairEcondBuilder::Idle) {
      h1L1aMatchTime_[0]->Fill(luv,lpgbtPairEcondBuilderPtr_->l1aTimer_);
      h1L1aMatchTime_[econdAsicDefinitionPtr_->numberOfElinks()]->Fill(luv,lpgbtPairEcondBuilderPtr_->l1aTimer_);
      //}
  }
  stateOlder_=stateOld_;
  stateOld_=lpgbtPairEcondBuilderPtr_->state_;
  
  l1aStateOld_=lpgbtPairEcondBuilderPtr_->l1aState_;
  */
}

void LpgbtPairEcondBuilderAnalyser::finalise() {    
  unsigned luv(lpgbtPairEcondBuilderPtr_->definition_.luv());
  h1EcondErrorFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfErrorWords());
  h1PacketOutMinDelayLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->packetOutMinDelay_);

  h1PacketWordsFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfPacketWords());
  h1ErrorWordsFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfErrorWords());
  h1DiscardedPacketsFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfDiscardedPackets());
  h1MainBufferLowFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfMainBufferLevel(0));
  h1MainBufferMediumFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfMainBufferLevel(1));
  h1MainBufferHighFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfMainBufferLevel(2));
  h1MainBufferAlmostFullFractionLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->fractionOfMainBufferLevel(4));
  h1EcondAsicThrottleNumberLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->numberOfEcondAsicThrottles_);
  h1EcondBeThrottleNumberLuv_->SetBinContent(luv+1,lpgbtPairEcondBuilderPtr_->numberOfEcondBeThrottles_);  
}
