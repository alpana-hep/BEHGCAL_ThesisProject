#include <string>

#include "LpgbtPairBuilderAnalyser.h"

bool LpgbtPairBuilderAnalyser::staticsInitialised_(false);

TH1F *LpgbtPairBuilderAnalyser::h1AvgRamWordsFslp_(0);
TH1I *LpgbtPairBuilderAnalyser::h1EcondBufferFslp_(0);

LpgbtPairBuilderAnalyser::LpgbtPairBuilderAnalyser(const LpgbtPairBuilder *e) :
  lpgbtPairBuilderPtr_(e) {
}

void LpgbtPairBuilderAnalyser::initialise(bool a) {
  std::string labelStubStatics("LpgbtPairBuilder_");
  std::string labelStubNonStatics(labelStubStatics+lpgbtPairBuilderPtr_->definition_.label()+"_");

  nonStaticsInitialised_=a;
  
  if(a) {
    h1EcondBuffer_[0]=new TH1F((labelStubNonStatics+"EcondBuffer0").c_str(),
			       ";Buffer occupancy (32-bit words)",100,0,500);
    h1EcondBuffer_[1]=new TH1F((labelStubNonStatics+"EcondBuffer1").c_str(),
			       ";Buffer occupancy (32-bit words)",100,0,5000);
  }
    
  if(!staticsInitialised_) {
    staticsInitialised_=true;
    h1AvgRamWordsFslp_=new TH1F((labelStubStatics+"AvgRamWordsFslp").c_str(),
				";FSLP;Average RAM words (72-bit words)",
				28*120,0,28*120);
    h1EcondBufferFslp_=new TH1I((labelStubStatics+"BufferFslp").c_str(),
				";FSLP;Max event buffer occupancy (72-bit words)",
				28*120,0,28*120);
  }
}

void LpgbtPairBuilderAnalyser::analyse() {

  if(nonStaticsInitialised_) {
    unsigned s(lpgbtPairBuilderPtr_->txRamWordPtr_->size());
    h1EcondBuffer_[0]->Fill(s);
    h1EcondBuffer_[1]->Fill(s);
  }
}

void LpgbtPairBuilderAnalyser::finalise() {
  unsigned fslp(lpgbtPairBuilderPtr_->definition_.fslp());

  double avg(0.0);
  if(lpgbtPairBuilderPtr_->numberOfOutputPackets_>0)
    avg=1.0*lpgbtPairBuilderPtr_->numberOfOutputRamWords_/
      lpgbtPairBuilderPtr_->numberOfOutputPackets_;
  h1AvgRamWordsFslp_->SetBinContent(fslp+1,avg);

  h1EcondBufferFslp_->SetBinContent(fslp+1,lpgbtPairBuilderPtr_->maxOfOutputBuffer_);
}
