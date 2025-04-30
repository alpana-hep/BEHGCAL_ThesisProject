#include <iostream>
#include <iomanip>
#include <cassert>

#include "LpgbtPairSerialiser.h"


LpgbtPairSerialiser::LpgbtPairSerialiser() :
  Entity("LpgbtPairSerialiser",Clocks::Clock0280),
  txElinkArrayPtr_(0),
  evenTick_(true),
  elinkCounter_(0) {
}

LpgbtPairSerialiser::~LpgbtPairSerialiser() {
}

// Initialisation
void LpgbtPairSerialiser::setDefinitionTo(const LpgbtPairDefinition &d) {
  lpgbtPairDefinition_=d;
  
}

void LpgbtPairSerialiser::initialise() {
  std::cout << "LpgbtPairSerialiser::initialise() ";
  lpgbtPairDefinition_.print();

  std::vector<EcondAsicDefinition> e;
  lpgbtPairDefinition_.econdAsicDefinitions(e);

  for(unsigned c(0);c<14;c++) {
    elinkNumber_[c]=14;
    elinkArrayNumber_[c][0]=28;
    elinkArrayNumber_[c][1]=28;
  }
  
  for(unsigned c(0);c<29;c++) {
    elinkArray_[c]=EcondAsicDefinition::IdlePattern;
  }
  
  unsigned nc(0),na(0);

  bool even(true);
  for(unsigned i(0);i<e.size();i++) {
    unsigned ne(e[i].numberOfElinks());

    if((ne%2)==0) {
      for(unsigned j(0);j<ne;j+=2) {
	elinkNumber_[nc]=i;
	elinkArrayNumber_[nc][0]=na;na++;
	elinkArrayNumber_[nc][1]=na;na++;
	nc++;
      }
      
    } else {
      if(even) {
	elinkNumber_[nc+7]=i;
	elinkArrayNumber_[nc+7][0]=na+ne-1;

	for(unsigned j(0);j<ne-1;j+=2) {
	  elinkNumber_[nc]=i;
	  elinkArrayNumber_[nc][0]=na;na++;
	  elinkArrayNumber_[nc][1]=na;na++;       
	  nc++;
	}
	na++;

      } else {
	elinkNumber_[nc]=i;
	elinkArrayNumber_[nc][1]=na;na++;
	nc++;

	for(unsigned j(1);j<ne-1;j+=2) {
	  elinkNumber_[nc]=i;
	  elinkArrayNumber_[nc][0]=na;na++;
	  elinkArrayNumber_[nc][1]=na;na++;       
	  nc++;
	}
      }

      even=!even;
    }
  }  

  nc=7;
  na=14;
  even=true;
  for(unsigned i(0);i<e.size();i++) {
    unsigned ne(e[i].numberOfElinks());

    if((ne%2)==0) {
      for(unsigned j(0);j<ne;j+=2) {
	elinkNumber_[nc]=i;
	elinkArrayNumber_[nc][0]=na;na++;
	elinkArrayNumber_[nc][1]=na;na++;
	nc++;
      }
      
    } else {
      if(even) {
	elinkArrayNumber_[nc][1]=na;na++;
	nc++;

	for(unsigned j(0);j<ne-1;j+=2) {
	  elinkNumber_[nc  ]=i;
	  elinkArrayNumber_[nc  ][0]=na;na++;
	  elinkArrayNumber_[nc  ][1]=na;na++;
	  nc++;
	}

      } else {
	elinkArrayNumber_[nc-8][0]=na;na++;

	for(unsigned j(1);j<ne-1;j+=2) {
	  elinkNumber_[nc]=i;	  
	  elinkArrayNumber_[nc][0]=na;na++;
	  elinkArrayNumber_[nc][1]=na;na++;
	  nc++;
	}
      }

      even=!even;
    }
  }

  std::cout << "LpgbtPairSerialiser::initialise()" << std::endl
	    << " Number of counters = " << nc << ", of array places = " << na
	    << std::endl;
  for(unsigned c(0);c<14;c++) {
    std::cout << "  Counter " << std::setw(2) << c << ", number = "
	      << elinkNumber_[c] << ", array elements = "
	      << elinkArrayNumber_[c][0] << ", "
	      << elinkArrayNumber_[c][1] << std::endl;
  }
    
  assert(nc<=14);
  assert(na<=28);

  std::cout << " Number of elinks = " << rxElinkPtrVector_.size() << std::endl;

  std::cout << std::endl;
}

// Connections
void LpgbtPairSerialiser::connect(EcondAsic &e) {
  const EcondAsicDefinition &d(e.definition());
  for(unsigned i(0);i<d.numberOfElinks();i++) {
    Connection<uint32_t> *w(new Connection<uint32_t>);
    connectRxPortTo(*w);
    e.connectTxPortTo(*w);
  }
}

// Inputs
void LpgbtPairSerialiser::connectRxPortTo(Connection<uint32_t> &w) {
  rxElinkPtrVector_.push_back(&w);
}

void LpgbtPairSerialiser::strobeRx() {
  for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
   rxElinkPtrVector_[i]->strobeOutput();
  }
}

// Output
void LpgbtPairSerialiser::connectTxPortTo(Connection< std::array<uint32_t,3> > &a) {
  if(txElinkArrayPtr_==0) txElinkArrayPtr_=&a;
}

void LpgbtPairSerialiser::strobeTx() {
  if(txElinkArrayPtr_!=0) txElinkArrayPtr_->strobeInput();
}

// Processing
void LpgbtPairSerialiser::process() {
  //if(Clocks::clocks().bxNumber()>=101586) debugPrint_=true;

  if(debugPrint_) {
    std::cout << "LpgbtPairSerialiser::process()  entering" << std::endl;
  }

  // Check connections
  assert(rxElinkPtrVector_.size()!=0);

  for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
    assert(rxElinkPtrVector_[i]->size()==0);
  }

  //assert(txElinkArrayPtr_!=0);
  //assert(txConnectionPtr_->size()<=evenTick_?1:0);
  
  // 40 MHz input section
  
  if(rxElinkPtrVector_[0]->get(elinkArray_[evenTick_?0:14])) { // Only after a 40 MHz tick
    if(debugPrint_) {
      std::cout << "Received from elink number " << 0
		<< ", data = 0x" << std::hex << std::setfill('0')
		<< elinkArray_[evenTick_?0:14]
		<< std::dec << std::setfill(' ') << std::endl;
    }
    
    for(unsigned i(1);i<rxElinkPtrVector_.size();i++) {
      //assert(rxElinkPtrVector_[i]->get(elinkArray_[(evenTick_?0:14)+i]));
      if(!rxElinkPtrVector_[i]->get(elinkArray_[(evenTick_?0:14)+i]))
	elinkArray_[(evenTick_?0:14)+i]=0xdeaddead;
      if(debugPrint_) {
	std::cout << "Received from elink number " << i
		  << ", data = 0x" << std::hex << std::setfill('0')
		  << elinkArray_[(evenTick_?0:14)+i]
		  << std::dec << std::setfill(' ') << std::endl;
      }
    }

    // Reset counter
    if(evenTick_) elinkCounter_=0;
    
    // Swap even-odd tick label
    evenTick_=!evenTick_;
  }
  
  // 280 MHz section

  assert(elinkCounter_<14);

  if(debugPrint_) {
    std::cout << "Elink array =" << std::hex << std::setfill('0');
    for(unsigned c(0);c<28;c++) {
      std::cout << " 0x" << std::setw(8) << elinkArray_[c];
      if(c==13) std::cout << std::endl << "             ";
    }
    std::cout << std::dec << std::setfill(' ') << std::endl;
  }
  
  if(debugPrint_) {
    std::cout << "Get data for counter = " << elinkCounter_
	      << ", array numbers = "
	      << elinkArrayNumber_[elinkCounter_][0] << ", "
	      << elinkArrayNumber_[elinkCounter_][1] << std::endl;
  }
  
  array_[0]=elinkNumber_[elinkCounter_];
  array_[1]=elinkArray_[elinkArrayNumber_[elinkCounter_][0]];
  array_[2]=elinkArray_[elinkArrayNumber_[elinkCounter_][1]];

  if(debugPrint_) {
    std::cout << "Sending elink number " << array_[0]
	      << ", data = 0x" << std::hex << std::setfill('0')
	      << std::setw(8) << array_[1] << ", 0x"
	      << std::setw(8)<< array_[2]
	      << std::dec << std::setfill(' ') << std::endl;
  }
  
  elinkCounter_++;
  
  if(txElinkArrayPtr_!=0) assert(txElinkArrayPtr_->set(array_));
  
  if(debugPrint_) {
    std::cout << "LpgbtPairSerialiser::process()  exiting" << std::endl;
  }
}

void LpgbtPairSerialiser::print() const {
  std::cout << "LpgbtPairSerialiser::print()" << std::endl << std::endl;
}
