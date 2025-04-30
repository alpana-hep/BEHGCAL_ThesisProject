#include <iostream>
#include <iomanip>
#include <cassert>
#include <queue>

#include "EcondBeHeader.h"
#include "EcondBeTrailer.h"
#include "EcondAsicHeader.h"
#include "EcondAsicHitCounts.h"
//#include "EcontAsicHeader.h"
#include "EcondAsic.h"
//#include "EcontAsic.h"
#include "SlinkDefinition.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"
#include "PacketCreator.h"

PacketCreator::PacketCreator() {
}
/*
bool PacketCreator::checkEcondAsicPacket16(const std::vector<uint16_t> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::checkEcondAsicPacket16()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);
  
  EcondAsicHeader h;
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << "PacketCreator::checkEcondAsicPacket16() word " << i
		<< " = " << v[i] << std::endl;
    }
    
    if(i<6) {
      h.setWord16To(i,v[i]);
      
      if(i==5) {
	if(p) h.print();
	if(h.packetLength()!=v.size()) {
	  std::cout << "PacketCreator::checkEcondAsicPacket16() header packet length "
		    << " = " << h.packetLength() << " != " << v.size()
		    << std::endl;
	  h.print();
	  reply=false;
	}
      }
      
    } else {
      if(v[i]!=2*i+1) {
	std::cout << "PacketCreator::checkEcondAsicPacket16() word " << i
		  << " = " << v[i] << " != " << 2*i+1 << std::endl;
	reply=false;
      }
    }
  }
  
  return reply;
}
*/
void PacketCreator::createEcondAsicPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createEcondAsicPacket32()  initial size = "
	      << v.size() << std::endl;
    d.print(" ");
    c.print(" ");
    std::cout << " Numbers of hits = " << n10 << ", " << n20 << ", " << n30 << std::endl << std::endl;
  }

  // Find length; three cases
  unsigned payloadLength(0);
  if(n10==0xffff && n20==0 && n30==0) {
    payloadLength=0;
  } else if(n20==0xffff && n30==0xffff) {
    payloadLength=n10;
    
  } else {
    payloadLength=EcondAsicHitCounts::daqRawDataPacketWords(d,n10,n20,n30)/2;
    payloadLength-=2;
  }

  // Header is always needed
  EcondAsicHeader h;
  // std::cout<<"before checking payload length "<<payloadLength<<std::endl;
  // std::cout << " Numbers of hits = " << n10 << ", " << n20 << ", " << n30 << std::endl << std::endl;
  h.setPayloadLengthTo(payloadLength);
  if(h.truncated()) h.setQualityFlagsTo(1);
  h.setCountersTo(c);
  h.setCrc();
  if(p) h.print(" ");
  
  v.push_back(h.word(0));
  v.push_back(h.word(1));
  
  if(n10==0xffff && n20==0 && n30==0) {

  } else {
    EcondAsicHitCounts eahc;
    eahc.setN10To(n10);
    if(n20==0xffff && n30==0xffff) {
      eahc.setN20To(1023);
      eahc.setN30To(1023);
    } else {
      eahc.setN20To(n20);
      eahc.setN30To(n30);
    }
    
    if(p) {
      eahc.print(" ");
    }
    
    v.push_back(eahc.word());
    uint32_t crc(v.back());

    uint32_t id(d.halfWordId());
    id=id<<16;
    
    for(unsigned i(1);i+1<h.payloadLength();i++) {
      uint32_t w(((c.bc()&0xf)<<12)+i+2);
      w|=id;
      
      v.push_back(w);
      crc+=v.back();
      
      if(p) std::cout << " Word " << std::setw(4) << i << " = 0x"
		      << std::hex << std::setfill('0')
		      << std::setw(8) << v.back()
		      << std::dec << std::setfill(' ') << std::endl;
    }
    
    crc=(~crc)+1;
    v.push_back(crc);
    
    if(p) std::cout << " Word " << std::setw(4) << v.size()-1 << " = 0x"
		    << std::hex << std::setfill('0')
		    << std::setw(8) << v.back()
		    << std::dec << std::setfill(' ') << std::endl;
  }
  
  if(p) {
    std::cout << "PacketCreator::createEcondAsicPacket32()  final size = "
	      << v.size() << std::endl;
  }
}

void PacketCreator::createEcondAsicPacket72(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createEcondAsicPacket72()  initial size = "
	      << v.size() << std::endl;
  }

  std::vector<uint32_t> v32;
  createEcondAsicPacket32(d,c,n10,n20,n30,v32,p);

  bool pad((v32.size()%2)!=0);
  if(pad) v32.push_back(0);
  
  RamWord rw;
  for(unsigned i(0);i<v32.size()/2;i++) {
    rw.clearBits();
    if(i==0) rw.setStartBitTo(true);
    if(i==(v32.size()/2)-1) {
      rw.setEndBitTo(true);
      rw.setPaddingBitTo(pad);
    }
    
    for(unsigned j(0);j<2;j++) {
      rw.setWordTo(j,v32[2*i+j]);
    }
    v.push_back(rw);
  }
  
  if(p) {
    std::cout << "PacketCreator::createEcondAsicPacket72()  final size = "
	      << v.size() << std::endl;
  }
}

void PacketCreator::createEcondBePacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createEcondBePacket32()  initial size = "
	      << v.size() << std::endl;
  }

  unsigned l(EcondAsicHitCounts::daqRawDataPacketWords(d,n10,n20,n30));

  EcondBeHeader ebh;
  ebh.setPacketLengthTo(l);
  ebh.setDataTypeTo(EcondHeader::Normal);
  ebh.setBufferStatusTo(EcondHeader::Low);
  ebh.setCountersTo(c);
  ebh.setHdmTo(d.type()!=1);
  ebh.setErrorBitsTo(0);
  if(p) ebh.print();

  for(unsigned i(0);i<sizeof(EcondBeHeader)/4;i++) {
    v.push_back(ebh.word(i));
  }

  unsigned preLength(v.size());
  createEcondAsicPacket32(d,c,n10,n20,n30,v,p);
    
  EcondBeTrailer ebt;
  ebt.setErrorBitsTo(0);
  ebt.setPacketLengthTo(v.size()-preLength);
  if(p) ebt.print();

  v.push_back(ebt.word());
  
  if(p) {
    std::cout << "PacketCreator::createEcondBePacket32()  final size = "
	      << v.size() << std::endl;
  }
}

void PacketCreator::createEcondBePacket72(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n10, unsigned n20, unsigned n30, std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createEcondBePacket72()  initial size = "
	      << v.size() << std::endl;
  }

  std::vector<uint32_t> v32;
  createEcondBePacket32(d,c,n10,n20,n30,v32,p);

  bool pad((v32.size()%2)!=0);
  if(pad) v32.push_back(0);
  
  RamWord rw;
  for(unsigned i(0);i<v32.size()/2;i++) {
    rw.clearBits();
    if(i==0) rw.setStartBitTo(true);
    if(i==(v32.size()/2)-1) {
      rw.setEndBitTo(true);
      rw.setPaddingBitTo(pad);
    }
    
    for(unsigned j(0);j<2;j++) {
      rw.setWordTo(j,v32[2*i+j]);
    }
    v.push_back(rw);
  }
  
  if(p) {
    std::cout << "PacketCreator::createEcondBePacket72()  final size = "
	      << v.size() << std::endl;
  }
}

void PacketCreator::createEcondAsicPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n16, std::vector<uint32_t> &, bool) {
  d.print();
  c.print();
  n16*=n16; // TO COMPILE!
}

void PacketCreator::createEcondBeBuilderPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, unsigned n16, std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createEcondBeBuilderPacket32()  initial size = "
	      << v.size() << std::endl;
  }

  EcondBeHeader ebh;
  ebh.setCountersTo(c);
  if(p) ebh.print();
  v.push_back(ebh.word(0));
  v.push_back(ebh.word(1));
  
  unsigned sInitial(v.size());
  createEcondAsicPacket32(d,c,n16,v,p);

  EcondBeTrailer ebt;
  ebt.setPacketLengthTo(v.size()-sInitial);
  if(p) ebt.print();
  v.push_back(ebt.word());

  if(p) {
    std::cout << "PacketCreator::createEcondBeBuilderPacket32()  final size = "
	      << v.size() << std::endl;
  }
}

void PacketCreator::createLpgbtPacket(const LpgbtDefinition &d, const FastControlCounters &c, const std::vector<unsigned> &v10, const std::vector<unsigned> &v20, const std::vector<unsigned> &v30, std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createLpgbtPacket()  initial size = "
	      << v.size() << std::endl;
  }
  unsigned startSize(v.size());
  
  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);
  assert(vEad.size()==v20.size());
  assert(vEad.size()==v30.size());
  
  for(unsigned i(0);i<vEad.size();i++) {
    if(p) std::cout << "Number of n20, n30 hits = "
		    << v20[i] << ", " << v30[i] << std::endl;
    createEcondBePacket72(vEad[i],c,v10[i],v20[i],v30[i],v,p);
  }

  if(p) v[startSize].print();
  for(unsigned i(startSize+1);i<v.size()-1;i++) {
    v[i].setStartBitTo(false);
    v[i].setEndBitTo(false);
    if(p) v[i].print();
  }
  if(p) v.back().print();
  
  if(p) {
    std::cout << "PacketCreator::createLpgbtPacket()  final size = "
	      << v.size() << std::endl;
  }
}

void PacketCreator::createSlinkPacket(const SlinkDefinition &d, const FastControlCounters &c, const std::vector<unsigned> &v10, const std::vector<unsigned> &v20, const std::vector<unsigned> &v30, std::vector<SlinkWord> &v, bool p) {
  if(p) {
    std::cout << "PacketCreator::createSlinkPacket()  initial size = "
	      << v.size() << std::endl;
  }

  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);
  assert(vEad.size()==v20.size());
  assert(vEad.size()==v30.size());

  SlinkBoe boe;
  boe.setSourceIdTo(d.sourceId());
  boe.setCountersTo(c);
  boe.setEventTypeTo(1); // Physics
  if(p) boe.print();
  v.push_back(boe);

  uint16_t crc(boe.crcIncrement());
  
  std::vector<uint32_t> v32;
  for(unsigned i(0);i<vEad.size();i++) {
    if(p) std::cout << "Number of n20, n30 hits = "
		    << v20[i] << ", " << v30[i] << std::endl;
    createEcondBePacket32(vEad[i],c,v10[i],v20[i],v30[i],v32,p);
  }

  while((v32.size()%4)!=0) v32.push_back(0);
  assert((v32.size()%4)==0);
  
  SlinkWord sw;
  for(unsigned i(0);i<v32.size()/4;i++) {
    for(unsigned j(0);j<4;j++) {
      sw.setWordTo(j,v32[4*i+j]);
    }
    v.push_back(sw);
    crc+=sw.crcIncrement();
  }
  
  SlinkEoe eoe;
  eoe.setFedUserIdTo(d.sourceId());
  eoe.setStatusTo(0);
  eoe.setCrcTo(0);
  eoe.setEventLengthTo(v.size()+1);
  crc+=eoe.crcIncrement();
  eoe.setCrcTo(crc);
  
  if(p) eoe.print();
  v.push_back(eoe);
  
  if(p) {
    std::cout << "PacketCreator::createSlinkPacket()  final size = "
	      << v.size() << std::endl;
  }
}
/*
void PacketCreator::createEcontAsicPacket(const EcontAsicDefinition &, unsigned bx, const std::vector<uint32_t> &vTc, std::vector<uint16_t> &v, bool p) {

  if(p) std::cout << "PacketCreator::createEcontAsicPacket() entering, BX = "
		  << bx << std::endl;

  if(vTc.size()!=48) {
    if(p) std::cout << " ERROR: number of TCs = " << vTc.size()
		    << " != 48" << std::endl;
    return;
  }

  unsigned nTc(0);
  unsigned mSum(0);
  for(unsigned i(0);i<vTc.size();i++) {
    if(p) std::cout << "  TC " << std::setw(2) << i << " = " << vTc[i];
    if(vTc[i]>5) {
      nTc++;
      if(p) std::cout << ", above threshold" << std::endl;
    } else {
      if(p) std::cout << std::endl;
    }
    mSum+=vTc[i];
  }
  if(p) std::cout << " Number of TCs above threshold = " << nTc << std::endl;
  
  uint16_t w;

  EcontAsicHeader eah;
  if(bx==1) eah.setHeaderCounterTo(31);
  else eah.setHeaderCounterTo(bx%31);
  if(nTc==0) eah.setDataTypeTo(0);
  else if(nTc<8) eah.setDataTypeTo(1);
  else eah.setDataTypeTo(2);
  eah.setModuleSumTo(20.0*log2(mSum)+0.5);
  if(p) eah.print(" ");

  v.push_back(eah.word());

  if(eah.dataType()==0) {
    if(p) std::cout << " No TCs" << std::endl;
  
  } else if(eah.dataType()==1) {
    if(p) std::cout << " Low occupancy" << std::endl;

    bool first(true);
    int nBits(19);
    uint32_t w32(0);

    for(unsigned i(0);i<vTc.size();i++) {
      if(vTc[i]>5) {
	if(first) {
	  first=false;
	  w=i<<7;
	  w|=vTc[i];
	  w|=nTc<<13;
	  v.push_back(w);
	  
	} else {
	  w32|=i<<(7+nBits);
	  w32|=vTc[i]<<nBits;
	  nBits-=13;
	  if(nBits<=3) {
	    v.push_back(w32>>16);
	    w32=(w32&0xffff)<<16;
	    nBits+=16;
	  }
	}
      }
    }
    if(nBits<19) v.push_back(w32>>16);
    
  } else {
    if(p) std::cout << " High occupancy" << std::endl;
    
    uint64_t bitmap(0),one64(1);
    for(unsigned i(0);i<vTc.size();i++) {
      if(vTc[i]>5) bitmap|=one64<<i;
    }
    v.push_back(bitmap&0xffff);
    v.push_back((bitmap>>16)&0xffff);
    v.push_back((bitmap>>32)&0xffff);
    
    unsigned nBits(25);
    uint32_t w32(0);
    for(unsigned i(0);i<vTc.size();i++) {
      if(vTc[i]>5) {
	w32|=vTc[i]<<nBits;
	nBits-=7;
	if(nBits<=9) {
	  v.push_back(w32>>16);
	  w32=(w32&0xffff)<<16;
	  nBits+=16;
	}
      }
    }
    if(nBits<25) v.push_back(w32>>16);
  }
  
  if(p) std::cout << "PacketCreator::createEcontAsicPacket() finishing, size = "
		  << v.size() << std::endl;

  return;
}
*/
