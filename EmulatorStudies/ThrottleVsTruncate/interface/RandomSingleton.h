#ifndef Emulation_HGCalBufferModel_RandomSingleton_h
#define Emulation_HGCalBufferModel_RandomSingleton_h

#include "TRandom3.h"

class RandomSingleton {
 public:
  static TRandom& random();
  
  static void setSeed(unsigned s);

 private:
  static TRandom3 fRandom;
};

//TRandom3 RandomSingleton::fRandom;

#endif
