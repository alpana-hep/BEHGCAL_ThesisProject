/*
g++  -Icommon/inc src/RelayCheck.cpp -Ioffline/inc -Iexpert/inc -o bin/RelayCheck.exe -lyaml-cpp -lhdf5_cpp -lhdf5 `root-config --libs --cflags`
*/

#include <iostream>
#include <iomanip>
#include <cassert>

#include <yaml-cpp/yaml.h>

#include "RelayReader.h"
#include "EcontEnergies.h"
#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"

#include "EcondHeader.h"
#include "EcondSubHeader.h"
//#include "ROOT.h"
//#include "TH1.h"
// void econtEnergies(const uint64_t *p, std::vector<uint16_t> &v) {
//   v.resize(24);

//   uint64_t e0,e1;
  
//   e0=p[0]<<32|(p[1]&0xffffffff);
//   e1=p[1]<<48|(p[2]&0xffffffff)<<16|(p[3]&0xffffffff)>>16;
//   //std::cout << "e0,e1 = " << std::hex << e0 << "," << e1 << std::endl;
  
//   v[ 0]=(e0>> 1)&0x7f;
//   v[ 1]=(e0>> 8)&0x7f;
//   v[ 2]=(e0>>15)&0x7f;
//   v[ 3]=(e0>>22)&0x7f;
//   v[ 4]=(e0>>29)&0x7f;
  
//   v[ 5]=(e1    )&0x7f;
//   v[ 6]=(e1>> 7)&0x7f;
//   v[ 7]=(e1>>14)&0x7f;
//   v[ 8]=(e1>>21)&0x7f;
//   v[ 9]=(e1>>28)&0x7f;
//   v[10]=(e1>>35)&0x7f;
//   v[11]=(e1>>42)&0x7f;
  
//   e0=(p[0]&0xffffffff00000000)    |(p[1]&0xffffffff00000000)>>32;
//   e1=(p[1]&0xffffffff00000000)<<16|(p[2]&0xffffffff00000000)>>16|(p[3]&0xffffffff00000000)>>48;
//   //std::cout << "e0,e1 = " << std::hex << e0 << "," << e1 << std::endl;
  
//   v[12]=(e0>> 1)&0x7f;
//   v[13]=(e0>> 8)&0x7f;
//   v[14]=(e0>>15)&0x7f;
//   v[15]=(e0>>22)&0x7f;
//   v[16]=(e0>>29)&0x7f;
  
//   v[17]=(e1    )&0x7f;
//   v[18]=(e1>> 7)&0x7f;
//   v[19]=(e1>>14)&0x7f;
//   v[20]=(e1>>21)&0x7f;
//   v[21]=(e1>>28)&0x7f;
//   v[22]=(e1>>35)&0x7f;
//   v[23]=(e1>>42)&0x7f;
// }

// void unpackerEnergies(const uint64_t *p, std::vector<uint16_t> &v) {
//   v.resize(24);

//   for(unsigned i(0);i<6;i++) {
//     v[2*i   ]=(p[i]    )&0x7f;
//     v[2*i+ 1]=(p[i]>>13)&0x7f;
//     v[2*i+12]=(p[i]>>32)&0x7f;
//     v[2*i+13]=(p[i]>>45)&0x7f;
//   }
// }

int main(int argc, char** argv) {
  if(argc<2) {
    std::cerr << argv[0] << ": No relay number specified" << std::endl;
    return 1;
  }

  uint32_t relayNumber(0);
  std::istringstream issRelay(argv[1]);
  issRelay >> relayNumber;
  std::ofstream myfile_all;
  myfile_all.open("relay_BX_orbitL1As.txt");
  //1695126523
  // root file to store the hist output
  TFile* fout = new TFile("Relay_check_SlinkHeader_1695672141_link2.root","RECREATE");
  // TH1D* h_validPattern_EoE;
  // TH1D* h_validPattern_BoE;

  // TH1D* h_eventLength;
  // TH1D* h_Flag_inEvent; // see if events are increasing by 1 (event counter is not jumping suddenly)
  // TH1D* h_BXnumber; 
  // TH1D* h_Econd_status;
  // TH1D* h_CRCflag;
  // TH1D* h_DAQCRCvsCRC; //see if DAQ CRC matches with Slink CRC
  
  if(relayNumber==0) {
    std::cerr << argv[0] << ": Relay number uninterpretable" << std::endl;
    return 2;
  }

  // Create the file reader
  Hgcal10gLinkReceiver::RelayReader _relayReader;
  _relayReader.enableLink(0,false);
  _relayReader.enableLink(1,false);
  _relayReader.enableLink(2,true);
  _relayReader.openRelay(relayNumber);
  
  // Make the buffer space for the records and some useful casts for configuration and event records
  Hgcal10gLinkReceiver::RecordT<32*4096-1> *rxxx(new Hgcal10gLinkReceiver::RecordT<32*4096-1>);
  Hgcal10gLinkReceiver::RecordYaml *rCfg((Hgcal10gLinkReceiver::RecordYaml*)rxxx);
  Hgcal10gLinkReceiver::RecordRunning *rEvent((Hgcal10gLinkReceiver::RecordRunning*)rxxx);

  unsigned nEvents(0);
  unsigned ncount_boe(0);
  unsigned ncount_eoe(0);                                                                                                                       while(_relayReader.read(rxxx)) {
    //    rxxx->RecordHeader::print();
    if(rxxx->state()!=Hgcal10gLinkReceiver::FsmState::Running) {
      //rxxx->print();
      if(rxxx->state()==Hgcal10gLinkReceiver::FsmState::Configuration) {
	// std::cout<<"rCfg->string()  "<<rCfg->string()<<std::endl;
	// YAML::Node n(YAML::Load(rCfg->string()));
	
	//       if(n["Source"].as<std::string>()=="Serenity") {
      	// }
	//       if(n["Source"].as<std::string>()=="TCDS2") {
	//       }
      }
      // else
      // 	rxxx->print();
    } else {
      
      nEvents++;
      //char* line = new char[100];
      
      // Access the Slink header ("begin-of-event")
      // This should always be present; check pattern is correct
      const Hgcal10gLinkReceiver::SlinkBoe *b(rEvent->slinkBoe());
      const Hgcal10gLinkReceiver::SlinkEoe *e(rEvent->slinkEoe());
      //const Hgcal10gLinkReceiver::EcondHeader *econd(rEvent->econdHeader());
      const Hgcal10gLinkReceiver::BePacketHeader *be(rEvent->bePacketHeader());
      const uint64_t *p64(((const uint64_t*)rEvent)+1);
      const Hgcal10gLinkReceiver::EcondHeader *econd((const Hgcal10gLinkReceiver::EcondHeader *)(p64+3) ); 
      const Hgcal10gLinkReceiver::EcondSubHeader* econd_sub((const Hgcal10gLinkReceiver::EcondSubHeader*) (p64+4));
      if(nEvents<=10) {
        b->print();
        e->print();
        be->print();
	econd->print();
	econd_sub->print();
	rEvent->print();	
        // sprintf(line,"%d %d %d %d\n",int(b->eventId()),int(e->bxId()),int(e->orbitId()),int(e->totalBx()));
	// myfile_all<<line;
        for(unsigned i(0);i<rEvent->payloadLength();i++) {

                std::cout << "Word " << std::setw(3) << i << " ";
                std::cout << std::hex << std::setfill('0');
                std::cout << "0x" << std::setw(16) << p64[i] << std::endl;
                std::cout << std::dec << std::setfill(' ');  
              }
	std::cout << std::endl;
      }
      assert(b!=nullptr);
      
      // Access the Slink trailer ("end-of-event")
      // This should always be present; check pattern is correct
      //const Hgcal10gLinkReceiver::SlinkEoe *e(rEvent->slinkEoe());
      assert(e!=nullptr);
      if(!e->validPattern()){
	 ncount_eoe++;
	 std::cout<< "invalid pattern for EOE"<<std::endl;
	 b->print();
	 e->print();
	 be->print();
	 econd->print();
	 econd_sub->print();
	 //assert(ncount_eoe<50);                                                                                                                                           
    //  }      
    
    //}
      
    }
  }
  std::cout << "Total number of events found = " << nEvents << std::endl;
  fout->cd();
  // h_EcondSta_vsBX->Write();
  // h_validPattern_EoE->Write();
  // h_validPattern_BoE->Write();
  // h_eventLength->Write();
  // h_Flag_inEvent->Write();
  // h_BXnumber->Write();
  // h_Econd_status->Write();
  // h_CRCflag->Write();
  // h_DAQCRCvsCRC->Write();
  // h_orbCounter->Write();
  // h_eventCounter->Write();
  // h_EcondSta_vsBX->Write();
  // h_EcondSta_vsBXcounter->Write();
  // h_EcondSta_vsEventcounter->Write();
  // h_eventLength_vsBXcounter->Write();
  // h_eventLength_vsECONDStatus->Write();
  // //h_eventLength_vsEventCounter->Write();
  // h_eventCounter_SLinkvsDAQ->Write();
  // h_BXCounter_SLinkvsDAQ->Write();
  // h_OrbCOunter_SLinkvsDAQ->Write();
  // h_eventCounter_SLinkvsDAQ_EconStatus5->Write();
  // h_BXCounter_SLinkvsDAQ_EconStatus5->Write();
  // h_OrbCounter_SLinkvsDAQ_EconStatus5->Write();
  // h_eventCounter_SLinkvsDAQ_EconStatus4->Write();
  // h_BXCounter_SLinkvsDAQ_EconStatus4->Write();
  // h_OrbCounter_SLinkvsDAQ_EconStatus4->Write();
  // h_eventCounter_SLinkvsDAQ_EvenLength1->Write();
  // h_BXCounter_SLinkvsDAQ_EvenLength1->Write();
  // h_OrbCounter_SLinkvsDAQ_EvenLength1->Write();
    fout->Write();
  fout->Close();
  //  h_EcondSta_vsBX->Write();
  delete rxxx;
  
  return 0;
}
