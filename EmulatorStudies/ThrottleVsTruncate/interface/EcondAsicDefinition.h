#ifndef Emulation_HGCalBufferModel_EcondAsicDefinition_h
#define Emulation_HGCalBufferModel_EcondAsicDefinition_h

#include <cstdint>
#include <string>
#include <map>

class EcondAsicDefinition {
 public:
  enum {
    IdlePattern=0x1d10 // Allow buffer occupancy bits in LSBs
  };
  
  EcondAsicDefinition();

  void setSectorNumberTo(unsigned s);
  
  void setDefinitionTo(unsigned l, unsigned u, unsigned v, unsigned t, char s, unsigned e);
  /* unsigned layer() const; */
  /* unsigned u() const; */
  /* unsigned v() const; */
  /* unsigned type() const; */
  /* char shape() const; */
  /* unsigned numberOfElinks() const; */

  //  void setDefinitionTo(unsigned l, unsigned u, unsigned v,unsigned t, char s, unsigned e1, int e2);
  unsigned layer() const;
  unsigned u() const;
  unsigned v() const;
  unsigned type() const;
  char shape() const;
  unsigned luv() const;
  
  unsigned numberOfElinks() const;
  //  int numberOfElinkss() const;
  uint16_t halfWordId() const;
  uint32_t idleWord() const;
  
  unsigned numberOfChannels() const;
  unsigned numberOfStandardChannels() const;
  unsigned numberOfHalfHgcrocs() const;

  static void readDaqModules();
  static unsigned numberOfChannels(unsigned t, char c);
  static unsigned numberOfStandardChannels(unsigned t, char c);
  static unsigned numberOfHalfHgcrocs(unsigned t, char c);

  void setTypeTo(unsigned t);
  void setNumberOfElinksTo(unsigned n);
  
  // Need to implement these when partials are understood
  unsigned numberOfSensorCells() const;
  unsigned numberOfTriggerCells() const;

  std::string label() const;

  bool operator==(const EcondAsicDefinition &d) const;

  bool operator<(const EcondAsicDefinition& other) const;

  void print(const std::string &s="") const;
    
 private:  
  static bool initialisedDaqModules_;
  static std::map<std::pair<unsigned,char>,unsigned> numberOfChannelsMap_;
  static std::map<std::pair<unsigned,char>,unsigned> numberOfHalfHgcrocsMap_;
  
  unsigned sectorNumber_;
  unsigned layer_;
  unsigned u_;
  unsigned v_;
  unsigned type_;
  char shape_;
  unsigned numberOfElinks_;
  //  int numberOfElinkss_;
};

#endif
