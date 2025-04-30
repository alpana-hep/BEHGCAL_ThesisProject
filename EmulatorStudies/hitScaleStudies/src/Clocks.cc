#include <cassert>
#include <iostream>
#include <iomanip>

#include "Clocks.h"

unsigned Clocks::frequencyMultiple_[Clocks::NumberOfFrequencies]={
  2*3,4*3,8*3,16*3,32*3,64*3,
  3*3,6*3,12*3,24*3,
  9*3,18*3,
  5*3,10*3,20*3,
  7*3,14*3,28
};

std::string Clocks::frequencyName_[Clocks::NumberOfFrequencies]={
  "Clock0040",
  "Clock0080",
  "Clock0160",
  "Clock0320",
  "Clock0640",
  "Clock1280",
  
  "Clock0060",
  "Clock0120",
  "Clock0240",
  "Clock0480",
  
  "Clock0180",
  "Clock0360",
  
  "Clock0100",
  "Clock0200",
  "Clock0400",
  
  "Clock0140",
  "Clock0280",
  "Clock0187"
};

Clocks Clocks::clocks_;

Clocks& Clocks::clocks() {
  return clocks_;
}
  
Clocks::Clocks() : initialised_(false), ticks_(0) {
  for(unsigned i(0);i<NumberOfFrequencies;i++) {
    enabled_[i]=false;
    count_[i]=0;
  }
}

void Clocks::enable(Clocks::Frequency f) {
  enabled_[f]=true;
}

void Clocks::initialise() {
  if(initialised_) return;

  /*  
  unsigned multiple[NumberOfFrequencies];

  unsigned n(0);

  //multiple[n++]=1;

  multiple[n++]=2;
  multiple[n++]=4;
  multiple[n++]=8;
  multiple[n++]=16;
  multiple[n++]=32;
  multiple[n++]=64;
  
  multiple[n++]=3;
  multiple[n++]=6;
  multiple[n++]=12;
  multiple[n++]=24;
  
  multiple[n++]=9;
  multiple[n++]=18;
  
  multiple[n++]=5;
  multiple[n++]=10;
  multiple[n++]=20;
  
  multiple[n++]=7;
  multiple[n++]=14;

  assert(n==NumberOfFrequencies);
  */
  
  unsigned lcm(64*9*5*7*3);

  tickVector_.resize(0);

  for(unsigned i(0);i<lcm;i++) {
    for(unsigned f(0);f<NumberOfFrequencies;f++) {
      if(enabled_[f]) {
	if((i%(lcm/frequencyMultiple_[f]))==0) {
	  tickVector_.push_back((Frequency)f);
	  std::cout << "Fundamental " << std::setw(5) << i
		    << ", tick " << std::setw(4) << tickVector_.size()-1
		    << " = " << frequencyName_[f] << std::endl;
	}
      }
    }    
  }

  // Check and print clock table
  unsigned counts[NumberOfFrequencies];
  for(unsigned f(0);f<NumberOfFrequencies;f++) {
    previousClock0040_[f]=false;
    counts[f]=0;
  }

  for(unsigned i(0);i<tickVector_.size();i++) counts[tickVector_[i]]++;

  std::cout << std::endl << "Clock::initialise() Clock total ticks = "
	    << tickVector_.size() << " in 150 ns (= 20/3 MHz period)" << std::endl;
  for(unsigned f(0);f<NumberOfFrequencies;f++) {
    if(enabled_[f]) {
      std::cout << " Frequency " // << std::setw(2) << f << " = "
		<< frequencyName_[f] << ", ticks = "
		<< counts[f] << " " << (enabled_[f]?"enabled":"disabled")
		<< std::endl;
    }
    
    if(enabled_[f]) assert(counts[f]==frequencyMultiple_[f]);
    else assert(counts[f]==0);
  }
  std::cout << std::endl;
  
  initialised_=true;
}

Clocks::Frequency Clocks::state() const {
  return state_;
}

unsigned long Clocks::tickNumber(Clocks::Frequency f) const {
  return count_[f]-1;
}

unsigned long Clocks::bxNumber() const {
  return tickNumber(Clock0040);
  //if(tickVector_.size()==0) return 0;
  //return ticks_/tickVector_.size();
}

unsigned long Clocks::bxCount() const {
  return count_[Clock0040];
}

double Clocks::timeMicrosec() const {
  return 0.05*tickNumber(state_)/frequencyMultiple_[state_];
}

bool Clocks::previousClock0040() const {
  return previousClock0040_[state_];
}

const std::string& Clocks::frequencyName(Clocks::Frequency f) {
  return frequencyName_[f];
}

void Clocks::update() {
  previousClock0040_[state_]=false;

  state_=tickVector_[ticks_%tickVector_.size()];
  count_[state_]++;
  ticks_++;

  if(state_==Clock0040) {
    for(unsigned f(0);f<NumberOfFrequencies;f++) previousClock0040_[f]=true;
  }
}
