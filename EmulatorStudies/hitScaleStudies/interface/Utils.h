#ifndef Emulation_HGCalBufferModel_Utils_h
#define Emulation_HGCalBufferModel_Utils_h
#include <string>

inline bool ends_with(std::string const& value, std::string const& ending) {
  if (ending.size() > value.size()) return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline double countsFraction(unsigned long n, unsigned long d) {
  if(d==0) return 0.0;
  return double(n)/d;
}

#endif
