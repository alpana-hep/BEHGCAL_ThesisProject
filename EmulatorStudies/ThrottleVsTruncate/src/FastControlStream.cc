#include <iostream>
#include <cassert>

#include "FastControlStream.h"

FastControlStream::FastControlStream() :
  Entity("FastControlStream",Clocks::Clock0040),
  rxCommandsPtr_(0),
  txRequestsPtr_(0),
  numberOfL1as_(0),
  numberOfNzsL1as_(0) {
}

FastControlStream::~FastControlStream() {
}

// Initialisation
void FastControlStream::setDefinitionTo(const FastControlStreamDefinition &d) {
  definition_=d;
}

void FastControlStream::setConfigurationTo(const FastControlStreamConfiguration &c) {
  configuration_=c;
}

void FastControlStream::setArgumentsTo(const Arguments &a) {
  configuration_.setNzsProbabilityTo(a.nzsProbability());
  Entity::setArgumentsTo(a);
}

void FastControlStream::initialise() {
  std::cout << "FastControlStream::initialise() "
	    << definition_.label() << std::endl;
  //definition_.print(" ");
  configuration_.print(" ");

  // Form offset for NZS from lpGBT and stream numbers
  noZeroSuppressionOffset_=definition_.lpgbtNumber()+10*definition_.streamNumber();
  
  std::cout << " No zero suppression offset = "
	    << noZeroSuppressionOffset_ << std::endl;

  // Allow for queue delays
  initialCommandsQueueSize_=rxCommandsPtr_->size()-1;
  initialRequestsQueueSize_=txRequestsPtr_->size()-1;
  
  std::cout << " RX requests connections = "
	    << rxRequestsPtrVector_.size() << std::endl
	    << " TX commands connections = "
	    << txCommandsPtrVector_.size() << std::endl
	    << " TX lpGBT word connections = "
	    << txLpgbtWordPtrVector_.size() << std::endl
	    << " Initial RX commands queue size_ = "
	    << initialCommandsQueueSize_ << std::endl
	    << " Initial TX requests queue size_ = "
	    << initialRequestsQueueSize_ << std::endl;
  std::cout << std::endl;
}

// Connection
void FastControlStream::connect(FastControlDriver &d) {
  Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
  connectRxPortTo(*c);
  d.connectTxPortTo(*c);
  
  Connection<FastControlRequests> *r(new Connection<FastControlRequests>);
  connectTxPortTo(*r);
  d.connectRxPortTo(*r);
}

// Input
void FastControlStream::connectRxPortTo(Connection<FastControlCommands> &c) {
  rxCommandsPtr_=&c;
  c.initialiseQueue(FastControlCommands()); // Idle
}

void FastControlStream::connectRxPortTo(Connection<FastControlRequests> &r) {
  rxRequestsPtrVector_.push_back(&r);
  r.initialiseQueue(FastControlRequests()); // Idle
}

void FastControlStream::strobeRx() {
  rxCommandsPtr_->strobeOutput();
  for(unsigned i(0);i<rxRequestsPtrVector_.size();i++) {
    rxRequestsPtrVector_[i]->strobeOutput();
  }
}

// Output
void FastControlStream::connectTxPortTo(Connection<FastControlRequests> &r) {
  txRequestsPtr_=&r;
}

void FastControlStream::connectTxPortTo(Connection<FastControlLpgbtWord> &w) {
  txLpgbtWordPtrVector_.push_back(&w);
}

void FastControlStream::connectTxPortTo(Connection<FastControlCommands> &c) {
  txCommandsPtrVector_.push_back(&c);
}

void FastControlStream::strobeTx() {
  txRequestsPtr_->strobeInput();
  for(unsigned i(0);i<txLpgbtWordPtrVector_.size();i++) {
    txLpgbtWordPtrVector_[i]->strobeInput();
  }
  for(unsigned i(0);i<txCommandsPtrVector_.size();i++) {
    txCommandsPtrVector_[i]->strobeInput();
  }
}

// Processing
void FastControlStream::process() {
  if(debugPrint_) {
    std::cout << "FastControlStream::process()  entering" << std::endl;
  }

  // Check connections are correct
  assert(rxCommandsPtr_!=0);
  assert(rxCommandsPtr_->size()==initialCommandsQueueSize_);
  assert(txRequestsPtr_!=0);
  assert(txRequestsPtr_->size()==initialRequestsQueueSize_);

  assert(rxRequestsPtrVector_.size()==txCommandsPtrVector_.size());
  for(unsigned i(0);i<rxRequestsPtrVector_.size();i++) {
    assert(rxRequestsPtrVector_[i]->size()==0);
    assert(txCommandsPtrVector_[i]->size()==0);
  }

  //for(unsigned i(0);i<txLpgbtWordPtrVector_.size();i++) {
    //assert(txLpgbtWordPtrVector_[i]->size()==0);
  //}
  
  // Handle requests
  subProcessRequests();
  
  // Handle commands
  subProcessCommands();
  

  if(debugPrint_) {
    std::cout << "FastControlStream::process()  exiting" << std::endl;
  }
}

void FastControlStream::subProcessRequests() {
  if(debugPrint_) {
    std::cout << " FastControlStream::subProcessRequests()  entering" << std::endl;
  }

  requests_.setIdle();

  // Receive input requests
  FastControlRequests input;
  for(unsigned i(0);i<rxRequestsPtrVector_.size();i++) {
    assert(rxRequestsPtrVector_[i]->get(input));
    if(debugPrint_) {
      std::cout << "  Input requests " << i << " ";
      input.print();
    }
    requests_.merge(input);
  }

  if(debugPrint_) {
    std::cout << "   Merged requests ";requests_.print();
  }

  assert(txRequestsPtr_->set(requests_));
  
  if(debugPrint_) {
    std::cout << " FastControlStream::subProcessRequests()  exiting" << std::endl;
  }
}

void FastControlStream::subProcessCommands() {
  if(debugPrint_) {
    std::cout << " FastControlStream::subProcessCommands()  entering" << std::endl;
  }

  // Receive input commands
  assert(rxCommandsPtr_->get(globalCommands_));

  if(debugPrint_) {
    std::cout << "   Global commands ";
    globalCommands_.print();
  }

  // Copy global to allow modification locally
  localCommands_=globalCommands_;
  
  // Add local requests to commands
  if(!requests_.idle()) {
    if(requests_.linkResetEcond()) localCommands_.setLinkResetEcondTo(true);
    if(requests_.eventBufferReset()) localCommands_.setEventBufferResetTo(true);
  }

  // Add calibration type (randomly selected)
  if(localCommands_.calPrepare()) {
    if((numberOfL1as_%2)==0) localCommands_.setCalPulseIntTo(true);
    else                     localCommands_.setCalPulseExtTo(true);
  }

    // Add no zero suppression to commands
  if(localCommands_.l1a()) {

    // Do regularly by period
    if(configuration_.noZeroSuppressionPeriod()>0) {
      if(((numberOfL1as_+noZeroSuppressionOffset_)%configuration_.noZeroSuppressionPeriod())==0) {
	localCommands_.setNoZeroSuppressionTo(true);
	numberOfNzsL1as_++;
      }
    }

    // Do randomly by probability
    if(configuration_.nzsProbability()>0.0) {
      if(RandomSingleton::random().Uniform()<configuration_.nzsProbability()) {
	localCommands_.setNoZeroSuppressionTo(true);
	numberOfNzsL1as_++;
      }
    }

    numberOfL1as_++;
  }
  
  if(debugPrint_) {
    std::cout << "    Local commands ";
    localCommands_.print();
  }

  // Merge in any commands not sent previously
  localCommands_.merge(droppedCommands_);

  if(debugPrint_) {
    std::cout << "  Dropped commands ";
    droppedCommands_.print();
    std::cout << "   Merged commands ";
    localCommands_.print();
  }

  // Transmit commands within BE
  for(unsigned i(0);i<txCommandsPtrVector_.size();i++) {
    assert(txCommandsPtrVector_[i]->set(localCommands_));
  }
  
  // Convert to transmitted lpGBT word and store any dropped commands
  droppedCommands_=lpgbtWord_.setTo(localCommands_);

  if(debugPrint_) {
    std::cout << "  Lpgbt word ";
    lpgbtWord_.print();
    std::cout << "  Dropped commands ";
    droppedCommands_.print();
  }

  // Transmit lpGBT words
  for(unsigned i(0);i<txLpgbtWordPtrVector_.size();i++) {
    assert(txLpgbtWordPtrVector_[i]->set(lpgbtWord_));
  }  
  
  if(debugPrint_) {
    std::cout << " FastControlStream::subProcessCommands()  exiting" << std::endl;
  }
}

void FastControlStream::print() const {
  std::cout << "FastControlStream::print() "
	    << definition_.label() << std::endl
	    << " Number of global normal L1As received = " << numberOfL1as_ << std::endl
	    << " Number of local no zero suppression L1As = "
	    << numberOfNzsL1as_ << std::endl;
  std::cout << std::endl;
}
