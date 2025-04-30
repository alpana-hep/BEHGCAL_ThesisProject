#include <unistd.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Arguments.h"
#include "Connection.h"
#include "GlobalConfiguration.h"
#include "Definer.h"
#include "EcondAsicHeader.h"
#include "EcondAsic.h"
#include "EcondAsicHitCounts.h"
#include "FastControlCommands.h"
#include "FastControlCounters.h"
#include "FastControlDriver.h"
#include "HGCHitProvider.h"
#include "HgcrocAsics.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH1I.h"

class TestEntity : public Entity {
 public:
  TestEntity() : Entity("TestEntity", Clocks::Clock0040), rxCommandsPtr_(0),
		 nEvents_(0),
		 sumN10_(0),
		 sumN20_(0),
		 sumN30_(0),
		 sumLength_(0),
		 fEnableAnalyser(false),
		 bx_(0) {}

  virtual ~TestEntity() {}

  void initialise() {
    // std::cout << "TestEntity::initialise()" << std::endl << std::endl;
  }

  void setDefinitionTo(const EcondAsicDefinition &d) {
    econdAsicDefinition_ = d;
    idleSource_ = (d.layer() << 8) + (d.u() << 4) + d.v();
  }
  
  const EcondAsicDefinition& definition() const {
    return econdAsicDefinition_;
  }
  
  void setId(unsigned i) { id_ = i; }

  void connect(HgcrocAsics &h) {
    Connection<std::array<uint16_t, 3> > *a(new Connection<std::array<uint16_t, 3> >);
    connectRxPortTo(*a);
    h.connectTxPortTo(*a);
    h.addDefinition(econdAsicDefinition_);
  }

  void connectRxPortTo(Connection<std::array<uint16_t, 3> > &a) { rxCommandsPtr_ = &a; }

  void strobeRx() { rxCommandsPtr_->strobeOutput(); }

  void enableAnalyser() {
    fEnableAnalyser=true;
    hN10=new TH1D((econdAsicDefinition_.label()+"_N10").c_str(),
		  ";Number of 10-bit hits/event;Number of events",
		  econdAsicDefinition_.numberOfChannels(),
		  0,econdAsicDefinition_.numberOfChannels());
    hN20=new TH1D((econdAsicDefinition_.label()+"_N20").c_str(),
		  ";Number of 20-bit hits/event;Number of events",
		  econdAsicDefinition_.numberOfChannels(),
		  0,econdAsicDefinition_.numberOfChannels());
    hN30=new TH1D((econdAsicDefinition_.label()+"_N30").c_str(),
		  ";Number of 30-bit hits/event;Number of events",
		  econdAsicDefinition_.numberOfChannels(),
		  0,econdAsicDefinition_.numberOfChannels());
  }

  void process() {
    if (debugPrint_) {
      std::cout << "TestEntity::process()  ID " << id_ << " entering" << std::endl;
    }

    if (rxCommandsPtr_->get(array_)) {
      if (debugPrint_) {
        econdAsicDefinition_.print();
        std::cout << "TestEntity:process() n10 = " << array_[0] << " n20 = " << array_[1] << " n30 = " << array_[2] << "\n";
      }

      nEvents_++;
      sumN10_+=array_[0];
      sumN20_+=array_[1];
      sumN30_+=array_[2];
      //sumLength_+=(EcondAsicHeader::daqRawDataPacketWords(econdAsicDefinition_,array_[0],array_[1],array_[2])+1)/2;
      sumLength_+=(EcondAsicHitCounts::daqRawDataPacketWords(econdAsicDefinition_,array_[0],array_[1],array_[2])+1)/2;

      if(fEnableAnalyser) {
	hN10->Fill(array_[0]);
	hN20->Fill(array_[1]);
	hN30->Fill(array_[2]);
      }
    }

    // if ((bx_ % 52) == 51) {  // Roughly 750 kHz
    //   assert(rxCommandsPtr_->get(array_));
    // }

    /*
    assert(rxCommandsPtr_!=0);

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
        if(debugPrint_) eah.print();
        th1iPacketSizeWords_->Fill(eah.packetWords());
        th1iPacketSizeBits_->Fill(16*eah.packetLength());
      }
    }
    if(debugPrint_) std::cout << std::dec << std::setfill(' ') << std::endl;


    */

    /*
    FastControlRequests merge;
    uint16_t w(0);
    if(bx_>=(3+id_)) w|=FastControlRequests::thrBit;
    if(bx_==(2*id_+5)) w|=FastControlRequests::ebrBit;
    merge.setWordTo(w);

    if(debugPrint_) {std::cout << " ";merge.print();}
    if(txRequestsPtr_!=0) assert(txRequestsPtr_->set(merge));
    */

    bx_++;

    if (debugPrint_) {
      std::cout << "TestEntity::process()  ID " << id_ << " exiting" << std::endl;
    }
  }

  void write(std::ostream &f) const {
    f << std::setw(2) << econdAsicDefinition_.layer()
      << " " << std::setw(2) << econdAsicDefinition_.u()
      << " " << std::setw(2) << econdAsicDefinition_.v()
      << " " << std::setw(2) << econdAsicDefinition_.type()
      << " " << std::setw(1) << econdAsicDefinition_.shape()
      << " " << std::setw(12) << (1.0*sumN10_)/nEvents_
      << " " << std::setw(12) << (1.0*sumN20_)/nEvents_
      << " " << std::setw(12) << (1.0*sumN30_)/nEvents_
      << " " << std::setw(12) << (1.0*sumLength_)/nEvents_ << std::endl;
  }

  double averageHits() {
    if(nEvents_==0) return 0.0;
    return (1.0*(sumN10_+sumN20_+sumN30_))/nEvents_;
  }
  
  double averageLength() {
    if(nEvents_==0) return 0.0;
    return (1.0*sumLength_)/nEvents_;
  }
  
  double minLength() const {
    //return (EcondAsicHeader::daqRawDataPacketWords(econdAsicDefinition_,0,0,0)+1)/2;
    return (EcondAsicHitCounts::daqRawDataPacketWords(econdAsicDefinition_,0,0,0)+1)/2;
  }
  
  double maxLength() const {
    //return (EcondAsicHeader::daqRawDataPacketWords(econdAsicDefinition_,0,0,econdAsicDefinition_.numberOfChannels())+1)/2;
    return (EcondAsicHitCounts::daqRawDataPacketWords(econdAsicDefinition_,0,0,econdAsicDefinition_.numberOfChannels())+1)/2;
  }
  
  void print() const {
    std::cout << "TestEntity::print()" << econdAsicDefinition_.label()
	      << ", number of events = " << nEvents_ << std::endl;
    std::cout << " Minimum length for  0 hits (32-bit words) = " << minLength() << " = " << 4.0*minLength() << " bytes" << std::endl;
    std::cout << " Maximum length for all N30 (32-bit words) = " << maxLength() << " = " << 4.0*maxLength() << " bytes" << std::endl;

    if(nEvents_>0) {
      std::cout << " Average number of 10-bit hits = " << (1.0*sumN10_)/nEvents_ << std::endl;
      std::cout << " Average number of 20-bit hits = " << (1.0*sumN20_)/nEvents_ << std::endl;
      std::cout << " Average number of 30-bit hits = " << (1.0*sumN30_)/nEvents_ << std::endl;
      std::cout << " Average length (32-bit words) = " << (1.0*sumLength_)/nEvents_ << std::endl;

    }
  }
  
 private:
  EcondAsicDefinition econdAsicDefinition_;

  Connection<std::array<uint16_t, 3> > *rxCommandsPtr_;
  std::vector<Connection<uint32_t> *> rxElinkPtrVector_;

  uint16_t idleSource_;

  std::array<uint16_t, 3> array_;

  unsigned long nEvents_;
  unsigned long sumN10_;
  unsigned long sumN20_;
  unsigned long sumN30_;
  unsigned long sumLength_;
  
  bool fEnableAnalyser;
  TH1D *hN10;
  TH1D *hN20;
  TH1D *hN30;

  unsigned id_;
  unsigned bx_;
};

int main(int argc, char *argv[]) {
  Arguments args;
  args.setFilenameTo("HGCalBEStudies_TTbar_EOL.root");
  args.setNumberTo(14);
  args.processArguments(argc,argv);
  args.print();

  GlobalConfiguration::configuration().setDaqPacketFormatTo((args.number()/4)%4);
  GlobalConfiguration::configuration().print();
  std::cout << std::endl;
  
  // AG 28/5/20 - build a vector of EcondAsicDefinitions from the mapping file. We do not guarantee
  // any kind of ordering here.
  Definer d;
  d.readFile("FeMappingV8.txt");
  std::vector<EcondAsicDefinition> edefs;

  const std::vector<LpgbtDefinition> &vLd(d.lpgbtDefinitionVector());

  bool newLpgbts(args.number()>15);

  unsigned nLpgbtOld(0);
  unsigned nLpgbtNew(0);

  for (auto const& lpgbtdef : vLd) {
    nLpgbtOld+=(lpgbtdef.singleLpgbt()?1:2);

    if(newLpgbts) {
      std::vector<EcondAsicDefinition> eLocal;
      lpgbtdef.econdAsicDefinitions(eLocal);
      if(eLocal.size()>0) {
	if(eLocal[0].layer()!=27 &&
	   eLocal[0].layer()!=28 &&
	   eLocal[0].layer()!=36) {
	  
	  nLpgbtNew+=(lpgbtdef.singleLpgbt()?1:2);
	  lpgbtdef.econdAsicDefinitions(edefs);
	}
      }
    } else {
      nLpgbtNew+=(lpgbtdef.singleLpgbt()?1:2);
      lpgbtdef.econdAsicDefinitions(edefs);
    }
  }
  std::cout << "Number of lpGBTs: old = " << nLpgbtOld
	    << ", new = " << nLpgbtNew << std::endl;
  
  // std::sort(edefs.begin(), edefs.end());

  TFile rootFile("HgcrocAsicsTest.root", "RECREATE");

  HgcrocAsics has;
  has.setDebugPrintTo(args.debugPrintHgcrocAsics());


  std::vector<TestEntity> vTe(edefs.size());

  EcondAsicDefinition edad;

  for (unsigned i(0); i < vTe.size(); i++) {
    vTe[i].setDefinitionTo(edefs[i]);
    vTe[i].connect(has);
  }

  // AG 28/5/20 - Let's just watch one of them
  vTe[0].setDebugPrintTo(args.debugPrintEcondAsic());
  vTe[0].enableAnalyser(); // Histograms

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

  std::string dataFile("HGCalBEStudies_TTbar_EOL.root");

  std::cout << "Hit corrections JSON file = " << jsonFile << std::endl;
  std::cout << "CMSSW data ntuple file = " << args.filename() << std::endl;
  std::cout << std::endl;
  
  has.setupHitProvider(std::string("data/CMSSW/")+jsonFile);
  has.hitProvider()->setFile(std::string("data/CMSSW/")+args.filename(),
                             "beLinksNTuple/EventTree"); // Andrew files
  //"EventTree"); // Alpana files

  Entity::printTable();
  Entity::doTick(args);
  Entity::printAll();

  // Write out averages
  double sh(0.0),shSil(0.0),shSci(0.0),
    sl(0.0),slSil(0.0),slSci(0.0),
    ml(0.0),mlSil(0.0),mlSci(0.0),
    xl(0.0),xlSil(0.0),xlSci(0.0);
  std::ofstream fout("HgcrocAsicsTest.txt");
  for(unsigned i(0);i<vTe.size();i++) {
    vTe[i].write(fout);
    sh+=vTe[i].averageHits();
    sl+=vTe[i].averageLength();    
    ml+=vTe[i].minLength();
    xl+=vTe[i].maxLength();    

    if(vTe[i].definition().type()<2) {
      shSil+=vTe[i].averageHits();
      slSil+=vTe[i].averageLength();    
      mlSil+=vTe[i].minLength();
      xlSil+=vTe[i].maxLength();

    } else {
      shSci+=vTe[i].averageHits();
      slSci+=vTe[i].averageLength();    
      mlSci+=vTe[i].minLength();
      xlSci+=vTe[i].maxLength();
    }
  }
  fout.close();

  std::cout << "Total average hits from FE = " << sh
	    << "/sector = " << 6.0*sh << "/HGCAL" << std::endl;
  std::cout << "Total average size of FE event data = " << sl
	    << " 32-bit words/sector = " << 6.0*sl*4.0e-6 << " MBytes/HGCAL" << std::endl;
  std::cout << "Total min size of FE event data = " << ml
	    << " 32-bit words/sector = " << 6.0*ml*4.0e-6 << " MBytes/HGCAL" << std::endl;
  std::cout << "Total max size of FE event data = " << xl
	    << " 32-bit words/sector = " << 6.0*xl*4.0e-6 << " MBytes/HGCAL" << std::endl << std::endl;

  std::cout << "Total average hits from silicon FE = " << shSil
	    << "/sector = " << 6.0*shSil << "/HGCAL" << std::endl;
  std::cout << "Total average size of silicon FE event data = " << slSil
	    << " 32-bit words/sector = " << 6.0*slSil*4.0e-6 << " MBytes/HGCAL" << std::endl;
  std::cout << "Total min size of silicon FE event data = " << mlSil
	    << " 32-bit words/sector = " << 6.0*mlSil*4.0e-6 << " MBytes/HGCAL" << std::endl;
  std::cout << "Total max size of silicon FE event data = " << xlSil
	    << " 32-bit words/sector = " << 6.0*xlSil*4.0e-6 << " MBytes/HGCAL" << std::endl << std::endl;

  std::cout << "Total average hits from scintillator FE = " << shSci
	    << "/sector = " << 6.0*shSci << "/HGCAL" << std::endl;
  std::cout << "Total average size of scintillator FE event data = " << slSci
	    << " 32-bit words/sector = " << 6.0*slSci*4.0e-6 << " MBytes/HGCAL" << std::endl;
  std::cout << "Total min size of scintillator FE event data = " << mlSci
	    << " 32-bit words/sector = " << 6.0*mlSci*4.0e-6 << " MBytes/HGCAL" << std::endl;
  std::cout << "Total max size of scintillator FE event data = " << xlSci
	    << " 32-bit words/sector = " << 6.0*xlSci*4.0e-6 << " MBytes/HGCAL" << std::endl;
  
  rootFile.Write();
  rootFile.Close();

  return 0;
};
