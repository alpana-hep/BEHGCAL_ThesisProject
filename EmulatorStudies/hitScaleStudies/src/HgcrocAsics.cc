#include <iostream>
#include <iomanip>
#include <cassert>

#include "Utils.h"
#include "HgcrocAsics.h"

HgcrocAsics::HgcrocAsics() :
  Entity("HgcrocAsics",Clocks::Clock0040),
  initialised_(false),
  hits_(nullptr),
  switchToRandom_(true),
  numberOfEvents_(0) {
}

HgcrocAsics::~HgcrocAsics() {
}

// Initialisation
void HgcrocAsics::setArgumentsTo(const Arguments &a) {
  setupHitProvider(std::string("data/CMSSW/")+a.hitModifierFile());
  hitProvider()->setReadUnmasked(a.config().at("hitModifierReadUnmasked").get<bool>());
  
  // AG: Not good to hardcode "data/CMSSW" here - we might want to specify an
  // absolute path, e.g. to a file on EOS at CERN
  if (ends_with(a.filename(), ".txt")) {
    hitProvider()->setFileList(a.filename(),
                            "beLinksNTuple/EventTree");
  } else {
    hitProvider()->setFile(a.filename(),
                            "beLinksNTuple/EventTree"); // Andrew files
  }

  hitProvider()->setCurrentEvent(a.config().at("fileStartEvent").get<int>());

  hitProvider()->setReadMode(a.config().at("fileReadMode").get<int>());
  hitProvider()->setRandomFrac(a.config().at("randomFileReadFraction").get<double>());

  Entity::setArgumentsTo(a);
}

void HgcrocAsics::initialise() {
  std::cout << "HgcrocAsics::initialise()" << std::endl
	    << " Number of data connections = "
	    << txNumbersPtrVector_.size() << std::endl
	    << " Number of ECON-D definitions = "
	    << econdAsicDefinitionVector_.size()
	    << std::endl;
  if(switchToRandom_) std::cout << "Random" << std::endl;
  
  if (initialised_) return;
  for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
    econdAsicDefinitionVector_[i].print();
  }
  std::cout << std::endl;
  
  assert(txNumbersPtrVector_.size()==econdAsicDefinitionVector_.size());

  // AG 28/5/20 - Need to sort the definition and connection vectors in tandem - do this by copying
  // into a vector of pairs, sorting, then copying back again, overwriting the originals
  using DefPair = std::pair<EcondAsicDefinition, Connection<std::array<uint16_t, 3>> *>;
  std::vector<DefPair> sorted(txNumbersPtrVector_.size());
  for (unsigned i(0); i < econdAsicDefinitionVector_.size(); i++) {
    sorted[i] = std::make_pair(econdAsicDefinitionVector_[i], txNumbersPtrVector_[i]);
  }
  std::sort(sorted.begin(), sorted.end(),
            [](DefPair const &p1, DefPair const &p2) { return p1.first < p2.first; });
  for (unsigned i(0); i < econdAsicDefinitionVector_.size(); i++) {
    econdAsicDefinitionVector_[i] = sorted[i].first;
    txNumbersPtrVector_[i] = sorted[i].second;
  }

  // AG 28/5/20 - Uncomment to verify the sorting has worked correctly
  // for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
  //   econdAsicDefinitionVector_[i].print();
  // }

  initialised_ = true;
}

void HgcrocAsics::setupHitProvider(std::string const& config) {
  initialise();
  if (hits_ == nullptr) {
    hits_ = new HGCHitProvider();
    switchToRandom_=false;
  }
  hits_->initialise(econdAsicDefinitionVector_, config);
}

// Connections
void HgcrocAsics::addDefinition(const EcondAsicDefinition &d) {
  econdAsicDefinitionVector_.push_back(d);
  initialised_ = false;
}
  
// Connections
void HgcrocAsics::connect(EcondAsic &e) {
  Connection< std::array<uint16_t,3> > *a(new Connection< std::array<uint16_t,3> >);
  connectTxPortTo(*a);
  e.connectRxPortTo(*a);
  addDefinition(e.definition());
  initialised_ = false;
}

// Output
void HgcrocAsics::connectTxPortTo(Connection< std::array<uint16_t,3> > &a) {
  txNumbersPtrVector_.push_back(&a);
  initialised_ = false;
}

void HgcrocAsics::strobeTx() {
  for(unsigned i(0);i<txNumbersPtrVector_.size();i++) {
    txNumbersPtrVector_[i]->strobeInput();
  }
}

// Processing
void HgcrocAsics::process() {
  // For process to work must have called initialise() first, to ensure vectors are sorted
  assert(initialised_);

  if(debugPrint_) {
    std::cout << "HgcrocAsics::process()  entering" << std::endl;
  }
  
  if(txNumbersPtrVector_.size()>0) {
    unsigned queueSize(txNumbersPtrVector_[0]->size());

    if(debugPrint_) {
      std::cout << " Queue size 0 = " << queueSize << std::endl;
    }

    // Check they all have seen the same number of events
    for(unsigned i(1);i<txNumbersPtrVector_.size();i++) {
      if(debugPrint_) {
	std::cout << " Queue size " << i << " = "
		  << txNumbersPtrVector_[i]->size() << std::endl;
      }
      assert(txNumbersPtrVector_[i]->size()==queueSize);
    }

    
    if(queueSize==0) {  // If true, must add at least one event to queue
      
      std::array<uint16_t,3> a;
      
      // Might be able to add multiple ones (not yet working)
      for(unsigned n(0);n<1;n++) {
	
        bool hitsOk = hits_ ? hits_->nextEvent() : false;
	if(debugPrint_) {
	  std::cout << " hitsOk = " << (hitsOk?"true":"false") << std::endl;
	}
	
	// Must send to all connected ECON-Ds
	for(unsigned i(0);i<txNumbersPtrVector_.size();i++) {
	  if (hitsOk) {
	    a[0]=hits_->transformedHits().at(i).nhits10;
	    a[1]=hits_->transformedHits().at(i).nhits20;
	    a[2]=hits_->transformedHits().at(i).nhits30;

	  } else {
	    // If we've reached the end of the file, or if no file was given
	    a[0]=RandomSingleton::random().Integer(20); // n10
	    a[1]=RandomSingleton::random().Integer( 5); // n20
	    a[2]=RandomSingleton::random().Integer( 2); // n30
	    
	    if(!switchToRandom_) {
	      std::cout << "WARNING: Changing to random number hit generation for BX number "
			<< Clocks::clocks().bxNumber() << std::endl;
	      switchToRandom_=true;
	      Entity::stopTick();
	    }
	  }
	  
	  if(debugPrint_) {
	    std::cout << " ECON-D " << std::setw(5) << i
		      << ", numbers of n20 hits = "
		      << std::setw(3) << a[0] << " and n30 hits = "
		      << std::setw(3) << a[1] << std::endl;
	  }
	  
	  // Do the actual transmission
	  assert(txNumbersPtrVector_[i]->set(a));
	}

	// Keep count of how many have been sent
	numberOfEvents_++;
      }
    }
  }
    
  if(debugPrint_) {
    std::cout << "HgcrocAsics::process()  exiting" << std::endl;
  }
}

void HgcrocAsics::print() const {
  std::cout << "HgcrocAsics::print()" << std::endl;
  std::cout << " Number of events sent = " << numberOfEvents_ << std::endl;
  std::cout << std::endl;
}
