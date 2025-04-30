#include <string>

#include "SlinkBuilderAnalyser.h"

bool SlinkBuilderAnalyser::staticsInitialised_(false);

TH1F *SlinkBuilderAnalyser::h1AvgSlinkWordsFs_(0);
TH1I *SlinkBuilderAnalyser::h1SlinkFifoFs_(0);
TH2F *SlinkBuilderAnalyser::h2OccupancyVsAvgSlinkWords_(0);

SlinkBuilderAnalyser::SlinkBuilderAnalyser(const SlinkBuilder *e) :
  slinkBuilderPtr_(e) {
}

void SlinkBuilderAnalyser::initialise(bool a) {
  std::string labelStubStatics("SlinkBuilder_");
  std::string labelStubNonStatics(labelStubStatics+slinkBuilderPtr_->definition_.label()+"_");

  nonStaticsInitialised_=a;
  
  if(a) {
    h1EcondBuffer_[0]=new TH1F((labelStubNonStatics+"EcondBuffer0").c_str(),
			       ";Buffer occupancy (32-bit words)",100,0,500);
    h1EcondBuffer_[1]=new TH1F((labelStubNonStatics+"EcondBuffer1").c_str(),
			       ";Buffer occupancy (32-bit words)",100,0,5000);
  }
    
  if(!staticsInitialised_) {
    staticsInitialised_=true;
    h1AvgSlinkWordsFs_=new TH1F((labelStubStatics+"AvgSlinkWordsFs").c_str(),
				";12xFPGA+Slink;Average Slink words (128-bit words)",
				28*12,0,28*12);
    h1SlinkFifoFs_=new TH1I((labelStubStatics+"BufferFs").c_str(),
				";12xFPGA+Slink;Max Slink FIFO occupancy (128-bit words)",
				28*12,0,28*12);

    const unsigned nBins(50);
    double edges[nBins+1];
    for(unsigned i(0);i<=nBins;i++) edges[i]=pow(10.0,0.1*i);

    h2OccupancyVsAvgSlinkWords_=new TH2F((labelStubStatics+"OccupancyVsAvgSlinkWords").c_str(),
					 ";Average Slink words (128-bit words);Max Slink FIFO occupancy (128-bit words)",
					 50,120,220,nBins,edges);
  }
}

void SlinkBuilderAnalyser::analyse() {
  /*
  if(nonStaticsInitialised_) {
    unsigned s(slinkBuilderPtr_->txRamWordPtr_->size());
    h1EcondBuffer_[0]->Fill(s);
    h1EcondBuffer_[1]->Fill(s);
  }
  */
}

void SlinkBuilderAnalyser::finalise() {
  unsigned fs(slinkBuilderPtr_->definition_.fs());

  double avg(0.0);
  if(slinkBuilderPtr_->numberOfOutputPackets_>0)
    avg=1.0*slinkBuilderPtr_->numberOfOutputSlinkWords_/
      slinkBuilderPtr_->numberOfOutputPackets_;
  h1AvgSlinkWordsFs_->SetBinContent(fs+1,avg);

  h1SlinkFifoFs_->SetBinContent(fs+1,slinkBuilderPtr_->maxOfOutputBuffer_);
  h2OccupancyVsAvgSlinkWords_->Fill(avg,slinkBuilderPtr_->maxOfOutputBuffer_);
}
