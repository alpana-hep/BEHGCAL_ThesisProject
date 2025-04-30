#include <iostream>
#include <iomanip>
#include <cassert>

#include "LpgbtPairSplitter.h"


LpgbtPairSplitter::LpgbtPairSplitter() :
  Entity("LpgbtPairSplitter",Clocks::Clock0280),
  evenTick_(true),
  elinkCounter_(0) {

  evenTick_=false; // DELETE
}

LpgbtPairSplitter::~LpgbtPairSplitter() {
}

// Initialisation
void LpgbtPairSplitter::setDefinitionTo(const LpgbtPairDefinition &d) {
  lpgbtPairDefinition_=d;
  
}

void LpgbtPairSplitter::initialise() {
  std::cout << "LpgbtPairSplitter::initialise() ";
  lpgbtPairDefinition_.print();

  std::vector<EcondAsicDefinition> e;
  lpgbtPairDefinition_.econdAsicDefinitions(e);

  for(unsigned c(0);c<14;c++) {
    elinkNumber_[c]=14;
    elinkArrayNumber_[c][0]=28;
    elinkArrayNumber_[c][1]=28;
  }
  
  for(unsigned c(0);c<29;c++) {
    elinkArray_[c]=0xdead0000;
  }
  
  unsigned nc(0),na(0),ni(14);
  unsigned ncSave,ncLo(0),ncHi(7),naLo(0),naHi(14);
  
  bool even(true);
  for(unsigned i(0);i<e.size();i++) {
    unsigned ne(e[i].numberOfElinks());

    uint32_t idleWord(e[i].idleWord());
    idleWord++; // TEMP!
    for(unsigned j(0);j<ne;j++) {
      elinkArray_[ni]=idleWord;
      ni++;
    }
    
    if((ne%2)==0) {
      for(unsigned j(0);j<ne;j+=2) {
	elinkNumber_[ncLo]=i;
	elinkArrayNumber_[ncLo][0]=naLo;naLo++;
	std::cout << "elinkArrayNumber_[" << ncLo << "][0] = " << elinkArrayNumber_[ncLo][0] << std::endl;
	elinkArrayNumber_[ncLo][1]=naLo;naLo++;
	std::cout << "elinkArrayNumber_[" << ncLo << "][1] = " << elinkArrayNumber_[ncLo][1] << std::endl;
	ncLo++;
      }

      for(unsigned j(0);j<ne;j+=2) {
	elinkNumber_[ncHi]=i;
	elinkArrayNumber_[ncHi][0]=naHi;naHi++;
	std::cout << "elinkArrayNumber_[" << ncHi << "][0] = " << elinkArrayNumber_[ncHi][0] << std::endl;
	elinkArrayNumber_[ncHi][1]=naHi;naHi++;
	std::cout << "elinkArrayNumber_[" << ncHi << "][1] = " << elinkArrayNumber_[ncHi][1] << std::endl;
	ncHi++;
      }
      
    } else {
      if(even) {
	for(unsigned j(0);j<ne-1;j+=2) {
	  elinkNumber_[ncLo]=i;
	  elinkArrayNumber_[ncLo][0]=naLo;naLo++;
	  std::cout << "elinkArrayNumber_[" << ncLo << "][0] = " << elinkArrayNumber_[ncLo][0] << std::endl;
	  elinkArrayNumber_[ncLo][1]=naLo;naLo++;
	  std::cout << "elinkArrayNumber_[" << ncLo << "][1] = " << elinkArrayNumber_[ncLo][1] << std::endl;
	  ncLo++;
	}

	elinkNumber_[ncHi]=i;
	elinkArrayNumber_[ncHi][0]=naLo;naLo++;
	std::cout << "elinkArrayNumber_[" << ncHi << "][0] = " << elinkArrayNumber_[ncHi][0] << std::endl;
	elinkArrayNumber_[ncHi][1]=naHi;naHi++;
	std::cout << "elinkArrayNumber_[" << ncHi << "][1] = " << elinkArrayNumber_[ncHi][1] << std::endl;
	ncHi++;
	
	for(unsigned j(0);j<ne-1;j+=2) {
	  elinkNumber_[ncHi]=i;
	  elinkArrayNumber_[ncHi][0]=naHi;naHi++;
	  std::cout << "elinkArrayNumber_[" << ncHi << "][0] = " << elinkArrayNumber_[ncHi][0] << std::endl;
	  elinkArrayNumber_[ncHi][1]=naHi;naHi++;
	  std::cout << "elinkArrayNumber_[" << ncHi << "][1] = " << elinkArrayNumber_[ncHi][1] << std::endl;
	  ncHi++;
	}

      } else {
	ncSave=ncLo;
	elinkNumber_[ncLo]=i;
	elinkArrayNumber_[ncLo][1]=naLo;naLo++;
	std::cout << "elinkArrayNumber_[" << ncLo << "][1] = " << elinkArrayNumber_[ncLo][1] << std::endl;
	ncLo++;
	
	for(unsigned j(0);j<ne-1;j+=2) {
	  elinkNumber_[ncLo]=i;
	  elinkArrayNumber_[ncLo][0]=naLo;naLo++;
	  std::cout << "elinkArrayNumber_[" << ncLo << "][0] = " << elinkArrayNumber_[ncLo][0] << std::endl;
	  elinkArrayNumber_[ncLo][1]=naLo;naLo++;
	  std::cout << "elinkArrayNumber_[" << ncLo << "][1] = " << elinkArrayNumber_[ncLo][1] << std::endl;
	  ncLo++;
	}

	for(unsigned j(0);j<ne-1;j+=2) {
	  elinkNumber_[ncHi]=i;
	  elinkArrayNumber_[ncHi][0]=naHi;naHi++;
	  std::cout << "elinkArrayNumber_[" << ncHi << "][0] = " << elinkArrayNumber_[ncHi][0] << std::endl;
	  elinkArrayNumber_[ncHi][1]=naHi;naHi++;
	  std::cout << "elinkArrayNumber_[" << ncHi << "][1] = " << elinkArrayNumber_[ncHi][1] << std::endl;
	  ncHi++;
	}

	elinkArrayNumber_[ncSave][0]=naHi;naHi++;
	std::cout << "elinkArrayNumber_[" << ncSave << "][0] = " << elinkArrayNumber_[ncSave][0] << std::endl;
      }

      even=!even;
    }
  }  

  std::cout << "LpgbtPairSplitter::initialise()" << std::endl
	    << " Number of counters = " << ncLo+ncHi-7
	    << ", of array places = " << naLo+naHi-14
	    << std::endl;
  for(unsigned c(0);c<14;c++) {
    std::cout << "  Counter " << std::setw(2) << c << ", number = "
	      << elinkNumber_[c] << ", array elements = "
	      << elinkArrayNumber_[c][0] << ", "
	      << elinkArrayNumber_[c][1] << std::endl;
  }
    
  assert(nc<=14);
  assert(na<=28);

  std::cout << " Number of  input elinks = " << rxElinkPtrVector_.size() << std::endl;
  std::cout << " Number of output elinks = " << txElinkArrayPtrVector_.size() << std::endl;

  assert(rxElinkPtrVector_.size()!=0);

  std::cout << std::endl;
}

// Connections
void LpgbtPairSplitter::connect(std::vector<Connection<uint32_t>*> &v) {
  for(unsigned i(0);i<v.size();i++) {
    Connection<uint32_t> *w(new Connection<uint32_t>);
    connectRxPortTo(*w);
  }
}

void LpgbtPairSplitter::connect(std::vector<Connection<std::array<uint32_t,2> > > &) {
}

void LpgbtPairSplitter::connect(EcondAsic &e) {
  e.connect(rxElinkPtrVector_);
}

void LpgbtPairSplitter::connect(LpgbtPairEcondBuilder &e) {
  Connection< std::array<uint32_t,2> > *w(new Connection< std::array<uint32_t,2> >);
  connectTxPortTo(*w);
  e.connectRxPortTo(*w);
  w->setMaxSizeTo(1);
}

// Inputs
void LpgbtPairSplitter::connectRxPortTo(Connection<uint32_t> &w) {
  rxElinkPtrVector_.push_back(&w);
}

void LpgbtPairSplitter::strobeRx() {
  if((elinkCounter_%7)==0) {
    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      rxElinkPtrVector_[i]->strobeOutput();
    }
  }
}

// Output
void LpgbtPairSplitter::connectTxPortTo(Connection< std::array<uint32_t,2> > &a) {
  txElinkArrayPtrVector_.push_back(&a);
}

void LpgbtPairSplitter::strobeTx() {
  for(unsigned i(0);i<txElinkArrayPtrVector_.size();i++) {
    txElinkArrayPtrVector_[i]->strobeInput();
  }
}

// Processing
void LpgbtPairSplitter::process() {
  //if(Clocks::clocks().bxNumber()>=101586) debugPrint_=true;

  if(debugPrint_) {
    std::cout << "LpgbtPairSplitter::process()  entering" << std::endl;
  }

  // Check connections
  //for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
    //std::cout << "HERE " << rxElinkPtrVector_[i]->size() << std::endl;
    //assert(rxElinkPtrVector_[i]->size()==0);
  //}

  
  //assert(txElinkArrayPtr_!=0);
  //assert(txConnectionPtr_->size()<=evenTick_?1:0);
  
  // 40 MHz input section
  
  if((elinkCounter_%7)==0) {
    unsigned eaNumber((elinkCounter_/7)==0?0:14);
    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      //assert(rxElinkPtrVector_[i]->get(elinkArray_[eaNumber+i]));
      if(!rxElinkPtrVector_[i]->get(elinkArray_[eaNumber+i]))
	elinkArray_[eaNumber+i]=0xdead1111;
      
      if(debugPrint_) {
	std::cout << "Received from elink number " << std::setw(2) << i
		  << ", data = 0x" << std::hex << std::setfill('0')
		  << std::setw(8) << elinkArray_[eaNumber+i]
		  << std::dec << std::setfill(' ') << std::endl;
      }
    }
  
    // Reset counter
    //if(evenTick_) elinkCounter_=0;
    
    // Swap even-odd tick label
    //evenTick_=!evenTick_;
  }
  
  // 280 MHz section

  assert(elinkCounter_<14);
  /*
  std::array<uint32_t,2> arr;
  if(elinkCounter_<rxElinkPtrVector_.size()) {
    assert(rxElinkPtrVector_[elinkArrayNumber_[elinkCounter_][0]]->get(arr[0]));
    assert(rxElinkPtrVector_[elinkArrayNumber_[elinkCounter_][1]]->get(arr[1]));
  }
  */
  
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
  
  array_[0]=elinkArray_[elinkArrayNumber_[elinkCounter_][0]];
  array_[1]=elinkArray_[elinkArrayNumber_[elinkCounter_][1]];

  if(debugPrint_) {
    std::cout << "Sending elink number " << elinkNumber_[elinkCounter_]
	      << ", data = 0x" << std::hex << std::setfill('0')
	      << std::setw(8) << array_[0] << ", 0x"
	      << std::setw(8)<< array_[1]
	      << std::dec << std::setfill(' ') << std::endl;
  }

  if(elinkNumber_[elinkCounter_]<txElinkArrayPtrVector_.size())
    assert(txElinkArrayPtrVector_[elinkNumber_[elinkCounter_]]->set(array_));
    
  elinkCounter_=(elinkCounter_+1)%14;
  
  if(debugPrint_) {
    std::cout << "LpgbtPairSplitter::process()  exiting" << std::endl;
  }
}

void LpgbtPairSplitter::print() const {
  std::cout << "LpgbtPairSplitter::print()" << std::endl << std::endl;
}
