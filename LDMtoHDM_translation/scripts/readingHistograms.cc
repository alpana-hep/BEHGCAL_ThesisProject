#include <stdio.h>
#include<fstream>
#include <vector>
#include <string>
#include <map>
#include<iostream>
using namespace std;

void readingHistograms(string root_file_name,string map_file)
{
  TFile* f = new TFile("/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/ldm_hdm_correction/Nuetrino_noise_sample/data_NG_nt_EOL.root","Recreate");
  TH1F* data_per_elink = new TH1F("data_per_elink","average econ data per elink",5066,0,5066);
  TH1F* Transformed_data_per_elink = new TH1F("Transformed_data_per_elink",";;transformed / original",5066,0,5066);
  TH1F* ratio_plot = new TH1F("ratio_plot","Pessimistics",5066,0,5066);
  ratio_plot->GetXaxis()->SetTitle("Econ-d's");
  ratio_plot->GetYaxis()->SetTitle("transformed/original");
  
  char* hname = new char[2000];
  char* hname1 = new char[2000];
  char* hname0 = new char[2000];
  
  char* hist_name = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
  char* hist_name3 = new char[200];
  
  sprintf(hname,"/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/ldm_hdm_correction/Nuetrino_noise_sample/%s",root_file_name.c_str());
  //  sprintf(hname0,"/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/%s",root_file_name1.c_str());
  
  TFile * inputfile = new TFile(hname,"READ");
  //TFile * inputfile1 = new TFile(hname0,"READ");
  
  sprintf(hname1,"/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/ldm_hdm_correction/Nuetrino_noise_sample/%s",map_file.c_str());

  std::ofstream file;
  file.open("/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/ldm_hdm_correction/Nuetrino_noise_sample/data_hits_NGun_nt_EOL.txt",ios::out);

  std::fstream mapping_file;
  mapping_file.open(hname1,ios::in);
  struct average_per_elinks_Map {
    unsigned int  layer, u, v, type,typee;
    char shape;
    unsigned nDaqLpgbt;
    int   idxDaqLpgbt1, idxDaqLpgbt2;
    int nDaqElinks1,nDaqElinks2; 
    float average_hits,Error_means,rms,rms_error;
  };
  std::vector<average_per_elinks_Map> lpgbt_vector;
  struct modulemap {
    unsigned int  layer, u, v, type,typee;
    char shape;
    //    int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
    
    int  nDaqElinks1, nDaqElinks2;
    int  idxDaqLpgbt1, idxDaqLpgbt2;
    unsigned nDaqLpgbt;//,nTpgLpgbt;
    int average_data_per_econ;
    
  };

  std::vector <modulemap> map_vec;
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
	//int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
	int  idxDaqLpgbt1, idxDaqLpgbt2;
	int nDaqElinks1, nDaqElinks2;
	unsigned nDaqLpgbt;//,nTpgLpgbt;
	int average_data_per_econ;
	
	mapping_file >> layer >> u >> v >> type >> typee >> shape
		     >> nDaqLpgbt 
		     >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2 >> average_data_per_econ;
	  //>> idxTpgLpgbt1 >> nTpgElinks1 >> idxTpgLpgbt2 >> nTpgElinks2;
	

	if (mapping_file.eof()) break;
	modulemap mm;
	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee =typee;
	mm.shape = shape;
	mm.nDaqLpgbt=nDaqLpgbt; 
	mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
	mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
	//	mm.nTpgLpgbt=nTpgLpgbt; mm.idxTpgLpgbt1=idxTpgLpgbt1; mm.nTpgElinks1=nTpgElinks1;
	//mm.idxTpgLpgbt2=idxTpgLpgbt2; mm.nTpgElinks2= nTpgElinks2;
	mm.average_data_per_econ = average_data_per_econ;
	map_vec.push_back(mm);
      }
      mapping_file.close();
    }
  cout<<map_vec.size()<<endl;
  int total_elinks[5066]={};
  float average_hits_30[5066]={};
  float Mean[5066]={};
  float Error_mean[5066]={};
  float rms[5066]={};
  float rms_error[5066]={};
  float average_hits_20[5066]={};
  float Trans_average_hits_30[5066]={};
  float Trans_average_hits_20[5066]={};
  bool hdm;
  float n20, n30, nWords, n20_Trans,n30_Trans,nWords_Trans,mean,Mean_Error;
  float total_econ_data[5066] = {};
  float average_econ_data_per_Elink[5066] = {};
  float Trans_average_econ_data_per_Elink[5066] = {};
  
  float transformed_econ_data[5066] = {};
  //counters
  int above_hard_limit=0,above_soft_limit =0, below_soft_limit= 0;
  int trans_above_hard_limit=0,trans_above_soft_limit =0, trans_below_soft_limit=0;
  
  for(int iModule = 0; iModule<(int(map_vec.size()));iModule++)
    {  average_per_elinks_Map map;
      // calculate total number of elinks connected to that module
      if(map_vec[iModule].nDaqLpgbt == 1)
	{
	  total_elinks[iModule]+= map_vec[iModule].nDaqElinks1;
	}
      else if (map_vec[iModule].nDaqLpgbt == 2)
	{
	  total_elinks[iModule]+=(map_vec[iModule].nDaqElinks1 +  map_vec[iModule].nDaqElinks2);

	}
      else if ( map_vec[iModule].nDaqLpgbt == 0)
	{
	  
	  total_elinks[iModule] =1;
	}
      else
	continue;
      //hdm/ldm modules
      
      if(map_vec[iModule].type ==0)
	{
	  hdm = true;
	}
      else
	{
	  hdm = false;
	}
      
      sprintf(hist_name,"orig_hits_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      //TH1F *resp = (TH1F*)inputfile->Get(hist_name);
      //sprintf(hist_name1,"orig_hits_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      // TH1F *resp1 = (TH1F*)inputfile->Get(hist_name1);
      //sprintf(hist_name2,"trans_hits_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      TH1F *resp = (TH1F*)inputfile->Get(hist_name);     
      Mean[iModule] = resp->GetMean();
      Error_mean[iModule] = resp->GetMeanError();
      rms[iModule]=resp->GetRMS();
      rms_error[iModule]=resp->GetRMSError();
      // sprintf(hist_name1,"orig_hits_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      // TH1F *resp1 = (TH1F*)inputfile->Get(hist_name1);
      // average_hits_30[iModule] = resp1->GetMean();
      
      // //      n20 = average_hits_20[iModule];
      // //      n30 = average_hits_30[iModule];

      // sprintf(hist_name2,"trans_hits_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      // TH1F *resp2 = (TH1F*)inputfile->Get(hist_name2);
      // Trans_average_hits_20[iModule] = resp2->GetMean();

      // sprintf(hist_name3,"trans_hits_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      // TH1F *resp3 = (TH1F*)inputfile1->Get(hist_name3);
      // Trans_average_hits_30[iModule] = resp3->GetMean();

      //      mean = Mean[iModule];
      //Mean_Error= Error_mean[iModule];
      //     n30 = average_hits_30[iModule];
      // n20_Trans=Trans_average_hits_20[iModule];
      //n30_Trans = Trans_average_hits_30[iModule];
      
      // //      calculate words for original hits
      // if(hdm)
      // 	{ // HDM

      // 	  assert((n20+n30)<=444); // 432 standard + 12 calibration cells
      // 	  if((n20+n30)<49)
      // 	    { // Low occupancy

      // 	      // Header + Contents + CM + Addresses + Channels + CRC
      // 	      nWords=7 + (6+3*(n20+n30)+14)/15 + 16 + (9*(n20+n30)+14)/15 + (20*n20+30*n30+14)/15 + 1;
      // 	    }
      // 	  else
      // 	    { // High occupancy

      // 	      // Header + Bitmap + Contents + CM + Channels + CRC
      // 	      nWords=7 + 30 + (3*(n20+n30)+14)/15 + 16 + (20*n20+30*n30+14)/15 + 1;
      // 	    }
      // 	}
      // else
      // 	{ // LDM
      // 	  assert((n20+n30)<=198); // 192 standard + 6 calibration cells

      // 	  if((n20+n30)<25)
      // 	    { // Low occupancy
      // 	      // Header + Contents + CM + Addresses + Channels + CRC
      // 	      nWords=6 + (5+3*(n20+n30)+14)/15 + 8 + (8*(n20+n30)+14)/15 + (20*n20+30*n30+14)/15 + 1;
      // 	    }
      // 	  else
      // 	    { // High occupancy
      // 	      // Header + Bitmap + Contents + CM + Channels + CRC
      // 	      nWords=6 + 14 + (3*(n20+n30)+14)/15 + 8 + (20*n20+30*n30+14)/15 + 1;

      // 	    }
      // 	}
      
      //      total_econ_data[iModule]+=nWords;
      // if(hdm)
      // 	{ // HDM

      // 	  assert((n20_Trans+n30_Trans)<=444); // 432 standard + 12 calibration cells
      // 	  if((n20_Trans+n30_Trans)<49)
      // 	    { // Low occupancy

      // 	      // Header + Contents + CM + Addresses + Channels + CRC
      // 	      nWords_Trans=7 + (6+3*(n20_Trans+n30_Trans)+14)/15 + 16 + (9*(n20_Trans+n30_Trans)+14)/15 + (20*n20_Trans+30*n30_Trans+14)/15 +1;
      // 	    }
      // 	  else
      // 	    { // High occupancy

      // 	      // Header + Bitmap + Contents + CM + Channels + CRC
      // 	      nWords_Trans=7 + 30 + (3*(n20_Trans+n30_Trans)+14)/15 + 16 + (20*n20_Trans+30*n30_Trans+14)/15 + 1;
      // 	    }
      // 	}
      // else
      // 	{ // LDM
      // 	  assert((n20_Trans+n30_Trans)<=198); // 192 standard + 6 calibration cells

      // 	  if((n20_Trans+n30_Trans)<25)
      // 	    { // Low occupancy
      // 	      // Header + Contents + CM + Addresses + Channels + CRC
      // 	      nWords_Trans=6 + (5+3*(n20_Trans+n30_Trans)+14)/15 + 8 + (8*(n20_Trans+n30_Trans)+14)/15 + (20*n20_Trans+30*n30_Trans+14)/15 + 1;
      // 	    }
      // 	  else
      // 	    { // High occupancy
      // 	      // Header + Bitmap + Contents + CM + Channels + CRC
      // 	      nWords_Trans=6 + 14 + (3*(n20_Trans+n30_Trans)+14)/15 + 8 + (20*n20_Trans+30*n30_Trans+14)/15 + 1;

      // 	    }
      // 	}

      //      transformed_econ_data[iModule]+=nWords_Trans;
      //cout<<total_econ_data[iModule]<<endl;
      //      average_econ_data_per_Elink[iModule] = total_econ_data[iModule]/total_elinks[iModule];
      //average_econ_data_per_Elink[iModule]= mean;
      
      //      Trans_average_econ_data_per_Elink[iModule] = transformed_econ_data[iModule]/total_elinks[iModule];
      
      // cout<<average_econ_data_per_Elink[iModule]<<endl;
      //      total_econ_data[iModule]+=(n20+n30);
      //      cout<<total_econ_data[iModule]<<endl;
      map.layer = map_vec[iModule].layer;
      map.u = map_vec[iModule].u;
      map.v = map_vec[iModule].v;
      map.type = map_vec[iModule].type;
      map.typee = map_vec[iModule].typee;
      
      map.shape = map_vec[iModule].shape;
      map.nDaqLpgbt = map_vec[iModule].nDaqLpgbt;
      map.idxDaqLpgbt1 = map_vec[iModule].idxDaqLpgbt1;
      map.idxDaqLpgbt2 = map_vec[iModule].idxDaqLpgbt2;
      map.nDaqElinks1 = map_vec[iModule].nDaqElinks1;
      map.nDaqElinks2 =map_vec[iModule].nDaqElinks2;
      //      map.average_econ_words = average_econ_data_per_Elink[iModule];
      map.average_hits = Mean[iModule];
      map.Error_means = Error_mean[iModule];
      map.rms= rms[iModule];
      map.rms_error = rms_error[iModule];
      lpgbt_vector.push_back(map);
    }
  for(int i =0; i<(int(lpgbt_vector.size()));i++)
    {
      //cout<<total_elinks[i]<<endl;
      //cout<<lpgbt_vector[i].average_econ_words<<endl;
      //data_per_elink->SetBinContent(i,lpgbt_vector[i].average_econ_words);
      // Transformed_data_per_elink->SetBinContent(i,lpgbt_vector[i].Trans_average_words);
      //      ratio_plot->SetBinContent(i,lpgbt_vector[i].Trans_average_words/lpgbt_vector[i].average_econ_words);

      file << lpgbt_vector[i].layer <<"\t"<< lpgbt_vector[i].u <<"\t"<< lpgbt_vector[i].v <<"\t"<< lpgbt_vector[i].type <<"\t"<<lpgbt_vector[i].typee<<"\t"<< lpgbt_vector[i].shape<<"\t"<<lpgbt_vector[i].average_hits<<"\t"<<"\t"<<lpgbt_vector[i].Error_means<<"\t\t"<<lpgbt_vector[i].rms<<"\t\t"<<lpgbt_vector[i].rms_error<<"\n";


	// lpgbt_vector[i].nDaqLpgbt <<"\t" << lpgbt_vector[i].idxDaqLpgbt1 << "\t"<<lpgbt_vector[i].nDaqElinks1<<"\t"<< lpgbt_vector[i].idxDaqLpgbt2<<"\t"<<lpgbt_vector[i].nDaqElinks2<<"\t"<<lpgbt_vector[i].average_hits<<"\t"<<"\t"<<lpgbt_vector[i].Error_means<<"\t"<<lpgbt_vector[i].rms<<"\t"<<lpgbt_vector[i].rms_error<<"\n";
    }

      // if(lpgbt_vector[i].average_econ_words >=106)
    // 	{
    // 	  above_hard_limit++;
    // 	  //	  cout<<lpgbt_vector[i].layer <<"\t"<< lpgbt_vector[i].u <<"\t"<< lpgbt_vector[i].v <<"\t"<< lpgbt_vector[i].type <<"\t"<< lpgbt_vector[i].shape<<endl;
    // 	}
    //   else if(lpgbt_vector[i].average_econ_words >=74)// && lpgbt_vector[i].average_econ_words < 106)
    // 	{
    // 	  above_soft_limit++;
    // 	}
    //   else if (lpgbt_vector[i].average_econ_words <74)
    // 	{
    // 	  below_soft_limit++;
    // 	}

    //   if(lpgbt_vector[i].Trans_average_words >=106)
    // 	{
    // 	  trans_above_hard_limit++;
    // 	  //	  cout<<lpgbt_vector[i].layer <<"\t"<< lpgbt_vector[i].u <<"\t"<< lpgbt_vector[i].v <<"\t"<< lpgbt_vector[i].type <<"\t"<< lpgbt_vector[i].shape<<endl;
    // 	}
    //   else if(lpgbt_vector[i].Trans_average_words >=74)// && lpgbt_vector[i].Trans_average_words < 106)
    // 	{
    // 	  trans_above_soft_limit++;
    // 	}
    //   else if (lpgbt_vector[i].Trans_average_words<74)
    // 	{
    // 	  trans_below_soft_limit++;
    // 	}
      
    // }
  
      //  cout<<"\t"<<above_hard_limit<<"\t"<<above_soft_limit<<"\t"<<below_soft_limit<<endl;
      //cout<<"\t"<<trans_above_hard_limit<<"\t"<<trans_above_soft_limit<<"\t"<<trans_below_soft_limit<<endl;
  //  ratio_plot->(TH1F* )Transformed_data_per_elink->Divide(&data_per_elink);
      // ratio_plot =(TH1F*) Transformed_data_per_elink->Clone("ratio_plot"); ratio_plot->Divide(Transformed_data_per_elink,data_per_elink);
  //  inputfile->Close();
  //inputfile1->Close();
  //data_per_elink->Draw();
  //  data_per_elink->Write();
      // f->WriteTObject(data_per_elink);
  //f->WriteTObject(Transformed_data_per_elink);
  //f->WriteTObject(ratio_plot);
  
  f->Close();
  file.close();
  

}
  
