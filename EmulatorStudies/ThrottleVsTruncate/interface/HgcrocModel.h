#ifndef Emulation_HGCalBufferModel_HgcrocModel_h
#define Emulation_HGCalBufferModel_HgcrocModel_h

#include <queue>

#include "FastControlCommands.h"
#include "FastControlCounters.h"


class HgcrocModel {
 public:
  enum {
    PacketTransmitTime=40,
    BufferSize=32
    //BufferSize=12 // Plenty of throttles
  };

  HgcrocModel();

  void update(const FastControlCommands &c);
  void eventBufferReset();
  
  bool l1a(FastControlCounters &c) const;
  
  unsigned occupancy() const;
  bool fullBuffer() const;
  unsigned activeBx() const;
  
  void print() const;
    
 private:
  FastControlCounters fFastControlCounters;

  bool newL1a_;
  unsigned bx_;
  unsigned activeBx_;
  std::queue<FastControlCounters> counterQueue_;
};

#endif
