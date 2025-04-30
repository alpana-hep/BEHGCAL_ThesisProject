#include <unistd.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"

#include "Arguments.h"
#include "Connection.h"
#include "GlobalConfiguration.h"
#include "Definer.h"
#include "FastControlCommands.h"
#include "FastControlRequests.h"
#include "FastControlCounters.h"
#include "FastControlDriver.h"
#include "EcondAsic.h"
#include "EcondAsicHeader.h"
#include "HgcrocAsics.h"

class TestEntity : public Entity {
public:
  TestEntity() : Entity("TestEntity",Clocks::Clock0040), rxCommandsPtr_(0), bx_(0),
		 throttling_(false) {
  }

  virtual ~TestEntity() {
  }
  
  void initialise() {
    std::string labelStub("EcondAsic_");
    labelStub+=econdAsicDefinition_.label()+"_";
      
    th1iPacketSizeWords_=new TH1I((labelStub+"PacketSizeWords").c_str(),
			     ";Packet size (32-bit words)",120,0,600);
    th1iPacketSizeBits_=new TH1I((labelStub+"PacketSizeBits").c_str(),
			     ";Packet size (bits)",100,0,20000);

    // Needs an initial idle word per elink
    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      rxElinkPtrVector_[i]->initialiseQueue(econdAsicDefinition_.idleWord());
    }
  }
  
  void setDefinitionTo(const EcondAsicDefinition &d) {
    econdAsicDefinition_=d;
    idleSource_=(d.layer()<<8)+(d.u()<<4)+d.v();
  }
  
  void setId(unsigned i) {
    id_=i;
  }
  /*
  void connect(FastControlDriver &d) {
    Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
    connectRxPortTo(*c);
    d.connectTxPortTo(*c);
  }
  */
  void connect(FastControlStream &s) {
    Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
    connectRxPortTo(*c);
    s.connectTxPortTo(*c);

    Connection<FastControlRequests> *r(new Connection<FastControlRequests>);
    connectTxPortTo(*r);
    s.connectRxPortTo(*r);
  }

  void connect(EcondAsic &e) {
    for(unsigned i(0);i<econdAsicDefinition_.numberOfElinks();i++) {
      Connection<uint32_t> *w(new Connection<uint32_t>);
      connectRxPortTo(*w);
      e.connectTxPortTo(*w);
    }
  }

  void connectRxPortTo(Connection<FastControlCommands> &c) {
    rxCommandsPtr_=&c;
    c.initialiseQueue(FastControlCommands()); // Idle
  }
  
  void connectRxPortTo(Connection<uint32_t> &w) {
    rxElinkPtrVector_.push_back(&w);
  }

  void strobeRx() {
    rxCommandsPtr_->strobeOutput();
    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      rxElinkPtrVector_[i]->strobeOutput();
    }
  }

  void connectTxPortTo(Connection<FastControlRequests> &r) {
    txRequestsPtr_=&r;
  }
  
  void strobeTx() {
    txRequestsPtr_->strobeInput();
  }
  
  void process() {
    if(debugPrint_) {
      std::cout << "TestEntity::process()  ID " << id_
		<< " entering" << std::endl;
    }

    assert(rxCommandsPtr_!=0);
    assert(txRequestsPtr_!=0);

    FastControlRequests merge;

    FastControlCommands input;
    assert(rxCommandsPtr_->get(input));
    if(debugPrint_) {std::cout << " ";input.print();}

    uint32_t elink[14];
    if(debugPrint_) std::cout << " Elinks = " << std::hex << std::setfill('0');

    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      assert(rxElinkPtrVector_[i]->get(elink[i]));
      if(debugPrint_) std::cout << " 0x" << std::setw(8) << elink[i];

      if((elink[i]&0xffff)==EcondAsicHeader::HeaderPattern) {
	EcondAsicHeader eah;
	eah.setWordTo(0,elink[i]); // Sufficient for packet length

	/* NEEDS RE-WRITING; status moved to idle
	if(eah.bufferStatus()==EcondHeader::High ||
	   eah.bufferStatus()==EcondHeader::Full) throttling_=true;
	if(eah.bufferStatus()==EcondHeader::Low) throttling_=false;
	*/
	assert(false);
	
	if(debugPrint_ && throttling_) eah.print();
	th1iPacketSizeWords_->Fill(eah.packetWords());
	th1iPacketSizeBits_->Fill(32*eah.packetWords());
      }
    }
    if(debugPrint_) std::cout << std::dec << std::setfill(' ') << std::endl;
    
    //uint16_t w(0);
    //if(bx_>=(3+id_)) w|=FastControlRequests::thrBit;
    //if(bx_==(2*id_+5)) w|=FastControlRequests::ebrBit;
    //merge.setWordTo(w);
    
    if(throttling_) merge.setEcondAsicThrottleTo(true);

    if(debugPrint_) {std::cout << " ";merge.print();}
    if(txRequestsPtr_!=0) assert(txRequestsPtr_->set(merge));
    
    bx_++;

    if(debugPrint_) {
      std::cout << "TestEntity::process()  ID " << id_
		<< " exiting" << std::endl;
    }
  }
  
private:
  EcondAsicDefinition econdAsicDefinition_;

  Connection<FastControlCommands> *rxCommandsPtr_;
  Connection<FastControlRequests> *txRequestsPtr_;
  std::vector< Connection<uint32_t>* > rxElinkPtrVector_;

  uint16_t idleSource_;
  
  unsigned id_;
  unsigned bx_;

  bool throttling_;
  
  TH1I *th1iPacketSizeWords_;
  TH1I *th1iPacketSizeBits_;
};


int main(int argc, char *argv[]) {
  Arguments args;
  args.setFilenameTo("FeMappingV8.txt");
  args.setLabelTo("Sector0_Layer25_U04_V05");
  //args.setLabelTo("Sector0_Lpgbt1234");
  args.setNumberTo(10);
  //args.setFilenameTo("HGCalBEStudies_EOL_9k_130620.root");
  args.setFilenameTo("HGCalBEStudies_TTbar_EOL.root");
  args.processArguments(argc,argv);
  args.print();

  /*
  std::cout << "ECON-D min and max raw data packet sizes" << std::endl
	    << " HDM min = " << EcondAsicHeader::daqRawDataPacketWords(true,0,0)
	    << ", max = " << EcondAsicHeader::daqRawDataPacketWords(true,0,444)
	    << std::endl
	    << " LDM min = " << EcondAsicHeader::daqRawDataPacketWords(false,0,0)
	    << ", max = " << EcondAsicHeader::daqRawDataPacketWords(false,0,198+33)
	    << std::endl << std::endl;
  */

  
  GlobalConfiguration::configuration().setDaqPacketFormatTo(args.number()/4);
  EcondAsicDefinition::readDaqModules();
  
  Definer def;
  def.readFile(args.mappingFile());
  const std::vector<LpgbtDefinition> &vLd(def.lpgbtDefinitionVector());

  // CLUDGE
  /*
  std::vector<LpgbtDefinition> vLd;
  vLd.push_back(LpgbtDefinition());
  vLd.back().setLpgbtNumbersTo(1234,1235);
  {
    EcondAsicDefinition ed;
    FastControlStreamDefinition fd;
    ed.setDefinitionTo(3,4,0,0,'F',2);
    fd.addModule(ed);
    ed.setDefinitionTo(3,4,1,0,'F',3);
    fd.addModule(ed);
    vLd.back().addFastControlStream(fd);
  }
  {
    EcondAsicDefinition ed;
    FastControlStreamDefinition fd;
    ed.setDefinitionTo(3,4,2,0,'F',2);
    fd.addModule(ed);
    ed.setDefinitionTo(3,4,3,0,'a',2);
    fd.addModule(ed);
    vLd.back().addFastControlStream(fd);
  }
  {
    EcondAsicDefinition ed;
    FastControlStreamDefinition fd;
    ed.setDefinitionTo(3,4,4,0,'b',1);
    fd.addModule(ed);
    ed.setDefinitionTo(3,4,5,0,'F',4);
    fd.addModule(ed);
    vLd.back().addFastControlStream(fd);
  }
  if(args.debugPrintBits()>0) vLd[0].print();
  */
  
  std::vector<EcondAsicDefinition> vEad;

  for(unsigned i(0);i<vLd.size() && vEad.size()==0;i++) {
    if(args.label()==vLd[i].label()) {
      vLd[i].econdAsicDefinitions(vEad);

    } else {
      std::vector<EcondAsicDefinition> v;
      vLd[i].econdAsicDefinitions(v);
      for(unsigned j(0);j<v.size() && vEad.size()==0;j++) {
	if(args.label()==v[j].label()) {
	  vEad.push_back(v[j]);
	}
      }
    }
  }

  if(vEad.size()==0) {
    std::cout << "Label " << args.label() << " not found" << std::endl;
    return 1;
  }

  std::cout << std::endl;
  for(unsigned i(0);i<vEad.size();i++) {
    vEad[i].print();
    std::cout << std::endl;
  }
  
  FastControlDriver fcd;

  FastControlStream fcs;
  fcs.connect(fcd);

  HgcrocAsics has;
  
  std::vector<EcondAsicConfiguration> vEac(vEad.size());
  for(unsigned i(0);i<vEac.size();i++) {
    vEac[i].setMediumBufferLevelTo(1000);
    vEac[i].setHighBufferLevelTo(1500);
  }
  
  std::vector<EcondAsic> vEa(vEad.size());
  for(unsigned i(0);i<vEa.size();i++) {
    vEa[i].setDefinitionTo(vEad[i]);
    vEa[i].setConfigurationTo(vEac[i]);
    vEa[i].connect(fcs);

    has.connect(vEa[i]);
  }
  
  std::vector<TestEntity> vTe(vEad.size());
  for(unsigned i(0);i<vTe.size();i++) {
    vTe[i].setDefinitionTo(vEad[i]);
    vTe[i].connect(fcs);
    vTe[i].connect(vEa[i]);
  }

  std::string jsonFile;
  if((args.number()%4)==0) {
    jsonFile="modifier_config_notransform.json";
  } else if((args.number()%4)==1) {
    jsonFile="modifier_config_optimistic.json";
  } else if((args.number()%4)==2) {
    jsonFile="modifier_config_fixdensity2.json";
  } else {
    jsonFile="modifier_config_pessimistic.json";
  }

  /*
  std::string dataFile;
  if((args.number()%4)==0) {
    dataFile="HGCalBEStudies_official_9k.root";
  } else if((args.number()%4)==1) {
    dataFile="HGCalBEStudies_official_NuGun_1k_180620.root";
  } else if((args.number()%4)==2) {
    //dataFile="HGCalBEStudies_EOL_9k.root";
    dataFile="HGCalBEStudies_EOL_9k_130620.root";
  } else {
    dataFile="HGCalBEStudies_EOL_NuGun_1k_180620.root";
  }
  */
  std::cout << "Hit corrections JSON file = " << jsonFile << std::endl;
  std::cout << "CMSSW data ntuple file = " << args.filename() << std::endl;
  std::cout << std::endl;

  has.setupHitProvider(std::string("data/CMSSW/")+jsonFile);
  has.hitProvider()->setFile(std::string("data/CMSSW/")+args.filename(),
                             "beLinksNTuple/EventTree");
   
  TFile rootFile("EcondAsicTest.root","RECREATE");

  Entity::printTable();
  Entity::doTick(args);
  Entity::printAll();

  rootFile.Write();
  rootFile.Close();

  return 0;
};
