#include <iostream>
#include <iomanip>
#include <cassert>

#include "GlobalConfiguration.h"
#include "EcondBeHeader.h"
#include "EcondBeTrailer.h"
#include "EcondAsicHeader.h"
#include "EcondAsicHitCounts.h"
//#include "EcontAsicHeader.h"
#include "LpgbtPairHeader.h"
#include "EcondAsic.h"
//#include "EcontAsic.h"
#include "SlinkBoe.h"
#include "SlinkEoe.h"
#include "PacketChecker.h"

PacketChecker::PacketChecker() {
}

bool PacketChecker::checkEcondAsicPacket16(const std::vector<uint16_t> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondAsicPacket16()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);
  
  EcondAsicHeader h;
  h.setWordTo(0,(v[1]<<16)|v[0]);
  h.setWordTo(1,(v[3]<<16)|v[2]);
  
  
  if(p) h.print();
  if(2*h.packetWords()!=v.size()) {
    std::cout << "PacketChecker::checkEcondAsicPacket16() header packet length "
	      << " = " << h.packetWords() << "*2 != " << v.size()
	      << std::endl;
    h.print();
    reply=false;
  }
  if(!h.valid()) {
    std::cout << "PacketChecker::checkEcondAsicPacket16() header not valid"
	      << std::endl;
    h.print();
    reply=false;
  }
  
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << "PacketChecker::checkEcondAsicPacket16() word " << i
		<< " = " << v[i] << std::endl;
    }

    if(i>=4) {
      if(v[i]!=2*i+1) {
	std::cout << "PacketChecker::checkEcondAsicPacket16() word " << i
		  << " = " << v[i] << " != " << 2*i+1 << std::endl;
	reply=false;
      }
    }
  }
  
  return reply;
}

bool PacketChecker::checkEcondAsicPacket32(const std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondAsicPacket32()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);

  EcondAsicHeader eah;
  eah.setWordTo(0,v[0]);
  eah.setWordTo(1,v[1]);
  if(p) eah.print();
  
  if(!eah.valid()) {
    if(!p) eah.print();
    std::cout << "PacketChecker::checkEcondAsicPacket32() header not valid"
	      << std::endl;
    reply=false;
  }

  EcondAsicHitCounts eahc;
  eahc.setWordTo(v[2]);
  if(p) eahc.print();
  /*  
  if(eahc.packetWords()!=v.size()) {
    if(!p) eahc.print();
    std::cout << "PacketChecker::checkEcondAsicPacket32() header packet length "
	      << " = " << eah.packetWords() << " != " << v.size()
	      << std::endl;
    reply=false;
  }
  */
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << " Word " << std::setw(4) << i << " = 0x"
		<< std::hex << std::setfill('0')
		<< std::setw(8) << v[i]
		<< std::dec << std::setfill(' ') << std::endl;
    }
    
    if(i>=3) {
      uint32_t w(((4*i+3)<<16)+4*i+1);
      //if(eah.padding() && i==v.size()-1) w=4*i+1;
      if(v[i]!=w) {
	std::cout << "PacketChecker::checkEcondAsicPacket32() word " << i
		  << " = " << v[i] << " != " << w << std::endl;
	reply=false;
      }
    }
  }
  
  return reply;
}

bool PacketChecker::checkEcondAsicPacket72(const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondAsicPacket72()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);
  std::vector<uint32_t> vEcondAsic(2*(v.size()-1));
  
  if(!v[0].startBit()) {
    std::cout << "PacketChecker::checkEcondAsicPacket72() word 0  no start bit ";
    v[0].print();
    reply=false;
  }
  
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << " Word " << std::setw(4) << i << " ";v[i].print();
    }
    
    if(i>0 && i<v.size()-1) {
      if(v[i].startBit() || v[i].endBit()) {
	std::cout << "PacketChecker::checkEcondAsicPacket72() word " << i
		  << "  start and/or end bit ";
	v[i].print();
	reply=false;
      }
    }
    
    if(i<v.size()-1) {
      vEcondAsic[2*i  ]=v[i].word(0);
      vEcondAsic[2*i+1]=v[i].word(1);
    }

    if(i==v.size()-1) {
      vEcondAsic.push_back(v[i].word(0));
      if(!v[i].paddingBit()) vEcondAsic.push_back(v[i].word(1));
    }
  }
  
  if(!checkEcondAsicPacket32(vEcondAsic,p)) {
    reply=false;
  }
  
  if(!v[v.size()-1].endBit()) {
    std::cout << "PacketChecker::checkEcondAsicPacket72() word " << v.size()-1
	      << "  no end bit ";
    v[v.size()-1].print();
    reply=false;
  }
  
  
  return reply;
}

bool PacketChecker::checkEcondBeBuilderPacket32(const std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondBeBuilderPacket32()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);

  std::vector<uint32_t> vEconAsic;
  FastControlCounters fcc;
  EcondAsicHeader h;
  EcondBeHeader ebh;
  
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << " Word " << std::setw(4) << i << " = "
		<< std::hex << std::setfill('0')
		<< std::setw(8) << v[i]
		<< std::dec << std::setfill(' ') << std::endl;
    }

    if(i<2) ebh.setWordTo(i,v[i]);
    
    if(i==1) {
      if(p) ebh.print();
      if(!ebh.valid()) {
	std::cout << "PacketChecker::checkEcondBeBuilderPacket32() header not valid"
		  << std::endl;
	ebh.print();
	reply=false;
      }  
    }
    
    if(i>1 && i<4) h.setWordTo(i-2,v[i]);
    if(i==3) {
      if(p) h.print();
      if(ebh.counters().bc()!=h.counters().bc()) {
	ebh.counters().print();
	h.counters().print();
	std::cout << "PacketChecker::checkEcondBeBuilderPacket32() word 0 BC counter " << ebh.counters().bc() << " != header BC counter " << h.counters().bc() << std::endl;
	reply=false;
      }
      
      if(ebh.counters().oc()!=h.counters().oc()) {
	ebh.counters().print();
	h.counters().print();
	std::cout << "PacketChecker::checkEcondBeBuilderPacket32() word 0 OC counter " << ebh.counters().oc() << " != header OC counter " << h.counters().oc() << std::endl;
	reply=false;
      }
      
      if(ebh.counters().ec()!=h.counters().ec()) {
	ebh.counters().print();
	h.counters().print();
	std::cout << "PacketChecker::checkEcondBeBuilderPacket32() word 0 EC counter " << ebh.counters().ec() << " != header EC counter " << h.counters().ec() << std::endl;
	reply=false;
      }
      if(!h.valid()) {
	std::cout << "PacketChecker::checkEcondAsicPacket32() header not valid"
		  << std::endl;
	h.print();
	reply=false;
      }  
    }
    
    if(i>1 && i<v.size()-1) vEconAsic.push_back(v[i]);

    if(i==v.size()-2) {
      if(!checkEcondAsicPacket32(vEconAsic,p)) reply=false;
    }
    
    if(i==v.size()-1) {
      EcondBeTrailer ebt;
      ebt.setWordTo(v[i]);
      if(p) ebt.print();

      if(!ebt.valid() || ebt.errorBits()!=0 || ebt.packetLength()!=v.size()-3) {
	std::cout << "PacketChecker::checkEcondBeBuilderPacket32() word " << i
		  << " = " << v[i] << " gives wrong EcondBeTrailer ";
	ebt.print();
	reply=false;
      }
    }
  }
  
  return reply;
}

bool PacketChecker::checkEcondBeBuilderPacket72(const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondBeBuilderPacket72()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);
  std::vector<uint32_t> vEcondBeBuilder(2*(v.size()-1));
  
  if(!v[0].startBit()) {
    std::cout << "PacketChecker::checkEcondBeBuilderPacket72() word 0  no start bit ";
    v[0].print();
    reply=false;
  }
  
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << " Word " << std::setw(4) << i << " ";v[i].print();
    }
    
    if(i>0 && i<v.size()-1) {
      if(v[i].startBit() || v[i].endBit()) {
	std::cout << "PacketChecker::checkEcondBeBuilderPacket72() word " << i
		  << "  start and/or end bit ";
	v[i].print();
	reply=false;
      }
    }
    
    if(i<v.size()-1) {
      vEcondBeBuilder[2*i  ]=v[i].word(0);
      vEcondBeBuilder[2*i+1]=v[i].word(1);
    }

    if(i==v.size()-1) {
      vEcondBeBuilder.push_back(v[i].word(0));
      if(!v[i].paddingBit()) vEcondBeBuilder.push_back(v[i].word(1));
    }
  }
  
  if(!checkEcondBeBuilderPacket32(vEcondBeBuilder,p)) {
    reply=false;
  }
  
  if(!v[v.size()-1].endBit()) {
    std::cout << "PacketChecker::checkEcondBeBuilderPacket72() word " << v.size()-1
	      << "  no end bit ";
    v[v.size()-1].print();
    reply=false;
  }
  
  
  return reply;
}

bool PacketChecker::checkLpgbtBuilderPacket72(const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkLpgbtBuilderPacket72()  size = "
	      << v.size() << std::endl;
  }
  
  bool reply(true);

  std::vector<uint32_t> vEcondBeBuilder;
  EcondBeTrailer ebt;
  
  if(!v[0].startBit()) {
    std::cout << "PacketChecker::checkLpgbtBuilderPacket72() word 0  no start bit ";
    v[0].print();
    reply=false;
  }
  
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << " Word " << std::setw(4) << i << " ";v[i].print();
    }
    
    if(i>0 && i<v.size()-1) {
      if(v[i].startBit() || v[i].endBit()) {
	std::cout << "PacketChecker::checkLpgbtBuilderPacket72() word " << i
		  << "  start and/or end bit ";
	v[i].print();
	reply=false;
      }
    }

    vEcondBeBuilder.push_back(v[i].word(0));

    ebt.setWordTo(v[i].word(0));    
    if(ebt.valid()) {
      if(!checkEcondBeBuilderPacket32(vEcondBeBuilder,p)) reply=false;
      vEcondBeBuilder.resize(0);
      vEcondBeBuilder.push_back(v[i].word(1));

    } else {
      vEcondBeBuilder.push_back(v[i].word(1));

      ebt.setWordTo(v[i].word(1));
      if(ebt.valid()) {
	if(!checkEcondBeBuilderPacket32(vEcondBeBuilder,p)) reply=false;
	vEcondBeBuilder.resize(0);
      }
    }
      
    if(i==v.size()-1) {
      vEcondBeBuilder.push_back(v[i].word(0));
      if(!v[i].paddingBit()) vEcondBeBuilder.push_back(v[i].word(1));
    }
  }
  
  if(!v[v.size()-1].endBit()) {
    std::cout << "PacketChecker::checkLpgbtBuilderPacket72() word " << v.size()-1
	      << "  no end bit ";
    v[v.size()-1].print();
    reply=false;
  }

  return reply;
}

bool PacketChecker::checkEcondAsicPacket32(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondAsicPacket32()  size = "
	      << v.size() << std::endl;
    d.print(" ");
    c.print(" ");
    std::cout << std::endl;
    
    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = 0x"
		<< std::hex << std::setfill('0')
		<< std::setw(8) << v[i]
		<< std::dec << std::setfill(' ') << std::endl;
    }
    std::cout << std::endl;
  }
  
  if(v.size()<2) {
    std::cout << "PacketChecker::checkEcondAsicPacket32() packet size "
	      << " = " << v.size() << " < 2"
	      << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      if(p) {
	std::cout << " Word " << std::setw(4) << i << " = 0x"
		  << std::hex << std::setfill('0')
		  << std::setw(8) << v[i]
		  << std::dec << std::setfill(' ') << std::endl;
      }
    }    

    std::cout << std::endl;
    return false;
  }

  EcondAsicHeader eah;
  eah.setWordTo(0,v[0]);
  eah.setWordTo(1,v[1]);

  if(p) {
    eah.print(" ");
  }

  bool reply(true);

  if(eah.totalPacketLength()!=v.size()) {
    std::cout << "PacketChecker::checkEcondAsicPacket32() header packet words "
	      << " = " << eah.packetWords() << " != " << v.size()-2
	      << std::endl;
    reply=false;
  }
  
  if(!(eah==c)) {
    std::cout << "PacketChecker::checkEcondAsicPacket32() header counters not as expected"
	      << std::endl;
    c.print();
    reply=false;
  }
  
  if(!eah.valid()) {
    std::cout << "PacketChecker::checkEcondAsicPacket32() header not valid"
	      << std::endl;
    reply=false;
  }

  if(eah.truncated()) {
    if(eah.payloadLength()!=0) {
      std::cout << "PacketChecker::checkEcondAsicPacket32() truncated header packet length"
		<< " = " << eah.payloadLength() << " != 0"
		<< std::endl;    
      reply=false;
    }

  } else {
    EcondAsicHitCounts eahc;
    eahc.setWordTo(v[2]);
    if(p) {
      eahc.print(" ");
    }
  
    unsigned xLength(EcondAsicHitCounts::daqRawDataPacketWords(d,eahc.n10(),eahc.n20(),eahc.n30()));
    
    if(2*eah.totalPacketLength()!=xLength) {
      std::cout << "PacketChecker::checkEcondAsicPacket32() header packet length "
		<< " = " << eah.totalPacketLength() << "*2 != " << xLength
		<< std::endl;
      reply=false;
    }
  }
  
  if(!reply) eah.print();

  uint32_t id(d.halfWordId());

  uint32_t crc(0);
  
  for(unsigned i(0);i<v.size();i++) {
    if(p) {
      std::cout << " Word " << std::setw(4) << i << " = 0x"
		<< std::hex << std::setfill('0')
		<< std::setw(8) << v[i]
		<< std::dec << std::setfill(' ') << std::endl;
    }
    if(i==2) crc=v[2];

    if(i>=3) {
      uint32_t w((id<<16)+((c.bc()&0xf)<<12)+i);
      if(i==v.size()-1) w=(~crc)+1;
      
      if(v[i]!=w) {
	std::cout << "PacketChecker::checkEcondAsicPacket32() word " << i
		  << " = " << v[i] << " != " << w << std::endl;
	reply=false;
      }
      
      crc+=v[i];
    }
  }

  if(p) {
    std::cout << " CRC 0x"
	      << std::hex << std::setfill('0')
	      << std::setw(8) << crc
	      << std::dec << std::setfill(' ')
	      << std::endl;
  }
  
  if(crc!=0) {
    std::cout << "PacketChecker::checkEcondAsicPacket32() CRC 0x"
	      << std::hex << std::setfill('0')
	      << std::setw(8) << crc
	      << std::dec << std::setfill(' ')
	      << " != 0" << std::endl;
    reply=false;
  }
  
  return reply;
}

bool PacketChecker::checkEcondAsicPacket72(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondAsicPacket72()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
  }
  
  bool reply(true);

  if(!v[0].startBit()) {
    std::cout << "PacketChecker::checkEcondAsicPacket72() first word has no start bit" << std::endl;
    v[0].print();
    reply=false;
  }
  
  if(!v.back().endBit()) {
    std::cout << "PacketChecker::checkEcondAsicPacket72() last word has no end bit" << std::endl;
    v.back().print();
    reply=false;
  }

  std::vector<uint32_t> v32;
  for(unsigned i(0);i<v.size()-1;i++) {
    v32.push_back(v[i].word(0));
    v32.push_back(v[i].word(1));
  }

  v32.push_back(v.back().word(0));
  if(v.back().paddingBit()) {
    assert(v.back().word(1)==0);
  } else {
    v32.push_back(v.back().word(1));
  }

  if(!checkEcondAsicPacket32(d,c,v32,p)) reply=false;

  if(p) {
    std::cout << "PacketChecker::checkEcondAsicPacket72() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}

bool PacketChecker::checkEcondBePacket32(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<uint32_t> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondBePacket32()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;
  }
  
  if(v.size()<2) {
    std::cout << "PacketChecker::checkEcondBePacket32() packet size "
	      << " = " << v.size() << " < 3"
	      << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      if(p) {
	std::cout << " Word " << std::setw(4) << i << " = 0x"
		  << std::hex << std::setfill('0')
		  << std::setw(8) << v[i]
		  << std::dec << std::setfill(' ') << std::endl;
      }
    }    

    std::cout << std::endl;
    return false;
  }

  EcondBeHeader ebh;
  ebh.setWordTo(0,v[0]);
  ebh.setWordTo(1,v[1]);
  if(p) ebh.print();

  bool reply(true);

  if(ebh.packetWords()!=v.size()-3) {
    std::cout << "PacketChecker::checkEcondBePacket32() BE header packet words "
	      << " = " << ebh.packetWords() << " != " << v.size()-3
	      << std::endl;
    reply=false;
  }
  
  if(!(ebh==c)) {
    std::cout << "PacketChecker::checkEcondBePacket32() BE header counters not as expected"
	      << std::endl;
    c.print();
    reply=false;
  }
  
  if(!ebh.valid()) {
    std::cout << "PacketChecker::checkEcondBePacket32() BE header not valid"
	      << std::endl;
    reply=false;
  }

  std::vector<uint32_t> vAsic(v.size()-3);
  for(unsigned i(2);i<v.size()-1;i++) {  
    vAsic[i-2]=v[i];
  }

  if(!checkEcondAsicPacket32(d,c,vAsic,p)) {
    std::cout << "PacketChecker::checkEcondBePacket32()  ERROR ASIC check failed"
	      << std::endl;
    reply=false;
  }
			 
  EcondBeTrailer ebt;
  ebt.setWordTo(v.back());
  if(p) ebt.print();

  return reply;
}

bool PacketChecker::checkEcondBePacket72(const EcondAsicDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkEcondBePacket72()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
  }

  assert(v[0].startBit());
  assert(v.back().endBit());

  bool reply(true);

  std::vector<uint32_t> v32;
  for(unsigned i(0);i<v.size()-1;i++) {
    v32.push_back(v[i].word(0));
    v32.push_back(v[i].word(1));
  }

  v32.push_back(v.back().word(0));
  if(v.back().paddingBit()) {
    assert(v.back().word(1)==0);
  } else {
    v32.push_back(v.back().word(1));
  }

  if(!checkEcondBePacket32(d,c,v32,p)) reply=false;

  if(p) {
    std::cout << "PacketChecker::checkEcondBePacket72() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}

bool PacketChecker::checkLpgbtPacket(const LpgbtDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkLpgbtPacket()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
  }

  assert(v[0].startBit());
  assert(v.back().endBit());

  bool reply(true);

  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);

  unsigned nLpgbtWord(0);
  std::vector<uint32_t> v32;
  
  for(unsigned i(0);i<vEad.size();i++) {  
    if(p) vEad[i].print();
    v32.resize(0);

    EcondBeHeader ebh;
    for(unsigned j(0);j<2;j++) {
      v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
      nLpgbtWord++;
      ebh.setWordTo(j,v32[j]);
    }
    if(p) ebh.print();
    
    for(unsigned j(0);j<ebh.packetWords();j++) {
      v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
      nLpgbtWord++;
    }

    v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
    
    if((nLpgbtWord%2)==0) {
      if(!v[nLpgbtWord/2].paddingBit()) {
	std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		  << "  Padding bit for ECON-D " << i << " not set"
		  << std::endl;
	reply=false;
      }
      nLpgbtWord++;
      if(v[nLpgbtWord/2].word(nLpgbtWord%2)!=0) {
	std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		  << "  Padding word for ECON-D " << i << " = "
		  << v[nLpgbtWord/2].word(nLpgbtWord%2) << " != 0"
		  << std::endl;
	reply=false;
      }
    } else {
      if(v[nLpgbtWord/2].paddingBit()) {
	std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		  << "  Padding bit for ECON-D " << i << " set"
		  << std::endl;
	reply=false;
      }
    }
    nLpgbtWord++;
    
    if(!PacketChecker::checkEcondBePacket32(vEad[i],c,v32,p)){
      std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		<< "  Failed EcondBe " << i << " packet check"
		<< std::endl;
      reply=false;
    }
  }

  if(p) {
    std::cout << "PacketChecker::checkLpgbtPacket() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}

bool PacketChecker::checkLpgbtPairPacket(const LpgbtPairDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkLpgbtPacket()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
  }

  assert(v[0].startBit());
  assert(v.back().endBit());

  bool reply(true);

  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);

  unsigned nLpgbtWord(0);
  std::vector<uint32_t> v32;
  
  for(unsigned i(0);i<vEad.size();i++) {  
    if(p) vEad[i].print();
    v32.resize(0);

    EcondBeHeader ebh;
    for(unsigned j(0);j<2;j++) {
      v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
      nLpgbtWord++;
      ebh.setWordTo(j,v32[j]);
    }
    if(p) ebh.print();
    
    for(unsigned j(0);j<ebh.packetWords();j++) {
      v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
      nLpgbtWord++;
    }

    v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
    
    if((nLpgbtWord%2)==0) {
      if(!v[nLpgbtWord/2].paddingBit()) {
	std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		  << "  Padding bit for ECON-D " << i << " not set"
		  << std::endl;
	reply=false;
      }
      nLpgbtWord++;
      if(v[nLpgbtWord/2].word(nLpgbtWord%2)!=0) {
	std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		  << "  Padding word for ECON-D " << i << " = "
		  << v[nLpgbtWord/2].word(nLpgbtWord%2) << " != 0"
		  << std::endl;
	reply=false;
      }
    } else {
      if(v[nLpgbtWord/2].paddingBit()) {
	std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		  << "  Padding bit for ECON-D " << i << " set"
		  << std::endl;
	reply=false;
      }
    }
    nLpgbtWord++;
    
    if(!PacketChecker::checkEcondBePacket32(vEad[i],c,v32,p)){
      std::cout << std::endl << "PacketChecker::checkLpgbtPacket() ERROR"
		<< "  Failed EcondBe " << i << " packet check"
		<< std::endl;
      reply=false;
    }
  }

  if(p) {
    std::cout << "PacketChecker::checkLpgbtPacket() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}
bool PacketChecker::checkLpgbtPairBuilderPacket(const LpgbtPairDefinition &d, const FastControlCounters &c, const std::vector<RamWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkLpgbtPairBuilderPacket() called, size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
  }

  bool reply(true);

  if(!v[0].startBit()) {
    std::cout << "PacketChecker::checkLpgbtPairBuilderPacket() first word has no start bit" << std::endl;
    v[0].print();
    reply=false;
  }
  
  if(!v.back().endBit()) {
    std::cout << "PacketChecker::checkLpgbtPairBuilderPacket() last word has no end bit" << std::endl;
    v.back().print();
    reply=false;
  }

  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);

  LpgbtPairHeader lph(v[0]);
  if(p) {
    lph.print(" ");
  }
  
  unsigned nLpgbtWord(2);
  std::vector<uint32_t> v32;

  for(unsigned i(0);i<vEad.size();i++) {  
    if(!lph.packetPresent(i)) {
      if(p) std::cout << "PacketChecker::checkLpgbtPairBuilderPacket()"
		      << "  Packet for ECON-D " << i << " not present"
		      << std::endl;
    } else {
      if(p) vEad[i].print(" ");
      v32.resize(0);
      
      EcondAsicHeader eh(v[nLpgbtWord/2]);
      //eh.setWordTo(0,v[nLpgbtWord/2].word(0));
      //eh.setWordTo(1,v[nLpgbtWord/2].word(1));
      
      if(p) eh.print();
      
      for(unsigned j(0);j<eh.totalPacketLength();j++) {
	v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
	nLpgbtWord++;
      }
      
      //v32.push_back(v[nLpgbtWord/2].word(nLpgbtWord%2));
      
      if((nLpgbtWord%2)==1) {
	if(!v[nLpgbtWord/2].paddingBit()) {
	  std::cout << std::endl << "PacketChecker::checkLpgbtPairBuilderPacket() ERROR"
		    << "  Padding bit for ECON-D " << i << " not set"
		    << std::endl;
	  reply=false;
	}
	
	if(v[nLpgbtWord/2].word(nLpgbtWord%2)!=0) {
	  std::cout << std::endl << "PacketChecker::checkLpgbtPairBuilderPacket() ERROR"
		    << "  Padding word for ECON-D " << i << " = "
		    << v[nLpgbtWord/2].word(nLpgbtWord%2) << " != 0"
		    << std::endl;
	  reply=false;
	}
	nLpgbtWord++;
	
      } else {
	if(v[(nLpgbtWord-1)/2].paddingBit()) {
	  std::cout << std::endl << "PacketChecker::checkLpgbtPairBuilderPacket() ERROR"
		    << "  Padding bit for ECON-D " << i << " set"
		    << std::endl;
	  reply=false;
	}
      }
      //nLpgbtWord++;
      
      if(!PacketChecker::checkEcondAsicPacket32(vEad[i],c,v32,p)){
	std::cout << std::endl << "PacketChecker::checkLpgbtPairBuilderPacket() ERROR"
		  << "  Failed EcondAsicPacket32 " << i << " packet check"
		  << std::endl;
	reply=false;
      }
    }
  }
  
  if(p) {
    std::cout << "PacketChecker::checkLpgbtPairBuilderPacket() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}

bool PacketChecker::checkSlinkPacket(const SlinkDefinition &d, const FastControlCounters &c, const std::vector<SlinkWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkSlinkPacket()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
    std::cout << std::endl;
  }

  if(v.size()<2) {
    std::cout << "PacketChecker::checkSlinkPacket()  size = "
	      << v.size() << " < 2" << std::endl;
    return false;
  }

  bool reply(true);
  
  SlinkBoe boe(v[0]);
  if(p) boe.print();

  uint16_t crc(boe.crcIncrement());

  if(!boe.valid()) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() BOE invalid"
	      << std::endl;
    boe.print();
    reply=false;
  }
  
  if(boe.sourceId()!=d.sourceId()) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() BOE source ID "
	      << boe.sourceId()<< " != " << d.sourceId()
	      << std::endl;
    boe.print();
    reply=false;
  }

  if(!(boe==c)) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() BOE counters incorrect"
	      << std::endl;
    c.print();
    boe.print();
    reply=false;
  }

  if(boe.eventType()!=1) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() BOE event type "
	      << boe.eventType()<< " != 1"
	      << std::endl;
    boe.print();
    reply=false;
  }

  for(unsigned i(1);i<v.size()-1;i++) crc+=v[i].crcIncrement();
  
  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);

  unsigned nSlinkWord(4);
  std::vector<uint32_t> v32;
  
  for(unsigned i(0);i<vEad.size();i++) {  
    if(p) vEad[i].print();
    v32.resize(0);

    EcondBeHeader ebh;
    for(unsigned j(0);j<2;j++) {
      v32.push_back(v[nSlinkWord/4].word(nSlinkWord%4));
      nSlinkWord++;
      ebh.setWordTo(j,v32[j]);
    }
    if(p) ebh.print();
    
    for(unsigned j(0);j<unsigned(ebh.packetWords()+1);j++) {
      v32.push_back(v[nSlinkWord/4].word(nSlinkWord%4));
      nSlinkWord++;
    }
    
    if(!PacketChecker::checkEcondBePacket32(vEad[i],c,v32,p)){
      std::cout << std::endl << "PacketChecker::checkSlinkPacket() ERROR"
		<< "  Failed EcondBe " << i << " packet check"
		<< std::endl;
      reply=false;
    }
  }

  if(p) std::cout << "Number of slink words used after ECON-Ds = "
		  << nSlinkWord << " compared with " << v.size() << std::endl;
  assert(((nSlinkWord+7)/4)==v.size());
  
  for(;nSlinkWord<v.size()-4;nSlinkWord++) {
    if(v[nSlinkWord/4].word(nSlinkWord%4)!=0) {
      std::cout << std::endl << "PacketChecker::checkSlinkPacket() ERROR"
		<< "  Padding word " << nSlinkWord << " = "
		<< v[nSlinkWord/4].word(nSlinkWord%4) << " != 0"
		<< std::endl;
      reply=false;
    }
  }
  
  SlinkEoe eoe(v.back());
  if(p) eoe.print();

  if(!eoe.valid()) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() EOE invalid"
	      << std::endl;
    eoe.print();
    reply=false;
  }
  
  if(eoe.fedUserId()!=d.sourceId()) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() EOE FED user ID "
	      << eoe.fedUserId() << " != " << d.sourceId()
	      << std::endl;
    eoe.print();
    reply=false;
  }

  if(eoe.status()!=0) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() EOE status "
	      << eoe.status() << " != 0"
	      << std::endl;
    eoe.print();
    reply=false;
  }

  uint16_t crcEoe(eoe.crc());
  eoe.setCrcTo(0);
  crc+=eoe.crcIncrement();

  if(crcEoe!=crc) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() EOE CRC "
	      << crcEoe << " != " << crc
	      << std::endl;
    eoe.print();
    reply=false;
  }

  if(eoe.eventLength()!=v.size()) {
    std::cout << "ERROR PacketChecker::checkSlinkPacket() EOE event length = "
	      << eoe.eventLength() << " != " << v.size() << std::endl;
    eoe.print();
    reply=false;
  }

  if(p) {
    std::cout << "PacketChecker::checkSlinkPacket() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}

bool PacketChecker::checkSlinkBuilderPacket(const SlinkDefinition &d, const FastControlCounters &c, const std::vector<SlinkWord> &v, bool p) {
  if(p) {
    std::cout << "PacketChecker::checkSlinkBuilderPacket()  size = "
	      << v.size() << std::endl;
    d.print();
    c.print();
    std::cout << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(4) << i << " = ";
      v[i].print();
    }
    std::cout << std::endl;
  }

  if(v.size()<2) {
    std::cout << "PacketChecker::checkSlinkBuilderPacket()  size = "
	      << v.size() << " < 2" << std::endl;
    return false;
  }

  bool reply(true);
  
  SlinkBoe boe(v[0]);
  if(p) boe.print();

  uint16_t crc(boe.crcIncrement());

  if(!boe.valid()) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() BOE invalid"
	      << std::endl;
    boe.print();
    reply=false;
  }
  
  if(boe.sourceId()!=d.sourceId()) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() BOE source ID "
	      << boe.sourceId()<< " != " << d.sourceId()
	      << std::endl;
    boe.print();
    reply=false;
  }

  if(!(boe==c)) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() BOE counters incorrect"
	      << std::endl;
    c.print();
    boe.print();
    reply=false;
  }

  if(boe.eventType()!=1) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() BOE event type "
	      << boe.eventType()<< " != 1"
	      << std::endl;
    boe.print();
    reply=false;
  }

  for(unsigned i(1);i<v.size()-1;i++) crc+=v[i].crcIncrement();
  
  std::vector<LpgbtPairDefinition> vLpd;
  d.lpgbtPairDefinitions(vLpd);
  
  unsigned nSlinkWord(4);
  std::vector<RamWord> vRw;
  std::vector<uint32_t> v32;
  
  for(unsigned i(0);i<vLpd.size();i++) {  
    if(p) vLpd[i].print();

    std::vector<EcondAsicDefinition> vEad;
    vLpd[i].econdAsicDefinitions(vEad);

    vRw.resize(0);

    vRw.push_back(RamWord());
    vRw.back().setWordTo(0,v[nSlinkWord/4].word(nSlinkWord%4));
    nSlinkWord++;
    vRw.back().setWordTo(1,v[nSlinkWord/4].word(nSlinkWord%4));
    nSlinkWord++;
    vRw.back().setStartBitTo(true);
    
    LpgbtPairHeader lph(vRw.back());
    if(p) lph.print(" ");

    for(unsigned j(0);j<lph.numberOfConnectedEconds();j++) {
      if(lph.packetPresent(j)) {
	v32.resize(0);

	EcondAsicHeader eh;
	for(unsigned k(0);k<2;k++) {
	  v32.push_back(v[nSlinkWord/4].word(nSlinkWord%4));
	  nSlinkWord++;
	  eh.setWordTo(k,v32[k]);
	}
	if(p) eh.print();

	for(unsigned k(0);k<eh.payloadLength();k++) {
	  v32.push_back(v[nSlinkWord/4].word(nSlinkWord%4));
	  nSlinkWord++;
	} 

	if(!PacketChecker::checkEcondAsicPacket32(vEad[j],c,v32,p)){
	  std::cout << std::endl << "PacketChecker::checkSlinkBuilderPacket() ERROR"
		    << "  Failed EcondBe " << i << " packet check"
		    << std::endl;
	  reply=false;
	}
      }
    }
    /*  
   if(!PacketChecker::checkLpgbtPairBuilderPacket(vLpd[i],c,vRw,p)){
      std::cout << std::endl << "PacketChecker::checkSlinkBuilderPacket() ERROR"
		<< "  Failed  checkLpgbtPairBuilderPacket " << i << " packet check"
		<< std::endl;
      reply=false;
    }
    */
  }
  /*
  std::vector<EcondAsicDefinition> vEad;
  d.econdAsicDefinitions(vEad);

  unsigned nSlinkWord(4);
  std::vector<uint32_t> v32;
  
  for(unsigned i(0);i<vEad.size();i++) {  
    if(p) vEad[i].print();
    v32.resize(0);

    EcondAsicHeader eh;
    for(unsigned j(0);j<2;j++) {
      v32.push_back(v[nSlinkWord/4].word(nSlinkWord%4));
      nSlinkWord++;
      eh.setWordTo(j,v32[j]);
    }
    if(p) eh.print();
    
    for(unsigned j(2);j<unsigned(eh.totalPacketLength());j++) {
      v32.push_back(v[nSlinkWord/4].word(nSlinkWord%4));
      nSlinkWord++;
    }
    
    if(!PacketChecker::checkEcondAsicPacket32(vEad[i],c,v32,p)){
      std::cout << std::endl << "PacketChecker::checkSlinkBuilderPacket() ERROR"
		<< "  Failed EcondBe " << i << " packet check"
		<< std::endl;
      reply=false;
    }
  }

  if(p) std::cout << "Number of slink words used after ECON-Ds = "
		  << nSlinkWord << " compared with " << v.size() << std::endl;
  assert(((nSlinkWord+7)/4)==v.size());
  
  for(;nSlinkWord<v.size()-4;nSlinkWord++) {
    if(v[nSlinkWord/4].word(nSlinkWord%4)!=0) {
      std::cout << std::endl << "PacketChecker::checkSlinkBuilderPacket() ERROR"
		<< "  Padding word " << nSlinkWord << " = "
		<< v[nSlinkWord/4].word(nSlinkWord%4) << " != 0"
		<< std::endl;
      reply=false;
    }
  }
  */
  
  SlinkEoe eoe(v.back());
  if(p) eoe.print();

  if(!eoe.valid()) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() EOE invalid"
	      << std::endl;
    eoe.print();
    reply=false;
  }
  
  if(eoe.fedUserId()!=d.sourceId()) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() EOE FED user ID "
	      << eoe.fedUserId() << " != " << d.sourceId()
	      << std::endl;
    eoe.print();
    reply=false;
  }

  if(eoe.status()!=0) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() EOE status "
	      << eoe.status() << " != 0"
	      << std::endl;
    eoe.print();
    reply=false;
  }

  uint16_t crcEoe(eoe.crc());
  eoe.setCrcTo(0);
  crc+=eoe.crcIncrement();

  if(crcEoe!=crc) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() EOE CRC "
	      << crcEoe << " != " << crc
	      << std::endl;
    eoe.print();
    reply=false;
  }

  if(eoe.eventLength()!=v.size()) {
    std::cout << "ERROR PacketChecker::checkSlinkBuilderPacket() EOE event length = "
	      << eoe.eventLength() << " != " << v.size() << std::endl;
    eoe.print();
    reply=false;
  }

  if(p) {
    std::cout << "PacketChecker::checkSlinkBuilderPacket() complete, size = "
	      << v.size() << std::endl;
  }
  
  return reply;
}

/*
bool PacketChecker::checkEcontAsicPacket(const EcontAsicDefinition &, unsigned bx, const std::vector<uint16_t> &v, bool p) {

  if(p) {
    std::cout << "PacketChecker::checkEcontAsicPacket() entering, size = "
	      << v.size() << std::endl;

    for(unsigned i(0);i<v.size();i++) {
      std::cout << " Word " << std::setw(2) << i << " = 0x"
		<< std::hex << std::setfill('0')
		<< std::setw(4) << v[i]
		<< std::dec << std::setfill(' ')
		<< std::endl;
    }
  }
    
  if(v.size()==0) {
    if(p) std::cout << " ERROR: number of words = " << v.size()
		    << " == 0" << std::endl;
    return false;
  }

  if(v.size()>25) {
    if(p) std::cout << " ERROR: number of words = " << v.size()
		    << " > 25" << std::endl;
    return false;
  }

  bool reply(true);

  //uint16_t w;

  EcontAsicHeader eah;
  eah.setWordTo(v[0]);
  if(p) eah.print(" ");

  if(eah.dataType()==3 || eah.dataType()==4 || eah.dataType()==5) {
    if(p) std::cout << " ERROR: Data type = " << (unsigned)eah.dataType()
		    << " != valid" << std::endl;
    reply=false;
  }

  if(eah.dataType()==0 || eah.dataType()==6 || eah.dataType()==7) {
    if(v.size()!=1) {
      if(p) std::cout << " ERROR: Data type = " << (unsigned)eah.dataType()
		      << " but size = " << v.size() << " != 1" << std::endl;
      reply=false;
    }
    
  } else if(eah.dataType()==1) {
    if(v.size()<2) {
      if(p) std::cout << " ERROR: Data type = " << (unsigned)eah.dataType()
		      << " but size = " << v.size() << " < 2" << std::endl;
      reply=false;

    } else {    
      unsigned nTc(v[1]>>13);
      unsigned expectedSize(1+(3+13*nTc+15)/16);
      
      if(v.size()!=expectedSize) {
	if(p) std::cout << " ERROR: Data type = " << (unsigned)eah.dataType()
			<< ", number of TCs = " << nTc
			<< " but size = " << v.size() << " != "
			<< expectedSize << std::endl;
	reply=false;
      }

      unsigned c((v[1]>>7)&0x3f);
      unsigned e(v[1]&0x7f);
      if(p) std::cout << "  TC 0/" << nTc << ": Channel = " << c << ", energy = " << e << std::endl;
      
      if(c>=48) {
	if(p) std::cout << " ERROR: TC " << 0 << ", channel = " << c
			<< " >= 48" << std::endl;
	reply=false;
      }

      unsigned cOld(c);
      for(unsigned i(1);i<nTc && i<2;i++) {
	c=(v[2]>>10)&0x3f;
	e=(v[2]>>3)&0x7f;
	
	if(p) std::cout << "  TC 1/" << nTc << ": Channel = " << c << ", energy = " << e << std::endl;
	
	if(c>=48) {
	  if(p) std::cout << " ERROR: TC " << i << ", channel = " << c
			  << " >= 48" << std::endl;
	  reply=false;
	}
	
	if(c<=cOld) {
	  if(p) std::cout << " ERROR: TC " << i << ", channel = " << c
			  << " <= previous = " << cOld << std::endl;
	  reply=false;
	}
	cOld=c;
      }
    }
      
  } else {
    if(v.size()<4) {
      if(p) std::cout << " ERROR: Data type = " << (unsigned)eah.dataType()
		      << " but size = " << v.size() << " < 4" << std::endl;
      reply=false;

    } else {
      uint64_t bitmap(0),w64,one64(1);
      for(unsigned i(0);i<3;i++) {
	w64=v[i+1];
	bitmap|=(w64<<(16*i));
      }
    
      unsigned nTc(0);
      for(unsigned i(0);i<48;i++) {
	if((bitmap&(one64<<i))!=0) nTc++;
      }
      
      unsigned expectedSize(4+(7*nTc+15)/16);
      if(v.size()!=expectedSize) {
	if(p) std::cout << " ERROR: Data type = " << (unsigned)eah.dataType()
			<< " but size = " << v.size() << " != "
			<< expectedSize << std::endl;
	reply=false;
      }
    }
  }
  
  if(p) std::cout << "PacketChecker::checkEcontAsicPacket() leaving" << std::endl;

  return reply;
}
*/
