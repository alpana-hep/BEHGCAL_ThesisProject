#ifndef Emulation_HGCalBufferModel_Sector_h
#define Emulation_HGCalBufferModel_Sector_h

#include <vector>
#include <queue>

#include "Arguments.h"
#include "FastControlDriver.h"
#include "HgcrocAsics.h"
#include "SectorDefinition.h"
#include "Fpga.h"
#include "DaqUnit.h"

class Sector {
 public:
  Sector();
  virtual ~Sector();

  // Initialisation
  void setDefinitionTo(const SectorDefinition &sd);
  void setArgumentsTo(const Arguments &a);
  
  // Connections
  void connect(FastControlDriver &fcd);
  void connect(HgcrocAsics &has);

  const std::vector<DaqUnit*>& daqUnit();
  
  void print(const std::string &s="") const;
  
 private:

  //Initialisation
  SectorDefinition definition_;
  std::vector<DaqUnit*> daqUnitPtrVector_;
};

#endif
