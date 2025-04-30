#include <string>
#include <sstream>

#include "LpgbtBuilderAnalyser.h"

LpgbtBuilderAnalyser::LpgbtBuilderAnalyser(const LpgbtBuilder *b) : singleLpgbtBuilderPtr_(b) {
  std::string labelStub("LpgbtBuilder_");
  labelStub+=singleLpgbtBuilderPtr_->definition_.label()+"_";

  nClock0280_=0;
    
  for(unsigned i(0);i<8;i++) {
    std::ostringstream sout;
    sout << "ActiveInput" << i;

    th1iActiveInputs_[i]=new TH1I((labelStub+sout.str()).c_str(),
				  ";Clock0280",7*NumberOfBx,0,7*NumberOfBx);
  }
}

void LpgbtBuilderAnalyser::analyse() {
  if(singleLpgbtBuilderPtr_->validInputs_!=0) {
    unsigned nValidInputs(0);
    for(unsigned i(0);i<7;i++) {
      if((singleLpgbtBuilderPtr_->validInputs_&(1<<i))!=0) {
	nValidInputs++;
	//th1iActiveInputs_[i]->Fill(Clocks::clocks().bxCount());
	th1iActiveInputs_[i]->Fill(nClock0280_);
	th1iActiveInputs_[7]->Fill(nClock0280_);
      }
    }
  }
  nClock0280_++;
}
