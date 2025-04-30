#include <iostream>
#include <iomanip>
#include <cassert>

#include "GlobalConfiguration.h"
#include "SlinkSender.h"


SlinkSender::SlinkSender() :
  Entity("SlinkSender",GlobalConfiguration::configuration().slinkClockFrequency()),
  rxSlinkWordPtr_(0),
  txSlinkWordPtr_(0),
  validBoolSlinkWord_(false),
  numberOfTicks_(0),
  sizeOfInputBuffer_(0),
  maxOfInputBuffer_(0),
  sizeOfOutputBuffer_(0),
  maxOfOutputBuffer_(0) {
}

SlinkSender::~SlinkSender() {
}

// Initialisation
void SlinkSender::setDefinitionTo(const SlinkDefinition &d) {
  slinkDefinition_=d;
}

void SlinkSender::initialise() {
  std::cout << "SlinkSender::initialise() ";
  slinkDefinition_.print();

  // Clock speed can be varied so print it out
  std::cout << " Clock frequency = "
	    << Clocks::frequencyName(frequency_) << std::endl;
  
  // Check inputs and outputs
  assert(rxSlinkWordPtr_!=0);
  std::cout << " Input buffer maximum size = "
	    << rxSlinkWordPtr_->maxSize() << std::endl;
  
  if(txSlinkWordPtr_==0) std::cout << " Output buffer disabled" << std::endl;
  else {
    std::cout << " Output buffer maximum size = "
	      << txSlinkWordPtr_->maxSize() << std::endl;
  }

  std::cout << std::endl;
}

// Connections
void SlinkSender::connect(SlinkBuilder &b) {
  Connection< std::pair<bool,SlinkWord> > *w(new Connection< std::pair<bool,SlinkWord> >());
  connectRxPortTo(*w);
  b.connectTxPortTo(*w);
  //w->setMaxSizeTo(16); // CONFIGURATION!
}

// Inputs
void SlinkSender::connectRxPortTo(Connection< std::pair<bool,SlinkWord> > &w) {
  rxSlinkWordPtr_=&w;
}

void SlinkSender::strobeRx() {
  if(rxSlinkWordPtr_!=0) rxSlinkWordPtr_->strobeOutput();
}

// Output
void SlinkSender::connectTxPortTo(Connection< std::pair<bool,SlinkWord> > &w) {
  txSlinkWordPtr_=&w;
}

void SlinkSender::strobeTx() {
  if(txSlinkWordPtr_!=0) txSlinkWordPtr_->strobeInput();
}

// Processing
void SlinkSender::process() {
  //if(Clocks::clocks().bxNumber()>=101586) debugPrint_=true;

  if(debugPrint_) {
    std::cout << "SlinkSender::process()  entering" << std::endl;
  }

  // Check buffers
  unsigned s(rxSlinkWordPtr_->size());
  sizeOfInputBuffer_+=s;
  if(maxOfInputBuffer_<s) maxOfInputBuffer_=s;
  
  if(debugPrint_) {
    std::cout << "  Input buffer size = " << s << std::endl;
  }
  
  if(txSlinkWordPtr_!=0) {
    s=txSlinkWordPtr_->size();
    sizeOfOutputBuffer_+=s;
    if(maxOfOutputBuffer_<s) maxOfOutputBuffer_=s;
    
    if(debugPrint_) {
      std::cout << "  Output buffer size = " << s << std::endl;
    }
  }

  // Check if there is no word waiting to be sent
  if(!validBoolSlinkWord_) {
    if(debugPrint_) {
      std::cout << "  No word waiting so try reading" << std::endl;
    }

    validBoolSlinkWord_=rxSlinkWordPtr_->get(boolSlinkWord_);
    
    // If word, then send
    if(validBoolSlinkWord_) {
      if(debugPrint_) {
	std::cout << "  Read successful, Slink flag = "
		  << boolSlinkWord_.first << " ";
	boolSlinkWord_.second.print();
      }
      
      if(txSlinkWordPtr_!=0) {
	validBoolSlinkWord_=!(txSlinkWordPtr_->set(boolSlinkWord_));
      } else {
	validBoolSlinkWord_=false;
      }
    }

  } else {

    // Try sending word
    if(txSlinkWordPtr_!=0) {
      validBoolSlinkWord_=!(txSlinkWordPtr_->set(boolSlinkWord_));
    } else {
      validBoolSlinkWord_=false;
    }

    // If word sent, then read
    if(!validBoolSlinkWord_) {
      if(rxSlinkWordPtr_!=0) {
	validBoolSlinkWord_=rxSlinkWordPtr_->get(boolSlinkWord_);
      }
    }
  }

  numberOfTicks_++;
  
  if(debugPrint_) {
    std::cout << "SlinkSender::process()  exiting" << std::endl;
  }
}

void SlinkSender::print() const {
  std::cout << "SlinkSender::print()" << std::endl
            << " Number of ticks = " << numberOfTicks_ << std::endl << std::endl;
  std::cout << " Input buffer average size = " << 1.0*sizeOfInputBuffer_/numberOfTicks_
            << ", maximum = " << maxOfInputBuffer_ << std::endl;
  std::cout << " Output buffer average size = " << 1.0*sizeOfOutputBuffer_/numberOfTicks_
            << ", maximum = " << maxOfOutputBuffer_ << std::endl;
  std::cout << std::endl;
}
