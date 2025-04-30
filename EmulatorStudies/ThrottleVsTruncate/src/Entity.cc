#include <iostream>
#include <iomanip>
#include <sstream>

#include "Backtrace.h"
#include "Entity.h"

bool Entity::stopTick_(false);
std::vector<Entity*> Entity::entityVector_[Clocks::NumberOfFrequencies];

Entity::Entity(const std::string &n, Clocks::Frequency f) :
  name_(n),
  frequency_(f),
  //entityClass_(e),
  debugPrint_(false),
  debugCheck_(false),
  doAnalysis_(false) {

  // Clean up!
  entityClass_=TestEntity;
  if(name_=="FastControlDriver") entityClass_=EntityFastControlDriver;
  if(name_=="HgcrocAsics") entityClass_=EntityHgcrocAsics;
  if(name_=="FastControlStream") entityClass_=EntityFastControlStream;
  if(name_=="EcondAsic") entityClass_=EntityEcondAsic;
  if(name_=="EcontAsic") entityClass_=EntityEcontAsic;
  if(name_=="LpgbtPairSplitter") entityClass_=EntityLpgbtPairSplitter;
  if(name_=="LpgbtPairEcondBuilder") entityClass_=EntityLpgbtPairEcondBuilder;
  if(name_=="LpgbtPairBuilder") entityClass_=EntityLpgbtPairBuilder;
  if(name_=="SlinkBuilder") entityClass_=EntitySlinkBuilder;
  if(name_=="SlinkSender") entityClass_=EntitySlinkSender;
  if(name_=="DthChecker") entityClass_=EntityDthChecker;
  if(name_=="D2sUnit") entityClass_=EntityD2sUnit;
  if(name_=="CentralDaq") entityClass_=EntityCentralDaq;
  
  // Add unique id to name
  idNumber_=(entityClass_<<16);
  idNumber_|=entityVector_[f].size();
  std::ostringstream sout;
  sout << "[0x" << std::hex << std::setfill('0')
    //<< std::setw(16) << long(this) << "]";
       << std::setw(8) << idNumber_ << "]";
  name_+=sout.str();

  entityVector_[f].push_back(this);
  Clocks::clocks().enable(f);




  callCount_=0;


}

void Entity::initialise() {
  std::cout << name_ << "::initialise() Dummy" << std::endl;
}

void Entity::setArgumentsTo(const Arguments &a) {
  if(callCount_>0) std::cerr << " " << name_ << "::setArgumentsTo()  Called "
			   << callCount_ << " times" << std::endl;
  callCount_++;

  debugPrint_=(a.debugPrintBits()&entityClass_)!=0;
  debugCheck_=(a.debugCheckBits()&entityClass_)!=0;
  doAnalysis_=(a.doAnalysisBits()&entityClass_)!=0;

  if(idNumber_==a.config().at("entityIdDebugPrint").get<uint32_t>()) debugPrint_=true;
  if(idNumber_==a.config().at("entityIdDebugCheck").get<uint32_t>()) debugCheck_=true;
  if(idNumber_==a.config().at("entityIdDoAnalysis").get<uint32_t>()) doAnalysis_=true;
}

void Entity::setDebugPrintTo(bool b) {
  debugPrint_=b;
}

void Entity::setDebugCheckTo(bool b) {
  debugCheck_=b;
}

void Entity::setDoAnalysisTo(bool b) {
  doAnalysis_=b;
}

void Entity::strobeRx() {
}

void Entity::strobeTx() {
}

void Entity::doTick(const Arguments &a) {
  Clocks::clocks().initialise();

  //for(unsigned f(0);f<Clocks::NumberOfFrequencies;f++) {
  //for(unsigned i(0);i<entityVector_[f].size();i++) {
  //}
  //}
  
  //doTick(a.numberOfBx());
  //}

  //void Entity::doTick(unsigned long b) {
  //Clocks::clocks().initialise();

  for(unsigned f(0);f<Clocks::NumberOfFrequencies;f++) {
    for(unsigned i(0);i<entityVector_[f].size();i++) {
      entityVector_[f][i]->setArgumentsTo(a);
      
      std::cout << "Entity " << i << " " << entityVector_[f][i]->name_
		<< ", calling initialise()" << std::endl;
      entityVector_[f][i]->initialise();
      
      std::cout << " Entity class " << std::setw(5)
		<< entityVector_[f][i]->entityClass_ << ", control flags"
		<< std::endl << "  Debug print = "
		<< (entityVector_[f][i]->debugPrint_?" true":"false")
		<< std::endl << "  Debug check = "
		<< (entityVector_[f][i]->debugCheck_?" true":"false")
		<< std::endl << "  Do analysis = "
		<< (entityVector_[f][i]->doAnalysis_?" true":"false")
		<< std::endl << std::endl;
    }
  }

  unsigned long b(a.numberOfBx());
  unsigned long period(b<10?1:b/10);
  
  Clocks::clocks().update();
  while(Clocks::clocks().bxCount()<=b && !stopTick_) {
    Clocks::Frequency f(Clocks::clocks().state());

    // Print progress
    if(f==Clocks::Clock0040 && (Clocks::clocks().bxNumber()%period)==0) {
      std::cout << "Entity::doTick() Processing BX number "
		<< std::setw(10) << Clocks::clocks().bxNumber()
		<< "/" << b << std::endl;
    }
    
    bool anyDebug(false);
    for(unsigned i(0);i<entityVector_[f].size() && !anyDebug;i++) {
      if(entityVector_[f][i]->debugPrint_) anyDebug=true;
    }
    
    if(anyDebug) std::cout << "Calling strobeRx  for clock frequency "
			   << Clocks::frequencyName(f)
			   << ", tick number "
			   << Clocks::clocks().tickNumber(f)
			   << ", time "
			   << Clocks::clocks().timeMicrosec()
			   << " usec"
			   << (Clocks::clocks().previousClock0040()?
			       ", previous Clock0040":"")
			   << std::endl;
    
    for(unsigned i(0);i<entityVector_[f].size();i++) {
      /*
      if(entityVector_[f][i]->debugPrint_) {
	std::cout << "Entity " << i << " "
		  << entityVector_[f][i]->name_
		  << ", calling strobeRx" << std::endl;
      }
      */
      entityVector_[f][i]->strobeRx();
    }
    
    if(anyDebug) {
      std::cout << std::endl
		<< "Calling processes for clock frequency "
		<< Clocks::frequencyName(f)
		<< ", tick number "
		<< Clocks::clocks().tickNumber(f)
		<< ", time "
		<< Clocks::clocks().timeMicrosec()
		<< " usec";
      if(Clocks::clocks().previousClock0040()) {
	std::cout << ", previous Clock0040 with BX number = "
		  << Clocks::clocks().bxNumber();
      }
      std::cout << std::endl;
    }
    
    for(unsigned i(0);i<entityVector_[f].size();i++) {
      if(entityVector_[f][i]->debugPrint_) {
	std::cout << "Entity " << i << "/" << entityVector_[f].size()
		  << " " << entityVector_[f][i]->name_
		  << ", calling process()" << std::endl;
      }
      entityVector_[f][i]->process();
    }
    
    if(anyDebug) {
      std::cout << std::endl
		<< "Calling strobeTx  for clock frequency "
		<< Clocks::frequencyName(f)
		<< ", tick number "
		<< Clocks::clocks().tickNumber(f)
		<< ", time "
		<< Clocks::clocks().timeMicrosec()
		<< " usec";
      if(Clocks::clocks().previousClock0040()) {
	std::cout << ", previous Clock0040 with BX number = "
		  << Clocks::clocks().bxNumber();
      }
      std::cout << std::endl;
    }
    
    for(unsigned i(0);i<entityVector_[f].size();i++) {
      /*
      if(entityVector_[f][i]->debugPrint_) {
	std::cout << "Entity " << i << " "
		  << entityVector_[f][i]->name_
		  << ", calling strobeTx" << std::endl;
      }
      */
      entityVector_[f][i]->strobeTx();
    }

    if(anyDebug) std::cout << std::endl;
    Clocks::clocks().update();
  }
}

void Entity::stopTick() {
  stopTick_=true;
}

void Entity::doClock() {
  Clocks::clocks().update();
  //Clocks::Frequency f(Clocks::clocks().state());
  /*
  std::cout << "Calling processes for clock frequency " << f << std::endl;
  
  for(unsigned i(0);i<entityVector_[f].size();i++) {
    std::cout << "Entity " << i << "/" << entityVector_[f].size()
	      << " " << entityVector_[f][i]->name_
	      << ", calling process()" << std::endl;
    entityVector_[f][i]->process();
  }
  
  std::cout << "Calling strobes for clock frequency " << f << std::endl;
  
  for(unsigned i(0);i<entityVector_[f].size();i++) {
    std::cout << "Entity " << i << " "
	      << entityVector_[f][i]->name_
	      << ", calling " << entityVector_[f][i]->portRxVector_.size()
	      << " strobes" << std::endl;

    for(unsigned j(0);j<entityVector_[f][i]->portRxVector_.size();j++) {
      std::cout << "Entity " << i << " "
		<< entityVector_[f][i]->name_
		<< ", calling strobe() " << j << " at "
		<< (uint64_t)entityVector_[f][i]->portRxVector_[j] << std::endl;
      entityVector_[f][i]->portRxVector_[j]->strobe();
    }
  }
  */
}

void Entity::printBxTick() const {
  std::cout << name_ << "::printBxTick()  BX number = "
	    << Clocks::clocks().bxNumber() << ", time = "
	    << Clocks::clocks().timeMicrosec() << " usec" << std::endl;
}

void Entity::entityAssert(bool b) const {
  if(b) return;
  printBxTick();
  backtraceAssert(false);
}

void Entity::print() const {
  //std::cout << name_ << "::print() Dummy" << std::endl;
}

void Entity::printAll() {
  std::cout << std::endl << "Entity::printAll()" << std::endl;
  for(unsigned f(0);f<Clocks::NumberOfFrequencies;f++) {
    for(unsigned i(0);i<entityVector_[f].size();i++) {
      std::cout << "Entity " << i << " " << entityVector_[f][i]->name_
		<< ", calling print()" << std::endl;
      entityVector_[f][i]->print();
    }
  }
}

void Entity::printTable() {
  std::cout << std::endl << "Entity::printTable()" << std::endl;
  for(unsigned f(0);f<Clocks::NumberOfFrequencies;f++) {
    if(entityVector_[f].size()>0) {
      std::cout << " Clock frequency " << Clocks::frequencyName((Clocks::Frequency)f)
		<< ", number of entities = "
		<< entityVector_[f].size() << std::endl;
      for(unsigned i(0);i<entityVector_[f].size();i++) {
	std::cout << "  Entity " << std::setw(3) << i
		  << " " << std::setw(20) << entityVector_[f][i]->name_
		  << std::endl;
      }
    }
  }
}
