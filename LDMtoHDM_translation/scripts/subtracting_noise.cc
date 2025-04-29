//in this script, files containing average hits for signal+noise and only noise are read and also only signal hits are calculated using probability methods...
#include<iostream>
#include<fstream>
#include<stdio.h>
#include <vector>
#include <string>
#include <map>
#include "TF2.h"
#include "TH2.h"
#include "TMath.h"
using namespace std;

void subtracting_noise(string signal_noise_file, string noise_file)
{
  char* hname = new char[2000]; //to read filenames provided by the command
  char* hname1 = new char[2000];
  sprintf(hname,"%s",signal_noise_file.c_str());
  sprintf(hname1,"%s",noise_file.c_str());
  std::fstream mapping_file; // file containig  average signal+noise hits
  mapping_file.open(hname,ios::in);
  std::fstream Ngun_file; //file that contains only noise hits for each of the modules
  Ngun_file.open(hname1,ios::in);
  std::ofstream file;
  file.open("average_signal_hits_EOl.txt",ios::out);
  struct modulemap {          //map to read and stored signal+noise file
    unsigned int  layer, u, v, type,typee;
    char shape;
    unsigned nDaqLpgbt;
    int   idxDaqLpgbt1, idxDaqLpgbt2;
    int nDaqElinks1, nDaqElinks2;
    //float R,CMS_occup,HD_occup,LD_occup;
    float average, error_mean,rms,rms_error;
  };
  std::vector <modulemap> map_vec;

  // struct output_map{  //map to stored signal only
  //   unsigned int  layer, u, v, type,typee;
  //   char shape;
  //   float average,error_mean,rms,rms_error;
  // };
  // std::vector<output_map> out_vec;


  struct noise_map {          //map to read and stored noise file
    unsigned int  layer, u, v, type,typee;
    char shape;
    //  unsigned nDaqLpgbt;
    //int   idxDaqLpgbt1, idxDaqLpgbt2;
    //int nDaqElinks1, nDaqElinks2;
    float R,average,error_mean,rms,rms_error;
  };
  std::vector <noise_map> noise_vec;
  if(!mapping_file.is_open())  //reading the mapping file
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      map_vec.clear();
      while (true) {
	unsigned int  layer, u, v, type,typee;
	char shape;
	int  idxDaqLpgbt1, idxDaqLpgbt2;
	int nDaqElinks1, nDaqElinks2;
	unsigned nDaqLpgbt;
	float average, error_mean,rms,rms_error;

	//float R,average,error_mean,rms;

	mapping_file >> layer >> u >> v >> type >>typee >> shape
	             >> nDaqLpgbt
		     >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2
		     >>average >>error_mean>>rms>>rms_error;

	if (mapping_file.eof()) break;
	modulemap mm;
	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee = typee;
	mm.shape = shape;
	mm.nDaqLpgbt=nDaqLpgbt; mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
	mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
	mm.average=average;
	//mm.average=average;
	//mm.error_mean=error_mean; mm.rms=rms;
	mm.error_mean = error_mean;
	mm.rms=rms;
	mm.rms_error=rms_error;

	map_vec.push_back(mm);
      }
      mapping_file.close();

    }

  //reading only noise file

  if(!Ngun_file.is_open())  //reading the mapping file
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      noise_vec.clear();
      while (true) {
	unsigned int  layer, u, v, type,typee;
	char shape;
	float R,average,error_mean,rms,rms_error;

	Ngun_file >> layer >> u >> v >> type >>typee >> shape
	  // >> nDaqLpgbt
	  //>> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2
	  //>>average_hits >> mean_error >>rms >>rms_error;
		  >>average>>error_mean>>rms >>rms_error;


	if (Ngun_file.eof()) break;
	noise_map mm;
	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee = typee;
	mm.shape = shape;
	//mm.nDaqLpgbt=nDaqLpgbt; mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
	//mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
	mm.R=R;
	mm.average=average;
	mm.error_mean=error_mean; mm.rms=rms;

	// mm.average_hits= average_hits;
	// mm.mean_error = mean_error;
	// mm.rms=rms;
	mm.rms_error=rms_error;

	noise_vec.push_back(mm);
	//      cout<<map_vec.size()<<endl;
      }
      Ngun_file.close();
    }
  float average_sN,average_N,average_s,prob_n,Nc_ldm=192,Nc_hdm=432,error_s,error_n,error_ns;
  char F;
  F='F';
  float var1,var2,count=0;
  for(int iModule =0;iModule<(int(map_vec.size()));iModule++)
    {
      for(int iNoise =0;iNoise<(int(noise_vec.size()));iNoise++)
	{         
	  if(map_vec[iModule].layer==noise_vec[iNoise].layer && map_vec[iModule].u==noise_vec[iNoise].u && map_vec[iModule].v==noise_vec[iNoise].v) //&& map_vec[iModule].shape==noise_vec[iNoise].shape == F)
	    {
	      
	      //	      if(map_vec[iModule].shape==F)
	      //{
		  
		  if(map_vec[iModule].typee ==0)
		    { count++;
		      average_N = noise_vec[iNoise].average; //noise hits in that module
		      prob_n= average_N/Nc_hdm;//prob of a noise hit
		      average_sN = map_vec[iModule].average; //signal+noise hits
		      average_s = (average_sN - average_N)/(1-prob_n); //average number of signal hits 
		      error_ns=map_vec[iModule].error_mean;//error in signal+noise
		      error_n= noise_vec[iModule].error_mean;//error in noise
		      
		      var1=error_ns/(average_sN-Nc_hdm); 
		      var2=error_n/(Nc_hdm-average_N);
		  
		  
		      //error in only signal hits
		      error_s= average_s*(sqrt((var1*var1)+(var2*var2)));
		      
		      file << map_vec[iModule].layer <<"\t"<< map_vec[iModule].u <<"\t"<< map_vec[iModule].v <<"\t"<<map_vec[iModule].type<<"\t"<<map_vec[iModule].typee<<"\t"<<map_vec[iModule].shape<<"\t\t"<<average_s<<"\t\t"<<error_s<<"\n";
		    }
		  else if (map_vec[iModule].typee ==1)
		    {
		      count++;
		      average_N = noise_vec[iNoise].average; //noise hits in that module
		      prob_n= average_N/Nc_ldm;//prob of a noise hit
		      average_sN = map_vec[iModule].average; //signal+noise hits
		      average_s = (average_sN - average_N)/(1-prob_n); //average number of signal hits
		      error_ns=map_vec[iModule].error_mean;//error in signal+noise
		      error_n= noise_vec[iModule].error_mean;//error in noise
		      
		      var1=error_ns/(average_sN-Nc_ldm);
		      var2=error_n/(Nc_ldm-average_N);
		      

		      //error in only signal hits
		      error_s= average_s*(sqrt((var1*var1)+(var2*var2)));
		      
		      file << map_vec[iModule].layer <<"\t"<< map_vec[iModule].u <<"\t"<< map_vec[iModule].v <<"\t"<<map_vec[iModule].type<<"\t"<<map_vec[iModule].typee<<"\t"<<map_vec[iModule].shape<<"\t\t"<<average_s<<"\t\t"<<error_s<<"\n";
		    }
		  //	}
		  //else
		  //c//ontinue;
	    } //luv type selection
	}//for loop of noise file
    }//for signal +noise loop
  cout<<count<<endl;
  file.close();
}
  
