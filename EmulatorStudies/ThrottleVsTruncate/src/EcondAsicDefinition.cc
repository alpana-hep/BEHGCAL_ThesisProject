#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <vector>

#include "EcondAsicDefinition.h"
#include "EcondAsicHitCounts.h"
#include "EcondAsicIdle.h"

bool EcondAsicDefinition::initialisedDaqModules_(false);
std::map<std::pair<unsigned,char>,unsigned> EcondAsicDefinition::numberOfChannelsMap_;
std::map<std::pair<unsigned,char>,unsigned> EcondAsicDefinition::numberOfHalfHgcrocsMap_;

EcondAsicDefinition::EcondAsicDefinition() :
  sectorNumber_(0),
  numberOfElinks_(1) {
}

void EcondAsicDefinition::setDefinitionTo(unsigned l, unsigned u, unsigned v,
					  unsigned t, char s, unsigned e) {
  layer_=l;
  u_=u;
  v_=v;
  type_=t;
  shape_=s;
  numberOfElinks_=e;
}
// void EcondAsicDefinition::setDefinitionTo(unsigned l, unsigned u, unsigned v,
//                                           unsigned t, char s, unsigned e , int e1) {
//   layer_=l;
//   u_=u;
//   v_=v;
//   type_=t;
//   shape_=s;
//   numberOfElinks_=e;
//   numberOfElinkss_=e1;
// }

void EcondAsicDefinition::setSectorNumberTo(unsigned s) {
  sectorNumber_=s;
}
  
unsigned EcondAsicDefinition::layer() const {
  return layer_;
}

unsigned EcondAsicDefinition::u() const {
  return u_;
}

unsigned EcondAsicDefinition::v() const {
  return v_;
}

unsigned EcondAsicDefinition::type() const {
  return type_;
}

char EcondAsicDefinition::shape() const {
  return shape_;
}

unsigned EcondAsicDefinition::luv() const {
  return 256*(layer_-1)+16*u_+v_;
}

unsigned EcondAsicDefinition::numberOfElinks() const {
  return numberOfElinks_;
}

//int  EcondAsicDefinition::numberOfElinkss() const {//
//return numberOfElinkss_;
//}

uint16_t EcondAsicDefinition::halfWordId() const {
  return 16*16*16*(layer_/10)+16*16*(layer_%10)+16*u_+v_;
}

uint32_t EcondAsicDefinition::idleWord() const {
  //return uint32_t(halfWordId())<<16|IdlePattern;
  return EcondAsicIdle().word();
}

bool EcondAsicDefinition::operator==(const EcondAsicDefinition &d) const {
  return layer_==d.layer_ && u_==d.u_ && v_==d.v_ && type_==d.type_ &&
    shape_==d.shape_ && numberOfElinks_==d.numberOfElinks_;
}

bool EcondAsicDefinition::operator<(const EcondAsicDefinition &other) const {
  return (*this).layer_ < other.layer_ ||
         ((*this).layer_ == other.layer_ &&
          ((*this).u_ < other.u_ || ((*this).u_ == other.u_ && (*this).v_ < other.v_)));
}

std::string EcondAsicDefinition::label() const {
  std::ostringstream sout;
  sout << "Sector" << sectorNumber_ << std::setfill('0')
       << "_Layer" << std::setw(2) << layer_;
  if(type_<2) {
    sout << "_U" << std::setw(2) << u_
	 << "_V" << std::setw(2) << v_;
  } else {
    sout << "_H" << std::setw(2) << u_
	 << "_F" << std::setw(2) << v_;
  }
  return sout.str();
}

unsigned EcondAsicDefinition::numberOfChannels() const {
  if(!initialisedDaqModules_) readDaqModules();
  return numberOfChannels(type_,shape_);
}

unsigned EcondAsicDefinition::numberOfStandardChannels() const {
  if(!initialisedDaqModules_) readDaqModules();
  return numberOfStandardChannels(type_,shape_);
}

unsigned EcondAsicDefinition::numberOfHalfHgcrocs() const {
  if(!initialisedDaqModules_) readDaqModules();
  return numberOfHalfHgcrocs(type_,shape_);
}

void EcondAsicDefinition::readDaqModules() {
  if(initialisedDaqModules_) return;
  
  std::ifstream fin("data/FE/DaqModules.txt");
  assert(fin);

  std::pair<std::pair<unsigned,char>,unsigned> value;
  fin >> value.first.first;

  std::vector< std::pair<unsigned,char> > v;

  while(fin) {
    fin >> value.first.second;
    v.push_back(value.first);

    fin >> value.second;
    numberOfChannelsMap_.insert(value);
    fin >> value.second;
    numberOfHalfHgcrocsMap_.insert(value);

    fin >> value.first.first;
  }

  initialisedDaqModules_=true;

  std::cout << "EcondAsicDefinition::readDaqModules() Number of modules = "
	    << v.size() << std::endl;

  EcondAsicDefinition d;
  for(unsigned i(0);i<v.size();i++) {
    d.setDefinitionTo(0,0,0,v[i].first,v[i].second,0);
    std::cout << std::setw(2) << v[i].first
	      << " " << v[i].second << " "
	      << std::setw(3) << numberOfChannelsMap_[v[i]] << " "
	      << std::setw(2) << numberOfHalfHgcrocsMap_[v[i]]
	      << ",  min, max words = " << std::setw(2)
	      << EcondAsicHitCounts::daqRawDataPacketWords(d,0,0,0)/2
	      << ", " << std::setw(3)
	      << EcondAsicHitCounts::daqRawDataPacketWords(d,0,0,d.numberOfChannels())/2
	      << std::endl;
  }
}

unsigned EcondAsicDefinition::numberOfChannels(unsigned t, char c) {
  // These counts include sensor cells and calibration cells
  return numberOfStandardChannels(t,c)+numberOfHalfHgcrocs(t,c);
  
  /*
  // Silicon
  if(t==0) { // HDM
    
    // Full
    if(c=='F') return 432+12;
    
    // ALL APPROX
    
    // Upper half
    //if(c=='a') return 220+6;
    
    // Lower half
    if(c=='a') return 220+6;
    
    // Five
    if(c=='b') return 370+10;
    
    // Semi
    if(c=='d') return 220+6;
    
    // Choptwo
    if(c=='g') return 370+10;
    
    return 0;
  }
  
  if(t==1) { // LDM
    
    // Full
    if(c=='F') return 192+6;
    
    // Upper half
    //if(c=='a') return 92+3;
    
    // Lower half
    if(c=='a') return 100+3;
    
    // Five
    if(c=='b') return 164+5;
    
    // Semi
    if(c=='d') return 100+3;
    
    // Full+three; left and right
    if(c=='L' || c=='R') return 192+6+34+1;
    
    return 0;
  }
  
  // Scintillator
  return 8*12; // ??? Not yet known
  */
}

unsigned EcondAsicDefinition::numberOfStandardChannels(unsigned t, char c) {
  return numberOfChannelsMap_[std::make_pair(t,c)];
}

unsigned EcondAsicDefinition::numberOfHalfHgcrocs(unsigned t, char c) {
  return numberOfHalfHgcrocsMap_[std::make_pair(t,c)];
}

void EcondAsicDefinition::setTypeTo(unsigned t) {
  type_=t;
}

void EcondAsicDefinition::setNumberOfElinksTo(unsigned n) {
  numberOfElinks_=n;
}

void EcondAsicDefinition::print(const std::string &s) const {
  std::cout << s << "EcondAsicDefinition::print()" << std::endl;
  if(type_<2) std::cout << s << " Sector, layer, u, v = " << sectorNumber_
			 << ", " << std::setw(2) << layer_
			 << ", " << std::setw(2) << u_
			 << ", " << std::setw(2) << v_
			 << ", (" << label() << ")" << std::endl
			 << s << " Type = " << (type_==0?"Si HDM":"Si LDM");
  else        std::cout << s << " Sector, layer, eta, phi = " << sectorNumber_
			 << ", " << std::setw(2) << layer_
			 << ", " << std::setw(2) << u_
			 << ", " << std::setw(2) << v_
			<< ", (" << label() << ")" << std::endl
			<< s << " Type = " << type_ << " = Scint";
  std::cout << ", shape = " << shape_
	    << ", number of elinks = " << numberOfElinks_
	    << ", number of channels = " << numberOfChannels()
	    << ", number of half-HGCROCs = " << numberOfHalfHgcrocs()
            << std::endl;
}
