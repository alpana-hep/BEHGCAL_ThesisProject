////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                 Instructions to run the script                 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// open .csv file & remove first line with strings (this line shows which column corresponds to which and c++ code won't be able to read ) & also replace ' ,' with ' ' in the csv file before using this code. /////
//  root -l 'readBEMappingFile.C("FPGA16_LINK_CONNECTIONS.csv")' 
// some hardcoded things to take care : nSlinks at L-76 && nFPGA -at L-175(can be fixed in future)

#include <vector>
#include<fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include "interface/json.hpp"
using Json = nlohmann::json;

using namespace std;
void readBEMappingFile(string file)
{

  char* fname = new char[200];
  sprintf(fname,"%s",file.c_str());
  std::ifstream mapfile;
  mapfile.open(fname, std::ios::in);
  struct SlinkTolpGBT_Map {
   unsigned int  index,SlinkNumber,lpgbtNumber, fpgaNumber, sectorNumber, nSlinklpgbt,nfpgalpgbt; float slinkrate,lpgbtrate, fpgarate  ;

    //    bool dlpgbt_flag;
    std::vector<int> s_idxLpgbt;
    std::vector<float>sLpgbtRates;
    std::vector<std::string>sLpgbtLabel;
    std::vector<int> d_idxLpgbt;
    std::vector<float>dLpgbtRates;
    std::vector<std::string>dLpgbtLabel;
    std::vector<std::tuple<int, float, std::string> >  dlpgbtinfo;
    std::vector<std::tuple<int, float, std::string> >  slpgbtinfo; 
    std::vector<std::tuple<int, int, float> > slinkinfo; //slink number, fpga number, slink rate
  };

  std::vector<SlinkTolpGBT_Map> Slink_vector;
  struct Input_Map {
    int  index,SlinkNumber,lpgbtNumber; float fpgaNumber, sectorNumber, nSlinklpgbt,nfpgalpgbt; float slinkrate,lpgbtrate, fpgarate  ;
    std::string lpgbtLabel;
  };
  std::vector<Input_Map> input_vector;
  //counter for Slinks
  int   nSlinks=0, nlpGBTs=0,nFPGAs=0;
  if (!mapfile.is_open())
    {
      std::cout<<"no such file"<<std::endl;
    }
  else
    {
      input_vector.clear();
      int index, SlinkNumber, lpgbtNumber; float fpgaNumber, sectorNumber, nSlinklpgbt,nfpgalpgbt, eventBuffer; float SlinkRate, lpgbtRate,  fpgarate ;
      std::string label;
      while( mapfile >> index >>SlinkNumber>>nSlinklpgbt>>lpgbtNumber>>lpgbtRate>>label >>SlinkRate >>fpgaNumber >>nfpgalpgbt >>fpgarate >>eventBuffer) {
	//mapfile>>index>>SlinkNumber>>lpgbtNumber>>label>>nSlinklpgbt>>SlinkRate>>lpgbtRate >>fpgaNumber>>eventBuffer >>nfpgalpgbt >>fpgarate;
        Input_Map mm;
        mm.index = index;
        mm.SlinkNumber=SlinkNumber;
	//cout<<SlinkNumber<<endl;
        mm.lpgbtNumber=lpgbtNumber;
        mm.slinkrate=SlinkRate;
        mm.lpgbtrate=lpgbtRate;
        mm.lpgbtLabel=label;
	mm.nSlinklpgbt=int(nSlinklpgbt);
	mm.fpgaNumber=int(fpgaNumber);
	mm.sectorNumber=0;
	mm.nfpgalpgbt=int(nfpgalpgbt);
	mm.fpgarate=fpgarate;
	nlpGBTs++;
        input_vector.push_back(mm);
      }
    }

  cout<<input_vector.size()<<endl;
  int nslinks=193; //313;//225; //number of slinks (+1 due to loop)                                                                                                                 
  int nLpgbt=0;//, ndlpgbt=0;                                                                                                                                          
 
  int count=0;

  for (int is=1;is<nslinks;is++)
    {
      SlinkTolpGBT_Map mm;
      mm.SlinkNumber= is;
      nLpgbt=0;
      count=0;
      unsigned int  SlinkNumber, lpgbtNumber,fpgaNumber, sectorNumber, nSlinklpgbt,nfpgalpgbt; 
      float slinkrate, lpgbtrate,  fpgarate ;      
      mm.dlpgbtinfo.clear();
      mm.slpgbtinfo.clear();
      mm.s_idxLpgbt.clear();
      mm.sLpgbtRates.clear();
      mm.sLpgbtLabel.clear();
      mm.d_idxLpgbt.clear();
      mm.dLpgbtRates.clear();
      mm.dLpgbtLabel.clear();
      for(int islink=0; islink<(int(input_vector.size()));islink++)
        {
	  
	  if(input_vector[islink].SlinkNumber == is)
            {
	    
	      fpgaNumber= input_vector[islink].fpgaNumber;
	      sectorNumber= input_vector[islink].sectorNumber; 
	      nLpgbt+=1;
	      slinkrate=input_vector[islink].slinkrate;
	      nfpgalpgbt=input_vector[islink].nfpgalpgbt;
	      fpgarate=input_vector[islink].fpgarate;
	      nSlinklpgbt=input_vector[islink].nSlinklpgbt;
	      
	      if(input_vector[islink].lpgbtLabel=="double")
                {
                  count+=2;
                  auto t = std::make_tuple(input_vector[islink].lpgbtNumber, input_vector[islink].lpgbtrate, input_vector[islink].lpgbtLabel);
                  mm.dlpgbtinfo.push_back(t);
                  mm.dLpgbtLabel.push_back(input_vector[islink].lpgbtLabel);
                  mm.d_idxLpgbt.push_back(input_vector[islink].lpgbtNumber);
                  mm.dLpgbtRates.push_back(input_vector[islink].lpgbtrate);

                }
              else
                {
                  count+=1;
                  auto t = std::make_tuple(input_vector[islink].lpgbtNumber, input_vector[islink].lpgbtrate, input_vector[islink].lpgbtLabel);
                  mm.slpgbtinfo.push_back(t);
                  mm.sLpgbtLabel.push_back(input_vector[islink].lpgbtLabel);
                  mm.s_idxLpgbt.push_back(input_vector[islink].lpgbtNumber);
                  mm.sLpgbtRates.push_back(input_vector[islink].lpgbtrate);
		  // if(is==275)
		  //   {
		  //     cout<<input_vector[islink].lpgbtNumber <<"\t"<<input_vector[islink].lpgbtrate<<endl;
		  //   }
                }
	    }
	      
	  //else continue;
	      
	}    
	
      mm.fpgaNumber= fpgaNumber;
      mm.sectorNumber= sectorNumber;
      nLpgbt+=1;
      mm.slinkrate=slinkrate;
      mm.nfpgalpgbt=nfpgalpgbt;
      mm.fpgarate=fpgarate;
      mm.nSlinklpgbt=nSlinklpgbt;
      Slink_vector.push_back(mm);
      //cout<<mm.nSlinklpgbt<<"\t"<<( mm.dlpgbtinfo.size()+ mm.slpgbtinfo.size())<<"\t"<<count<<endl;
    }
  cout<<Slink_vector.size()<<"\t"<<count<<endl;
  //making json file
  std::ofstream o("BEDAQ_SlinksToLpGBT_16FPGA_Mapping_19032022.json");
  struct jsonfile {
    //std::string slinkLabel;
    //std::vector<string> lpgbtLabel;                                                                                                                                  
    // unsigned int slinkIndex, nlpgbtsPairs,nlpgbts,fpgaNumber, sectorNumber,nfpgalpgbt;
    // float slinkrate, fpgarate;
    std::vector<std::tuple<int, int,float, std::string> >  lpgbtinfo;
    std::vector<std::pair<float,float>> lpgbtPairrates;
    std::vector<std::pair<int, int>>lpgbtPairIndex;
    std::vector<std::pair<std::string, std::string>>lpgbtPairlabel;
    //<index,lpgbtnu, rates,
    std::vector<std::tuple<int,int,float,std::pair<std::string,std::vector<std::pair<int, int>>>, std::pair<std::string,std::vector<std::pair<float,float>>>>> SlinkInfo;
    //    std::vector<std::tuple<int,int, float, std::string>> lpgbtinfo;                                                                                              
  };
  std::vector<jsonfile> BE_Map;
  BE_Map.clear();
  //  Json j_map(jsonfile);
  int scounts=0, lpgbtcounts=0;
  int cloop=0;
  int SlinkCount=0;
  int fpgaCount=0;
  Json arr = Json::array();
  int slinklpgbtss=0;
  int nFpga =16;
  for(int iF=0; iF<nFpga; iF++)
    {
      SlinkCount=0;
      Json j;
      //      j["A Sec Index"]=0;//Slink_vector[i].sectorNumber;
      j["FPGA"]["INDEX"]= iF;//Slink_vector[is].fpgaNumber-1;
      // j["FPGA"]["fpga Rate"]=Slink_vector[is].fpgarate;
      // j["FPGA"]["Total lpGBTs"]=Slink_vector[is].nfpgalpgbt;
      jsonfile jj;

      for(int is=0;is<(int(Slink_vector.size()));is++)
	{
      if( Slink_vector[is].fpgaNumber == iF+1)
	{
	  j["FPGA"]["fpga Rate"]=Slink_vector[is].fpgarate;
	  j["FPGA"]["Total lpGBTs"]=Slink_vector[is].nfpgalpgbt;
	  //      Json j;
      //      Json j1;
	  //jsonfile jj;
      // jj.slinkIndex=Slink_vector[is].SlinkNumber;
      // jj.slinkrate=Slink_vector[is].slinkrate ;
      //fill the json file
      // j["A Sec Index"]=Slink_vector[is].sectorNumber;
      // j["FPGA"]["INDEX"]= Slink_vector[is].fpgaNumber-1;
      // j["Total FPGA"]= 28;
      // j["FPGA"]["fpga Rate"]=Slink_vector[is].fpgarate;
      // j["FPGA"]["Total lpGBTs"]=Slink_vector[is].nfpgalpgbt;
      //SlinkCount++;
	  //      scounts=0;
      // if(is%12==0)
      //   {
      //     SlinkCount=0;
      //   }
      // else if (is%12!=0)
      //   {
      //     SlinkCount++;
      //   }
      
      // j["FPGA"]["slink"]["Index"]=SlinkCount;
      // j["FPGA"]["slink"]["Dual Index"]=Slink_vector[is].SlinkNumber;
      // //      j["FPGA"]["Slink"]["Slink label"]= sLd[is].label();
      // j["FPGA"]["slink"]["average rate"]=Slink_vector[is].slinkrate;
      // j["FPGA"]["slink"]["Total Slink lpgbt"]=Slink_vector[is].nSlinklpgbt;
      //      SlinkCount++;
      std::vector<std::pair<float,float>>lpgbtPairrates;
      std::vector<std::pair<int, int>> lpgbtPairIdx;
      lpgbtPairrates.clear();
      lpgbtPairIdx.clear();
      auto dlpgbtinfo = Slink_vector[is].dlpgbtinfo;
      for (int it =0; it<(int(dlpgbtinfo.size()));it++)
        {
          scounts+=2;
          lpgbtcounts+=1;
	  auto t =dlpgbtinfo.at(it);
	  std::pair<float,float>LpgbtRates;
          LpgbtRates.first=std::get<1>(t);
          LpgbtRates.second=-1;//std::get<1>(t);
	  jj.lpgbtPairrates.push_back(LpgbtRates);
	  lpgbtPairrates.push_back(LpgbtRates);
	  std::pair<int, int>idx;
	  idx.first=std::get<0>(t);
	  idx.second=-1;//std::get<0>(t);
	  jj.lpgbtPairIndex.push_back(idx);
	  lpgbtPairIdx.push_back(idx);
	}
      auto slpgbtinfo = Slink_vector[is].slpgbtinfo;
      auto slpgbtrates = Slink_vector[is].sLpgbtRates;
      if((int(slpgbtinfo.size()))%2==0)
        {
	  //cout<<slpgbtinfo.size()<<" nefore"<<endl;
	  for (int it=0; it<(int(slpgbtinfo.size()));)
	    {

              auto t1 =slpgbtinfo.at(it);
              auto max_ = *std::max_element(slpgbtinfo.begin(), slpgbtinfo.end(),
					    [](const std::tuple<int, float, std::string> &x,
					       const std::tuple<int, float, std::string> &y) {
					      return std::get<1>(x) < std::get<1>(y);
					    });

	      auto min_ = *std::min_element(slpgbtinfo.begin(), slpgbtinfo.end(),
					    [](const std::tuple<int, float, std::string> &x,
					       const std::tuple<int, float, std::string> &y) {
					      return std::get<1>(x) < std::get<1>(y);
					    });
	      
	      auto max_value=std::get<1>(max_);
	      auto min_value=std::get<1>(min_);
	      //cout<<max_value<<"\t"<<min_value<<"\t"<<Slink_vector[is].SlinkNumber<<endl;
	      std::pair<float,float>LpgbtRates;
	      std::pair<int, int>idx;

	      if (max_value == std::get<1>(t1))
		{
		  scounts+=2;
		  lpgbtcounts+=2;
		  LpgbtRates.first=std::get<1>(t1);
		  idx.first=std::get<0>(t1);
		  for(int it1 =0; it1<(int(slpgbtinfo.size()));)
		    {
		      auto t2=slpgbtinfo.at(it1);
                      if(min_value == std::get<1>(t2) && std::get<1>(t2)!= max_value)
                        {
			  LpgbtRates.second=std::get<1>(t2);
			  idx.second=std::get<0>(t2);
			  slpgbtinfo.erase(it1+slpgbtinfo.begin());
			  // it1=0;
			  break;
                        }
                      else
                        {
                          it1++;
                        }
                    }
		  jj.lpgbtPairIndex.push_back(idx);
		  jj.lpgbtPairrates.push_back(LpgbtRates);
		  slpgbtinfo.erase(it+slpgbtinfo.begin());
		  lpgbtPairIdx.push_back(idx);
		  lpgbtPairrates.push_back(LpgbtRates);

		  //cout<<slpgbtinfo.size()<<" after"<<endl;
                  it=0;

		}
	      else if(min_value==std::get<1>(t1))
                {
                  scounts+=2;
                  lpgbtcounts+=2;
                  LpgbtRates.first=std::get<1>(t1);
                  idx.first=std::get<0>(t1);
                  for(int it1 =0; it1<(int(slpgbtinfo.size()));)
                    {
                      auto t2=slpgbtinfo.at(it1);
                      if(max_value == std::get<1>(t2) && std::get<1>(t2)!= min_value)
                        {
                          LpgbtRates.second=std::get<1>(t2);
                          idx.second=std::get<0>(t2);
                          slpgbtinfo.erase(it1+slpgbtinfo.begin());
                          // it1=0;                                                                                                                                      
                          break;
                        }
                      else
                        {
                          it1++;
                        }
                    }
                  jj.lpgbtPairIndex.push_back(idx);
                  jj.lpgbtPairrates.push_back(LpgbtRates);
                  slpgbtinfo.erase(it+slpgbtinfo.begin());
		  lpgbtPairIdx.push_back(idx);
                  lpgbtPairrates.push_back(LpgbtRates);

                  //cout<<slpgbtinfo.size()<<" after"<<endl;
                  it=0;

                }

	      else{it++; continue;}
	    }
	}
      else
        {
          int cin= 0;
          float max_rates= *std::max_element(Slink_vector[is].sLpgbtRates.begin(),Slink_vector[is].sLpgbtRates.end());
          for (int it =cin; it<(int(slpgbtinfo.size())) ;it++)
            {
              auto t1 =slpgbtinfo.at(it);

              if(max_rates==std::get<1>(t1))
                {
		  scounts+=1;
                  lpgbtcounts+=1;
		  std::pair<float,float>LpgbtRates;
		  std::pair<int, int>idx;

                  LpgbtRates.first=std::get<1>(t1);
		  LpgbtRates.second=-1;
		  idx.first=std::get<0>(t1);
		  idx.second=-1;
		  jj.lpgbtPairIndex.push_back(idx);
                  jj.lpgbtPairrates.push_back(LpgbtRates);
		  lpgbtPairIdx.push_back(idx);
                  lpgbtPairrates.push_back(LpgbtRates);

		  slpgbtinfo.erase(it+slpgbtinfo.begin());
                }
            }
	  for (int it=0; it<(int(slpgbtinfo.size()));)
            {

              auto t1 =slpgbtinfo.at(it);
              auto max_ = *std::max_element(slpgbtinfo.begin(), slpgbtinfo.end(),
                                            [](const std::tuple<int, float, std::string> &x,
                                               const std::tuple<int, float, std::string> &y) {
                                              return std::get<1>(x) < std::get<1>(y);
                                            });
              auto min_ = *std::min_element(slpgbtinfo.begin(), slpgbtinfo.end(),
                                            [](const std::tuple<int, float, std::string> &x,
                                               const std::tuple<int, float, std::string> &y) {
                                              return std::get<1>(x) < std::get<1>(y);
                                            });

              auto max_value=std::get<1>(max_);
              auto min_value=std::get<1>(min_);
	      std::pair<float,float>LpgbtRates;
	      std::pair<int, int>idx;
	      if (max_value == std::get<1>(t1))
                {
                  scounts+=2;
                  lpgbtcounts+=2;
                  LpgbtRates.first=std::get<1>(t1);
		  idx.first=std::get<0>(t1);
		  for(int it1 =0; it1<(int(slpgbtinfo.size()));)
		    {
                      auto t2=slpgbtinfo.at(it1);
                      if(min_value == std::get<1>(t2) && std::get<1>(t2)!= max_value)
                        {
                          LpgbtRates.second=std::get<1>(t2);
                          idx.second=std::get<0>(t2);
                          slpgbtinfo.erase(it1+slpgbtinfo.begin());
			  break;
                        }
                      else
                        {
                          it1++;
			}
                    }
                  jj.lpgbtPairIndex.push_back(idx);
                  jj.lpgbtPairrates.push_back(LpgbtRates);
		  lpgbtPairIdx.push_back(idx);
                  lpgbtPairrates.push_back(LpgbtRates);

                  slpgbtinfo.erase(it+slpgbtinfo.begin());
                  it=0;

		}
	      else if(min_value==std::get<1>(t1))
                {
                  scounts+=2;
                  lpgbtcounts+=2;
                  LpgbtRates.first=std::get<1>(t1);
                  idx.first=std::get<0>(t1);
                  for(int it1 =0; it1<(int(slpgbtinfo.size()));)
                    {
                      auto t2=slpgbtinfo.at(it1);
                      if(max_value == std::get<1>(t2) && std::get<1>(t2)!= min_value)
                        {
                          LpgbtRates.second=std::get<1>(t2);
                          idx.second=std::get<0>(t2);
                          slpgbtinfo.erase(it1+slpgbtinfo.begin());
                          // it1=0;                                                                                                                                     \
                                                                                                                                                                         
                          break;
                        }
                      else
                        {
                          it1++;
                        }
                    }
                  jj.lpgbtPairIndex.push_back(idx);
                  jj.lpgbtPairrates.push_back(LpgbtRates);
		  lpgbtPairIdx.push_back(idx);
                  lpgbtPairrates.push_back(LpgbtRates);

                  slpgbtinfo.erase(it+slpgbtinfo.begin());
                  //cout<<slpgbtinfo.size()<<" after"<<endl;                                                                                                             
                  it=0;

                }

	      else{it++; continue;}
	    }
	  //#slinklpgbtss+=Slink_vector[is].nSlinklpgbt;
	}
    
      if(scounts!=Slink_vector[is].nSlinklpgbt)
        {
 
          cloop++;
 
        }
      std::pair<std::string,std::vector<std::pair<float,float>>>LpgbtPairRates_v1;
      std::pair<std::string, std::vector<std::pair<int, int>>>LpgbtPairIndex_v1;
      LpgbtPairRates_v1.first="Lpgbt Pair rates";
      LpgbtPairRates_v1.second=lpgbtPairrates;
      LpgbtPairIndex_v1.first= "Lpgbt Index";
      LpgbtPairIndex_v1.second=lpgbtPairIdx;
      
      auto t = std::make_tuple(SlinkCount,Slink_vector[is].nSlinklpgbt, Slink_vector[is].slinkrate, LpgbtPairIndex_v1,LpgbtPairRates_v1);//lpgbtPairIdx, lpgbtPairrates);
      jj.SlinkInfo.push_back(t);
      SlinkCount++;
      // j["FPGA"]["slink"]["lpgbt pairs"]["Lpgbt Index"]=jj.lpgbtPairIndex;
      // j["FPGA"]["slink"]["lpgbt pairs"]["Lpgbt Rates"]=jj.lpgbtPairrates;
      //      Json j3 = Json::parse(R"({"happy": true, "pi": 3.141})");
      //Json arr = Json::array();
      // arr.push_back(j);
      // BE_Map.push_back(jj);
      // Json j3;
      // j3["Sector"] = BE_Map[is];
      //      Json j_map(jj);
      //      //std::cout << arr.dump(4) << std::endl;
      // o <<arr.dump(4) <<"\n";
      //      o<< std::setw(4) << j<<","<<"\n" ;
	}
      else continue;
      slinklpgbtss+=Slink_vector[is].nSlinklpgbt;
	}

      // std::pair<std::string,float>LpgbtPairRates;
      // std::pair<std::string, int>LpgbtPairIndex;

      j["FPGA"]["slink"]= jj.SlinkInfo;
      arr.push_back(j);
    }
  cout<<slinklpgbtss<<endl;
  o<<arr.dump(4) <<"\n";

  // Json j;
  // j["Dash"]=BE_Map[0];
  //  o<< std::setw(4) << j_map<<"\n" ;  
  std::cout<<"lpgbt"<<lpgbtcounts<<"cloop"<<cloop<<std::endl;
}
