#ifndef Emulation_HGCalBufferModel_PacketCreator_h
#define Emulation_HGCalBufferModel_PacketCreator_h

#include <cstdint>
#include <vector>
#include <queue>

#include "RamWord.h"
#include "SlinkWord.h"
#include "EcondAsicDefinition.h"
//#include "EcontAsic.h"
#include "SlinkDefinition.h"
#include "FastControlCounters.h"

class PacketCreator {
 public:
  PacketCreator();

  static void createEcondAsicPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<uint32_t> &v, bool p);
  static void createEcondAsicPacket72(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<RamWord> &v, bool p);
  
  static void createEcondBePacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<uint32_t> &v, bool p);
  static void createEcondBePacket72(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<RamWord> &v, bool p);

  static void createEcondAsicPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n16, std::vector<uint32_t> &v, bool p);
  static void createEcondBeBuilderPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n16, std::vector<uint32_t> &v, bool p);

  static void createLpgbtPacket(const LpgbtDefinition &d, const FastControlCounters &c, const std::vector<unsigned> &v10, const std::vector<unsigned> &v20, const std::vector<unsigned> &v30, std::vector<RamWord> &v, bool p);
  static void createSlinkPacket(const SlinkDefinition &d, const FastControlCounters &c, const std::vector<unsigned> &v10, const std::vector<unsigned> &v20, const std::vector<unsigned> &v30, std::vector<SlinkWord> &v, bool p);



  //static void createEcontAsicPacket(const EcontAsicDefinition &d, unsigned bx, const std::vector<uint32_t> &vTc, std::vector<uint16_t> &v, bool p);
};

#endif
