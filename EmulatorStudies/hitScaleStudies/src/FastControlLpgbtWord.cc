#include <iostream>
#include <iomanip>
#include <string>

#include "FastControlLpgbtWord.h"

FastControlLpgbtWord::FastControlLpgbtWord() {
  word_[0]=0;
  word_[1]=0;
}

FastControlCommands FastControlLpgbtWord::commands() {
  FastControlCommands c;
  
  // Complete cludge; should be rewritten to add real constraints
  // to combinations of commands 
  c.setWordTo(word_[1]<<16|word_[0]);

  return c;
}

FastControlCommands FastControlLpgbtWord::setTo(const FastControlCommands &c) {

  // Return any commands qhich could not be sent
  FastControlCommands dropped;

  // Complete cludge; should be rewritten to add real constraints
  // to combinations of commands (see comments at the end of the file)
  word_[0]=c.word()&0xffff;

  // Take difference to find remaining bits
  dropped.setWordTo((c.word()&0xffff)-word_[0]);
  
  // This should be left unchanged
  word_[1]=c.word()>>16;

  return dropped;
}

void FastControlLpgbtWord::print() const {
  std::cout << "FastControlLpgbtWord::print()  Words = 0x"
	    << std::hex << std::setfill('0')
	    << std::setw(4) << word_[0] << ", 0x"
	    << std::setw(4) << word_[1]
	    << std::hex << std::setfill(' ')
	    << std::endl;
}

/* Latest table from
   https://gitlab.cern.ch/tquast/hgc-daq-codebook/blob/master/codes/codebook_v1.py

commands_to_codes = {
	"kCode": 54,
	"L1A": 75,
	"PreL1A": 204,
	"L1A+PreL1A": 225,
	"OrbitSync": 29,
	"OrbitSync+L1A": 113,
	"OrbitSync+PreL1A": 163,
	"OrbitSync+L1A+PreL1A": 165,
	"NZS+L1A": 15,
	"NZS+L1A+PreL1A": 43,
	"CalPulseInt": 45,
	"CalPulseInt+L1A": 57,
	"CalPulseInt+PreL1A": 85,
	"CalPulseExt": 120,
	"CalPulseExt+L1A": 135,
	"CalPulseExt+PreL1A": 147,
	"OrbitCounterReset+OrbitSync": 149,
	"EventCounterReset": 169,
	"EventBufferClear": 209,
	"LinkResetROCT": 153,
	"LinkResetROCD": 154,
	"LinkResetECONT": 170,
	"LinkResetECOND": 184,
	"ChipSync": 210,
	"CalPulseInt+L1A+PreL1A": 226,
	"CalPulseExt+L1A+PreL1A": 240,
	"Spare-1": 71,
	"Spare-2": 83,
	"Spare-3": 89,
	"Spare-4": 90,
	"Spare-5": 92,
	"Spare-6": 139
}

*/
