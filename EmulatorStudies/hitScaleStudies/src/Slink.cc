#include <iostream>
#include <cassert>

#include "Slink.h"


Slink::Slink() {
}

Slink::~Slink() {
}

void Slink::setDefinitionTo(const SlinkDefinition &sd) {
  definition_=sd;

  slinkBuilderPtr_=new SlinkBuilder();
  slinkBuilderPtr_->setDefinitionTo(sd);

  /* Obsolete
  slinkSenderPtr_=new SlinkSender();
  slinkSenderPtr_->setDefinitionTo(sd);
  slinkSenderPtr_->connect(*slinkBuilderPtr_);
  */

  const std::vector<LpgbtPairDefinition>& vLpd(definition_.lpgbtPairDefinitions());

  for(unsigned l(0);l<vLpd.size();l++) {
    lpgbtPairPtrVector_.push_back(new LpgbtPair());
    lpgbtPairPtrVector_[l]->setDefinitionTo(vLpd[l]);
    slinkBuilderPtr_->connect(*(lpgbtPairPtrVector_[l]));
  }
}

const SlinkDefinition& Slink::definition() const {
  return definition_;
}

// Arguments

void Slink::setArgumentsTo(const Arguments &a) {
  //slinkBuilderPtr_->setDebugPrintTo(a.debugPrintSlinkBuilder());
  //slinkSenderPtr_->setDebugPrintTo(a.debugPrintSlinkBuilder());
  slinkBuilderPtr_->setArgumentsTo(a);
  //slinkSenderPtr_->setArgumentsTo(a);

  for(unsigned l(0);l<lpgbtPairPtrVector_.size();l++) {
    lpgbtPairPtrVector_[l]->setArgumentsTo(a);
  }
}

// Connections

void Slink::connect(FastControlDriver &fcd) {
  slinkBuilderPtr_->connect(fcd);
  for(unsigned l(0);l<lpgbtPairPtrVector_.size();l++) {
    lpgbtPairPtrVector_[l]->connect(fcd);
  }
}

void Slink::connect(HgcrocAsics &has) {
  for(unsigned l(0);l<lpgbtPairPtrVector_.size();l++) {
    lpgbtPairPtrVector_[l]->connect(has);
  }
}

void Slink::connect(DthChecker &dc) {
  Connection< std::pair<bool,SlinkWord> > *w(new Connection< std::pair<bool,SlinkWord> >());
  dc.connectRxPortTo(*w);
  connectTxPortTo(*w);
}

void Slink::connectTxPortTo(Connection< std::pair<bool,SlinkWord> > &w) {
  //slinkSenderPtr_->connectTxPortTo(w);  
  slinkBuilderPtr_->connectTxPortTo(w);  
}

void Slink::print(const std::string &s) const {
  std::cout << s << "Slink::print() " << definition_.label() << std::endl;
  std::cout << s << " Number of lpGBT pairs = " << lpgbtPairPtrVector_.size() << std::endl;
  for(unsigned l(0);l<lpgbtPairPtrVector_.size();l++) {
    lpgbtPairPtrVector_[l]->print(s+" ");
  }
  std::cout << std::endl;
}
