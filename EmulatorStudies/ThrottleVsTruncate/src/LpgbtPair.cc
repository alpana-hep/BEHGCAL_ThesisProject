#include <iostream>
#include <cassert>

#include "LpgbtPair.h"

LpgbtPair::LpgbtPair() {
}

LpgbtPair::~LpgbtPair() {
}

void LpgbtPair::setDefinitionTo(const LpgbtPairDefinition &lpd) {
  definition_=lpd;

  lpgbtPairSplitter_=new LpgbtPairSplitter();
  lpgbtPairSplitter_->setDefinitionTo(definition_);

  lpgbtPairBuilder_=new LpgbtPairBuilder();
  lpgbtPairBuilder_->setDefinitionTo(definition_);

  const std::vector<LpgbtDefinition>& vLd(definition_.lpgbtDefinitions());
  for(unsigned l(0);l<vLd.size();l++) {

    const std::vector<FastControlStreamDefinition>& vFcsd(vLd[l].fastControlStreamDefinitions());
    for(unsigned f(0);f<vFcsd.size();f++) {
      fastControlStreamVector_.push_back(new FastControlStream());
      fastControlStreamVector_.back()->setDefinitionTo(vFcsd[f]);
      fastControlStreamVector_.back()->setDebugPrintTo(true);

      const std::vector<EcondAsicDefinition>& vEad(vFcsd[f].econdAsicDefinition());

      econdAsicVectorVector_.push_back(std::vector<EcondAsic*>());
      lpgbtPairEcondBuilderVectorVector_.push_back(std::vector<LpgbtPairEcondBuilder*>());

      for(unsigned e(0);e<vEad.size();e++) {
	econdAsicVectorVector_.back().push_back(new EcondAsic());
        econdAsicVectorVector_.back().back()->setDefinitionTo(vEad[e]);
        econdAsicVectorVector_.back().back()->connect(*(fastControlStreamVector_.back()));
	lpgbtPairSplitter_->connect(*(econdAsicVectorVector_.back().back()));

        lpgbtPairEcondBuilderVectorVector_.back().push_back(new LpgbtPairEcondBuilder());
        lpgbtPairEcondBuilderVectorVector_.back().back()->setDefinitionTo(vEad[e]);
	lpgbtPairEcondBuilderVectorVector_.back().back()->connect(*(fastControlStreamVector_.back()));
	lpgbtPairSplitter_->connect(*(lpgbtPairEcondBuilderVectorVector_.back().back()));
	lpgbtPairBuilder_->connect(*(lpgbtPairEcondBuilderVectorVector_.back().back()));
      }
    }
  }
}

// Arguments

void LpgbtPair::setArgumentsTo(const Arguments &) {
  /*
  //lpgbtPairSplitter_->setDebugPrintTo(a.debugPrintLpgbtBuilder());
  //lpgbtPairBuilder_->setDebugPrintTo(a.debugPrintLpgbtBuilder());
  lpgbtPairSplitter_->setArgumentsTo(a);
  lpgbtPairBuilder_->setArgumentsTo(a);
  
  for(unsigned f(0);f<fastControlStreamVector_.size();f++) {
    //fastControlStreamVector_[f]->setDebugPrintTo(a.debugPrintFastControlStream());
    fastControlStreamVector_[f]->setArgumentsTo(a);
    for(unsigned e(0);e<econdAsicVectorVector_[f].size();e++) {
      //econdAsicVectorVector_[f][e]->setDebugPrintTo(a.debugPrintEcondAsic());
      econdAsicVectorVector_[f][e]->setArgumentsTo(a);
    }
    for(unsigned e(0);e<lpgbtPairEcondBuilderVectorVector_[f].size();e++) {
      //lpgbtPairEcondBuilderVectorVector_[f][e]->setDebugPrintTo(a.debugPrintEcondAsic());
      lpgbtPairEcondBuilderVectorVector_[f][e]->setArgumentsTo(a);
    }
  }

  */
  assert(false);
}

// Connections

void LpgbtPair::connect(FastControlDriver &fcd) {
  lpgbtPairBuilder_->connect(fcd);
  for(unsigned f(0);f<fastControlStreamVector_.size();f++) {
    fastControlStreamVector_[f]->connect(fcd);

    for(unsigned e(0);e<lpgbtPairEcondBuilderVectorVector_[f].size();e++) {
      lpgbtPairEcondBuilderVectorVector_[f][e]->connect(fcd);
    }
  }
}

void LpgbtPair::connect(HgcrocAsics &has) {
  for(unsigned f(0);f<fastControlStreamVector_.size();f++) {
    for(unsigned e(0);e<econdAsicVectorVector_[f].size();e++) {
      has.connect(*(econdAsicVectorVector_[f][e]));
    }
  }
}

void LpgbtPair::connectTxPortTo(Connection<RamWord> &w) {
  lpgbtPairBuilder_->connectTxPortTo(w);
}

void LpgbtPair::print(const std::string &s) const {
  std::cout << s << "LpgbtPair::print() " << definition_.label() << std::endl;
  for(unsigned f(0);f<fastControlStreamVector_.size();f++) {
    for(unsigned e(0);e<econdAsicVectorVector_[f].size();e++) {
      std::cout << s << " EcondAsic "
		<< econdAsicVectorVector_[f][e]->definition().label()
		<< std::endl;
    }
  }

  std::cout << std::endl;
}
