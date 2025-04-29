#include<iostream>
#include<fstream>
#include<stdio.h>
#include <vector>
#include <string>
#include <map>

using  namespace std;

float g2(float x, float p0, float p1)//,float p2) //define the expo function which will calculate average occupancies
{

  float function = exp(p0+p1*x);
  return function;
}

void correlation_ldm_hdm(string param_file, string geo_file, string data_file)
{
  //define a 2d histogram representing correlation between HDM and LDM occupancies at given R
  TH2F* ldm_hdm = new TH2F("ldm_hdm","correlation between HDM and LDM numbers",100,0,200,100,0,450);
  ldm_hdm->GetXaxis()->SetTitle("LDM hit number");
  ldm_hdm->GetYaxis()->SetTitle("HDM hit number");
  

  // TH3F* ldm_hdm_r = new TH3F("ldm_hdm_r","correlation between HDM and LDM numbers with 'r'",100,0,300,100,0,300,100,0,2000);
  // ldm_hdm_r->GetXaxis()->SetTitle("hits for ldm");
  // ldm_hdm_r->GetYaxis()->SetTitle("hits for hdm");
  // ldm_hdm_r->GetZaxis()->SetTitle("distance from beamline");
  
  
  char* hname = new char[2000];
  char* hname1 = new char[2000];
  char* hname2 = new char[2000];
  TFile* f = new TFile("ldm_hdm_correlation_hits_wE.root","Recreate");
  //read the input function param file,geometry file,original data file  
  sprintf(hname,"%s",param_file.c_str());
  sprintf(hname1,"%s",geo_file.c_str());
  sprintf(hname2,"%s",data_file.c_str());
  
  
  std::fstream params_file;
  params_file.open(hname,ios::in);
  std::fstream Geometry_file;
  Geometry_file.open(hname1,ios::in);
  std::fstream mapping_file;
  mapping_file.open(hname2,ios::in);
  std::ofstream file;
  file.open("ldm_hdm_corrections_v0.txt",ios::out);
  //output txt file which will contain final result
  struct modulemap { //map for data_nt_elink.txt
    unsigned int  layer, u, v, type,typee;
    char shape;
    unsigned nDaqLpgbt;
    int   idxDaqLpgbt1, idxDaqLpgbt2;
    int nDaqElinks1, nDaqElinks2;
    float average_hits,mean_error,rms,rms_error;
  };
  std::vector <modulemap> map_vec;

  struct geometry_map { //to read geometry file
    unsigned  int  layer, u, v, type;
    unsigned int  size;
    float x,y;
    string shape;

  };

 std::vector <geometry_map> geo_map_vec;

 struct param_map { //to read parameters file fo function
   int i;
   float lp0,lp1;
   float hp0,hp1;
 };

 std::vector <param_map> para_map_vec;
 //mapping file(updated version)
 if(!mapping_file.is_open())
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
       float average_hits,mean_error,rms,rms_error;
       mapping_file >> layer >> u >> v >> type >>typee >> shape
	 //		    >> nDaqLpgbt
	 //	    >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2
		    >>average_hits >>mean_error;//>>rms>>rms_error;

       if (mapping_file.eof()) break;
       modulemap mm;
       mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee = typee;
       mm.shape=shape;
       //       mm.nDaqLpgbt=nDaqLpgbt; mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
       //mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
       mm.average_hits = average_hits;
       mm.mean_error=mean_error;
       // mm.rms=rms;
       //mm.rms_error=rms_error;
       map_vec.push_back(mm);
       //      cout<<map_vec.size()<<endl;
     }
     mapping_file.close();
   }
 //geometry file reading starts here
 if(!Geometry_file.is_open())
   {
     std::cout << " file not opened" << std::endl;
   }
 else
   {
     geo_map_vec.clear();
     cout<<geo_map_vec.size()<<endl;
     while (Geometry_file)
       {
	 unsigned  int  layer, u, v, type;
	 unsigned int  size;
	 float x,y;
	 string shape;
	 //   //      cout<<"........"<<endl;
	 Geometry_file >> layer >> shape >> size >> x >> y >> type >> u >> v;
	 //cout<<layer<<endl;
	 if (Geometry_file.eof()) break;
	 geometry_map mmm;
	 mmm.layer=layer; mmm.shape = shape; mmm.size = size; mmm.x = x; mmm.y =y;  mmm.type=type;
	 mmm.u=u; mmm.v=v;
	 geo_map_vec.push_back(mmm);
       }
     Geometry_file.close();
   }
 
 //reading of parameter file
 if(!params_file.is_open())
   {
     std::cout << " file not opened" << std::endl;
   }
 else
   {
     para_map_vec.clear();
     //cout<<para_map_vec.size()<<endl;
     while (params_file)
       {
	 int i;
	 float lp0,lp1,hp0,hp1;//,lp2,hp2;
	 //   //      cout<<"........"<<endl;
	 params_file >> i >> hp0 >>hp1 >>lp0 >> lp1;
	 //cout<<layer<<endl;
	 if (params_file.eof()) break;
	 param_map mmm;
	 mmm.i=i; mmm.hp0 = hp0; mmm.hp1 = hp1; mmm.lp0 =lp0;  mmm.lp1=lp1;
	 //	 mmm.hp2=hp2;mmm.lp2=lp2;
	 para_map_vec.push_back(mmm);
       }
     params_file.close();
   }
 

 //main script
 // define the arrays to store fH anf fL
 float Fl[100]={};
 float Fh[100]={};
 float r,X,Y,fH,fL;
 for(int iModule =0;iModule<(int(map_vec.size()));iModule++)
   {
     for(int iGeo =0;iGeo<(int(geo_map_vec.size()));iGeo++)
       {
	 if(geo_map_vec[iGeo].layer == map_vec[iModule].layer && geo_map_vec[iGeo].u == map_vec[iModule].u && geo_map_vec[iGeo].v == map_vec[iModule].v)
	   {
	     X = geo_map_vec[iGeo].x;
	     Y = geo_map_vec[iGeo].y;
	     r = sqrt(X*X+Y*Y);
	     if(map_vec[iModule].type != map_vec[iModule].typee)
	       {
		 for(int j= 0;j<(int(para_map_vec.size()));j++)
		   {
		     if(para_map_vec[j].i == map_vec[iModule].layer)
		       {
			 fH = g2(r,para_map_vec[j].hp0,para_map_vec[j].hp1);//,para_map_vec[j].hp2);
			 fL = g2(r,para_map_vec[j].lp0,para_map_vec[j].lp1);//,para_map_vec[j].lp2);
			 //Fh[j]=fH;
			 //Fl[j]=fL;
			 
			 ldm_hdm->Fill(fL,fH);
			 // ldm_hdm_r->Fill(fH,fL,r);
			 cout<<fH<<"\t"<<fL<<"\t"<<map_vec[iModule].average_hits<<"\t"<<fL-map_vec[iModule].average_hits<<"\t"<<r<<endl;//map_vec[iModule].type<<"\t"<<map_vec[iModule].typee<<endl;
			 //			 cout<<map_vec[iModule].shape<<endl;
			 file << map_vec[iModule].layer <<"\t"<< map_vec[iModule].u <<"\t"<< map_vec[iModule].v <<"\t"<<map_vec[iModule].type <<"\t"<<map_vec[iModule].typee<<"\t"<<map_vec[iModule].shape<<"\t"<<r<<"\t"<<"\t"<<map_vec[iModule].average_hits<<"\t \t"<<fH<<"\t \t"<<fL<<"\t"<<map_vec[iModule].mean_error<<"\n";
		       }
		   }
	       }
	   }
       }
   }

  ldm_hdm->Draw("colz");
  //ldm_hdm_r->Draw();
			 
  ldm_hdm->Write();
  file.close();


  f->Close();

















}
