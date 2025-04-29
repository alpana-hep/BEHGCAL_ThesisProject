#include <stdio.h>
#include<fstream>
#include <vector>
#include <string>
#include <map>
#include<iostream>
using namespace std;
  

void oneDplots(string root_file_name,string map_file)
{
  char* title= new char[200];
  //char* hname = new char[200];
  //char* hist_name = new char[200];
  char* stat_name = new char[200];
  //char* hname1 = new char[2000];
  char* path = new char[2000];
  char* full_path = new char[2000];
  char* path1 = new char[2000];
  char* full_path1 = new char[2000];
  char* path2 = new char[2000];
  char* full_path2 = new char[2000];
  //  char* plot_name = new char[200];
  char* old_rel = new char[200];
  char* new_rel = new char[200];
  char* old_release = new char[50];
  char* new_release = new char[50];
  char* hist = new char[200];
  
  TFile* f = new TFile("data_per_elink_nt_EOL_correct.root","Recreate");
  TH1F* data_per_elink = new TH1F("data_per_elink","average econ data per elink",5066,0,5066);
  TH1F* Transformed_data_per_elink = new TH1F("Transformed_data_per_elink",";;transformed / original",5066,0,5066);
  TH1F* ratio_plot = new TH1F("ratio_plot","Pessimistics",5066,0,5066);
  ratio_plot->GetXaxis()->SetTitle("Econ-d's");
  ratio_plot->GetYaxis()->SetTitle("transformed/original");
  
  char* hname = new char[2000];
  char* hname1 = new char[2000];
  char* hname0 = new char[2000];
char* name = new char[2000];  
  char* hist_name = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
  char* hist_name3 = new char[200];
  
  sprintf(hname,"%s",root_file_name.c_str());
  //  sprintf(hname0,"/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/%s",root_file_name1.c_str());
  sprintf(path2,"Results/1d_plots/afterQT");//,root_file_name.c_str());
  //  sprintf(path2,"subset_fitting/",root_file_name.c_str());
  
  TFile * inputfile = new TFile(hname,"READ");
  //TFile * inputfile1 = new TFile(hname0,"READ");
  
  sprintf(hname1,"%s",map_file.c_str());

  std::ofstream file;
  file.open("fit_function_parameters_temp_updatedntuples_v2.txt",ios::out);

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
    int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;

    int  nDaqElinks1, nDaqElinks2;
    int  idxDaqLpgbt1, idxDaqLpgbt2;
    unsigned nDaqLpgbt;//,nTpgLpgbt;
    float average_data_per_econ,Error_means,rms,rms_error;


  };
  std::vector <modulemap> map_vec;
  
  //std::vector <n10_map> n10_map_vec;
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
	int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
	int  idxDaqLpgbt1, idxDaqLpgbt2;
	int nDaqElinks1, nDaqElinks2;
	unsigned nDaqLpgbt,nTpgLpgbt;
	float average_data_per_econ,Error_means,rms,rms_error;
	
	
	mapping_file >> layer >> u >> v >> type  >> shape
		     >> nDaqLpgbt >>nTpgLpgbt 
	  	     >> idxDaqLpgbt1 >> nDaqElinks1 >> idxDaqLpgbt2 >> nDaqElinks2 //>> average_data_per_econ;//>>Error_means>>rms>>rms_erro
		  >> idxTpgLpgbt1 >> nTpgElinks1 >> idxTpgLpgbt2 >> nTpgElinks2;
	

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
  // int total_elinks[5066]={};
  // float average_hits_30[5066]={};
  // float Mean[5066]={};
  // float Error_mean[5066]={};
  // float rms[5066]={};
  // float rms_error[5066]={};
  // float average_hits_20[5066]={};
  // float Trans_average_hits_30[5066]={};
  // float Trans_average_hits_20[5066]={};
  // bool hdm;
  // float n20, n30, nWords, n20_Trans,n30_Trans,nWords_Trans,mean,Mean_Error;
  // float total_econ_data[5066] = {};
  // float average_econ_data_per_Elink[5066] = {};
  // float Trans_average_econ_data_per_Elink[5066] = {};
  
  // float transformed_econ_data[5066] = {};
  //counters
  int above_hard_limit=0,above_soft_limit =0, below_soft_limit= 0;
  int trans_above_hard_limit=0,trans_above_soft_limit =0, trans_below_soft_limit=0;

  int count=0;
  //parameters to store fitted function parameters
  float constant1[5066]={};
  float constant2[5066]={};
  float constant3[5066]={};
  float Mean_G[5066]={};
  float Sigma_G[5066]={};
  float Mean_P[5066]={};
  float Mean_l[5066]={};
  float Sigma_l[5066]={};
  //  float par[8];
  //for(int iModule = 3550; iModule<5066;iModule++)//(int(map_vec.size()));iModule++)
  for(int iModule = 0; iModule<(int(map_vec.size()));iModule++)
    {  average_per_elinks_Map map;
      
      //      sprintf(path2,"example_fits/layers/Layer_%02d",map_vec[iModule].layer,root_file_name.c_str());
      if(map_vec[iModule].nDaqLpgbt==0) continue;
      //      else continue;
      sprintf(hist_name,"orig_hits_10_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      // if (map_vec[iModule].layer ==44 && map_vec[iModule].u ==15 && map_vec[iModule].v==4)
      // 	{
      // if(map_vec[iModule].u<=5 && map_vec[iModule].v<=5)
      // 	{
      // 	  if(map_vec[iModule].layer<=6){
      count++;
      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
      Canvas_n2->SetFillColor(0);
      Canvas_n2->SetBorderMode(0);
      Canvas_n2->SetBorderSize(2);
      gStyle->SetOptFit(111111);
      sprintf(full_path2,"%s/Module_overlay_%d_%d_%d_%d.pdf",path2,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

      TH1F *resp = (TH1F*)inputfile->Get(hist_name);
      //resp2->SetName("");
      sprintf(hist_name,"orig_hits_20_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      TH1F *resp1 = (TH1F*)inputfile->Get(hist_name);
      sprintf(hist_name,"orig_hits_30_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	

      gStyle->SetOptFit(1);
      //      gPad->SetLogy();
      resp2->GetXaxis()->SetTitle("Module occupancy");
      resp2->GetXaxis()->SetTitleSize(0.05);
      resp2->GetXaxis()->SetTitleOffset(0.99);
      cout<<resp2->GetMean()<<endl;
              //resp2->GetXaxis()->SetRangeUser(0,400);
      resp2->SetLineColorAlpha(kRed,1);
      resp2->SetMarkerColor(kRed);
      resp2->SetLineWidth(2);
      TPaveStats *ptstats2 = new TPaveStats(0.6,0.6,0.9,0.9,"brNDC");
          ptstats2->SetBorderSize(1);
          ptstats2->SetFillColor(0);
          ptstats2->SetLineColor(kRed);
          ptstats2->SetTextAlign(12);
          ptstats2->SetTextColor(kRed);
          ptstats2->SetTextFont(42);
          ptstats2->SetOptStat(1111);
          ptstats2->SetOptFit(1);
          resp2->GetListOfFunctions()->Add(ptstats2);
          ptstats2->SetParent(resp2);
	  sprintf(name,"Module_n30_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
	  resp2->SetName(name);
              // TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
              // func->SetParameter(0,1000); // set starting values                                                                                                                                           
              // func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                              
              // func->SetParameter(2,resp2->GetRMS());
              // func->SetParameter(3,20000);
              // func->SetParameter(4,resp2->GetMean()+2);
              // func->SetParameter(5,100);
              // func->SetParameter(6,resp2->GetMean());
              // func->SetParameter(7,resp2->GetRMS());
              // func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");

              // float r1 = resp2->GetMean()- 4*resp2->GetRMS();
      float r2 = resp2->GetMean()+10*resp2->GetRMS();
      resp2->GetXaxis()->SetRange(0,r2);
 	      // if(r1<0){
              //   func->SetRange(0,r2);}
              // else if(r1>0){
              //   func->SetRange(r1,r2);}
              // resp2->Fit("fit_func","ME");
              // resp2->GetFunction("fit_func")->SetLineColor(kBlack);
              // TF1* f3 = resp2->GetFunction("fit_func");
              // TF1 *f_l =  (TF1*)f3->Clone("f3");
              // constant1[iModule]= f_l->GetParameter(0);
              // constant2[iModule]= f_l->GetParameter(3);
              // constant3[iModule]=f_l->GetParameter(5);
              // Mean_G[iModule]=f_l->GetParameter(1);
              // Mean_P[iModule]=f_l->GetParameter(4);
              // Sigma_G[iModule]=f_l->GetParameter(2);
              // Mean_l[iModule]=f_l->GetParameter(6);
              // Sigma_l[iModule]=f_l->GetParameter(7);
              // file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              // //f_l->SetRange(0, 400);
              resp2->Draw("HIST");
	      // f_l->Draw("Lsame");
              // auto legend = new TLegend(0.75,0.25,0.9,0.32);
              // legend->SetHeader("","C");
              // legend->AddEntry(resp2,"orig","l");
              // legend->AddEntry(f_l,"fit funct","l");
              // gStyle->SetLegendTextSize(0.03);
              // legend->Draw("sames");
 
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);
    }
	   
	    
  f->Close();
  file.close();
	
    
  cout<<"total"<<count<<endl;
  
}
  
