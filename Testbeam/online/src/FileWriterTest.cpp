#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cassert>

#include "Record.h"
#include "RecordRunning.h"
#include "FileWriter.h"
//#include "FileWriter.hSave"

using namespace Hgcal10gLinkReceiver;

int main(int argc, char *argv[]) {
  FileWriter fw;

  //fw.setDirectory("junk");
  fw.open(123456,0);
  //fw.open(0xffffffff,0);
  
  RecordT<1023> rt;
  Record *r((Record*)(&rt));;
  RecordRunning *rEvent((RecordRunning*)(&rt));;

  rt.setPayloadLength(255);

  rt.setState(FsmState::Starting);
  rt.setUtc();
  rt.print();
  
  fw.write(r);
  
  rt.setState(FsmState::Running);
  SlinkBoe *boe((SlinkBoe*)rEvent->slinkBoe());
  SlinkEoe *eoe((SlinkEoe*)rEvent->slinkEoe());
  boe->setBoeHeader(0x55);
  eoe->setEoeHeader(0xaa);
  eoe->setOrbitId(0);
  
  for(unsigned i(0);i<10*1000000;i++) {
    rt.setSequenceCounter(i);
    boe->setEventId(i+1);
    eoe->setBxId((i%3564)+1);
    eoe->setOrbitId(i/3564);

    fw.write(r);
    if(i<3) rEvent->print();
  }

  rt.setState(FsmState::Stopping);
  rt.setUtc();
  rt.print();

  fw.write(r);

  fw.close();
  
  return 0;
}
