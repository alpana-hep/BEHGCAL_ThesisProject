#include <iostream>
#include<fstream>
#include "Definer.h"
#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"
#include "json.hpp"
using json = nlohmann::json;

Definer::Definer() {
}

const std::vector<LpgbtDefinition>& Definer::lpgbtDefinitionVector() const {
  return lpgbtDefinitionVector_;
}

void Definer::readFile(const std::string &filename) // added by Alpana on 08May2020
{
  // code to read the file goes here.
  char* fname = new char[200];
  sprintf(fname,"data/FE/%s",filename.c_str());
  std::ifstream mapfile;
  mapfile.open(fname, std::ios::in);

  if(filename.substr(filename.find_last_of(".") + 1) == "json") {
    std::cout << "Reading FE mapping json file " << filename << std::endl;
    json j= json::parse(mapfile);
    std::cout << " Total number of entries = " << j.size() << std::endl;

    unsigned nSingles(0),nDoubles(0);
    int econd=0;
    for(int i_entry=0;i_entry<(int(j.size()));i_entry++)
      {
	econd+=int(j[i_entry]["modules"].size());
	// LpgbtDefinition lpgbt1;
	// LpgbtDefinition lpgbt2;
	if(j[i_entry]["lpGBTs"].size()==2)
	  {
	    /*
	    bool dlpgbt=false;
	    for(int m_entry=0;m_entry<(int(j[i_entry]["modules"].size()));m_entry++)
	      {
		if(int(j[i_entry]["modules"][m_entry]["elinks"][0])!=0 && int(j[i_entry]["modules"][m_entry]["elinks"][1]!=0))
		  dlpgbt=true;
	      }
	    */
	    bool dlpgbt=true;  // FORCE ALL HD PAIRS TO BE DOUBLES
	    if(dlpgbt)
	      {
		nDoubles++;
		LpgbtDefinition Dlpgbt;
		Dlpgbt.setLpgbtNumbersTo(j[i_entry]["lpGBTs"][0], j[i_entry]["lpGBTs"][1]);
		for(int m_entry=0;m_entry<(int(j[i_entry]["modules"].size()));m_entry++)
		  {
		    int e=0;
		    e=int(j[i_entry]["modules"][m_entry]["elinks"][0]) + int(j[i_entry]["modules"][m_entry]["elinks"][1]);
		    std::string shape= j[i_entry]["modules"][m_entry]["shape"].get<std::string>();
		    char s = shape[0];
		    FastControlStreamDefinition fcsd;
		    fcsd.setNumbersTo(Dlpgbt.lpgbtNumber(),m_entry);
		    EcondAsicDefinition ead;
		    ead.setDefinitionTo(int(j[i_entry]["layer"]),int(j[i_entry]["modules"][m_entry]["u"]),int(j[i_entry]["modules"][m_entry]["v"]),j[i_entry]["type"],s,e);
		    fcsd.addModule(ead); 
		    Dlpgbt.addFastControlStream(fcsd);
		  }
		lpgbtDefinitionVector_.push_back(Dlpgbt);
	      
	      }
	    else
	      {
		nSingles+=2;
		LpgbtDefinition lpgbt1;
		LpgbtDefinition lpgbt2;

		lpgbt1.setLpgbtNumberTo(j[i_entry]["lpGBTs"][0]);
		lpgbt2.setLpgbtNumberTo(j[i_entry]["lpGBTs"][1]);
		for(int m_entry=0;m_entry<(int(j[i_entry]["modules"].size()));m_entry++)
		  {
		    std::string shape= j[i_entry]["modules"][m_entry]["shape"].get<std::string>();
		    char s = shape[0];
		    if(j[i_entry]["modules"][m_entry]["elinks"][1]==0)
		      {
			int e= int(j[i_entry]["modules"][m_entry]["elinks"][0]);
			FastControlStreamDefinition fcsd; 
			fcsd.setNumbersTo(lpgbt2.lpgbtNumber(),m_entry);
			EcondAsicDefinition ead;
			ead.setDefinitionTo(int(j[i_entry]["layer"]),int(j[i_entry]["modules"][m_entry]["u"]),int(j[i_entry]["modules"][m_entry]["v"]),j[i_entry]["type"],s,e);
			fcsd.addModule(ead);
			lpgbt1.addFastControlStream(fcsd);
		      }
		    else if (j[i_entry]["modules"][m_entry]["elinks"][0]==0)
		      {
			int e= int(j[i_entry]["modules"][m_entry]["elinks"][1]);
			FastControlStreamDefinition fcsd;
                        fcsd.setNumbersTo(lpgbt2.lpgbtNumber(),m_entry);
                        EcondAsicDefinition ead;
                        ead.setDefinitionTo(int(j[i_entry]["layer"]),int(j[i_entry]["modules"][m_entry]["u"]),int(j[i_entry]["modules"][m_entry]["v"]),j[i_entry]["type"],s,e);
                        fcsd.addModule(ead);
			lpgbt2.addFastControlStream(fcsd);

		      }
		  }
		lpgbtDefinitionVector_.push_back(lpgbt2);
		lpgbtDefinitionVector_.push_back(lpgbt1);		      		    
	      }		
	  } //size ==2
	else if (j[i_entry]["lpGBTs"].size()==1)
	  {
	    nSingles++;
	    LpgbtDefinition Slpgbt;
	    Slpgbt.setLpgbtNumberTo(j[i_entry]["lpGBTs"][0]);
	    for(int m_entry=0;m_entry<(int(j[i_entry]["modules"].size()));m_entry++)
	      {
		std::string shape= j[i_entry]["modules"][m_entry]["shape"].get<std::string>();
		char s = shape[0];
		int e= int(j[i_entry]["modules"][m_entry]["elinks"][0]);
		FastControlStreamDefinition fcsd;
		fcsd.setNumbersTo(Slpgbt.lpgbtNumber(),m_entry);
		EcondAsicDefinition ead;
		ead.setDefinitionTo(int(j[i_entry]["layer"]),int(j[i_entry]["modules"][m_entry]["u"]),int(j[i_entry]["modules"][m_entry]["v"]),j[i_entry]["type"\
																			  ],s,e);
		fcsd.addModule(ead);
		Slpgbt.addFastControlStream(fcsd);
	      }
	    lpgbtDefinitionVector_.push_back(Slpgbt);
	  }
      
	//std::cout<<j[i_entry]["lpGBTs"].size()<<std::endl;
      }
    std::cout<< " Number of lpGBT definitions = " << lpgbtDefinitionVector_.size()
	     << " of which singles = " << nSingles << ", doubles = " << nDoubles << std::endl
	     << " Total number of ECON-Ds = "<< econd << std::endl;
  }
  else {
    std::cout << "Reading FE mapping txt file " << filename << std::endl;
  // store all lpGBTs in this map after their classification of single or double  
  struct lpGBT_Definition_Map {
    unsigned int  layer, u, v, type;
    char shape;
    unsigned  nDaqLpgbt;
    int  idxDaqLpgbt1, idxDaqLpgbt2;
    int nDaqElinks1, nDaqElinks2;
    //    int  nTpgLpgbt, idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;                                                                
    std::pair< int, int> idxLpgbt;
  };
  std::vector<lpGBT_Definition_Map> lpgbt_vector; // will contain all the information regarding s/d lpGBTs

  // to read the information from mapping file
  struct modulemap {
    unsigned int  layer, u, v, type;
    char shape;
    int nDaqElinks1, nDaqElinks2;
    int  idxDaqLpgbt1, idxDaqLpgbt2;
    int  idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
    unsigned  nDaqLpgbt,  nTpgLpgbt;
  };
  std::vector <modulemap> map_vec; // contains all information from the mapping file
  //  std::cout<<"check1"<<std::endl;
  //first for mapfile lpGBTs..
  if (!mapfile.is_open())
    {
      std::cout<<"no such file"<<std::endl;
    }
  else
    {
      map_vec.clear();
      //      lpgbtDefinitionVector_.clear();
      //start reading the file
      while(true)
	{
	 unsigned layer, u, v, type;
	  char shape;
	  int nDaqElinks1, nDaqElinks2;
	  int   idxDaqLpgbt1,idxDaqLpgbt2;
	  unsigned nTpgLpgbt,  nDaqLpgbt;
	  int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
	  mapfile >> layer >> u >> v >> type >> shape
		  >> nDaqLpgbt >> nTpgLpgbt
		  >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2
		  >> idxTpgLpgbt1 >> nTpgElinks1 >> idxTpgLpgbt2 >> nTpgElinks2;
	  if (mapfile.eof()) break;
	  modulemap mm;
	  mm.layer=layer; mm.u=u; mm.v=v; mm.type=type;
	  mm.shape=shape;
	  mm.nDaqLpgbt=nDaqLpgbt; mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
	  mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
	  mm.nTpgLpgbt=nTpgLpgbt; mm.idxTpgLpgbt1=idxTpgLpgbt1; mm.nTpgElinks1=nTpgElinks1;
	  mm.idxTpgLpgbt2=idxTpgLpgbt2; mm.nTpgElinks2= nTpgElinks2;
	  
	  map_vec.push_back(mm);
	}
      mapfile.close();
    }
  //std::cout<<"check2"<<std::endl;

  // logic to select signle or double lpgbts
  int nLpgbts=0;//,count=0;
  for(int imap=0; imap<(int(map_vec.size())); imap++)
    {
      lpGBT_Definition_Map map;
      int j = map_vec[imap].idxDaqLpgbt1;
      int k = map_vec[imap].idxDaqLpgbt2;
      nLpgbts=map_vec[imap].idxDaqLpgbt1;
      if ( map_vec[imap].nDaqLpgbt !=0) //ignoring shape c modules for now only
        {
	  if(k!=-1)
            {
	      map.layer = map_vec[imap].layer;
              map.u = map_vec[imap].u;
              map.v = map_vec[imap].v;
              map.type = map_vec[imap].type;
              map.shape = map_vec[imap].shape;
              map.nDaqLpgbt = 2;
              map.idxDaqLpgbt1 = map_vec[imap].idxDaqLpgbt1;
              map.idxDaqLpgbt2 = map_vec[imap].idxDaqLpgbt2;
              map.nDaqElinks1 = map_vec[imap].nDaqElinks1;
	      map.nDaqElinks2 = map_vec[imap].nDaqElinks2;
              map.idxLpgbt.first= j;
              map.idxLpgbt.second =k;
              lpgbt_vector.push_back(map);
	      //std::cout<<"check14"<<std::endl;

             for(int l = imap-2;l<imap ;l++) // to check if they are connected to any other modules
                {
	  	  if(l<0)
                    {continue;}
                  else  if (j==  map_vec[l].idxDaqLpgbt1 && map_vec[l].nDaqLpgbt== 1 && map_vec[l].idxDaqLpgbt2 ==-1)
                    {
	  	      map.layer = map_vec[l].layer;
                      map.u = map_vec[l].u;
                      map.v = map_vec[l].v;
                      map.type = map_vec[l].type;
                      map.shape = map_vec[l].shape;
                      map.nDaqLpgbt = 2;
                      map.idxDaqLpgbt1 = map_vec[l].idxDaqLpgbt1;
                      map.idxDaqLpgbt2 = map_vec[l].idxDaqLpgbt2;
                      map.nDaqElinks1 = map_vec[l].nDaqElinks1;
                      map.nDaqElinks2 =map_vec[l].nDaqElinks2;
                      map.idxLpgbt.first= j;
                      map.idxLpgbt.second =k;
                      lpgbt_vector.push_back(map);
	  	    }
                }
	      // std::cout<<"check1"<<std::endl;

              for(int l = imap+1;l<imap+3;l++) //// to check if they are connected to any other modules 
                {
	  	  //std::cout<<"check1"<<std::endl;

                  if (k== map_vec[l].idxDaqLpgbt1 &&  map_vec[l].nDaqLpgbt==1)
                    {
	  	      map.layer = map_vec[l].layer;
                      map.u = map_vec[l].u;
                      map.v = map_vec[l].v;
                      map.type = map_vec[l].type;
                      map.shape = map_vec[l].shape;
                      map.nDaqLpgbt = 2;
                      map.idxDaqLpgbt1 = map_vec[l].idxDaqLpgbt1;
                      map.idxDaqLpgbt2 = map_vec[l].idxDaqLpgbt2;
                      map.nDaqElinks1 = map_vec[l].nDaqElinks1;
                      map.nDaqElinks2 =map_vec[l].nDaqElinks2;
                      map.idxLpgbt.first= j;
                      map.idxLpgbt.second =k;
                      lpgbt_vector.push_back(map);
	  	    }
		}
	    }
	  else
            {
	      for(int l = imap-1;l<imap;l++)
                {
	      	  if(l<0)
	  	    {
	  	      if(j != map_vec[l+1].idxDaqLpgbt2 && map_vec[l+2].nDaqLpgbt!=2)
	  		{
			  map.layer = map_vec[imap].layer;
			  map.u = map_vec[imap].u;
			  map.v = map_vec[imap].v;
			  map.type = map_vec[imap].type;
			  map.shape = map_vec[imap].shape;
			  map.nDaqLpgbt = 1;
			  map.idxDaqLpgbt1 = map_vec[imap].idxDaqLpgbt1;
			  map.idxDaqLpgbt2 = map_vec[imap].idxDaqLpgbt2;
			  map.nDaqElinks1 = map_vec[imap].nDaqElinks1;
			  map.nDaqElinks2 =map_vec[imap].nDaqElinks2;
			  //map_vec[l].nDaqElinks2 =0;                                                                                                                                                         
			  map.idxLpgbt.first= map_vec[imap].idxDaqLpgbt1;
			  map.idxLpgbt.second =k;
			  lpgbt_vector.push_back(map);		
			  //continue;
	  		}
	  	    }
	  	  else if (imap==1)
	  	    {
	  	      if (j != map_vec[l].idxDaqLpgbt2 && map_vec[l+2].nDaqLpgbt!=2)
	  		{
	  		  map.layer = map_vec[imap].layer;
	  		  map.u = map_vec[imap].u;
	  		  map.v = map_vec[imap].v;
	  		  map.type = map_vec[imap].type;
	  		  map.shape = map_vec[imap].shape;
	  		  map.nDaqLpgbt = 1;
	  		  map.idxDaqLpgbt1 = map_vec[imap].idxDaqLpgbt1;
	  		  map.idxDaqLpgbt2 = map_vec[imap].idxDaqLpgbt2;
	  		  map.nDaqElinks1 = map_vec[imap].nDaqElinks1;
	  		  map.nDaqElinks2 =map_vec[imap].nDaqElinks2;
	  		  //map_vec[l].nDaqElinks2 =0;                                                                                                                                                   
                          
	  		  map.idxLpgbt.first= map_vec[imap].idxDaqLpgbt1;
	  		  map.idxLpgbt.second =k;
	  		  lpgbt_vector.push_back(map);
	  		}
	  	    }

	  	  else if( j != map_vec[l].idxDaqLpgbt2 && j != map_vec[l-1].idxDaqLpgbt2 && map_vec[l+2].nDaqLpgbt!=2)
                    {
	      	      map.layer = map_vec[imap].layer;
                      map.u = map_vec[imap].u;
                      map.v = map_vec[imap].v;
                      map.type = map_vec[imap].type;
                      map.shape = map_vec[imap].shape;
                      map.nDaqLpgbt = 1;
                      map.idxDaqLpgbt1 = map_vec[imap].idxDaqLpgbt1;
                      map.idxDaqLpgbt2 = map_vec[imap].idxDaqLpgbt2;
                      map.nDaqElinks1 = map_vec[imap].nDaqElinks1;
	      	      map.nDaqElinks2 =map_vec[imap].nDaqElinks2;
	      	      //map_vec[l].nDaqElinks2 =0;
                      map.idxLpgbt.first= map_vec[imap].idxDaqLpgbt1;
                      map.idxLpgbt.second =k;
                      lpgbt_vector.push_back(map);
	      	    }
		    
	     	}
	    }//else k==-1
    	}// lpgbt!=0
    }//map vector size
  //  //read the vector array and remove extra elements
  for( int ilpgbt =0; ilpgbt<(int(lpgbt_vector.size()));ilpgbt++)
    {
      // removing double lpGBTs are counted as single l
      if((lpgbt_vector[ilpgbt].idxLpgbt.second ==-1) && (lpgbt_vector[ilpgbt].idxLpgbt.first == lpgbt_vector[ilpgbt+1].idxLpgbt.first && lpgbt_vector[ilpgbt+1].idxLpgbt.second != -1))
        {
          lpgbt_vector.erase(lpgbt_vector.begin()+ilpgbt);

        }
      else
        continue;
   }

  std::cout<<lpgbt_vector.size()<<"\t"<<"lpgbts"<<std::endl;
  int econd=0, count=0;
    for(int i=0;i<nLpgbts+1;i++)
      {
	for( int ilpgbt =0; ilpgbt<(int(lpgbt_vector.size()));ilpgbt++) //(int(lpgbt_vector.size()));ilpgbt++)
	  {
	    if(i == lpgbt_vector[ilpgbt].idxLpgbt.first)                                             
	      {
		count++;
		//std::cout<<i<<std::endl;
		if (lpgbt_vector[ilpgbt].idxLpgbt.second !=-1)  //select only double lpGBTs
		  {		    		    
		    LpgbtDefinition dlpgbt;
		    dlpgbt.setLpgbtNumbersTo(lpgbt_vector[ilpgbt].idxLpgbt.first, lpgbt_vector[ilpgbt].idxLpgbt.second);
		    int c=0;
		    //	      FastControlStreamDefinition fcsd;
		    for(int l= 0; l<(int(lpgbt_vector.size()));l++)//ilpgbt+1;l<ilpgbt+4;l++)// check how many modules are connected to  this double lpgbt
		      {
			if (lpgbt_vector[l].idxLpgbt.first == lpgbt_vector[ilpgbt].idxLpgbt.first || lpgbt_vector[l].idxLpgbt.second == lpgbt_vector[ilpgbt].idxLpgbt.first)
			  {
			    
			    FastControlStreamDefinition fcsd; 
			    fcsd.setNumbersTo(dlpgbt.lpgbtNumber(),c);
			    EcondAsicDefinition ead;
			    int e;// e = total number of elinks
			    if(lpgbt_vector[l].nDaqElinks2 == -1)
			      {
				e =lpgbt_vector[l].nDaqElinks1;
			      }
			    else 
			      {
				e = lpgbt_vector[l].nDaqElinks1+lpgbt_vector[l].nDaqElinks2;
			      }
			    ead.setDefinitionTo(lpgbt_vector[l].layer,lpgbt_vector[l].u,lpgbt_vector[l].v,lpgbt_vector[l].type,lpgbt_vector[l].shape,e);
			    econd++;
			    fcsd.addModule(ead);
			    c++;
			    dlpgbt.addFastControlStream(fcsd);
			  }
			else continue;
		      }
		    lpgbtDefinitionVector_.push_back(dlpgbt);
		  }
		else
		  {
		    LpgbtDefinition slpgbt;
		    slpgbt.setLpgbtNumberTo(lpgbt_vector[ilpgbt].idxLpgbt.first);
	  
		    int c=0;
		    for(int l= 0; l<(int(lpgbt_vector.size()));l++)//ilpgbt+1;l<ilpgbt+4;l++)// check how many modules are connected to  this double lpgbt                                                 
                      {
                        if (lpgbt_vector[l].idxLpgbt.first == lpgbt_vector[ilpgbt].idxLpgbt.first)
                          {
			    
                            FastControlStreamDefinition fcsd;
                            fcsd.setNumbersTo(slpgbt.lpgbtNumber(),c);
                            EcondAsicDefinition ead;
                            int e;// e = total number of elinks                                                                                                                                            
                            if(lpgbt_vector[l].nDaqElinks2 == -1)
                              {
                                e =lpgbt_vector[l].nDaqElinks1;
                              }
                            else
                              {
                                e = lpgbt_vector[l].nDaqElinks1+lpgbt_vector[l].nDaqElinks2;
                              }
                            ead.setDefinitionTo(lpgbt_vector[l].layer,lpgbt_vector[l].u,lpgbt_vector[l].v,lpgbt_vector[l].type,lpgbt_vector[l].shape,e);
                            econd++;
                            fcsd.addModule(ead);
                            c++;
                            slpgbt.addFastControlStream(fcsd);
                          }
			else continue;
		      }
		    lpgbtDefinitionVector_.push_back(slpgbt);
		    }
		//std::cout<<"econd"<<"\t"<<econd<<std::endl;
		//	econd=0;
		break;
	      }

	    // else 
	    //   continue;
	  }
	//else continue;
      }

    std::cout<<lpgbtDefinitionVector_.size()<<"count"<<econd<<std::endl;
  }
  // std::cout<<count1<<std::endl;
}

void Definer::defineSlink(const std::vector<unsigned> &lpgbts, SlinkDefinition &) {

  if(int(lpgbts.size())<0) return;

  for(unsigned i(0);i<lpgbts.size();i++) {
    for(unsigned j(0);j<lpgbtDefinitionVector_.size();j++) {
      if(lpgbtDefinitionVector_[j].singleLpgbt()) {
	if(lpgbts[i]==lpgbtDefinitionVector_[j].lpgbtNumber()) {
	  //d.addLpgbt(lpgbtDefinitionVector_[j]);
	}
      } else {
	if(lpgbts[i]==lpgbtDefinitionVector_[j].lpgbtNumber(0) ||
	   lpgbts[i]==lpgbtDefinitionVector_[j].lpgbtNumber(1)) {
	  //d.addLpgbt(lpgbtDefinitionVector_[j]);
	}
      }
    }
  }
}
