#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

#include "Backtrace.h"
#include "GlobalConfiguration.h"
#include "EcondAsicHitCounts.h"


EcondAsicHitCounts::EcondAsicHitCounts() {
  word_=0;
}

uint32_t EcondAsicHitCounts::word() const {
  return word_;
}

void EcondAsicHitCounts::setWordTo(uint32_t w) {
  word_=w;
}

uint16_t EcondAsicHitCounts::n10() const {
  return (word_)&0x3ff;
}

void EcondAsicHitCounts::setN10To(uint16_t n) {
  backtraceAssert(n<1024);
  word_=(word_&0xfffffc00)|n;
}

uint16_t EcondAsicHitCounts::n20() const {
  return (word_>>10)&0x3ff;
}

void EcondAsicHitCounts::setN20To(uint16_t n) {
  backtraceAssert(n<1024);
  word_=(word_&0xfff003ff)|(n<<10);
}

uint16_t EcondAsicHitCounts::n30() const {
  return (word_>>20)&0x3ff;
}

void EcondAsicHitCounts::setN30To(uint16_t n) {
  backtraceAssert(n<1024);
  word_=(word_&0xc00fffff)|(n<<20);
}

uint16_t EcondAsicHitCounts::packetWords(const EcondAsicDefinition &e) const {
  return daqRawDataPacketWords(e,n10(),n20(),n30());
}

bool EcondAsicHitCounts::operator==(const EcondAsicHitCounts &h) const {
  return word_==h.word_;
}

bool EcondAsicHitCounts::valid() const {
  return (word_&0xc0000000)==0;
}

unsigned EcondAsicHitCounts::daqRawDataPacketWords(const EcondAsicDefinition &e, unsigned n10, unsigned n20, unsigned n30) {

  // Return value is in units of 16-bit words

  if(GlobalConfiguration::configuration().daqPacketFormat()==0) {
    return daqRawDataPacketWordsSep19(e,n10+n20,n30);
  } else if(GlobalConfiguration::configuration().daqPacketFormat()==1) {
    return daqRawDataPacketWordsNov19(e,n10+n20,n30);
  } else if(GlobalConfiguration::configuration().daqPacketFormat()==2) {
    return daqRawDataPacketWordsJun20(e,n10+n20,n30);
  }
  return daqRawDataPacketWordsDec20(e,n10,n20,n30);
}

unsigned EcondAsicHitCounts::daqRawDataPacketWordsSep19(const EcondAsicDefinition &e, unsigned n20, unsigned n30) {
  bool hdm(e.type()!=1);

  unsigned nLdm[193]={
    192,
    256,
    288,
    320,
    352,
    368,
    416,
    432,
    496,
    512,
    544,
    576,
    608,
    640,
    672,
    688,
    752,
    784,
    816,
    848,
    864,
    912,
    928,
    992,
    1008,
    1024,
    1056,
    1072,
    1104,
    1120,
    1136,
    1184,
    1200,
    1232,
    1248,
    1264,
    1296,
    1312,
    1360,
    1376,
    1392,
    1424,
    1440,
    1472,
    1488,
    1504,
    1552,
    1568,
    1600,
    1616,
    1632,
    1664,
    1680,
    1728,
    1744,
    1760,
    1792,
    1808,
    1840,
    1856,
    1872,
    1920,
    1936,
    1968,
    1984,
    2000,
    2032,
    2048,
    2096,
    2112,
    2128,
    2160,
    2176,
    2208,
    2224,
    2240,
    2288,
    2304,
    2336,
    2352,
    2368,
    2400,
    2416,
    2464,
    2480,
    2496,
    2528,
    2544,
    2576,
    2592,
    2608,
    2656,
    2672,
    2704,
    2720,
    2736,
    2768,
    2784,
    2832,
    2848,
    2864,
    2896,
    2912,
    2944,
    2960,
    2976,
    3024,
    3040,
    3072,
    3088,
    3104,
    3136,
    3152,
    3200,
    3216,
    3232,
    3264,
    3280,
    3312,
    3328,
    3344,
    3392,
    3408,
    3440,
    3456,
    3472,
    3504,
    3520,
    3568,
    3584,
    3600,
    3632,
    3648,
    3680,
    3696,
    3712,
    3760,
    3776,
    3808,
    3824,
    3840,
    3872,
    3888,
    3936,
    3952,
    3968,
    4000,
    4016,
    4048,
    4064,
    4080,
    4128,
    4144,
    4176,
    4192,
    4208,
    4240,
    4256,
    4304,
    4320,
    4336,
    4368,
    4384,
    4416,
    4432,
    4448,
    4496,
    4512,
    4544,
    4560,
    4576,
    4608,
    4624,
    4672,
    4688,
    4704,
    4736,
    4752,
    4784,
    4800,
    4816,
    4864,
    4880,
    4912,
    4928,
    4944,
    4976,
    4992,
    5040,
    5056,
    5072,
    5104,
    5120
  };

  unsigned nHdm[435]={
    336,
    400,
    432,
    464,
    496,
    512,
    560,
    576,
    640,
    656,
    688,
    720,
    752,
    784,
    816,
    832,
    896,
    928,
    960,
    992,
    1008,
    1056,
    1072,
    1136,
    1152,
    1184,
    1216,
    1248,
    1280,
    1312,
    1328,
    1392,
    1424,
    1456,
    1488,
    1504,
    1552,
    1568,
    1632,
    1648,
    1680,
    1712,
    1744,
    1776,
    1808,
    1824,
    1888,
    1920,
    1952,
    1984,
    2000,
    2048,
    2064,
    2128,
    2144,
    2160,
    2192,
    2208,
    2240,
    2256,
    2272,
    2320,
    2336,
    2368,
    2384,
    2400,
    2432,
    2448,
    2496,
    2512,
    2528,
    2560,
    2576,
    2608,
    2624,
    2640,
    2688,
    2704,
    2736,
    2752,
    2768,
    2800,
    2816,
    2864,
    2880,
    2896,
    2928,
    2944,
    2976,
    2992,
    3008,
    3056,
    3072,
    3104,
    3120,
    3136,
    3168,
    3184,
    3232,
    3248,
    3264,
    3296,
    3312,
    3344,
    3360,
    3376,
    3424,
    3440,
    3472,
    3488,
    3504,
    3536,
    3552,
    3600,
    3616,
    3632,
    3664,
    3680,
    3712,
    3728,
    3744,
    3792,
    3808,
    3840,
    3856,
    3872,
    3904,
    3920,
    3968,
    3984,
    4000,
    4032,
    4048,
    4080,
    4096,
    4112,
    4160,
    4176,
    4208,
    4224,
    4240,
    4272,
    4288,
    4336,
    4352,
    4368,
    4400,
    4416,
    4448,
    4464,
    4480,
    4528,
    4544,
    4576,
    4592,
    4608,
    4640,
    4656,
    4704,
    4720,
    4736,
    4768,
    4784,
    4816,
    4832,
    4848,
    4896,
    4912,
    4944,
    4960,
    4976,
    5008,
    5024,
    5072,
    5088,
    5104,
    5136,
    5152,
    5184,
    5200,
    5216,
    5264,
    5280,
    5312,
    5328,
    5344,
    5376,
    5392,
    5440,
    5456,
    5472,
    5504,
    5520,
    5552,
    5568,
    5584,
    5632,
    5648,
    5680,
    5696,
    5712,
    5744,
    5760,
    5808,
    5824,
    5840,
    5872,
    5888,
    5920,
    5936,
    5952,
    6000,
    6016,
    6048,
    6064,
    6080,
    6112,
    6128,
    6176,
    6192,
    6208,
    6240,
    6256,
    6288,
    6304,
    6320,
    6368,
    6384,
    6416,
    6432,
    6448,
    6480,
    6496,
    6544,
    6560,
    6576,
    6608,
    6624,
    6656,
    6672,
    6688,
    6736,
    6752,
    6784,
    6800,
    6816,
    6848,
    6864,
    6912,
    6928,
    6944,
    6976,
    6992,
    7024,
    7040,
    7056,
    7104,
    7120,
    7152,
    7168,
    7184,
    7216,
    7232,
    7280,
    7296,
    7312,
    7344,
    7360,
    7392,
    7408,
    7424,
    7472,
    7488,
    7520,
    7536,
    7552,
    7584,
    7600,
    7648,
    7664,
    7680,
    7712,
    7728,
    7760,
    7776,
    7792,
    7840,
    7856,
    7888,
    7904,
    7920,
    7952,
    7968,
    8016,
    8032,
    8048,
    8080,
    8096,
    8128,
    8144,
    8160,
    8208,
    8224,
    8256,
    8272,
    8288,
    8320,
    8336,
    8384,
    8400,
    8416,
    8448,
    8464,
    8496,
    8512,
    8528,
    8576,
    8592,
    8624,
    8640,
    8656,
    8688,
    8704,
    8752,
    8768,
    8784,
    8816,
    8832,
    8864,
    8880,
    8896,
    8944,
    8960,
    8992,
    9008,
    9024,
    9056,
    9072,
    9120,
    9136,
    9152,
    9184,
    9200,
    9232,
    9248,
    9264,
    9312,
    9328,
    9360,
    9376,
    9392,
    9424,
    9440,
    9488,
    9504,
    9520,
    9552,
    9568,
    9600,
    9616,
    9632,
    9680,
    9696,
    9728,
    9744,
    9760,
    9792,
    9808,
    9856,
    9872,
    9888,
    9920,
    9936,
    9968,
    9984,
    10000,
    10048,
    10064,
    10096,
    10112,
    10128,
    10160,
    10176,
    10224,
    10240,
    10256,
    10288,
    10304,
    10336,
    10352,
    10368,
    10416,
    10432,
    10464,
    10480,
    10496,
    10528,
    10544,
    10592,
    10608,
    10624,
    10656,
    10672,
    10704,
    10720,
    10736,
    10784,
    10800,
    10832,
    10848,
    10864,
    10896,
    10912,
    10960,
    10976,
    10992,
    11024,
    11040,
    11072,
    11088,
    11104,
    11152,
    11168,
    11200,
    11216,
    11232,
    11264,
    11280,
    11328,
    11344,
    11360,
    11392,
    11408,
    11440,
    11456
  };

  unsigned nArray;
  if(hdm) nArray=(nHdm[n20+n30]+15)/16;
  else nArray=(nLdm[n20+n30]+15)/16;

  unsigned nWords;

  if(hdm) { // HDM
    backtraceAssert((n20+n30)<=444); // 432 standard + 12 calibration cells

    if((n20+n30)<48) { // Low occupancy

      // Header + Number + CRC + Addresses + Channels + Common mode
      nWords=(32 + 6 + 10 + 9*(n20+n30) + 22*n20+32*n30 + 240 + 15)/16;

    } else { // High occupancy

      // Header + Bitmap + CRC + Channels + Common mode
      nWords=(32 + 444 + 14 + 22*n20+32*n30 + 240 + 15)/16;
    }

  } else { // LDM
    backtraceAssert((n20+n30)<=198); // 192 standard + 6 calibration cells, and 32 standard + 1 calibration cells on c0!

    if((n20+n30)<24) { // Low occupancy

      // Header + Number + CRC + Addresses + Channels + Common mode
      nWords=(32 + 5 + 10 + 8*(n20+n30) + 22*n20+32*n30 + 120 + 15)/16;

    } else { // High occupancy

      // Header + Bitmap + CRC + Channels + Common mode
      nWords=(32 + 198 + 14 + 22*n20+32*n30 + 120 + 15)/16;
    }
  }

  // Turn off printout (but allow to compile)
  if(nArray==0) std::cout << "Exact, array = " << nWords << ", " << nArray << std::endl;

  // Make sure header will fit
  if(nWords<6) nWords=6;
  return nWords;
}

unsigned EcondAsicHitCounts::daqRawDataPacketWordsNov19(const EcondAsicDefinition &e, unsigned n20, unsigned n30) {
  bool hdm(e.type()!=1);

  const bool includeLRshapes(false); // Allow CM and bit map for F+cO modules
  const unsigned wordSize(15); // Reserve valid bit (15) or not (16)
  
  unsigned nWords;

  if(hdm) { // HDM
    backtraceAssert((n20+n30)<=444); // 432 standard + 12 calibration cells

    if((n20+n30)<49) { // Low occupancy

      // Header + Contents + CM + Addresses + Channels + CRC
      nWords=7 + (6+3*(n20+n30)+wordSize-1)/wordSize + (24*10+wordSize-1)/wordSize + (9*(n20+n30)+wordSize-1)/wordSize + (20*n20+30*n30+wordSize-1)/wordSize + 1;

    } else { // High occupancy

      // Header + Bitmap + Contents + CM + Channels + CRC
      nWords=7 + (444+wordSize-1)/wordSize + (3*(n20+n30)+wordSize-1)/wordSize + (24*10+wordSize-1)/wordSize + (20*n20+30*n30+wordSize-1)/wordSize + 1;
    }

  } else { // LDM
    backtraceAssert((n20+n30)<=198+(includeLRshapes?33:0)); // 192 standard + 6 calibration cells, and 32 standard + 1 calibration cells on c0!

    if((n20+n30)<25) { // Low occupancy

      // Header + Contents + CM + Addresses + Channels + CRC
      nWords=6 + (5+3*(n20+n30)+wordSize-1)/wordSize + ((includeLRshapes?14*10:12*10)+wordSize-1)/wordSize + (8*(n20+n30)+wordSize-1)/wordSize + (20*n20+30*n30+wordSize-1)/wordSize + 1;

    } else { // High occupancy

      // Header + Bitmap + Contents + CM + Channels + CRC
      nWords=6 + (198+(includeLRshapes?33:0)+wordSize-1)/wordSize + (3*(n20+n30)+wordSize-1)/wordSize + ((includeLRshapes?14*10:12*10)+wordSize-1)/wordSize + (20*n20+30*n30+wordSize-1)/wordSize + 1;
    }
  }
  
  return nWords;
}

unsigned EcondAsicHitCounts::daqRawDataPacketWordsJun20(const EcondAsicDefinition &e, unsigned n20, unsigned n30) {
  //bool hdm(e.type()!=1);

  unsigned nWords(0);

  // Check limits
  //backtraceAssert((n20+n30)<=444); // 432 standard + 12 calibration cells
  
  std::vector<unsigned> subPacketVector(e.numberOfHalfHgcrocs());

  unsigned nEach20(n20/subPacketVector.size());
  unsigned nHigh20(n20%subPacketVector.size());
  unsigned nEach30(n30/subPacketVector.size());
  unsigned nHigh30(n30%subPacketVector.size());

  unsigned nCheck20(0);
  unsigned nCheck30(0);

  for(unsigned i(0);i<subPacketVector.size();i++) {
    unsigned nSp20(nEach20+(i<nHigh20?1:0));
    unsigned nSp30(nEach30+(i<nHigh30?1:0));
    nCheck20+=nSp20;
    nCheck30+=nSp30;

    if((nSp20+nSp30)<=5) { // Low occupancy

      if(true && (nSp20+nSp30)==0) {

	// Header only 
	subPacketVector[i]=1;
	
      } else {

	// Header+Addresses + (2+N) channels
	subPacketVector[i]=2+(22*nSp20+32*nSp30+31)/32;
      }
      
    } else { // High occupancy

      // Header+Bitmap + (2+N) channels
      subPacketVector[i]=2+(22*nSp20+32*nSp30+31)/32;
    }
  
    nWords+=2*subPacketVector[i];
  }
  
  backtraceAssert(nCheck20==n20);
  backtraceAssert(nCheck30==n30);
  
  // Header + CRC
  nWords+=2*3;
  
  return nWords;
}

unsigned EcondAsicHitCounts::daqRawDataPacketWordsDec20(const EcondAsicDefinition &e, unsigned n10, unsigned n20, unsigned n30) {
  //bool hdm(e.type()!=1);

  unsigned nWords(0);// To compile

  // Check limits
  //backtraceAssert((n20+n30)<=444); // 432 standard + 12 calibration cells
  
  std::vector<unsigned> subPacketVector(e.numberOfHalfHgcrocs());

  unsigned nEach10(n10/subPacketVector.size());
  unsigned nHigh10(n10%subPacketVector.size());
  unsigned nEach20(n20/subPacketVector.size());
  unsigned nHigh20(n20%subPacketVector.size());
  unsigned nEach30(n30/subPacketVector.size());
  unsigned nHigh30(n30%subPacketVector.size());

  unsigned nCheck10(0);
  unsigned nCheck20(0);
  unsigned nCheck30(0);

  for(unsigned i(0);i<subPacketVector.size();i++) {
    unsigned nSp10(nEach10+(i<nHigh10?1:0));
    unsigned nSp20(nEach20+(i<nHigh20?1:0));
    unsigned nSp30(nEach30+(i<nHigh30?1:0));

    nCheck10+=nSp10;
    nCheck20+=nSp20;
    nCheck30+=nSp30;

    if((nSp10+nSp20+nSp30)==0) { // Empty

      // Header only 
      subPacketVector[i]=1;
      //subPacketVector[i]=0; // No packet at all
      
      //} else if((nSp10+nSp20+nSp30)==1) { // 1 hit

      //subPacketVector[i]=2;
      
    } else {
      
      // Header+channels
      subPacketVector[i]=2+(16*nSp10+24*nSp20+32*nSp30+31)/32; // Real
      //subPacketVector[i]=2+(12*nSp10+22*nSp20+34*nSp30+31)/32; // Squeezed
    }
  
    nWords+=2*subPacketVector[i];
  }
  
  backtraceAssert(nCheck10==n10);
  backtraceAssert(nCheck20==n20);
  backtraceAssert(nCheck30==n30);
  
  // Overall header + CRC
  nWords+=2*3;
  
  return nWords;
}

void EcondAsicHitCounts::print(const std::string &s) const {
  std::cout << s << "EcondAsicHitCounts::print() ";
  std::cout << s << " Number word =" << std::hex << std::setfill('0')
	    << " 0x" << std::setw(8) << word_
	    << std::dec << std::setfill(' ') << std::endl;

  std::cout << s << " Number of 10, 20 and 30 bit hits = " << n10()
	    << ", " << n20() << ", " << n30() << std::endl;
  
  std::cout << s << " Valid = " << (valid()?"true":"false") << std::endl;
}
