#include<iostream>
#include<fstream>
#include<stdio.h>
#include <vector>
#include <string>
#include <map>

using  namespace std;

void ldm_hdm_correction(string map_file, string geometry_file)
{
  //  TH2F* average_vs_r_ldm = new TH2F("average_vs_r_ldm","",100,0,2000,100,0,700);
  //TH2F* average_vs_r_hdm = new TH2F("average_vs_r_hdm","",100,0,2000,100,0,700);
  TFile* f = new TFile("average_vs_r_signal_only.root","Recreate"); // output file containing graphs
  //  TGraphErrors* average_vs_r_ldm_in_layer[50];
  //  TGraphErrors* average_vs_r_hdm_in_layer[50];

  
  // for (Int_t i = 1; i<51;i++)
  //   {
  //     char Name[50],Titlee[50];
  //     float x,y,ex,ey; int n;
  //     sprintf(Name,"average_vs_r_ldm_in_layer%02d",i);
  //     sprintf(Titlee,"average data vs r ldm in layer%02d",i);
  //     //      average_vs_r_ldm_in_layer[i] = new TGraphErrors(n,x,y,ex,ey);
  //     average_vs_r_ldm_in_layer[i]->GetXaxis()->SetTitle("distance from the beam line");
  //     average_vs_r_ldm_in_layer[i]->GetYaxis()->SetTitle("average data");
  //     //average_vs_r_ldm_in_layer.SetTitle(Titlee);
  //     //average_vs_r_ldm_in_layer.SetName(Name);
  //     //      average_vs_r_ldm_in_layer[i]->Sumw2(); 
  //     average_vs_r_ldm_in_layer[i]->SetMarkerColor(4);
  //     average_vs_r_ldm_in_layer[i]->SetMarkerStyle(21);


  //   }
  // for (Int_t i = 1; i<51;i++)
  //   {
  //     char Name[50],Titlee[50];
  //     float x,y,ex,ey; int n;
  //     sprintf(Name,"average_vs_r_hdm_in_layer%02d",i);
  //     sprintf(Titlee,"average data vs r in Layer%02d",i);
  //     //average_vs_r_hdm_in_layer[i] = new TGraphErrors(n,x,y,ex,ey);
  //     average_vs_r_hdm_in_layer[i]->GetXaxis()->SetTitle("distance from the beam line");
  //     average_vs_r_hdm_in_layer[i]->GetYaxis()->SetTitle("average data");
  //     //average_vs_r_hdm_in_layer->SetTitle(Titlee);
  //     //average_vs_r_hdm_in_layer->SetName(Name);
  //     average_vs_r_hdm_in_layer[i]->SetMarkerColor(4);
  //     average_vs_r_hdm_in_layer[i]->SetMarkerStyle(21);

      
  //    }
  
  // TH2F *average_vs_r_ldm_in_layer[50];
  // TH2F *average_vs_r_hdm_in_layer[50];
  // int count=0;  
  // for (Int_t i = 1; i<51;i++)
  //   {
  //     char Name[50],Titlee[50];
  //     sprintf(Name,"average_vs_r_ldm_in_layer%02d",i);
  //     sprintf(Titlee,"average data vs r ldm in layer%02d",i);
  //     average_vs_r_ldm_in_layer[i] = new TH2F(Name,Titlee,100,0,1600,100,0,700);
  //     average_vs_r_ldm_in_layer[i]->GetXaxis()->SetTitle("distance from the beam line");
  //     average_vs_r_ldm_in_layer[i]->GetYaxis()->SetTitle("average data");
  //     average_vs_r_ldm_in_layer[i]->Sumw2();
  //     //average_vs_r_ldm_in_layer[i]->SetFillColor(kRed);

      
  //   }
  // for (Int_t i = 1; i<51;i++)
  //   {
  //     char Name[50],Titlee[50];
  //     sprintf(Name,"average_vs_r_hdm_in_layer%02d",i);
  //     sprintf(Titlee,"average data vs r in Layer%02d",i);
  //     average_vs_r_hdm_in_layer[i] = new TH2F(Name,Titlee,100,0,1600,100,0,700);
  //     average_vs_r_hdm_in_layer[i]->GetXaxis()->SetTitle("distance from the beam line");
  //     average_vs_r_hdm_in_layer[i]->GetYaxis()->SetTitle("average data");
  //     //average_vs_r_DM_in_layer[i]->SetFillColor(kRed);
  //     average_vs_r_hdm_in_layer[i]->Sumw2();
  //   }

  // define multigraph to hold TGraphErrors for each layer
  int n=50;
  //  TMultiGraph*mg = new TMultiGraph();
  TGraphErrors *gr1[n];          //declaration of TGraphs
  TGraphErrors *gr2[n];
  char F ;          //used to select full shape modules
  F= 'F';
  char* hname = new char[2000]; //to read filenames provided by the command
  char* hname1 = new char[2000];
  //  char* hname0 = new char[2000];

  // char* hist_name = new char[200];
  // char* hist_name1 = new char[200];
  // char* hist_name2 = new char[200];
  // char* hist_name3 = new char[200];
  sprintf(hname,"%s",map_file.c_str());
  sprintf(hname1,"%s",geometry_file.c_str());
  std::fstream mapping_file; // file containig  mapping file infor plus average hits information
  mapping_file.open(hname,ios::in); 
  std::fstream Geometry_file; //geometry file containing x and y coordinates for each modules
  Geometry_file.open(hname1,ios::in);

  struct modulemap {          //map to read and stored mapping file infor
    unsigned int  layer, u, v, type,typee;
    char shape;
    unsigned nDaqLpgbt;
    int   idxDaqLpgbt1, idxDaqLpgbt2;
    int nDaqElinks1, nDaqElinks2;
    float average_hits,mean_error,rms,rms_error;
  };
  
  //  cout<<"...."<<endl;
  std::vector <modulemap> map_vec;

  struct output_hdm_map{  //map to stored HDM info of average hits and "r"
    unsigned int  layer, u, v, type,typee;
    char shape;
    float R,average_hits,mean_error,rms;
  };
  std::vector<output_hdm_map> out_hdm_vec;


  struct output_ldm_map{  ///map to stored LDM info of average hits and "r"  /
    unsigned int  layer, u, v, type,typee;
    char shape;
    float R,average_hits,mean_error,rms;
  };

  std::vector<output_ldm_map> out_ldm_vec;

  
  struct geometry_map {        //map to read and store geometry file info
    unsigned  int  layer, u, v, type;
    unsigned int  size;
    float x,y;
    string shape;

  };

  std::vector <geometry_map> geo_map_vec;
  //  cout<<geo_map_vec.size()<<endl;
  // reading the file containing baseline information for modules
  
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
  	float average_hits, mean_error,rms,rms_error;
  	mapping_file >> layer >> u >> v >> type >>typee >> shape
	  //>> nDaqLpgbt
	  //	     >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2
		     >>average_hits >> mean_error;// >>rms >>rms_error;

  	if (mapping_file.eof()) break;
  	modulemap mm;
  	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee = typee;
	mm.shape = shape;
	//  	mm.nDaqLpgbt=nDaqLpgbt; mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
  	//mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
  	mm.average_hits= average_hits;
	mm.mean_error = mean_error;
	//mm.rms=rms;
	//mm.rms_error=rms_error;

  	map_vec.push_back(mm);
	//	cout<<map_vec.size()<<endl;
      }
      mapping_file.close();
    }
  //cout<<"ttt"<<endl;
  //reading geometry file
  
  if(!Geometry_file.is_open()) //reading the geometry file
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      geo_map_vec.clear();
      //  cout<<geo_map_vec.size()<<endl;
      while (Geometry_file)
	 {
	   unsigned  int  layer, u, v, type;
	   unsigned int  size;
	   float x,y;
	   string shape;
	   // 	//	cout<<"........"<<endl;
	   Geometry_file >> layer >> shape >> size >> x >> y >> type >> u >> v;
	   //cout<<layer<<endl;
	   if (Geometry_file.eof()) break;
	   geometry_map mmm;
	   mmm.layer=layer; mmm.shape = shape; mmm.size = size; mmm.x = x; mmm.y =y;  mmm.type=type;
	   mmm.u=u; mmm.v=v; 
	   //cout<<mmm.shape<<endl;
	   geo_map_vec.push_back(mmm);
	   //cout<<geo_map_vec.size()<<endl;
	 }
       //   cout<<"endline"<<endl;
       Geometry_file.close();
    }
  //cout<<"fff"<<endl;
  //reading the needed file is done here.
  float r,X,Y;
  //main code starts
  for(int iModule=0;iModule<(int(map_vec.size()));iModule++) //looping over mapping map
    {
      //         cout<<<<endl;
      output_hdm_map mm;  //declaration of maps starts here
      output_ldm_map mmm;
      
      for(int iGeo=0;iGeo<(int(geo_map_vec.size()));iGeo++) //looping over geo map
	{
	  //cout<<geo_map_vec[iGeo].layer<<endl;

	  //**********condition to find the mapping module in geo file*********************//
	  if(geo_map_vec[iGeo].layer == map_vec[iModule].layer && geo_map_vec[iGeo].u == map_vec[iModule].u && geo_map_vec[iGeo].v == map_vec[iModule].v && map_vec[iModule].shape == F)//  && geo_map_vec[iGeo].size == 200)
	    { //count++;
	      //cout<<map_vec[iModule].shape<<endl;
	      // cout<<geo_map_vec[iGeo].layer<<"\t"<<geo_map_vec[iGeo].u<<"\t"<<geo_map_vec[iGeo].v <<endl;

	      
	      X = geo_map_vec[iGeo].x;
	      Y = geo_map_vec[iGeo].y;
	      r = sqrt(X*X+Y*Y);        // distance of the module from the beam line
	      
	      //cout<<map_vec[iModule].average_data_per_econ<<endl;
	      
	      if(map_vec[iModule].type ==0) //selecting only HDM
		{
		  if(map_vec[iModule].type == map_vec[iModule].typee) //geometry consistency check
		    {

		      mm.layer = map_vec[iModule].layer;
		      mm.u=map_vec[iModule].u; mm.v=map_vec[iModule].v; mm.type=map_vec[iModule].type; mm.typee=map_vec[iModule].typee;
		      mm.shape=map_vec[iModule].shape;
		      mm.R= r;
		      mm.average_hits= map_vec[iModule].average_hits;
		      mm.mean_error=map_vec[iModule].mean_error;
		      mm.rms=map_vec[iModule].rms;
		      out_hdm_vec.push_back(mm);
		      //cout<<geo_map_vec[iGeo].layer<<"\t"<<geo_map_vec[iGeo].u<<"\t"<<geo_map_vec[iGeo].v <<"\t"<<map_vec[iModule].average_data_per_econ<<"\t"<<r<<endl;
		      //cout<<geo_map_vec[iGeo].size<<endl;  
		      //		      average_vs_r_hdm_in_layer[map_vec[iModule].layer]->Sumw2();
		      //average_vs_r_hdm_in_layer[map_vec[iModule].layer]->Fill(r,map_vec[iModule].average_data_per_econ);
		      //average_vs_r_hdm_in_layer[map_vec[iModule].layer]->Sumw2();
		    }
			
		  else //moving a CMSSW  LD type module which is HD in mapping file to LDM curve
		    {
		      mmm.layer = map_vec[iModule].layer;
		      mmm.u=map_vec[iModule].u; mmm.v=map_vec[iModule].v; mmm.type=map_vec[iModule].type; mmm.typee=map_vec[iModule].typee;
		      mmm.shape=map_vec[iModule].shape;
		      mmm.R= r;
		      mmm.average_hits= map_vec[iModule].average_hits;
		      mmm.mean_error=map_vec[iModule].mean_error;
		      mmm.rms=map_vec[iModule].rms;
		      out_ldm_vec.push_back(mmm);
		      
		      //average_vs_r_ldm_in_layer[map_vec[iModule].layer]->Sumw2();
		      
		      //average_vs_r_ldm_in_layer[map_vec[iModule].layer]->Fill(r,map_vec[iModule].average_data_per_econ);
		      //average_vs_r_ldm_in_layer[map_vec[iModule].layer]->Sumw2();
		      //		      continue;
		    }
		}
	      else if (map_vec[iModule].type ==1) //selecting LD module
		{
		  //cout<<geo_map_vec[iGeo].size<<endl;
		  if(geo_map_vec[iGeo].size ==200) //LD modules having thickness 200um
		    {
		      // count++;
		      //cout<<geo_map_vec[iGeo].size<<endl;
		      if(map_vec[iModule].type == map_vec[iModule].typee) //geometry inconsistency
			{

			  mmm.layer = map_vec[iModule].layer;
			  mmm.u=map_vec[iModule].u; mmm.v=map_vec[iModule].v; mmm.type=map_vec[iModule].type; mmm.typee=map_vec[iModule].typee;
			  mmm.shape=map_vec[iModule].shape;
			  mmm.R= r;
			  mmm.average_hits= map_vec[iModule].average_hits;
			  mmm.mean_error=map_vec[iModule].mean_error;
			  mmm.rms=map_vec[iModule].rms;
			  out_ldm_vec.push_back(mmm);
			  
			  //cout<<geo_map_vec[iGeo].layer<<"\t"<<geo_map_vec[iGeo].u<<"\t"<<geo_map_vec[iGeo].v <<"\t"<<map_vec[iModule].average_data_per_econ<<"\t"<<r<<endl;
			  //  average_vs_r_ldm_in_layer[map_vec[iModule].layer]->Sumw2();
			  
			  //average_vs_r_ldm_in_layer[map_vec[iModule].layer]->Fill(r,map_vec[iModule].average_data_per_econ);
			  //average_vs_r_ldm_in_layer[map_vec[iModule].layer]->Sumw2();
			}
		      else  //moving a CMSSW  HD type module which is LD in mapping file to HDM curve  
			{
			  mm.layer = map_vec[iModule].layer;
			  mm.u=map_vec[iModule].u; mm.v=map_vec[iModule].v; mm.type=map_vec[iModule].type; mm.typee=map_vec[iModule].typee;
			  mm.shape=map_vec[iModule].shape;
			  mm.R= r;
			  mm.average_hits= map_vec[iModule].average_hits;
			  mm.mean_error=map_vec[iModule].mean_error;
			  mm.rms=map_vec[iModule].rms;
			  out_hdm_vec.push_back(mm);
			  
			  //continue;
			  //average_vs_r_hdm_in_layer[map_vec[iModule].layer]->Sumw2();
			  
			  //average_vs_r_hdm_in_layer[map_vec[iModule].layer]->Fill(r,map_vec[iModule].average_data_per_econ);
			  //average_vs_r_hdm_in_layer[map_vec[iModule].layer]->Sumw2();
			}
		    }
		  else
		    continue;
		}
	      
	
	      else
		continue;
	    }
	  
	}
    }
  cout<<",,,,,,,,,,"<<endl;  
  //fill tgraoh using the module_map for hdm and ldm

  //for(int iModule=0;iModule<(int(out_hdm_vec.size()));iModule++)
    // {
  int N=100;
  int countt=0,count_ldm=0;
  float ex[100]={};
  float ey[100]={};
  int count_num=0;
  float x[100]={};
  float y[100]={};
  float rad[100]={};
  float ave[100]={}; float erad[100]={};
  float eave[100]={};
  struct vector_map{
    float x,y,ex,ey;
  };
  std::vector<vector_map> vec_map;
  //vector<float> Xx_;
  //vector<float> Yy_;
  //vector<float> eXx_;
  //vector<float> eYy_;
  float var1=0,var2=0, var3=0,var4=0;
  for(int i =1; i<37;i++) //because we only have HDM till layer 36
    {
      vector_map mm;
      //looping over HDM map to fill the info in graphs
      for(int iModule=0;iModule<(int(out_hdm_vec.size()));iModule++)
	{
	  //countt++;
	  if(i == out_hdm_vec[iModule].layer)
	    {
	      //countt++;
	      x[countt]=out_hdm_vec[iModule].R;
	      y[countt]= out_hdm_vec[iModule].average_hits;
	      ex[countt]=0;
	      ey[countt]= out_hdm_vec[iModule].mean_error;
	      mm.x = x[countt];
	      mm.y = y[countt]; mm.ex =0; mm.ey=ey[countt];
	      //cout<<out_hdm_vec[iModule].layer<<out_hdm_vec[iModule].u<<out_hdm_vec[iModule].v<<"\t"<<x[countt]<<"\t \t"<<y[countt]<<"\t\t"<<ey[countt]<<endl;
	      vec_map.push_back(mm);
	      countt++;
	    }
	}
      float n = vec_map.size();
      //countt=0;
      //      cout<<vec_map.size()<<endl;
      
      for(int j =0; j<n;j++) //selecting same points and replacing it with the mean values and also reducing the size of v                                                         vector
	 {
	   	   
	   for(int k=0;k<n;k++)
      	    {
	    
	      //cout<<k<<endl;
	      if((int(vec_map[j].x))==(int(vec_map[k].x)))
	  	{
	  	  
		  count_num++;
		  if(k!=j)
		    {
		 
	      	      vec_map.erase(vec_map.begin()+k);
		      n = vec_map.size();
		      
		    }
		  else
		    continue;
		}
	    
	      
	      else
		continue;
	      
	      //	      else
	      // //continue;
	//      	    }
      //	  if(vec_map[j].x<400)
      //{
	      //      vec_map.erase(vec_map.begin()+k);
	    }
	  // cout<<count_num<<endl;
	   ave[j] =vec_map[j].y;
	   rad[j]=vec_map[j].x;
	   eave[j]=vec_map[j].ey;
	   erad[j]=vec_map[j].ex;
	   // var2/count_num;
	   //rad[j]= var1/count_num;
	   //eave[j]= var4/count_num;
	   //erad[j]= var3/count_num;
	  // cout<<count_num<<endl;
	   // var1=0;
	   //var2=0;
	   //var3=0;
	   //v/ar4=0;
	  count_num=0;
	
	  // cout<<rad[j]<<"\t"<<ave[j]<<"\t"<<erad[j]<<"\t"<<eave[j]<<endl;
      // 	  count_num=0;
	  //cout<<vec_map.size()<<endl;
	 n = vec_map.size();
	 
	 }
      if(i==1 || (i>=29 && i<37))
	{
	  countt= (int(vec_map.size()));
	}
      else if(i>1 && i <=28)
	{
	  countt= (int(vec_map.size()))+1;
	}
    
      
      //cout<<vec_map.size()<<endl;
      //for(int m = 0; m<(int(vec_map.size()))+1;m++)
      //{
      //  cout<<rad[m]<<"\t"<<ave[m]<<"\t"<<eave[m]<<endl;
      //}
      auto c1 = new TCanvas("c1","Graph Example",200,10,700,500);
      gStyle->SetOptStat(111110);
      gr1[i] = new TGraphErrors(countt,rad,ave,erad,eave);
      char Name[50];
      sprintf(Name,"average_vs_r_hdm_in_layer%02d",i);
      c1->SetName(Name);
      gr1[i]->SetName(Name);
      gr1[i]->GetXaxis()->SetTitle("distance from the beamline");
      gr1[i]->GetYaxis()->SetTitle("average hits");
      gr1[i]->SetTitle("average hits vs distance from beampipe for each HD modules");
      gr1[i]->SetMarkerColor(4);
      gr1[i]->SetMarkerStyle(21);
      
      //      gr1[i]->Draw("ap");
      c1-> Update();
      //c1->Write();
      gr1[i]->Write();
      countt=0;
      vec_map.clear();
    }

  countt=0;
  count_num=0;
  //Tgraphs for ldm curves
  float eX[100]={};
  float eY[100]={};
  float Xx[100]={};
  float Yy[100]={};
  float eRad[100]={};
  float eAve[100]={};
  float Rad[100]={};
  float Ave[100]={};
  float Var1=0,Var2=0, Var3=0,Var4=0;
  
  for(int i =1; i<37;i++)
    {
      //looping over LDM map to fill the info in graphs
      
      for(int iModule=0;iModule<(int(out_ldm_vec.size()));iModule++)
	{
	  vector_map mm;
	  //count_ldm++;
	  if(i == out_ldm_vec[iModule].layer)
	    {
	      //count_ldm++;
	      Xx[count_ldm]=out_ldm_vec[iModule].R;
	      Yy[count_ldm]= out_ldm_vec[iModule].average_hits;
	      eX[count_ldm]=0;
	      eY[count_ldm]=out_ldm_vec[iModule].mean_error;
	      mm.x = Xx[count_ldm];
	      mm.y = Yy[count_ldm]; mm.ex =0; mm.ey=eY[count_ldm];
	      cout<<out_ldm_vec[iModule].layer<<out_ldm_vec[iModule].u<<out_ldm_vec[iModule].v<<endl;//<<"\t"<<Xx[countt]<<"\t \t"<<Yy[countt]<<"\t\t"<<eY[countt]<<endl; 
	      //cout<<Xx[count_ldm]<<"\t"<<Yy[count_ldm]<<"\t"<<eY[count_ldm]<<endl;
	      vec_map.push_back(mm);
	      count_ldm++;
	      
	      //  cout<<Xx[count_ldm]<<"\t"<<Yy[count_ldm]<<endl;
	      
	    }

	}
      count_ldm=0;
      count_num=0;
      //  cout<<vec_map.size()<<endl;
      int Num_ele = (int(vec_map.size()));
      for(int j =0; j<Num_ele;j++) //selecting same points and replacing it with the mean values and also reducing the size of v\vector
	{
	  // if(vec_map[j].x>1000)
	  //   {
	  //     vec_map.erase(vec_map.begin()+j);
	  //   }
	  // //cout<<j<<"\t"<<(int(vec_map[j].x))<<endl;
	  for(int k=0;k<Num_ele;k++)
	    {
	      //cout<<k<<endl;
	      if((int(vec_map[j].x))==(int(vec_map[k].x)))
	  	{
	  	  if(k!=j)
	  	    {
		      vec_map.erase(vec_map.begin()+k);
		      Num_ele = (int(vec_map.size()));
	  	    }    //		      cout<<vec_map.size()<<endl;
	  	  //}
	  	      //else
	  	      //continue;
	  	}
	      else
	  	continue;

	      //              else
	      //continue;
	    }
	  
	  
	  //cout<<count_num<<endl;
	  // Var1+=vec_map[j].x;
	  // Var2+=vec_map[j].y;
	  // Var3+=vec_map[j].ex;
	  // Var4+=vec_map[j].ey;
	  
	  Ave[j] = vec_map[j].y;//Var2;///(count_num+1);
	  Rad[j] = vec_map[j].x; // Var1;//(count_num+1);
	  eAve[j]= vec_map[j].ey; //Var4;//(count_num+1);
	  eRad[j]= vec_map[j].ex;//Var3;//(count_num+1);
	  //cout<<count_num+1<<endl;
	  // Var1=0;
	  // Var2=0;
	  // Var3=0;
	  // Var4=0
	    
	  count_num=0;
	  Num_ele = (int(vec_map.size()));
	  
	  //	  cout<<Rad[j]<<"\t"<<Ave[j]<<"\t"<<eRad[j]<<"\t"<<eAve[j]<<endl;
	  //          count_num=0;
	  //	  cout<<vec_map.size()<<endl;
	}
      //cout<<count_ldm<<endl;
      //countt= (int(vec_map.size()));
      if(i==2 || i ==3 || i==10 || i==14 || i==16 || i==29 || i==34 || i == 36)
       	{
       	  countt= (int(vec_map.size()))+1;
       	}
      // else if((i>1 && i <=12) || i ==15 || i==16)
      // 	{
      // 	  countt= (int(vec_map.size()))+1;
      // 	}
      else
      	{
	  countt= (int(vec_map.size()));
       	}

      cout<<countt<<endl;
		      
      auto c1 = new TCanvas("c1","Graph Example",200,10,700,500);
      gStyle->SetOptStat(111110);
      
      gr2[i] = new TGraphErrors(countt,Rad,Ave,eRad,eAve);
      char Name[50];
      sprintf(Name,"average_vs_r_ldm_in_layer%02d",i);
      c1->SetName(Name);
      
      gr2[i]->SetName(Name);
      gr2[i]->GetXaxis()->SetTitle("distance from the beamline");
      gr2[i]->GetYaxis()->SetTitle("average hits");
      gr2[i]->SetTitle("average hits vs distance from beampipe for each LD modules");
      
      gr2[i]->SetMarkerColor(4);
      gr2[i]->SetMarkerStyle(21);

      //gr2[i]->Draw("ap");
      c1->Update();
      //c1->Write();
      //f->WriteTObject(gr2[i]);
      gr2[i]->Write();
      count_ldm=0;
      countt=0;
      vec_map.clear();
      
    }
  
      //      char Name[50],Titlee[50];
      //sprintf(Name,"average_vs_r_hdm_in_layer%02d",out_hdm_vec[iModule].layer);
      //sprintf(Titlee,"average data vs r hdm in layer%02d",out_hdm_vec[iModule].layer);    
          
  // //  cout<<count<<endl;
  // for(int k =1; k<51;k++)
  //   {
  //     //      average_vs_r_ldm_in_layer[k]->Draw("E");
  //     //      average_vs_r_hdm_in_layer[k]->Draw("E");
      
  //      average_vs_r_ldm_in_layer[k]->Write();
  //  average_vs_r_hdm_in_layer[k]->Write();
  //}
  //  average_vs_r_hdm->Draw("colz");
  //  average_vs_r_ldm->Draw("sames");
  f->Close();  
}
  
