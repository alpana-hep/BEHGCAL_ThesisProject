#include <iostream>
#include <cassert>

#include "Fpga.h"


Fpga::Fpga() {
}

Fpga::~Fpga() {
}

void Fpga::setDefinitionTo(const FpgaDefinition &fd) {
  definition_=fd;

  const std::vector<SlinkDefinition>& vSd(definition_.slinkDefinitions());

  for(unsigned s(0);s<vSd.size();s++) {
    slinkPtrVector_.push_back(new Slink());
    slinkPtrVector_.back()->setDefinitionTo(vSd[s]);

    //dthCheckerPtrVector_.push_back(new DthChecker());
    //dthCheckerPtrVector_.back()->setDefinitionTo(vSd[s]);
    
    //slinkPtrVector_[s]->connect(*(dthCheckerPtrVector_[s]));
  }
}

// Arguments

void Fpga::setArgumentsTo(const Arguments &a) {
  for(unsigned s(0);s<slinkPtrVector_.size();s++) {
    slinkPtrVector_[s]->setArgumentsTo(a);
  }

  for(unsigned d(0);d<dthCheckerPtrVector_.size();d++) {
    //dthCheckerPtrVector_[d]->setDebugPrintTo(a.debugPrintDthChecker());
    dthCheckerPtrVector_[d]->setArgumentsTo(a);
  }
}

// Connections

void Fpga::connect(FastControlDriver &fcd) {
  for(unsigned s(0);s<slinkPtrVector_.size();s++) {
    slinkPtrVector_[s]->connect(fcd);
  }

  for(unsigned d(0);d<dthCheckerPtrVector_.size();d++) {
    dthCheckerPtrVector_[d]->connect(fcd);
  }
}

void Fpga::connect(HgcrocAsics &has) {
  for(unsigned s(0);s<slinkPtrVector_.size();s++) {
    slinkPtrVector_[s]->connect(has);
  }
}

// Accessors

std::vector<Slink*>& Fpga::slinkPtrVector() {
  return slinkPtrVector_;
}

void Fpga::print(const std::string &s) const {
  std::cout << s << "Fpga::print() " << definition_.label() << std::endl;
  
  std::cout << s << " Number of Slinks = " << slinkPtrVector_.size() << std::endl;
  for(unsigned l(0);l<slinkPtrVector_.size();l++) {
    slinkPtrVector_[l]->print(s+" ");
  }
  
  std::cout << s <<" Number of DTH checkers = " << dthCheckerPtrVector_.size() << std::endl;  
  //for(unsigned d(0);d<dthCheckerPtrVector_.size();d++) {
    //dthCheckerPtrVector_[d]->print();
  //}
  std::cout << std::endl;
}
