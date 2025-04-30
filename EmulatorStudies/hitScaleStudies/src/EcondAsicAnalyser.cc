#include <string>

#include "EcondAsicAnalyser.h"

bool EcondAsicAnalyser::staticsInitialised_(false);

TH1I *EcondAsicAnalyser::h1NumberOfElinks_;
TH1I *EcondAsicAnalyser::h1NumberOfElinksLuv_;  
TH1I *EcondAsicAnalyser::h1AverageN10Hits_;
TH1F *EcondAsicAnalyser::h1AverageN10HitsLuv_;
TH1I *EcondAsicAnalyser::h1AverageN20Hits_;
TH1F *EcondAsicAnalyser::h1AverageN20HitsLuv_;
TH1I *EcondAsicAnalyser::h1AverageN30Hits_;
TH1F *EcondAsicAnalyser::h1AverageN30HitsLuv_;
TH1I *EcondAsicAnalyser::h1AverageTotalHits_;
TH1F *EcondAsicAnalyser::h1AverageTotalHitsLuv_;
TH1I *EcondAsicAnalyser::h1AveragePacketWords_;
TH1F *EcondAsicAnalyser::h1AveragePacketWordsLuv_;
TH1I *EcondAsicAnalyser::h1AveragePacketBits_;
TH1F *EcondAsicAnalyser::h1AveragePacketBitsLuv_;
TH1I *EcondAsicAnalyser::h1LinkPacketOccupancy_;
TH1F *EcondAsicAnalyser::h1LinkPacketOccupancyLuv_;
TH1I *EcondAsicAnalyser::h1MaxBuffer_;
TH1I *EcondAsicAnalyser::h1MaxBufferLuv_;
TH1I *EcondAsicAnalyser::h1FractionTruncatedPackets_;
TH1F *EcondAsicAnalyser::h1FractionTruncatedPacketsLuv_;
TH1I *EcondAsicAnalyser::h1TruncatedPackets_;
TH1F *EcondAsicAnalyser::h1TruncatedPacketsLuv_;
TH1F *EcondAsicAnalyser::h1TruncatedVsEconD_;
TH1I *EcondAsicAnalyser::h1FractionBufferStatus_[4];
TH1F *EcondAsicAnalyser::h1FractionBufferStatusLuv_[4];
TH1I *EcondAsicAnalyser::h1FractionCorruptIdleWords_;
TH1F *EcondAsicAnalyser::h1FractionCorruptIdleWordsLuv_;
TH1I *EcondAsicAnalyser::h1FractionCorruptPacketWords_;
TH1F *EcondAsicAnalyser::h1FractionCorruptPacketWordsLuv_;

TH2I *EcondAsicAnalyser::h2TruncationLpo_;
TH1I *EcondAsicAnalyser::h1PacketOutMinDelayLuv_;
TH1I *EcondAsicAnalyser::h1PacketOutMaxDelayLuv_;

EcondAsicAnalyser::EcondAsicAnalyser(EcondAsic *e) :
  econdAsicPtr_(e),
  econdAsicDefinitionPtr_(&(e->definition_)) {
}

void EcondAsicAnalyser::initialise(bool a) {
  std::string labelStub("EcondAsic_");
  labelStub+=econdAsicDefinitionPtr_->label()+"_";

  const unsigned luvRange(256*52);
  
  if(a) {
    h1EcondBuffer_[0]=new TH1F((labelStub+"EcondBuffer0").c_str(),
			       ";Buffer occupancy (32-bit words)",400,0,400);
    h1EcondBuffer_[1]=new TH1F((labelStub+"EcondBuffer1").c_str(),
			       ";Buffer occupancy (32-bit words)",400,0,4000);
    
    h1EcondAsicL1a_[0]=new TH1I((labelStub+"HgcrocL1a").c_str(),
				";HGCROC L1A (Orbit 0, BX number)",3564,0,3564);
    h1EcondAsicL1a_[1]=new TH1I((labelStub+"EcondAsicL1a").c_str(),
				";HGCROC L1A (Orbit 0, BX number)",3564,0,3564);
    h1EcondAsicThr_=new TH1I((labelStub+"EcondAsicThr").c_str(),
				";ECON-D Throttle (Orbit 0, BX number)",3564,0,3564);
    h1PacketOutDelay_=new TH1I((labelStub+"EcondAsicPacketOutDelay").c_str(),
			       ";ECON-D packet out delay (BX)",
			       200,0,200);
  }

  if(!staticsInitialised_) {
    staticsInitialised_=true;
    
    h1NumberOfElinks_   =new TH1I("EcondAsic_NumberOfElinks",
				  ";Number of elinks;Number of ECON-Ds",
				  8,0,8);
    h1NumberOfElinksLuv_=new TH1I("EcondAsic_NumberOfElinksLuv",
				  ";LUV;Number of elinks",
				  luvRange,0,luvRange);
    
    h1AverageN10Hits_     =new TH1I("EcondAsic_AverageN10Hits",
				    ";Average number of N10 hits;Number of ECON-Ds",
				    120,0,240);
    h1AverageN10HitsLuv_  =new TH1F("EcondAsic_AverageN10HitsLuv",
				    ";LUV;Average number of N10 hits",
				    luvRange,0,luvRange);
    h1AverageN20Hits_     =new TH1I("EcondAsic_AverageN20Hits",
				    ";Average number of N20 hits;Number of ECON-Ds",
				    120,0,240);
    h1AverageN20HitsLuv_  =new TH1F("EcondAsic_AverageN20HitsLuv",
				    ";LUV;Average number of N20 hits",
				    luvRange,0,luvRange);
    h1AverageN30Hits_     =new TH1I("EcondAsic_AverageN30Hits",
				    ";Average number of N30 hits;Number of ECON-Ds",
				    120,0,240);
    h1AverageN30HitsLuv_  =new TH1F("EcondAsic_AverageN30HitsLuv",
				    ";LUV;Average number of N30 hits",
				    luvRange,0,luvRange);
    h1AverageTotalHits_   =new TH1I("EcondAsic_AverageTotalHits",
				    ";Average total number of hits;Number of ECON-Ds",
				    120,0,240);
    h1AverageTotalHitsLuv_=new TH1F("EcondAsic_AverageTotalHitsLuv",
				    ";LUV;Average total number of hits",
				    luvRange,0,luvRange);
    
    h1AveragePacketWords_   =new TH1I("EcondAsic_AveragePacketWords",
				      ";Average number of packet 32-bit words;Number of ECON-Ds",
				      120,0,240);
    h1AveragePacketWordsLuv_=new TH1F("EcondAsic_AveragePacketWordsLuv",
				      ";LUV;Average number of packet 32-bit words",
				      luvRange,0,luvRange);
    h1AveragePacketBits_    =new TH1I("EcondAsic_AveragePacketBits",
				      ";Average number of packet bits;Number of ECON-Ds",
				      80,0,8000);
    h1AveragePacketBitsLuv_ =new TH1F("EcondAsic_AveragePacketBitsLuv",
				      ";LUV;Average number of packet bits",
				      luvRange,0,luvRange);
    
    h1LinkPacketOccupancy_   =new TH1I("EcondAsic_LinkPacketOccupancy",
				       ";Link packet occupancy;Number of ECON-Ds",
				       100,0,1);
    h1LinkPacketOccupancyLuv_=new TH1F("EcondAsic_LinkPacketOccupancyLuv",
				       ";LUV;Link packet occupancy",
				       luvRange,0,luvRange);
    
    h1MaxBuffer_              =new TH1I("EcondAsic_MaxBuffer",
					";Maximum buffer occupancy (32-bit words);Number of ECON-Ds",
					160,0,3200);
    h1MaxBufferLuv_             =new TH1I("EcondAsic_MaxBufferLuv",
					  ";LUV;Maximum buffer occupancy (32-bit words)",
					  luvRange,0,luvRange);
    h1FractionBufferStatus_[0]   =new TH1I("EcondAsic_FractionEmptyBuffer",
					  ";Fraction empty buffer;Number of ECON-Ds",
					  100,0,1);
    h1FractionBufferStatusLuv_[0]=new TH1F("EcondAsic_FractionEmptyBufferLuv",
					  ";LUV;Fraction empty buffer",
					  luvRange,0,luvRange);
    h1FractionBufferStatus_[1]   =new TH1I("EcondAsic_FractionLowBuffer",
					  ";Fraction low buffer;Number of ECON-Ds",
					  100,0,1);
    h1FractionBufferStatusLuv_[1]=new TH1F("EcondAsic_FractionLowBufferLuv",
					  ";LUV;Fraction low buffer",
					  luvRange,0,luvRange);
    h1FractionBufferStatus_[2]   =new TH1I("EcondAsic_FractionMediumBuffer",
					  ";Fraction medium buffer;Number of ECON-Ds",
					  100,0,0.01);
    h1FractionBufferStatusLuv_[2]=new TH1F("EcondAsic_FractionMediumBufferLuv",
					  ";LUV;Fraction medium buffer",
					  luvRange,0,luvRange);
    h1FractionBufferStatus_[3]   =new TH1I("EcondAsic_FractionHighBuffer",
					  ";Fraction high buffer;Number of ECON-Ds",
					  100,0,0.001);
    h1FractionBufferStatusLuv_[3]=new TH1F("EcondAsic_FractionHighBufferLuv",
					  ";LUV;Fraction high buffer",
					  luvRange,0,luvRange);
    
    h1FractionTruncatedPackets_     =new TH1I("EcondAsic_FractionTruncated",
					      ";Fraction of truncated packets;Number of ECON-Ds",
					      100,0,0.001);
    h1FractionTruncatedPacketsLuv_  =new TH1F("EcondAsic_FractionTruncatedLuv",
					      ";LUV;Fraction of truncated packets",
					      luvRange,0,luvRange);
    h1TruncatedPackets_     =new TH1I("EcondAsic_Truncated",
                                              "; No of truncated packets;Number of ECON-Ds",
                                              100,0,0.001);
    h1TruncatedPacketsLuv_  =new TH1F("EcondAsic_TruncatedLuv",
                                              ";LUV; No of truncated packets",
                                              luvRange,0,luvRange);
    h1TruncatedVsEconD_ = new TH1F("TruncatedVsEconD_",";LUV; Frequency",luvRange,0,luvRange);
    h1FractionCorruptIdleWords_     =new TH1I("EcondAsic_FractionCorruptIdleWords",
					      ";Fraction of corrupt idle words;Number of ECON-Ds",
					      100,0,1);
    h1FractionCorruptIdleWordsLuv_  =new TH1F("EcondAsic_FractionCorruptIdleWordsLuv",
						";LUV;Fraction of corrupt idle words",
					      luvRange,0,luvRange);
    h1FractionCorruptPacketWords_   =new TH1I("EcondAsic_FractionCorruptPacketWords",
					      ";Fraction of corrupt idle words;Number of ECON-Ds",
					      100,0,1);
    h1FractionCorruptPacketWordsLuv_=new TH1F("EcondAsic_FractionCorruptPacketWordsLuv",
					      ";LUV;Fraction of corrupt idle words",
					      luvRange,0,luvRange);
    
    h2TruncationLpo_=new TH2I("EcondAsic_TruncationLpo",
			      ";Link packet occupancy (%);Fraction of truncated packets",
			      100,0,100,100,0,0.1);
    
    h1PacketOutMinDelayLuv_=new TH1I("EcondAsic_PacketOutMinDelayLuv",
				     ";LUV;Packet out minimum delay (BX)",
				     luvRange,0,luvRange);
    h1PacketOutMaxDelayLuv_=new TH1I("EcondAsic_PacketOutMaxDelayLuv",
				     ";LUV;Packet out maximum delay (BX)",
				     luvRange,0,luvRange);
  }
}

void EcondAsicAnalyser::analyse() {
  unsigned s(econdAsicPtr_->bufferQueue_.size());
  h1EcondBuffer_[0]->Fill(s);
  h1EcondBuffer_[1]->Fill(s);

  unsigned long n(econdAsicPtr_->numberOfTicks_);
  if(n<3564) {
    if(econdAsicPtr_->commands_.l1a()) h1EcondAsicL1a_[0]->SetBinContent(n+1,1);
    if(econdAsicPtr_->newHgcrocL1a_) h1EcondAsicL1a_[1]->SetBinContent(n+1,1);
    //if(econdAsicPtr_->throttle_) h1EcondAsicThr_->SetBinContent(n+1,1);
  }

  while(!econdAsicPtr_->packetOutDelayQueue_.empty()) {
    h1PacketOutDelay_->Fill(econdAsicPtr_->packetOutDelayQueue_.front());
    econdAsicPtr_->packetOutDelayQueue_.pop();
  }
}

void EcondAsicAnalyser::finalise() {    
  unsigned luv(econdAsicDefinitionPtr_->luv());
  //double lpo(100.0*econdAsicPtr_->fractionOfPacketWordsOut());

  h1NumberOfElinks_->Fill(econdAsicDefinitionPtr_->numberOfElinks());
  h1NumberOfElinksLuv_->SetBinContent(luv+1,econdAsicDefinitionPtr_->numberOfElinks());
  
  h1AverageN10Hits_->Fill(econdAsicPtr_->averageN10Hits());
  h1AverageN10HitsLuv_->SetBinContent(luv+1,econdAsicPtr_->averageN10Hits());
  h1AverageN20Hits_->Fill(econdAsicPtr_->averageN20Hits());
  h1AverageN20HitsLuv_->SetBinContent(luv+1,econdAsicPtr_->averageN20Hits());
  h1AverageN30Hits_->Fill(econdAsicPtr_->averageN30Hits());
  h1AverageN30HitsLuv_->SetBinContent(luv+1,econdAsicPtr_->averageN30Hits());
  h1AverageTotalHits_->Fill(econdAsicPtr_->averageTotalHits());
  h1AverageTotalHitsLuv_->SetBinContent(luv+1,econdAsicPtr_->averageTotalHits());
  
  h1AveragePacketWords_->Fill(econdAsicPtr_->averagePacketWords());
  h1AveragePacketWordsLuv_->SetBinContent(luv+1,econdAsicPtr_->averagePacketWords());
  h1AveragePacketBits_->Fill(32.0*econdAsicPtr_->averagePacketWords());
  h1AveragePacketBitsLuv_->SetBinContent(luv+1,32.0*econdAsicPtr_->averagePacketWords());
  
  h1LinkPacketOccupancy_->Fill(econdAsicPtr_->ratioWordsInToOut());
  h1LinkPacketOccupancyLuv_->SetBinContent(luv+1,econdAsicPtr_->ratioWordsInToOut());
  
  h1MaxBuffer_->Fill(econdAsicPtr_->maxBufferSize_);
  h1MaxBufferLuv_->SetBinContent(luv+1,econdAsicPtr_->maxBufferSize_);
  for(unsigned i(0);i<4;i++) {
    h1FractionBufferStatus_[i]->Fill(econdAsicPtr_->fractionOfBufferStatus((EcondAsicIdle::BufferStatus)i));
    h1FractionBufferStatusLuv_[i]->SetBinContent(luv+1,econdAsicPtr_->fractionOfBufferStatus((EcondAsicIdle::BufferStatus)i));
  }
  
  h1FractionTruncatedPackets_->Fill(econdAsicPtr_->fractionOfTruncatedPackets());
  h1FractionTruncatedPacketsLuv_->SetBinContent(luv+1,econdAsicPtr_->fractionOfTruncatedPackets());
  h1TruncatedPackets_->Fill(econdAsicPtr_->TruncatedPackets());
  h1TruncatedPacketsLuv_->SetBinContent(luv+1,econdAsicPtr_->TruncatedPackets());
  if(econdAsicPtr_->TruncatedPackets()>0)
    h1TruncatedVsEconD_->Fill(luv+1);
  h1FractionCorruptIdleWords_->Fill(econdAsicPtr_->fractionOfCorruptIdleWords());
  h1FractionCorruptIdleWordsLuv_->SetBinContent(luv+1,econdAsicPtr_->fractionOfCorruptIdleWords());
  h1FractionCorruptPacketWords_->Fill(econdAsicPtr_->fractionOfCorruptPacketWords());
  h1FractionCorruptPacketWordsLuv_->SetBinContent(luv+1,econdAsicPtr_->fractionOfCorruptPacketWords());

  //h2TruncationLpo_->Fill(lpo,1.0*econdAsicPtr_->numberOfTruncatedPackets_/econdAsicPtr_->numberOfPacketsIn_);
  
  h1PacketOutMinDelayLuv_->SetBinContent(luv+1,econdAsicPtr_->packetOutMinDelay_);
  h1PacketOutMaxDelayLuv_->SetBinContent(luv+1,econdAsicPtr_->packetOutMaxDelay_);
}
