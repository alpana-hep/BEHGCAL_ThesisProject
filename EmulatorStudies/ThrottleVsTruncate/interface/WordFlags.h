#ifndef Emulation_HGCalBufferModel_WordFlags_h
#define Emulation_HGCalBufferModel_WordFlags_h

#include <cstdint>

class WordFlags {
public:
  WordFlags();
  WordFlags(uint8_t b);
  
  void clearBits();
  void setBitsTo(uint8_t b);

  void setStartBit();
  void setEndBit();
  void setPaddingBit();
  void setTruncationBit();
  void setErrorBit();
  void setInvalidBit();

  void setStartBitTo(bool b);
  void setEndBitTo(bool b);
  void setPaddingBitTo(bool b);
  void setPaddingCountTo(unsigned c);
  void setTruncationBitTo(bool b);
  void setErrorBitTo(bool b);
  void setInvalidBitTo(bool b);

  ////////////////////////////////////////////////////////

  uint8_t bits() const;  
  bool noBits() const;  
  bool startBit() const;  
  bool endBit() const;  
  bool paddingBit() const;  
  unsigned paddingCount() const;  
  bool truncationBit() const;  
  bool errorBit() const;
  bool invalidBit() const;

  void print() const;
  
private:
  uint8_t fBits;
};

#endif
