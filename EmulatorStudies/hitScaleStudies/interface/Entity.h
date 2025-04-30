#ifndef Emulation_HGCalBufferModel_Entity_h
#define Emulation_HGCalBufferModel_Entity_h

#include <vector>
#include <string>

#include "Clocks.h"
#include "Arguments.h"


class Entity {
 public:
  enum EntityClass {
    EntityFastControlDriver    =    1,
    EntityHgcrocAsics          =    2,
    EntityFastControlStream    =    4,
    EntityEcondAsic            =    8,
    EntityEcontAsic            =   16,
    EntityLpgbtPairSplitter    =   32,
    EntityLpgbtPairEcondBuilder=   64,
    EntityLpgbtPairBuilder     =  128,
    EntitySlinkBuilder         =  256,
    EntitySlinkSender          =  512,
    EntityDthChecker           = 1024,
    EntityD2sUnit              = 2048,
    EntityCentralDaq           = 4096,
    
    TestEntity                 =32768
  };
  
  Entity(const std::string &n, Clocks::Frequency f);

  virtual void setArgumentsTo(const Arguments &a);
  virtual void initialise();

  // For debugging
  void setDebugPrintTo(bool b);
  void setDebugCheckTo(bool b);
  void setDoAnalysisTo(bool b);
  
  void printBxTick() const;
  void entityAssert(bool b) const;
  
  virtual void process() = 0;

  static void doClock();
  static void doTick(unsigned long b=1);
  static void doTick(const Arguments &a);
  static void stopTick();

  virtual void strobeTx();
  virtual void strobeRx();

  virtual void print() const;

  static void printTable();
  static void printAll();

 protected:
  uint32_t idNumber_;
  std::string name_;
  const Clocks::Frequency frequency_;
  EntityClass entityClass_;
  
  // Flags to control what is done
  bool debugPrint_;
  bool debugCheck_;
  bool doAnalysis_;



  unsigned callCount_;


  
 private:
  static bool stopTick_;
  static std::vector<Entity*> entityVector_[Clocks::NumberOfFrequencies];
};

#endif
