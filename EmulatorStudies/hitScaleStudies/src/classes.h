#include "EcondAsicHeader.h"
#include "EcondData.h"
#include "LpgbtPairData.h"
#include "LpgbtPairHeader.h"
#include "SlinkData.h"
#include "EventData.h"
#include "SectorDefinition.h"
#include "FpgaDefinition.h"
#include "SlinkDefinition.h"
#include "LpgbtPairDefinition.h"
#include "LpgbtDefinition.h"
#include "FastControlStreamDefinition.h"
#include "EcondAsicDefinition.h"
#include <vector>
namespace {
  struct dictionary {
    EcondAsicHeader dummy1;
    EcondData dummy2;
    LpgbtPairData dummy3;
    LpgbtPairHeader dummy4;
    SlinkBoe dummy5;
    SlinkEoe dummy6;
    SlinkWord dummy7;
    SlinkData dummy8;
    EventData dummy9;
    std::vector<SlinkData> dummy10;
    std::vector<LpgbtPairData> dummy11;
    std::vector<EcondData> dummy12;
    EcondAsicDefinition dummy13;
  };
}  // namespace