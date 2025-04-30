#ifndef Emulation_HGCalBufferModel_Clocks_h
#define Emulation_HGCalBufferModel_Clocks_h

#include <vector>
#include <string>

class Clocks {
 public:
  enum Frequency {
    // Fundamental
    //Clock0020,
    
    // 2^n up to n=6
    Clock0040,
    Clock0080,
    Clock0160,
    Clock0320,
    Clock0640,
    Clock1280,
    
    // 3 x 2^n up to n=3
    Clock0060,
    Clock0120,
    Clock0240,
    Clock0480,
    
    // 3^2 x 2^n up to n=1
    Clock0180,
    Clock0360,
    
    // 5 x 2^n up to n=2
    Clock0100,
    Clock0200,
    Clock0400,

    // 7 x 2^n up to n=2
    Clock0140,
    Clock0280,
    Clock0187, // 560/3
    
    NumberOfFrequencies
  };

  static Clocks& clocks();
  
  void enable(Frequency f);

  void initialise();
  
  void update();

  Frequency state() const;
  unsigned long tickNumber(Frequency f) const;
  unsigned long bxNumber() const;
  unsigned long bxCount() const;
  double timeMicrosec() const;
  bool previousClock0040() const;
  
  static const std::string& frequencyName(Clocks::Frequency f);
  
 private:  
  Clocks();

  // Initialisation
  bool enabled_[NumberOfFrequencies];
  std::vector<Frequency> tickVector_;
  bool initialised_;
  
  // State
  unsigned long ticks_;
  Frequency state_;
  unsigned long count_[NumberOfFrequencies];
  bool previousClock0040_[NumberOfFrequencies];
  
  static Clocks clocks_;
  static unsigned frequencyMultiple_[NumberOfFrequencies];
  static std::string frequencyName_[NumberOfFrequencies];
};

#endif
