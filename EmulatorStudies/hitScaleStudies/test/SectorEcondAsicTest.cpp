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

    hNumberOfElinks=new TH1I("NumberOfElinks",";ECON-D (LUV);Number of elinks",50*256,0,50*256);
  
    hAveragePacketWords[0]=new TH1F("AveragePacketWords1",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[1]=new TH1F("AveragePacketWords2",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[2]=new TH1F("AveragePacketWords3",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[3]=new TH1F("AveragePacketWords4",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[4]=new TH1F("AveragePacketWords5",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[5]=new TH1F("AveragePacketWords6",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[6]=new TH1F("AveragePacketWords7",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    hAveragePacketWords[7]=new TH1F("AveragePacketWords8",";ECON-D (LUV);Average packet size (32-bit words)",50*256,0,50*256);
    
    hAveragePacketWordsNorm[0]=new TH1F("AveragePacketWordsNorm1",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[1]=new TH1F("AveragePacketWordsNorm2",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[2]=new TH1F("AveragePacketWordsNorm3",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[3]=new TH1F("AveragePacketWordsNorm4",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[4]=new TH1F("AveragePacketWordsNorm5",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[5]=new TH1F("AveragePacketWordsNorm6",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[6]=new TH1F("AveragePacketWordsNorm7",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
    hAveragePacketWordsNorm[7]=new TH1F("AveragePacketWordsNorm8",";ECON-D (LUV);Average packet size (32-bit words)/Number of elinks",50*256,0,50*256);
  }

  virtual ~TestEntity() {
  }
  
  void initialise() {
  }

  void setDefinitionTo(const std::vector<EcondAsicDefinition> &d) {
    econdAsicDefinitionVector_=d;

    for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
      std::string labelStub("EcondAsic_");
      labelStub+=econdAsicDefinitionVector_[i].label()+"_";

      th1iPacketSizeWordsVector_.push_back(new TH1I((labelStub+"PacketSizeWords").c_str(),
						    ";Packet size (32-bit words)",120,0,600));
      th1iPacketSizeBitsVector_.push_back(new TH1I((labelStub+"PacketSizeBits").c_str(),
						   ";Packet size (bits)",100,0,20000));
    }
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
  /*
  void connect(FastControlStream &s) {
    Connection<FastControlCommands> *c(new Connection<FastControlCommands>);
    connectRxPortTo(*c);
    s.connectTxPortTo(*c);

    Connection<FastControlRequests> *r(new Connection<FastControlRequests>);
    connectTxPortTo(*r);
    s.connectRxPortTo(*r);
  }
  */
  void connect(EcondAsic &e) {
    rxElinkPtrVector_.push_back(std::vector< Connection<uint32_t>* >());
    econdAsicPtrVector_.push_back(&e);
    //idleSource_=e.definition().halfWordId();
    
    for(unsigned i(0);i<e.definition().numberOfElinks();i++) {
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
    rxElinkPtrVector_.back().push_back(&w);
    w.initialiseQueue(EcondAsicHeader::HeaderPattern);
  }

  void strobeRx() {
    //rxCommandsPtr_->strobeOutput();
    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      for(unsigned j(0);j<rxElinkPtrVector_[i].size();j++) {
	rxElinkPtrVector_[i][j]->strobeOutput();
      }
    }
  }

  void connectTxPortTo(Connection<FastControlRequests> &r) {
    txRequestsPtr_=&r;
  }
  
  void strobeTx() {
    //txRequestsPtr_->strobeInput();
  }
  
  void process() {
    if(debugPrint_) {
      std::cout << "TestEntity::process()  ID " << id_
		<< " entering" << std::endl;
    }

    //assert(rxCommandsPtr_!=0);
    //assert(txRequestsPtr_!=0);

    //FastControlRequests merge;

    //FastControlCommands input;
    //assert(rxCommandsPtr_->get(input));
    //if(debugPrint_) {std::cout << " ";input.print();}

    for(unsigned i(0);i<rxElinkPtrVector_.size();i++) {
      uint32_t elink[14];
      if(debugPrint_) std::cout << " Elinks = " << std::hex << std::setfill('0');

      for(unsigned j(0);j<rxElinkPtrVector_[i].size();j++) {
	assert(rxElinkPtrVector_[i][j]->get(elink[j]));
	if(debugPrint_) std::cout << " 0x" << std::setw(8) << elink[j];

	if((elink[j]&0xffff)==EcondAsicHeader::HeaderPattern) {
	  EcondAsicHeader eah;
	  eah.setWordTo(0,elink[j]); // Sufficient for packet length

	  /* NEEDS RE-WRITE: status now in idle
	  if(eah.bufferStatus()==EcondHeader::High ||
	     eah.bufferStatus()==EcondHeader::Full) throttling_=true;
	  if(eah.bufferStatus()==EcondHeader::Low) throttling_=false;
	  */
	  assert(false);
	  
	  if(debugPrint_ && throttling_) eah.print();
	  th1iPacketSizeWordsVector_[i]->Fill(eah.packetWords());
	  th1iPacketSizeBitsVector_[i]->Fill(32*eah.packetWords());
	}
      }      
      if(debugPrint_) std::cout << std::dec << std::setfill(' ') << std::endl;
    }
    
    //if(txRequestsPtr_!=0) assert(txRequestsPtr_->set(merge));
    
    bx_++;

    if(debugPrint_) {
      std::cout << "TestEntity::process()  ID " << id_
		<< " exiting" << std::endl;
    }
  }

  void finish() {

    // Histogram summary values for each EcondAsic
    double totalAveragePacketWords(0.0);
    for(unsigned i(0);i<econdAsicDefinitionVector_.size();i++) {
      EcondAsicDefinition &ead(econdAsicDefinitionVector_[i]);
      
      unsigned luv(256*(ead.layer()-1)+16*ead.u()+ead.v());
      //std::cout << std::setw(4) << i << ", LUV = " << luv << std::endl;
      
      unsigned ne(ead.numberOfElinks());
      double aps(econdAsicPtrVector_[i]->averagePacketWords());
      totalAveragePacketWords+=aps;
      
      hNumberOfElinks->SetBinContent(luv+1,ne);
      hAveragePacketWords[ne-1]->SetBinContent(luv+1,aps);
      hAveragePacketWordsNorm[ne-1]->SetBinContent(luv+1,aps/ne);
    }
    
    std::cout << "Total average packet size = " << totalAveragePacketWords
	      << " words = " << 0.000032*totalAveragePacketWords
	      << " Mbits, for whole HGCAL = "
	      << 6*0.000032*totalAveragePacketWords << " Mbit"
	      << std::endl;
  }
  
private:
  //EcondAsicDefinition econdAsicDefinition_;
  std::vector<EcondAsicDefinition> econdAsicDefinitionVector_;
  std::vector<EcondAsic*> econdAsicPtrVector_;

  Connection<FastControlCommands> *rxCommandsPtr_;
  Connection<FastControlRequests> *txRequestsPtr_;
  //std::vector< Connection<uint32_t>* > rxElinkPtrVector_;
  std::vector< std::vector< Connection<uint32_t>* > > rxElinkPtrVector_;

  //uint16_t idleSource_;
  
  unsigned id_;
  unsigned bx_;

  bool throttling_;
  
  TH1I *hNumberOfElinks;
  TH1F *hAveragePacketWords[8];
  TH1F *hAveragePacketWordsNorm[8];
  std::vector<TH1I*> th1iPacketSizeWordsVector_;
  std::vector<TH1I*> th1iPacketSizeBitsVector_;
};


int main(int argc, char *argv[]) {
  Arguments args;
  args.setFilenameTo("data/CMSSW/HGCalBEStudies_TTbar_EOL.root");
  args.setNumberTo(2);
  args.setOutputnameTo("SectorEcondAsicTest.root");
  args.processArguments(argc,argv);
  args.print();

  TFile rootFile(args.outputname().c_str(),"RECREATE");

  /*
  std::cout << "ECON-D min and max raw data packet sizes" << std::endl
	    << " HDM min = " << EcondAsicHeader::daqRawDataPacketWords(true,0,0)
	    << ", max = " << EcondAsicHeader::daqRawDataPacketWords(true,0,444)
	    << std::endl
	    << " LDM min = " << EcondAsicHeader::daqRawDataPacketWords(false,0,0)
	    << ", max = " << EcondAsicHeader::daqRawDataPacketWords(false,0,198+33)
	    << std::endl << std::endl;
  */
  
  Definer def;
  std::cout << "Reading mapping file = " << args.mappingFile() << std::endl;
  def.readFile(args.mappingFile());
  
  const std::vector<LpgbtDefinition> &vLd(def.lpgbtDefinitionVector());

  std::vector<EcondAsicDefinition> vEad;
  for(unsigned i(0);i<vLd.size();i++) {
    vLd[i].econdAsicDefinitions(vEad);
  }
  
  FastControlDriverConfiguration fcdc;
  // Turn off calibration L1As
  fcdc.setCalOrbitPeriodTo(0);
  
  FastControlDriver fcd;
  fcd.setConfigurationTo(fcdc);
  fcd.setDebugPrintTo(args.debugPrintFastControlDriver());  

  FastControlStreamConfiguration fcsc;
  // Turn off non-zero suppression
  fcsc.setNoZeroSuppressionPeriodTo(0);
  
  FastControlStream fcs;
  fcs.setConfigurationTo(fcsc);
  fcs.connect(fcd);
  fcs.setDebugPrintTo(args.debugPrintFastControlStream());  

  HgcrocAsics has;
  has.setDebugPrintTo(args.debugPrintHgcrocAsics());  
  
  std::vector<EcondAsicConfiguration> vEac(vEad.size());
  //for(unsigned i(0);i<vEac.size();i++) {
  //vEac[i].setMediumBufferLevelTo(100);
  //vEac[i].setHighBufferLevelTo(200);
  //vEac[i].setFullBufferLevelTo(300);
  //}
  
  std::vector<EcondAsic> vEa(vEad.size());
  for(unsigned i(0);i<vEa.size();i++) {
    vEa[i].setDefinitionTo(vEad[i]);
    vEa[i].setConfigurationTo(vEac[i]);
    vEa[i].connect(fcs);
    vEa[i].setDebugPrintTo(args.debugPrintBitsArg()?
    			   args.debugPrintEcondAsic():i==0);
    //vEa[i].enableAnalyser();

    has.connect(vEa[i]);
  }
  /*
  std::vector<TestEntity> vTe(vEad.size());
  for(unsigned i(0);i<vTe.size();i++) {
    vTe[i].setDefinitionTo(vEad[i]);
    vTe[i].connect(fcs);
    vTe[i].connect(vEa[i]);
    vTe[i].setDebugPrintTo(args.debugPrintBitsArg()?
			   args.debugPrintTestEntity():i==0);
  }
  */
  TestEntity te;
  te.setDefinitionTo(vEad);
  //te.connect(fcs);
  for(unsigned i(0);i<vEa.size();i++) te.connect(vEa[i]);
  te.setDebugPrintTo(args.debugPrintBitsArg()?
		     args.debugPrintTestEntity():true);

  // Select data file and corrections
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

  // std::string dataFile("HGCalBEStudies_TTbar_EOL.root");

  std::cout << "Hit corrections JSON file = " << jsonFile << std::endl;
  std::cout << "CMSSW data ntuple file = " << args.filename() << std::endl;
  std::cout << std::endl;

  
  has.setupHitProvider(std::string("data/CMSSW/")+jsonFile);
  has.hitProvider()->setFile(args.filename(), "beLinksNTuple/EventTree");

  Entity::printTable();
  Entity::doTick(args);
  Entity::printAll();

  te.finish();
  
  rootFile.Write();
  rootFile.Close();

  return 0;
};
