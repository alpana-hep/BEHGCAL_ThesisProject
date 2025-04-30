#ifndef Emulation_HGCalBufferModel_DaqUnit_h
#define Emulation_HGCalBufferModel_DaqUnit_h

#include <vector>
#include <queue>

#include "Arguments.h"
#include "FastControlDriver.h"
#include "Fpga.h"
#include "DthChecker.h"
#include "D2sUnit.h"

class DaqUnit {
 public:
  DaqUnit();
  virtual ~DaqUnit();

  // Initialisation
  void setDefinitionTo(const FpgaDefinition &fd);
  void setArgumentsTo(const Arguments &a);
  
  // Connections
  void connect(FastControlDriver &fcd);
  void connect(HgcrocAsics &has);

  const std::vector<D2sUnit*>& d2sUnit();
  
  void print(const std::string &s="") const;
  
 private:

  //Initialisation
  std::vector<FpgaDefinition> definitionVector_;

  std::vector<Fpga*> fpgaPtrVector_;  
  std::vector<DthChecker*> dthCheckerPtrVector_;

  unsigned numberOfSlinks_;
  std::vector<D2sUnit*> d2sUnitPtrVector_;
};

#endif
