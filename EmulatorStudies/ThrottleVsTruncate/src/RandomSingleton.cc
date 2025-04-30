#include <iostream>

#include "RandomSingleton.h"

TRandom3 RandomSingleton::fRandom;

TRandom& RandomSingleton::random() {
  return fRandom;
}

void RandomSingleton::setSeed(unsigned s) {
  fRandom.SetSeed(s);
}
