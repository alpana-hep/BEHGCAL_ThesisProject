#include <iostream>
#include <cassert>

#include "DaqUnit.h"
#include "DaqUnit.h"


DaqUnit::DaqUnit() :
  numberOfSlinks_(0) {
}

DaqUnit::~DaqUnit() {
}

void DaqUnit::setDefinitionTo(const FpgaDefinition &fd) {
  assert(definitionVector_.size()<2);
  definitionVector_.push_back(fd);

  assert(fpgaPtrVector_.size()<2);
  fpgaPtrVector_.push_back(new Fpga());
  fpgaPtrVector_.back()->setDefinitionTo(fd);

  std::vector<Slink*> &vSl(fpgaPtrVector_.back()->slinkPtrVector());
  const std::vector<SlinkDefinition> &vSd(fd.slinkDefinitions());
  assert(vSd.size()==vSl.size());
  
  for(unsigned s(0);s<vSl.size();s++) {
    assert(numberOfSlinks_<24);
    
    if((numberOfSlinks_%5)==0) {
      d2sUnitPtrVector_.push_back(new D2sUnit());
    }
    d2sUnitPtrVector_.back()->setDefinitionTo(vSd[s]);
    d2sUnitPtrVector_.back()->connect(*(vSl[s]));
    numberOfSlinks_++;
  }    

  // BIT OF A HACK FOR NOW
  for(unsigned s(0);s<vSl.size();s++) {
    //dthCheckerPtrVector_.push_back(new DthChecker());
    //dthCheckerPtrVector_.back()->setDefinitionTo(vSd[s]);
    //vSl[s]->connect(*(dthCheckerPtrVector_.back()));
  }
  print();
}

// Arguments
/*
void DaqUnit::setArgumentsTo(const Arguments &a) {
  for(unsigned f(0);f<fpgaVector_.size();f++) {
    // for(unsigned f(0);f<fpgaPtrVector_.size();f++) {
    //fpgaPtrVector_[f]->setArgumentsTo(a);
    fpgaVector_[f].setArgumentsTo(a);
  }
}
*/
// Connections
void DaqUnit::connect(FastControlDriver &fcd) {
  for(unsigned f(0);f<fpgaPtrVector_.size();f++) {
    fpgaPtrVector_[f]->connect(fcd);
  }
  for(unsigned d(0);d<dthCheckerPtrVector_.size();d++) {
    dthCheckerPtrVector_[d]->connect(fcd);
  }
  for(unsigned d(0);d<d2sUnitPtrVector_.size();d++) {
    d2sUnitPtrVector_[d]->connect(fcd);
  }
}

void DaqUnit::connect(HgcrocAsics &has) {
  for(unsigned f(0);f<fpgaPtrVector_.size();f++) {
    fpgaPtrVector_[f]->connect(has);
  }
}

const std::vector<D2sUnit*>& DaqUnit::d2sUnit() {
  return d2sUnitPtrVector_;
}

void DaqUnit::print(const std::string &s) const {
  std::cout << s << "DaqUnit::print()" << std::endl;
    //<< definition_->label() << std::endl;

  std::cout << s << " Number of FPGA definitions = "
	    << definitionVector_.size() << std::endl;
  std::cout << s << " Number of FPGAs = "
	    << fpgaPtrVector_.size() << std::endl;
  for(unsigned f(0);f<fpgaPtrVector_.size();f++) {
    //fpgaPtrVector_[f]->print(s+" ");
  }
  std::cout << s << " Number of DTH checkers = "
	    << dthCheckerPtrVector_.size() << std::endl;
  for(unsigned d(0);d<dthCheckerPtrVector_.size();d++) {
    //dthCheckerPtrVector_[d]->print();
  }
  std::cout << s << " Number of D2S units = "
	    << d2sUnitPtrVector_.size() << std::endl;
  for(unsigned d(0);d<d2sUnitPtrVector_.size();d++) {
    //d2sUnitPtrVector_[d]->print();
  }

  std::cout << std::endl;
}
