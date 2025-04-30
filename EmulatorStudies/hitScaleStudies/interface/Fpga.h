#ifndef Emulation_HGCalBufferModel_Fpga_h
#define Emulation_HGCalBufferModel_Fpga_h

#include <vector>
#include <queue>

#include "Arguments.h"
#include "FastControlDriver.h"
#include "HgcrocAsics.h"
#include "FpgaDefinition.h"
#include "Slink.h"

class Fpga {
 public:
  Fpga();
  virtual ~Fpga();

  // Initialisation
  void setDefinitionTo(const FpgaDefinition &fd);
  void setArgumentsTo(const Arguments &a);
  
  // Connections
  void connect(FastControlDriver &fcd);
  void connect(HgcrocAsics &has);

  // Accessor
  std::vector<Slink*>& slinkPtrVector();
  
  void print(const std::string &s="") const;
  
 private:

  // Initialisation
  FpgaDefinition definition_;
  
  std::vector<Slink*> slinkPtrVector_;
  std::vector<DthChecker*> dthCheckerPtrVector_;
};

#endif
