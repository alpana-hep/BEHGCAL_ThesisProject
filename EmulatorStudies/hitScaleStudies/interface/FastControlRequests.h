#ifndef Emulation_HGCalBufferModel_FastControlRequests_h
#define Emulation_HGCalBufferModel_FastControlRequests_h

#include <cstdint>
#include <string>

class FastControlRequests {
 public:
  enum Request {
    HgcrocBeThrottle,
    EcondAsicThrottle,
    EcondBeThrottle,
    LinkResetEcond,
    EventBufferReset,
    Range
  };
  
  FastControlRequests();

  // Directly handle all requests
  uint16_t word() const;
  void setWordTo(uint16_t c);

  // Get individual requests
  bool hgcrocBeThrottle() const;
  bool econdAsicThrottle() const;
  bool econdBeThrottle() const;
  bool linkResetEcond() const;
  bool eventBufferReset() const;

  bool throttle() const;
  
  bool idle() const;

  // Set individual requests  
  void setHgcrocBeThrottleTo(bool b);  
  void setEcondAsicThrottleTo(bool b);  
  void setEcondBeThrottleTo(bool b);  
  void setLinkResetEcondTo(bool b);
  void setEventBufferResetTo(bool b);
  
  void setIdle();

  // Merge requests
  void merge(const FastControlRequests &r);
  
  void print(const std::string &s="") const;
    
  static const std::string& requestName(Request r);

 private:  
  uint16_t word_;

  static std::string requestName_[Range];
};

#endif
