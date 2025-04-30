#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include "json.hpp"
using json = nlohmann::json;

void readinJsonFile(string filename, string outname)
{
  std::ifstream mapfile(filename);
  json j = json::parse(mapfile);
  assert(j.is_array());
  std::cout<<j.size()<<std::endl;
  char* fname = new char[200];
  sprintf(fname,"%s",outname.c_str());//luv_ncells_map_cmssw_May2022_V10.txt");
  std::ofstream file;
  file.open(fname, std::ios::out);

  //std::ifstream ncellfile(
  for (unsigned iM = 0; iM < j.size(); iM++) 
    {
      //cout<<j[iM]["n_cells"]<<"\t"<<j[iM]["cmssw_n_cells"]<<"\t"<<j[iM]["key"][0]<<"\t"<<j[iM]["key"][1]<<"\t"<<j[iM]["key"][2]<<endl;
      file <<j[iM]["key"][0]<<" "<<j[iM]["key"][1]<<" "<<j[iM]["key"][2]<<" "<<j[iM]["type"]<<" "<<j[iM]["cmssw_type"]<<" "<<j[iM]["n_cells"]<<" "<<j[iM]["cmssw_n_cells"]<<"\n";
      if(j[iM]["key"][0] == 34 && j[iM]["key"][1]==2 && j[iM]["key"][2] ==12)
	cout<<j[iM]["key"][0]<<" "<<j[iM]["key"][1]<<" "<<j[iM]["key"][2]<<" "<<j[iM]["n_cells"]<<" "<<j[iM]["cmssw_n_cells"]<<endl;
    }    
  file.close();
}
