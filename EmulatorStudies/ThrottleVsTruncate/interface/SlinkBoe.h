#ifndef Emulation_HGCalBufferModel_SlinkBoe_h
#define Emulation_HGCalBufferModel_SlinkBoe_h

#include "SlinkWord.h"
#include "FastControlCounters.h"


class SlinkBoe : public SlinkWord {
 public:
  enum {
    BoePattern=0x55
  };
  
  SlinkBoe();

  SlinkBoe(const SlinkWord &w);

  SlinkBoe(uint32_t si, uint32_t bi, uint32_t ei, uint16_t et);
  SlinkBoe(uint32_t si, const FastControlCounters &c, uint16_t et);

  void setSourceIdTo(uint32_t s);  
  void setBxIdTo(uint32_t b);
  void setEventIdTo(uint32_t e);  
  void setEventTypeTo(uint16_t t);

  // Assess to data fields
  uint32_t sourceId() const;
  uint32_t bxId() const;
  uint16_t bc() const;
  uint32_t oc() const;
  uint32_t ec() const;
  uint32_t eventId() const;
  uint16_t eventType() const;
  uint8_t boe() const;
  bool valid() const;

  // HGCAL specific (using source id)
  unsigned detector() const;
  
  unsigned sectorNumber() const;
  unsigned fpgaNumber() const;
  unsigned slinkNumber() const;
  unsigned numberOfLpgbtPairs() const;

  void setSectorNumberTo(unsigned s);
  void setFpgaNumberTo(unsigned f);
  void setSlinkNumberTo(unsigned s);
  void setNumberOfLpgbtPairsTo(unsigned n);

  // Counters
  const FastControlCounters& counters() const;
  void setCountersTo(const FastControlCounters &c);
  bool operator==(const FastControlCounters &c) const;

  void print(const std::string &s="") const;
};

#endif
