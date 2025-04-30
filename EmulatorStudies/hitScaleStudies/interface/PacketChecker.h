#ifndef Emulation_HGCalBufferModel_PacketChecker_h
#define Emulation_HGCalBufferModel_PacketChecker_h

#include <cstdint>
#include <vector>

#include "EcondAsicDefinition.h"
//#include "EcontAsic.h"
#include "FastControlCounters.h"
#include "RamWord.h"
#include "SlinkWord.h"
#include "SlinkDefinition.h"
#include "LpgbtPairDefinition.h"

class PacketChecker {
 public:
  PacketChecker();

  static bool checkEcondAsicPacket16(const std::vector<uint16_t> &v, bool p=false);
  static bool checkEcondAsicPacket32(const std::vector<uint32_t> &v, bool p=false);
  static bool checkEcondAsicPacket72(const std::vector<RamWord> &v, bool p);
  
  static bool checkEcondBeBuilderPacket32(const std::vector<uint32_t> &v, bool p=false);
  static bool checkEcondBeBuilderPacket72(const std::vector<RamWord> &v, bool p=false);
  
  //static bool checkLpgbtBuilderPacket32(const std::vector<uint32_t> &v, bool p);
  static bool checkLpgbtBuilderPacket72(const std::vector<RamWord> &v, bool p=false);

  static bool checkEcondAsicPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<uint32_t> &v, bool p);
  static bool checkEcondAsicPacket72(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p);

  static bool checkEcondBePacket32(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<uint32_t> &v, bool p);
  static bool checkEcondBePacket72(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p);

  static bool checkLpgbtPacket(const LpgbtDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p);
  static bool checkLpgbtPairPacket(const LpgbtPairDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p);
  static bool checkLpgbtPairBuilderPacket(const LpgbtPairDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p);
  static bool checkSlinkPacket(const SlinkDefinition &d, const FastControlCounters &c, const std::vector<SlinkWord> &v, bool p);
  static bool checkSlinkBuilderPacket(const SlinkDefinition &d, const FastControlCounters &c, const std::vector<SlinkWord> &v, bool p);



  //static bool checkEcontAsicPacket(const EcontAsicDefinition &d, unsigned bx, const std::vector<uint16_t> &v, bool p);
};

#endif
