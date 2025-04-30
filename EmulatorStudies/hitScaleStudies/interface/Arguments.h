#ifndef Emulation_HGCalBufferModel_Arguments_h
#define Emulation_HGCalBufferModel_Arguments_h

/*

Handles program arguments.
General parameters:
 -b sets number of BX to process
 -? sets number of orbits to process (-b is ignored)
 -r sets random seed, a value of 0 sets seed to current UTC in seconds
 -x sets 25 Gbit/s Slink

Fast control configuration parameters
 -t sets trigger method (see FastControlDriver)
 -k sets trigger rate in kHz
 -z sets probability/BX for local NZS (x 10^6)
 -q sets calibration period (units of orbits)

Mapping parameters
 -m sets FE mapping file
 -n sets BE mapping file

HGCROC ASICs input data file
 -f sets filename; a generic string

ECON-D configuration parameters
 -s sets scale factor for numbers of hits (x 10^3)
 -e sets throttle level for buffer

Generic parameters
 -l sets a label; a generic string with any use
 -u sets an unsigned; a generic unsigned with any use

Switches for all entities
 -a sets analysis bits; see bit list in Entity.cc
 -c sets packet check bits; see bit list in Entity.cc
 -d sets debug bits; see bit list in Entity.cc

 -o sets output filename XXXX OBSOLETE!
*/

#include <unistd.h>

#include <cstdint>
#include <string>
#include "json.hpp"

class Arguments {
 public:
  Arguments();
  Arguments(int argc, char *argv[]);

  void setDefaults();
  void processArguments(int argc, char *argv[]);

  bool numberOfBxArg() const;
  unsigned long numberOfBx() const;
  void setNumberOfBxTo(unsigned long n);

  bool numberOfOrbitsArg() const;
  unsigned long numberOfOrbits() const;

  bool triggerMethodArg() const;
  unsigned triggerMethod() const;

  bool calOrbitPeriodArg() const;
  unsigned calOrbitPeriod() const;

  bool nzsProbabilityArg() const;
  double nzsProbability() const;

  bool econdMediumBufferLevelArg() const;
  unsigned econdMediumBufferLevel() const;

  bool econdHighBufferLevelArg() const;
  unsigned econdHighBufferLevel() const;

  bool econdLpgbtDelayLengthArg() const;
  unsigned econdLpgbtDelayLength() const;

  bool econdHitScalingArg() const;
  unsigned econdHitScaling() const;
 unsigned econdHitAdditiveArg() const; //added by Alpana
 unsigned econdHitlayerArg() const;
  bool triggerRateArg() const;
  unsigned triggerRate() const;
  unsigned econdScintHitScalingArg() const;
  bool hitModifierFileArg() const;
  std::string hitModifierFile() const;
  void sethitModifierFileTo(const std::string &f);

  bool mappingFileArg() const;
  std::string mappingFile() const;
  void setMappingFileTo(const std::string &f);
  
  bool feMappingFileArg() const;
  std::string feMappingFile() const;
  void setFeMappingFileTo(const std::string &f);
  
  bool beMappingFileArg() const;
  std::string beMappingFile() const;
  void setBeMappingFileTo(const std::string &f);
  
  bool filenameArg() const;
  std::string filename() const;
  void setFilenameTo(const std::string &f);
  
  bool slink25GbitPerSecArg() const;
  bool slink25GbitPerSec() const;
  void setSlink25GbitPerSecTo(bool b);

  bool outputnameArg() const;
  std::string outputname() const;
  void setOutputnameTo(const std::string &f);
  
  bool labelArg() const;
  std::string label() const;
  void setLabelTo(const std::string &l);

  bool numberArg() const;
  unsigned number() const;
  void setNumberTo(unsigned n);

  bool debugPrintBitsArg() const;
  uint16_t debugPrintBits() const;

  bool debugCheckBitsArg() const;
  uint16_t debugCheckBits() const;

  bool doAnalysisBitsArg() const;
  uint16_t doAnalysisBits() const;

  bool debugPrintFastControlDriver() const;
  bool debugPrintFastControlStream() const;
  bool debugPrintHgcrocAsics() const;
  bool debugPrintEcondAsic() const;
  bool debugPrintEcontAsic() const;
  bool debugPrintEcondBeBuilder() const;
  bool debugPrintLpgbtBuilder() const;
  bool debugPrintSlinkBuilder() const;
  bool debugPrintDthChecker() const;
  bool debugPrintTestEntity() const;

  bool randomSeedArg() const;
  unsigned randomSeed() const;

  std::string summaryString() const;
  void print() const;

  std::string encodeArgs() const;

  std::string fullOutputFilename() const;

  nlohmann::json const& config() const;

 private:
  void processJSON(std::string const& filename);
  void processSingleArg(std::string const& arg);

  nlohmann::json js_;
};

#endif
