#ifndef Emulation_HGCalBufferModel_SlinkEoe_h
#define Emulation_HGCalBufferModel_SlinkEoe_h

#include "SlinkWord.h"


class SlinkEoe : public SlinkWord {
 public:
  enum {
    EoePattern=0xaa
  };
  
  SlinkEoe();

  SlinkEoe(const SlinkWord &w);

  SlinkEoe(uint32_t f, uint16_t s, uint16_t c, uint32_t l);

  void initialise();

  void setFedUserIdTo(uint32_t f);
  void setSubSystemUserTo(uint32_t f); // Alternative name   
  void setStatusTo(uint16_t s);  
  void setCrcTo(uint16_t c);  
  void setEventLengthTo(uint32_t l);
  void incrementEventLength();
  
  uint32_t fedUserId() const;
  uint32_t subSystemUser() const; // Alternative name   
  uint16_t status() const;
  uint16_t crc() const;
  uint32_t eventLength() const;
  uint8_t eoe() const;

  bool valid() const;

  void print(const std::string &s="") const;
};

#endif
