#include <stdio.h>
#include<fstream>
#include <vector>
#include <string>
#include <map>
#include<iostream>
using namespace std;
  

void n30_fitting(string root_file_name,string map_file)
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
  
  char* hist_name = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
  char* hist_name3 = new char[200];
  
  sprintf(hname,"%s",root_file_name.c_str());
  //  sprintf(hname0,"/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/%s",root_file_name1.c_str());
  sprintf(path2,"Results/n30/v1",root_file_name.c_str());
  //  sprintf(path2,"subset_fitting/",root_file_name.c_str());
  
  TFile * inputfile = new TFile(hname,"READ");
  //TFile * inputfile1 = new TFile(hname0,"READ");
  
  sprintf(hname1,"%s",map_file.c_str());

  std::ofstream file;
  file.open("fit_function_parameters_n30_updatedntuples_v2.txt",ios::out);

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

      if(map_vec[iModule].nDaqLpgbt==0) continue;

      //      sprintf(path2,"example_fits/layers/Layer_%02d",map_vec[iModule].layer,root_file_name.c_str());
      //           if((map_vec[iModule].layer==31 && map_vec[iModule].u==8 && map_vec[iModule].v==10))
      //      if( (map_vec[iModule].layer==31 && map_vec[iModule].u ==8 && map_vec[iModule].v==10)|| (map_vec[iModule].layer==33 && map_vec[iModule].u ==9 && ma \
p_vec[iModule].v==8)||(map_vec[iModule].layer==34 && map_vec[iModule].u ==9 && map_vec[iModule].v==1)||(map_vec[iModule].layer==34 && map_vec[iModule].u ==8 && map_vec[iModule].v==10)|| (map_vec[iModule].layer==35 && map_vec[iModule].u ==9 && map_vec[iModule].v==3)|| (map_vec[iModule].layer==35 && map_vec[iModule].u ==5 && map_vec[iModule].v==9))
      // if(map_vec[iModule].u==8 && map_vec[iModule].v ==9 && map_vec[iModule].layer==18)
      // 	     {
      sprintf(hist_name,"orig_hits_30_%d_%d_%d_%d",map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
      
      if(map_vec[iModule].u<=5 && map_vec[iModule].v<=5)
      	{
 	  if(map_vec[iModule].layer<=6){
 	    count++;
              TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
              Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
              Canvas_n2->SetFillColor(0);
              Canvas_n2->SetBorderMode(0);
              Canvas_n2->SetBorderSize(2);
              gStyle->SetOptFit(111111);
              sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

              TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
              //resp2->SetName("");                                                                                                                                                                        
              gStyle->SetOptFit(1);
              gPad->SetLogy();
              resp2->GetXaxis()->SetTitle("number of hits");
              resp2->GetXaxis()->SetTitleSize(0.05);
              //resp2->GetXaxis()->SetLimits(0,400);
              resp2->SetLineColorAlpha(kRed,1);
              resp2->SetMarkerColor(kRed);
              resp2->SetLineWidth(2);

              TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
              func->SetParameter(0,1000); // set starting values                                                                                                                                           
              func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                              
              func->SetParameter(2,resp2->GetRMS());
              func->SetParameter(3,20000);
              func->SetParameter(4,resp2->GetMean()+2);
              func->SetParameter(5,100);
              func->SetParameter(6,resp2->GetMean());
              func->SetParameter(7,resp2->GetRMS());
              func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");

              float r1 = resp2->GetMean()- 4*resp2->GetRMS();
              float r2 = resp2->GetMean()+4*resp2->GetRMS();
 	      if(r1<0){
                func->SetRange(0,r2);}
              else if(r1>0){
                func->SetRange(r1,r2);}
              resp2->Fit("fit_func","ME");
              resp2->GetFunction("fit_func")->SetLineColor(kBlack);
              TF1* f3 = resp2->GetFunction("fit_func");
              TF1 *f_l =  (TF1*)f3->Clone("f3");
              constant1[iModule]= f_l->GetParameter(0);
              constant2[iModule]= f_l->GetParameter(3);
              constant3[iModule]=f_l->GetParameter(5);
              Mean_G[iModule]=f_l->GetParameter(1);
              Mean_P[iModule]=f_l->GetParameter(4);
              Sigma_G[iModule]=f_l->GetParameter(2);
              Mean_l[iModule]=f_l->GetParameter(6);
              Sigma_l[iModule]=f_l->GetParameter(7);
              file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              f_l->SetRange(0, 400);
              resp2->Draw("");
             f_l->Draw("Lsame");
              auto legend = new TLegend(0.75,0.25,0.9,0.32);
              legend->SetHeader("","C");
              legend->AddEntry(resp2,"orig","l");
              legend->AddEntry(f_l,"fit funct","l");
              gStyle->SetLegendTextSize(0.03);
              legend->Draw("sames");
 
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);
            }

 	else if( map_vec[iModule].layer>6 && map_vec[iModule].layer<=10)
 	    {
 	      count++;
 	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
 	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
 	      Canvas_n2->SetFillColor(0);
 	      Canvas_n2->SetBorderMode(0);
 	      Canvas_n2->SetBorderSize(2);
 	      gStyle->SetOptFit(111111);
 	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
	      
 	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
 	      //resp2->SetName("");
 	      gStyle->SetOptFit(1);
 	      gPad->SetLogy();
 	      resp2->GetXaxis()->SetTitle("number of hits");
 	      resp2->GetXaxis()->SetTitleSize(0.05);
 	      //resp2->GetXaxis()->SetLimits(0,400);
 	      resp2->SetLineColorAlpha(kRed,1);
 	      resp2->SetMarkerColor(kRed);
 	      resp2->SetLineWidth(2);

 	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
 	      //TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::LogNormal(x,[6],0,1)");
 	      //TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::GammaDist(x	\,[6],0,1)");
 	      func->SetParameter(0,2000); // set starting values
 	      func-> SetParameter(1,resp2->GetMean()); // set starting values
 	      func->SetParameter(2,resp2->GetRMS());
 	      func->SetParameter(3,10000);
 	      func->SetParameter(4,resp2->GetMean());
 	      func->SetParameter(5,10000);
 	      func->SetParameter(6,resp2->GetMean()-2);
 	      func->SetParameter(7,resp2->GetRMS()+2);
 	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      
 	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
 	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
 	      if(r1<0){
 	  	func->SetRange(0,r2);}
 	      else if(r1>0){
 	  	func->SetRange(r1,r2);}
 	      resp2->Fit("fit_func","ME");
 	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
 	      TF1* f3 = resp2->GetFunction("fit_func");
 	      TF1 *f_l =  (TF1*)f3->Clone("f3");
 	      constant1[iModule]= f_l->GetParameter(0);
 	      constant2[iModule]= f_l->GetParameter(3);
 	      constant3[iModule]=f_l->GetParameter(5);
 	      Mean_G[iModule]=f_l->GetParameter(1);
 	      Mean_P[iModule]=f_l->GetParameter(4);
 	      Sigma_G[iModule]=f_l->GetParameter(2);
 	      Mean_l[iModule]=f_l->GetParameter(6);
 	      Sigma_l[iModule]=f_l->GetParameter(7);
 	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
 	      f_l->SetRange(0, 400);
 	      resp2->Draw("");
 	     f_l->Draw("Lsame");
 	      auto legend = new TLegend(0.75,0.25,0.9,0.32);
 	      legend->SetHeader("","C");
 	      legend->AddEntry(resp2,"orig","l");
 	      legend->AddEntry(f_l,"fit funct","l");
 	      gStyle->SetLegendTextSize(0.03);
 	      legend->Draw("sames");
 	      Canvas_n2->Modified();
 	      Canvas_n2->cd();
 	      Canvas_n2->SetSelected(Canvas_n2);
 	      Canvas_n2->SaveAs(full_path2);
 	    }
 	else if (map_vec[iModule].layer>10 && map_vec[iModule].layer<=17)
 	  {
 	    count++;
 	    TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
 	    Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
 	    Canvas_n2->SetFillColor(0);
 	    Canvas_n2->SetBorderMode(0);
 	    Canvas_n2->SetBorderSize(2);
 	    gStyle->SetOptFit(111111);
 	    sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

 	    TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
 	    gStyle->SetOptFit(1);
 	    gPad->SetLogy();
 	    resp2->GetXaxis()->SetTitle("number of hits");
 	    resp2->GetXaxis()->SetTitleSize(0.05);
 	    //resp2->GetXaxis()->SetLimits(0,400);
 	    resp2->SetLineColorAlpha(kRed,1);
 	    resp2->SetMarkerColor(kRed);
 	    resp2->SetLineWidth(2);

 	    TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
 	    func->SetParameter(0,600); // set starting values                                                                                                                                         
 	    func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                             
 	    func->SetParameter(2,resp2->GetRMS()+1);
 	    func->SetParameter(3,10000);
 	    func->SetParameter(4,resp2->GetMean());
 	    func->SetParameter(5,5000);
 	    func->SetParameter(6,resp2->GetMean());
 	    func->SetParameter(7,resp2->GetRMS());
 	    func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
 	    float r1 = resp2->GetMean()- 4*resp2->GetRMS();
 	    float r2 = resp2->GetMean()+4*resp2->GetRMS();
 	    if(r1<0){
 	      func->SetRange(0,r2);}
 	    else if(r1>0){
 	      func->SetRange(r1,r2);}
 	    resp2->Fit("fit_func","ME");
 	    resp2->GetFunction("fit_func")->SetLineColor(kBlack);
 	    TF1* f3 = resp2->GetFunction("fit_func");
 	    TF1 *f_l =  (TF1*)f3->Clone("f3");
 	    constant1[iModule]= f_l->GetParameter(0);
 	    constant2[iModule]= f_l->GetParameter(3);
 	    constant3[iModule]=f_l->GetParameter(5);
 	    Mean_G[iModule]=f_l->GetParameter(1);
 	    Mean_P[iModule]=f_l->GetParameter(4);
 	    Sigma_G[iModule]=f_l->GetParameter(2);
 	    Mean_l[iModule]=f_l->GetParameter(6);
 	    Sigma_l[iModule]=f_l->GetParameter(7);
 	    file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
 	    f_l->SetRange(0, 400);
 	    resp2->Draw("");
 	    f_l->Draw("Lsame");
 	    auto legend = new TLegend(0.75,0.25,0.9,0.32);
 	    legend->SetHeader("","C");
 	    legend->AddEntry(resp2,"orig","l");
 	    legend->AddEntry(f_l,"fit funct","l");
 	    gStyle->SetLegendTextSize(0.03);
 	    legend->Draw("sames");
 	    Canvas_n2->Modified();
 	    Canvas_n2->cd();
 	    Canvas_n2->SetSelected(Canvas_n2);
 	    Canvas_n2->SaveAs(full_path2);
 	  }
 	else if (map_vec[iModule].layer>17 && map_vec[iModule].layer<=36)
          {
            count++;
            TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
            Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
            Canvas_n2->SetFillColor(0);
            Canvas_n2->SetBorderMode(0);
            Canvas_n2->SetBorderSize(2);
            gStyle->SetOptFit(111111);
            sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

            TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
            gStyle->SetOptFit(1);
            gPad->SetLogy();
            resp2->GetXaxis()->SetTitle("number of hits");
            resp2->GetXaxis()->SetTitleSize(0.05);
            //resp2->GetXaxis()->SetLimits(0,400);
            resp2->SetLineColorAlpha(kRed,1);
            resp2->SetMarkerColor(kRed);
            resp2->SetLineWidth(2);

            TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
            func->SetParameter(0,1000); // set starting values                                                                                                                                             
            func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                                
            func->SetParameter(2,resp2->GetRMS()+2);
            func->SetParameter(3,10000);
            func->SetParameter(4,resp2->GetMean());
            func->SetParameter(5,5000);
            func->SetParameter(6,resp2->GetMean());
            func->SetParameter(7,resp2->GetRMS());
            func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
            float r1 = resp2->GetMean()- 4*resp2->GetRMS();
            float r2 = resp2->GetMean()+4*resp2->GetRMS();
            if(r1<0){
              func->SetRange(0,r2);}
            else if(r1>0){
              func->SetRange(r1,r2);}
            resp2->Fit("fit_func","ME");
 resp2->GetFunction("fit_func")->SetLineColor(kBlack);
            TF1* f3 = resp2->GetFunction("fit_func");
            TF1 *f_l =  (TF1*)f3->Clone("f3");
            constant1[iModule]= f_l->GetParameter(0);
            constant2[iModule]= f_l->GetParameter(3);
            constant3[iModule]=f_l->GetParameter(5);
            Mean_G[iModule]=f_l->GetParameter(1);
            Mean_P[iModule]=f_l->GetParameter(4);
            Sigma_G[iModule]=f_l->GetParameter(2);
            Mean_l[iModule]=f_l->GetParameter(6);
            Sigma_l[iModule]=f_l->GetParameter(7);
            file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
            f_l->SetRange(0, 400);
            resp2->Draw("");
            f_l->Draw("Lsame");
            auto legend = new TLegend(0.75,0.25,0.9,0.32);
            legend->SetHeader("","C");
            legend->AddEntry(resp2,"orig","l");
            legend->AddEntry(f_l,"fit funct","l");
            gStyle->SetLegendTextSize(0.03);
            legend->Draw("sames");
            Canvas_n2->Modified();
            Canvas_n2->cd();
            Canvas_n2->SetSelected(Canvas_n2);
            Canvas_n2->SaveAs(full_path2);
          }

 	else if(map_vec[iModule].layer>=37 && map_vec[iModule].layer<=44)
 	    {
 	      count++;
 	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
 	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
 	      Canvas_n2->SetFillColor(0);
 	      Canvas_n2->SetBorderMode(0);
 	      Canvas_n2->SetBorderSize(2);
 	      gStyle->SetOptFit(111111);
 	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

 	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
 	      //resp2->SetName("");
 	      gStyle->SetOptFit(1);
 	      gPad->SetLogy();
 	      resp2->GetXaxis()->SetTitle("number of hits");
 	      resp2->GetXaxis()->SetTitleSize(0.05);
 	      //resp2->GetXaxis()->SetLimits(0,400);
 	      resp2->SetLineColorAlpha(kRed,1);
 	      resp2->SetMarkerColor(kRed);
 	      resp2->SetLineWidth(2);
 	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
 	      func->SetParameter(0,1000); // set starting values
 	      func-> SetParameter(1,resp2->GetMean()); // set starting values
 	      func->SetParameter(2,resp2->GetRMS()+2);
 	      func->SetParameter(3,30000);
 	      func->SetParameter(4,resp2->GetMean());
 	      func->SetParameter(5,10000);
 	      func->SetParameter(6,resp2->GetMean()-2);
 	      func->SetParameter(7,resp2->GetRMS()+2);
 	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      
 	      //float r1 = resp2->GetMean()- 4*resp2->GetRMS();
 	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
 	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
 	      if(r1<0){
 	  	func->SetRange(0,r2);}
 	      else if(r1>0){
 	  	func->SetRange(r1,r2);}
 	      resp2->Fit("fit_func","ME");
 	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
 	      TF1* f3 = resp2->GetFunction("fit_func");
 	      TF1 *f_l =  (TF1*)f3->Clone("f3");
 	      constant1[iModule]= f_l->GetParameter(0);
 	      constant2[iModule]= f_l->GetParameter(3);
 	      constant3[iModule]=f_l->GetParameter(5);
 	      Mean_G[iModule]=f_l->GetParameter(1);
 	      Mean_P[iModule]=f_l->GetParameter(4);
 	      Sigma_G[iModule]=f_l->GetParameter(2);
 	      Mean_l[iModule]=f_l->GetParameter(6);
 	      Sigma_l[iModule]=f_l->GetParameter(7);
 	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
 	      f_l->SetRange(0, 400);
 	      resp2->Draw("");
 	     f_l->Draw("Lsame");
 	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
 	     legend->SetHeader("","C");
 	     legend->AddEntry(resp2,"orig","l");
 	     legend->AddEntry(f_l,"fit funct","l");
 	     gStyle->SetLegendTextSize(0.03);
 	     legend->Draw("sames");
 	      Canvas_n2->Modified();
 	      Canvas_n2->cd();
 	      Canvas_n2->SetSelected(Canvas_n2);
 	      Canvas_n2->SaveAs(full_path2);
 	    }
 	  else if(map_vec[iModule].layer>=45 && map_vec[iModule].layer<=50)
 	    {
 	      count++;
 	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
 	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
 	      Canvas_n2->SetFillColor(0);
 	      Canvas_n2->SetBorderMode(0);
 	      Canvas_n2->SetBorderSize(2);
 	      gStyle->SetOptFit(111111);
 	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

 	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
 	      //resp2->SetName("");
 	      gStyle->SetOptFit(1);
 	      gPad->SetLogy();
 	      resp2->GetXaxis()->SetTitle("number of hits");
 	      resp2->GetXaxis()->SetTitleSize(0.05);
 	      //resp2->GetXaxis()->SetLimits(0,400);
 	      resp2->SetLineColorAlpha(kRed,1);
 	      resp2->SetMarkerColor(kRed);
 	      resp2->SetLineWidth(2);
 	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
 	      func->SetParameter(0,4000); // set starting values
 	      func-> SetParameter(1,resp2->GetMean()); // set starting values
 	      func->SetParameter(2,resp2->GetRMS()+2);
 	      func->SetParameter(3,30000);
 	      func->SetParameter(4,resp2->GetMean());
 	      func->SetParameter(5,40000);
 	      func->SetParameter(6,resp2->GetMean()-2);
 	      func->SetParameter(7,resp2->GetRMS()+4);
 	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
 	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
 	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
 	      if(r1<0){
 	  	func->SetRange(0,r2);}
 	      else if(r1>0){
 	  	func->SetRange(r1,r2);}
 	      resp2->Fit("fit_func","ME");
 	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
 	      TF1* f3 = resp2->GetFunction("fit_func");
 	      TF1 *f_l =  (TF1*)f3->Clone("f3");
 	      constant1[iModule]= f_l->GetParameter(0);
 	      constant2[iModule]= f_l->GetParameter(3);
 	      constant3[iModule]=f_l->GetParameter(5);
 	      Mean_G[iModule]=f_l->GetParameter(1);
 	      Mean_P[iModule]=f_l->GetParameter(4);
 	      Sigma_G[iModule]=f_l->GetParameter(2);
 	      Mean_l[iModule]=f_l->GetParameter(6);
 	      Sigma_l[iModule]=f_l->GetParameter(7);
 	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
 	      f_l->SetRange(0, 400);
 	      resp2->Draw("");
 	     f_l->Draw("Lsame");
 	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
 	     legend->SetHeader("","C");
 	     legend->AddEntry(resp2,"orig","l");
 	     legend->AddEntry(f_l,"fit funct","l");
 	     gStyle->SetLegendTextSize(0.03);
 	     legend->Draw("sames");
 	      Canvas_n2->Modified();
 	      Canvas_n2->cd();
 	      Canvas_n2->SetSelected(Canvas_n2);
 	      Canvas_n2->SaveAs(full_path2);
 	    }
 	}
      else if ((map_vec[iModule].u>=6 && map_vec[iModule].u<=8) && (map_vec[iModule].v>=6 &&map_vec[iModule].v<=8))
	//if ((map_vec[iModule].u>=6 && map_vec[iModule].u<=8) && (map_vec[iModule].v>=6 &&map_vec[iModule].v<=8)) 
      	{
      	  if(map_vec[iModule].layer<=2)
      	    {
      	      count++;
      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
      	      Canvas_n2->SetFillColor(0);
      	      Canvas_n2->SetBorderMode(0);
      	      Canvas_n2->SetBorderSize(2);
      	      gStyle->SetOptFit(111111);
      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
      	      //resp2->SetName("");
      	      gStyle->SetOptFit(1);
      	      gPad->SetLogy();
      	      resp2->GetXaxis()->SetTitle("number of hits");
      	      resp2->GetXaxis()->SetTitleSize(0.05);
      	      //resp2->GetXaxis()->SetLimits(0,400);
      	      resp2->SetLineColorAlpha(kRed,1);
      	      resp2->SetMarkerColor(kRed);
      	      resp2->SetLineWidth(2);
      	      //TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::GammaDist(x,[4],0,1)+[5]*TMath::Landau(x,[6],[7])");
      	       TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
      	      //TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::GammaDist(x,[6],0,1)");
      	      func->SetParameter(0,1500); // set starting values
      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
      	      func->SetParameter(2,resp2->GetRMS());
      	      func->SetParameter(3,20000);
      	      // func->FixParameter(4,2);
      	      func->SetParameter(4,resp2->GetMean());
      	      func->SetParameter(5,2000);
      	      //func->FixParameter(6,0.001);
      	      func->SetParameter(6,2*(resp2->GetMean()));
      	      func->SetParameter(7,resp2->GetRMS()-4);
      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
      	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
      	      if(r1<0){
      		func->SetRange(0,r2);}
      	      else if(r1>0){
      		func->SetRange(r1,r2);}
      	      resp2->Fit("fit_func","ME");
      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
      	      TF1* f3 = resp2->GetFunction("fit_func");
      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
      	      constant1[iModule]= f_l->GetParameter(0);
      	      constant2[iModule]= f_l->GetParameter(3);
      	      constant3[iModule]=f_l->GetParameter(5);
      	      Mean_G[iModule]=f_l->GetParameter(1);
      	      Mean_P[iModule]=f_l->GetParameter(4);
      	      Sigma_G[iModule]=f_l->GetParameter(2);
      	      Mean_l[iModule]=f_l->GetParameter(6);
      	      Sigma_l[iModule]=f_l->GetParameter(7);
      	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
      	      f_l->SetRange(0, 400);
      	      resp2->Draw("");
      	     f_l->Draw("Lsame");
      	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
      	     legend->SetHeader("","C");
      	     legend->AddEntry(resp2,"orig","l");
      	     legend->AddEntry(f_l,"fit funct","l");
      	     gStyle->SetLegendTextSize(0.03);
      	     legend->Draw("sames");
      	      Canvas_n2->Modified();
      	      Canvas_n2->cd();
      	      Canvas_n2->SetSelected(Canvas_n2);
      	      Canvas_n2->SaveAs(full_path2);
      	    }
      	  else if(map_vec[iModule].layer<=10 && map_vec[iModule].layer>=3)
      	    {
      	      count++;
      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
      	      Canvas_n2->SetFillColor(0);
      	      Canvas_n2->SetBorderMode(0);
      	      Canvas_n2->SetBorderSize(2);
      	      gStyle->SetOptFit(111111);
      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
      	      //resp2->SetName("");
      	      gStyle->SetOptFit(1);
      	      gPad->SetLogy();
      	      resp2->GetXaxis()->SetTitle("number of hits");
      	      resp2->GetXaxis()->SetTitleSize(0.05);
      	      //resp2->GetXaxis()->SetLimits(0,400);
      	      resp2->SetLineColorAlpha(kRed,1);
      	      resp2->SetMarkerColor(kRed);
      	      resp2->SetLineWidth(2);
      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
      	      //TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::GammaDist(x,[6],0,1)");    
      	      func->SetParameter(0,2500); // set starting values
      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
      	      func->SetParameter(2,resp2->GetRMS());
      	      func->SetParameter(3,10000);
      	      func->SetParameter(4,resp2->GetMean());
      	      func->SetParameter(5,2000);
      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
      	      //func->SetParameter(6,resp2->GetMean());
      	      func->SetParameter(7,resp2->GetRMS());
      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
      	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
      	      if(r1<0){
      	  	func->SetRange(0,r2);}
      	      else if(r1>0){
      	  	func->SetRange(r1,r2);}
      	      resp2->Fit("fit_func","ME");
      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
      	      TF1* f3 = resp2->GetFunction("fit_func");
      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
      	      constant1[iModule]= f_l->GetParameter(0);
      	      constant2[iModule]= f_l->GetParameter(3);
      	      constant3[iModule]=f_l->GetParameter(5);
      	      Mean_G[iModule]=f_l->GetParameter(1);
      	      Mean_P[iModule]=f_l->GetParameter(4);
      	      Sigma_G[iModule]=f_l->GetParameter(2);
      	      Mean_l[iModule]=f_l->GetParameter(6);
      	      Sigma_l[iModule]=f_l->GetParameter(7);
      	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
      	      f_l->SetRange(0, 400);
      	      resp2->Draw("");
      	     f_l->Draw("Lsame");
      	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
      	     legend->SetHeader("","C");
      	     legend->AddEntry(resp2,"orig","l");
      	     legend->AddEntry(f_l,"fit funct","l");
      	     gStyle->SetLegendTextSize(0.03);
      	     legend->Draw("sames");
      	      Canvas_n2->Modified();
      	      Canvas_n2->cd();
      	      Canvas_n2->SetSelected(Canvas_n2);
      	      Canvas_n2->SaveAs(full_path2);
	      
      	    }
      	  else if(map_vec[iModule].layer>=11 && map_vec[iModule].layer<=21)
      	    {
      	      count++;
      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
      	      Canvas_n2->SetFillColor(0);
      	      Canvas_n2->SetBorderMode(0);
      	      Canvas_n2->SetBorderSize(2);
      	      gStyle->SetOptFit(111111);
      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
      	      //resp2->SetName("");
      	      gStyle->SetOptFit(1);
      	      gPad->SetLogy();
      	      resp2->GetXaxis()->SetTitle("number of hits");
      	      resp2->GetXaxis()->SetTitleSize(0.05);
      	      //resp2->GetXaxis()->SetLimits(0,400);
      	      resp2->SetLineColorAlpha(kRed,1);
      	      resp2->SetMarkerColor(kRed);
      	      resp2->SetLineWidth(2);
      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	      // if(map_vec[iModule].layer==18  && map_vec[iModule].u==8 && map_vec[iModule].v==8)
      	      func->SetParameter(0,2500); // set starting values
      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
      	      func->SetParameter(2,resp2->GetRMS());
      	      func->SetParameter(3,10000);
      	      func->SetParameter(4,resp2->GetMean());
      	      func->SetParameter(5,2000);
      	      func->SetParameter(6,resp2->GetMean()-1);//-0.5*resp2->GetMean());
      	      //func->SetParameter(6,resp2->GetMean());
      	      func->SetParameter(7,resp2->GetRMS()-1);
      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
      	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
      	      if(r1<0){
      	  	func->SetRange(0,r2);}
      	      else if(r1>0){
      	  	func->SetRange(r1,r2);}
      	      resp2->Fit("fit_func","ME");
      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
      	      TF1* f3 = resp2->GetFunction("fit_func");
      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
      	      constant1[iModule]= f_l->GetParameter(0);
      	      constant2[iModule]= f_l->GetParameter(3);
      	      constant3[iModule]=f_l->GetParameter(5);
      	      Mean_G[iModule]=f_l->GetParameter(1);
      	      Mean_P[iModule]=f_l->GetParameter(4);
      	      Sigma_G[iModule]=f_l->GetParameter(2);
      	      Mean_l[iModule]=f_l->GetParameter(6);
      	      Sigma_l[iModule]=f_l->GetParameter(7);
      	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
      	      f_l->SetRange(0, 400);
      	      resp2->Draw("");
      	     f_l->Draw("Lsame");
      	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
      	     legend->SetHeader("","C");
      	     legend->AddEntry(resp2,"orig","l");
      	     legend->AddEntry(f_l,"fit funct","l");
      	     gStyle->SetLegendTextSize(0.03);
      	     legend->Draw("sames");
      	      Canvas_n2->Modified();
      	      Canvas_n2->cd();
      	      Canvas_n2->SetSelected(Canvas_n2);
      	      Canvas_n2->SaveAs(full_path2);
	      
      	    }
      	  else if(map_vec[iModule].layer>=22 && map_vec[iModule].layer<=42)
            {
              count++;
              TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
              Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
              Canvas_n2->SetFillColor(0);
              Canvas_n2->SetBorderMode(0);
              Canvas_n2->SetBorderSize(2);
              gStyle->SetOptFit(111111);
              sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

              TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
              //resp2->SetName("");                                                                                                                                                                        
              gStyle->SetOptFit(1);
              gPad->SetLogy();
              resp2->GetXaxis()->SetTitle("number of hits");
              resp2->GetXaxis()->SetTitleSize(0.05);
              //resp2->GetXaxis()->SetLimits(0,400);
              resp2->SetLineColorAlpha(kRed,1);
              resp2->SetMarkerColor(kRed);
              resp2->SetLineWidth(2);
              TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	      if((map_vec[iModule].layer==29  && map_vec[iModule].u==8 && map_vec[iModule].v==8)||(map_vec[iModule].layer==39  && map_vec[iModule].u==7 && map_vec[iModule].v==8))//||(map_vec[iModule].layer==41  && map_vec[iModule].u==5 && map_vec[iModule].v==6))
		{
		  func->SetParameter(0,25000); 
              func-> SetParameter(1,0.5*resp2->GetMean());
              func->SetParameter(2,0.5*resp2->GetRMS());
              func->SetParameter(3,100000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,200000);
              func->SetParameter(6,0.3*resp2->GetMean());
	      func->SetParameter(7,0.2*resp2->GetRMS());
		}
	      // else if (map_vec[iModule].layer==41  && map_vec[iModule].u==5 && map_vec[iModule].v==6)
	      // 	{
	      // 	      func->SetParameter(0,250);
	      // 	      func-> SetParameter(1,0.5*resp2->GetMean());
	      // 	      func->SetParameter(2,0.5*resp2->GetRMS());
	      // 	      func->SetParameter(3,100000);
	      // 	      func->SetParameter(4,resp2->GetMean());
	      // 	      func->SetParameter(5,200000);
	      // 	      func->SetParameter(6,0.3*resp2->GetMean());
	      // 	      func->SetParameter(7,0.2*resp2->GetRMS());

	      // 	}
	      else
		{
              func->SetParameter(0,2500); // set starting values                                                                                                                                           
              func-> SetParameter(1,resp2->GetMean()-2); // set starting values                                                                                                                              
              func->SetParameter(2,resp2->GetRMS());
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,2000);
              func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());                                                                                                                          
              func->SetParameter(7,resp2->GetRMS());
		}
              func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
              float r1 = resp2->GetMean()- 4*resp2->GetRMS();
              float r2 = resp2->GetMean()+4*resp2->GetRMS();

              if(r1<0){
                func->SetRange(0,r2);}
              else if(r1>0){
                func->SetRange(r1,r2);}
              resp2->Fit("fit_func","ME");
      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
      	      TF1* f3 = resp2->GetFunction("fit_func");
              TF1 *f_l =  (TF1*)f3->Clone("f3");
              constant1[iModule]= f_l->GetParameter(0);
              constant2[iModule]= f_l->GetParameter(3);
              constant3[iModule]=f_l->GetParameter(5);
              Mean_G[iModule]=f_l->GetParameter(1);
              Mean_P[iModule]=f_l->GetParameter(4);
              Sigma_G[iModule]=f_l->GetParameter(2);
              Mean_l[iModule]=f_l->GetParameter(6);
              Sigma_l[iModule]=f_l->GetParameter(7);
              file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              f_l->SetRange(0, 400);
              resp2->Draw("");
             f_l->Draw("Lsame");
             auto legend = new TLegend(0.75,0.25,0.9,0.32);
             legend->SetHeader("","C");
             legend->AddEntry(resp2,"orig","l");
             legend->AddEntry(f_l,"fit funct","l");
             gStyle->SetLegendTextSize(0.03);
             legend->Draw("sames");
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);

            }


      	  else if(map_vec[iModule].layer>=43 && map_vec[iModule].layer<=47)
      	    {
      	      count++;
      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
      	      Canvas_n2->SetFillColor(0);
      	      Canvas_n2->SetBorderMode(0);
      	      Canvas_n2->SetBorderSize(2);
      	      gStyle->SetOptFit(111111);
      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
      	      //resp2->SetName("");
      	      gStyle->SetOptFit(1);
      	      gPad->SetLogy();
      	      resp2->GetXaxis()->SetTitle("number of hits");
      	      resp2->GetXaxis()->SetTitleSize(0.05);
      	      //resp2->GetXaxis()->SetLimits(0,400);
      	      resp2->SetLineColorAlpha(kRed,1);
      	      resp2->SetMarkerColor(kRed);
      	      resp2->SetLineWidth(2);
	      
      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
      	      func->SetParameter(0,40000); // set starting values
      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
      	      func->SetParameter(2,resp2->GetRMS());
      	      func->SetParameter(3,10000);
      	      func->SetParameter(4,resp2->GetMean());
      	      func->SetParameter(5,2000);
      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
      	      //func->SetParameter(6,resp2->GetMean());
      	      func->SetParameter(7,resp2->GetRMS());
      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
      	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
	      
      	      if(r1<0){
      	  	func->SetRange(0,r2);}
      	      else if(r1>0){
      	  	func->SetRange(r1,r2);}
      	      resp2->Fit("fit_func","ME");
      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
      	      TF1* f3 = resp2->GetFunction("fit_func");
      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
      	      constant1[iModule]= f_l->GetParameter(0);
      	      constant2[iModule]= f_l->GetParameter(3);
      	      constant3[iModule]=f_l->GetParameter(5);
      	      Mean_G[iModule]=f_l->GetParameter(1);
      	      Mean_P[iModule]=f_l->GetParameter(4);
      	      Sigma_G[iModule]=f_l->GetParameter(2);
      	      Mean_l[iModule]=f_l->GetParameter(6);
      	      Sigma_l[iModule]=f_l->GetParameter(7);
      	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
      	      f_l->SetRange(0, 400);
      	      resp2->Draw("");
      	     f_l->Draw("Lsame");
      	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
      	     legend->SetHeader("","C");
      	     legend->AddEntry(resp2,"orig","l");
      	     legend->AddEntry(f_l,"fit funct","l");
      	     gStyle->SetLegendTextSize(0.03);
      	     legend->Draw("sames");
      	      Canvas_n2->Modified();
      	      Canvas_n2->cd();
      	      Canvas_n2->SetSelected(Canvas_n2);
      	      Canvas_n2->SaveAs(full_path2);
	      
      	    }
      	  else if(map_vec[iModule].layer>=48 && map_vec[iModule].layer<=50)
      	    {
      	      count++;
      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
      	      Canvas_n2->SetFillColor(0);
      	      Canvas_n2->SetBorderMode(0);
      	      Canvas_n2->SetBorderSize(2);
      	      gStyle->SetOptFit(111111);
      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
      	      //resp2->SetName("");
      	      gStyle->SetOptFit(1);
      	      gPad->SetLogy();
      	      resp2->GetXaxis()->SetTitle("number of hits");
      	      resp2->GetXaxis()->SetTitleSize(0.05);
      	      //resp2->GetXaxis()->SetLimits(0,400);
      	      resp2->SetLineColorAlpha(kRed,1);
      	      resp2->SetMarkerColor(kRed);
      	      resp2->SetLineWidth(2);

      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
      	      func->SetParameter(0,100000); // set starting values
      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
      	      func->SetParameter(2,resp2->GetRMS());
      	      func->SetParameter(3,30000);
      	      func->SetParameter(4,resp2->GetMean());
      	      func->SetParameter(5,1000);
      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
      	      //func->SetParameter(6,resp2->GetMean());
      	      func->SetParameter(7,resp2->GetRMS());
      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
      	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
	      
      	      if(r1<0){
      		func->SetRange(0,r2);}
      	      else if(r1>0){
      		func->SetRange(r1,r2);}
      	      resp2->Fit("fit_func","ME");
      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
      	      TF1* f3 = resp2->GetFunction("fit_func");
      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
      	      constant1[iModule]= f_l->GetParameter(0);
      	      constant2[iModule]= f_l->GetParameter(3);
      	      constant3[iModule]=f_l->GetParameter(5);
      	      Mean_G[iModule]=f_l->GetParameter(1);
      	      Mean_P[iModule]=f_l->GetParameter(4);
      	      Sigma_G[iModule]=f_l->GetParameter(2);
      	      Mean_l[iModule]=f_l->GetParameter(6);
      	      Sigma_l[iModule]=f_l->GetParameter(7);
      	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
      	      f_l->SetRange(0, 400);
      	      resp2->Draw("");
      	     f_l->Draw("Lsame");
      	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
      	     legend->SetHeader("","C");
      	     legend->AddEntry(resp2,"orig","l");
      	     legend->AddEntry(f_l,"fit funct","l");
      	     gStyle->SetLegendTextSize(0.03);
      	     legend->Draw("sames");
      	      Canvas_n2->Modified();
      	      Canvas_n2->cd();
      	      Canvas_n2->SetSelected(Canvas_n2);
      	      Canvas_n2->SaveAs(full_path2);

      	    }
	  
      	}
      //else  if (((map_vec[iModule].v==9||map_vec[iModule].v==10||map_vec[iModule].v==11) && map_vec[iModule].u<=15) ||((map_vec[iModule].u==9||map_vec[iModule].u==10||map_vec[iModule].u==11) && map_vec[iModule].v<=15)||( map_vec[iModule].u==15 &&  map_vec[iModule].v<=15))
      else  if(( map_vec[iModule].v>=9&& map_vec[iModule].u<=15)||( map_vec[iModule].u>=9 && map_vec[iModule].v<=15))
       	{
	  if(map_vec[iModule].layer<=13&&map_vec[iModule].layer>=1)
	    {
	      count++;
	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      Canvas_n2->SetFillColor(0);
	      Canvas_n2->SetBorderMode(0);
	      Canvas_n2->SetBorderSize(2);
	      gStyle->SetOptFit(111111);
	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
	      
	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      //resp2->SetName("");
	      gStyle->SetOptFit(1);
	      gPad->SetLogy();
	      resp2->GetXaxis()->SetTitle("number of hits");
	      resp2->GetXaxis()->SetTitleSize(0.05);
	      //resp2->GetXaxis()->SetLimits(0,400);
	      resp2->SetLineColorAlpha(kRed,1);
	      resp2->SetMarkerColor(kRed);
	      resp2->SetLineWidth(2);
	      
	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	      func->SetParameter(0,2000); // set starting values
	      func-> SetParameter(1,resp2->GetMean()); // set starting values
	      func->SetParameter(2,resp2->GetRMS());
	      func->SetParameter(3,10000);
	      func->SetParameter(4,resp2->GetMean());
	      func->SetParameter(5,5000);
	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
	      //func->SetParameter(6,resp2->GetMean());
	      func->SetParameter(7,resp2->GetRMS());
	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      
	      if(r1<0){
	  	func->SetRange(0,r2);}
	      else if(r1>0){
	  	func->SetRange(r1,r2);}
	      resp2->Fit("fit_func","ME");
	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      TF1* f3 = resp2->GetFunction("fit_func");
	      TF1 *f_l =  (TF1*)f3->Clone("f3");
	      constant1[iModule]= f_l->GetParameter(0);
	      constant2[iModule]= f_l->GetParameter(3);
	      constant3[iModule]=f_l->GetParameter(5);
	      Mean_G[iModule]=f_l->GetParameter(1);
	      Mean_P[iModule]=f_l->GetParameter(4);
	      Sigma_G[iModule]=f_l->GetParameter(2);
	      Mean_l[iModule]=f_l->GetParameter(6);
	      Sigma_l[iModule]=f_l->GetParameter(7);
	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      f_l->SetRange(0, 400);
	      resp2->Draw("");
	      f_l->Draw("Lsame");
	      auto legend = new TLegend(0.75,0.25,0.9,0.32);
	      legend->SetHeader("","C");
	      legend->AddEntry(resp2,"orig","l");
	      legend->AddEntry(f_l,"fit funct","l");
	      gStyle->SetLegendTextSize(0.03);
	      legend->Draw("sames");
	      Canvas_n2->Modified();
	      Canvas_n2->cd();
	      Canvas_n2->SetSelected(Canvas_n2);
	      Canvas_n2->SaveAs(full_path2);
	    }
	  
	  else  if(map_vec[iModule].layer<=19 && map_vec[iModule].layer>=14)
            {
              count++;
              TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
              Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
              Canvas_n2->SetFillColor(0);
              Canvas_n2->SetBorderMode(0);
              Canvas_n2->SetBorderSize(2);
              gStyle->SetOptFit(111111);
              sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

              TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
              //resp2->SetName("");                                                                                                                                                                        
              gStyle->SetOptFit(1);
              gPad->SetLogy();
              resp2->GetXaxis()->SetTitle("number of hits");
              resp2->GetXaxis()->SetTitleSize(0.05);
              //resp2->GetXaxis()->SetLimits(0,400);
              resp2->SetLineColorAlpha(kRed,1);
              resp2->SetMarkerColor(kRed);
              resp2->SetLineWidth(2);

              TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	      if ((map_vec[iModule].layer==15 && map_vec[iModule].u ==10 && map_vec[iModule].v==5)||(map_vec[iModule].layer==16 && map_vec[iModule].u ==6 && map_vec[iModule].v==10))//|| (map_vec[iModule].layer==18 && map_vec[iModule].u ==8 && map_vec[iModule].v==9))
		{
		  func->SetParameter(0,100); // set starting values                                                                                                
		  func-> SetParameter(1,0.5*resp2->GetMean()); // set starting values                                                                                        
              func->SetParameter(2,0.05*resp2->GetRMS());
              func->SetParameter(3,1000000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,50000);
              func->SetParameter(6,0.3*resp2->GetMean());//-0.5*resp2->GetMean());                                                                                    
             func->SetParameter(7,0.05*resp2->GetRMS());

		}
	      else if (map_vec[iModule].layer==18 && map_vec[iModule].u ==8 && map_vec[iModule].v==9)
		{
		  func->SetParameter(0,100); 
                  func-> SetParameter(1,0.5*resp2->GetMean()); 
              func->SetParameter(2,resp2->GetRMS());
              func->SetParameter(3,1000000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,500);
              func->SetParameter(6,0.003*resp2->GetMean());
              func->SetParameter(7,0.05*resp2->GetRMS());

		}
	      else
		{
              func->SetParameter(0,1000); // set starting values                                                                                                                                          
              func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                             
              func->SetParameter(2,resp2->GetRMS()-1);
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,5000);
              func->SetParameter(6,resp2->GetMean()-2);//-0.5*resp2->GetMean());                                                                                                                         
              func->SetParameter(7,resp2->GetRMS()-1);
		}
              func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
              float r1 = resp2->GetMean()- 4*resp2->GetRMS();
              float r2 = resp2->GetMean()+4*resp2->GetRMS();
	      if(r1<0){
                func->SetRange(0,r2);}
              else if(r1>0){
                func->SetRange(r1,r2);}
              resp2->Fit("fit_func","ME");
              resp2->GetFunction("fit_func")->SetLineColor(kBlack);
              TF1* f3 = resp2->GetFunction("fit_func");
              TF1 *f_l =  (TF1*)f3->Clone("f3");
              constant1[iModule]= f_l->GetParameter(0);
              constant2[iModule]= f_l->GetParameter(3);
              constant3[iModule]=f_l->GetParameter(5);
              Mean_G[iModule]=f_l->GetParameter(1);
              Mean_P[iModule]=f_l->GetParameter(4);
              Sigma_G[iModule]=f_l->GetParameter(2);
              Mean_l[iModule]=f_l->GetParameter(6);
              Sigma_l[iModule]=f_l->GetParameter(7);
              file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              f_l->SetRange(0, 400);
              resp2->Draw("");
             f_l->Draw("Lsame");
             auto legend = new TLegend(0.75,0.25,0.9,0.32);
             legend->SetHeader("","C");
             legend->AddEntry(resp2,"orig","l");
             legend->AddEntry(f_l,"fit funct","l");
             gStyle->SetLegendTextSize(0.03);
             legend->Draw("sames");
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);
	    }
	    else if(map_vec[iModule].layer<=25 && map_vec[iModule].layer>=20)
            {
              count++;
              TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
              Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
              Canvas_n2->SetFillColor(0);
              Canvas_n2->SetBorderMode(0);
              Canvas_n2->SetBorderSize(2);
              gStyle->SetOptFit(111111);
              sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

              TH1F *resp2 = (TH1F*)inputfile->Get(hist_name); 
              gStyle->SetOptFit(1);
              gPad->SetLogy();
              resp2->GetXaxis()->SetTitle("number of hits");
              resp2->GetXaxis()->SetTitleSize(0.05);
              //resp2->GetXaxis()->SetLimits(0,400);
              resp2->SetLineColorAlpha(kRed,1);
              resp2->SetMarkerColor(kRed);
              resp2->SetLineWidth(2);

              TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
              func->SetParameter(0,100); // set starting values                                                                                                                                          
              func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                             
              func->SetParameter(2,resp2->GetRMS()-2);
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,5000);
              func->SetParameter(6,resp2->GetMean()-2);//-0.5*resp2->GetMean());                                                                                                                           
              func->SetParameter(7,resp2->GetRMS());
              func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
              float r1 = resp2->GetMean()- 4*resp2->GetRMS();
              float r2 = resp2->GetMean()+4*resp2->GetRMS();
              if(r1<0){
                func->SetRange(0,r2);}
              else if(r1>0){
	    	func->SetRange(r1,r2);}
              resp2->Fit("fit_func","ME");
              resp2->GetFunction("fit_func")->SetLineColor(kBlack);
              TF1* f3 = resp2->GetFunction("fit_func");
              TF1 *f_l =  (TF1*)f3->Clone("f3");
              constant1[iModule]= f_l->GetParameter(0);
              constant2[iModule]= f_l->GetParameter(3);
              constant3[iModule]=f_l->GetParameter(5);
              Mean_G[iModule]=f_l->GetParameter(1);
              Mean_P[iModule]=f_l->GetParameter(4);
              Sigma_G[iModule]=f_l->GetParameter(2);
              Mean_l[iModule]=f_l->GetParameter(6);
              Sigma_l[iModule]=f_l->GetParameter(7);
              file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              f_l->SetRange(0, 400);
              resp2->Draw("");
             f_l->Draw("Lsame");
             auto legend = new TLegend(0.75,0.25,0.9,0.32);
             legend->SetHeader("","C");
             legend->AddEntry(resp2,"orig","l");
             legend->AddEntry(f_l,"fit funct","l");
             gStyle->SetLegendTextSize(0.03);
             legend->Draw("sames");
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);
            }
	    else if(map_vec[iModule].layer<=35 && map_vec[iModule].layer>=26)
            {
              count++;
              TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
              Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
              Canvas_n2->SetFillColor(0);
              Canvas_n2->SetBorderMode(0);
              Canvas_n2->SetBorderSize(2);
              gStyle->SetOptFit(111111);
              sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

              TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
       
              gStyle->SetOptFit(1);
              gPad->SetLogy();
              resp2->GetXaxis()->SetTitle("number of hits");
              resp2->GetXaxis()->SetTitleSize(0.05);
              //resp2->GetXaxis()->SetLimits(0,400);
              resp2->SetLineColorAlpha(kRed,1);
              resp2->SetMarkerColor(kRed);
              resp2->SetLineWidth(2);
	      
              TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	      if((map_vec[iModule].u==6 && map_vec[iModule].v ==11 && map_vec[iModule].layer==26) || (map_vec[iModule].u==5 && map_vec[iModule].v ==11 && map_vec[iModule].layer==28))
		{
		  func->SetParameter(0,100); // set starting values
		  func-> SetParameter(1,resp2->GetMean()-2); // set starting values                                                                                              
              func->SetParameter(2,resp2->GetRMS()-2);
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,5050);
              func->SetParameter(6,resp2->GetMean());
              func->SetParameter(7,resp2->GetRMS()-2);

		}
	      else if( (map_vec[iModule].layer==31 && map_vec[iModule].u ==8 && map_vec[iModule].v==10)|| (map_vec[iModule].layer==33 && map_vec[iModule].u ==9 && map_vec[iModule].v==8)||(map_vec[iModule].layer==34 && map_vec[iModule].u ==9 && map_vec[iModule].v==1)||(map_vec[iModule].layer==34 && map_vec[iModule].u ==8 && map_vec[iModule].v==10)|| (map_vec[iModule].layer==35 && map_vec[iModule].u ==9 && map_vec[iModule].v==3)|| (map_vec[iModule].layer==35 && map_vec[iModule].u ==5 && map_vec[iModule].v==9))
                {
                  func->SetParameter(0,100);
                  func-> SetParameter(1,0.5*resp2->GetMean());
              func->SetParameter(2,0.05*resp2->GetRMS());
              func->SetParameter(3,1000000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,50000);
              func->SetParameter(6,0.3*resp2->GetMean());
              func->SetParameter(7,0.5*resp2->GetRMS());

                }

	      else
		{
              func->SetParameter(0,100); // set starting values                                                                                                                                            
              func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                             
              func->SetParameter(2,resp2->GetRMS());
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,5000);
              func->SetParameter(6,resp2->GetMean()); 
              func->SetParameter(7,resp2->GetRMS()-2);
		}
              func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
              float r1 = resp2->GetMean()- 4*resp2->GetRMS();
              float r2 = resp2->GetMean()+4*resp2->GetRMS();
              if(r1<0){
                func->SetRange(0,r2);}
              else if(r1>0){
                func->SetRange(r1,r2);}
              resp2->Fit("fit_func","ME");
              resp2->GetFunction("fit_func")->SetLineColor(kBlack);
              TF1* f3 = resp2->GetFunction("fit_func");
              TF1 *f_l =  (TF1*)f3->Clone("f3");
              constant1[iModule]= f_l->GetParameter(0);
              constant2[iModule]= f_l->GetParameter(3);
              constant3[iModule]=f_l->GetParameter(5);
	      Mean_G[iModule]=f_l->GetParameter(1);
              Mean_P[iModule]=f_l->GetParameter(4);
              Sigma_G[iModule]=f_l->GetParameter(2);
              Mean_l[iModule]=f_l->GetParameter(6);
              Sigma_l[iModule]=f_l->GetParameter(7);
              file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              f_l->SetRange(0, 400);
              resp2->Draw("");
             f_l->Draw("Lsame");
             auto legend = new TLegend(0.75,0.25,0.9,0.32);
             legend->SetHeader("","C");
             legend->AddEntry(resp2,"orig","l");
             legend->AddEntry(f_l,"fit funct","l");
             gStyle->SetLegendTextSize(0.03);
             legend->Draw("sames");
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);
            }
	    else if(map_vec[iModule].layer<=50 && map_vec[iModule].layer>=36)
            {
              count++;
              TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
              Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
              Canvas_n2->SetFillColor(0);
              Canvas_n2->SetBorderMode(0);
              Canvas_n2->SetBorderSize(2);
              gStyle->SetOptFit(111111);
              sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

              TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);

              gStyle->SetOptFit(1);
              gPad->SetLogy();
              resp2->GetXaxis()->SetTitle("number of hits");
              resp2->GetXaxis()->SetTitleSize(0.05);
              //resp2->GetXaxis()->SetLimits(0,400);
              resp2->SetLineColorAlpha(kRed,1);
              resp2->SetMarkerColor(kRed);
              resp2->SetLineWidth(2);

              TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	       if(map_vec[iModule].u==9 && map_vec[iModule].v ==8 && map_vec[iModule].layer==37)
                {
		  func->SetParameter(0,100); // set starting values                                                                                                                                      
              func-> SetParameter(1,resp2->GetMean()-2); // set starting values                                                                                                                             
              func->SetParameter(2,resp2->GetRMS());
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,5000);
              func->SetParameter(6,resp2->GetMean()-2);
              func->SetParameter(7,resp2->GetRMS()-2);

		}
	        else if (map_vec[iModule].layer==46  && map_vec[iModule].u==15 && map_vec[iModule].v==11)
                {
                      func->SetParameter(0,250);
                      func-> SetParameter(1,0.5*resp2->GetMean());
                      func->SetParameter(2,0.5*resp2->GetRMS());
                      func->SetParameter(3,100000);
                      func->SetParameter(4,resp2->GetMean());
                      func->SetParameter(5,20000);
                      func->SetParameter(6,0.3*resp2->GetMean());
                      func->SetParameter(7,resp2->GetRMS());

                }

	       else if (map_vec[iModule].u==5 && map_vec[iModule].v ==8 && map_vec[iModule].layer==37)
		 {
		   func->SetParameter(0,100);
		   func-> SetParameter(1,0.5*resp2->GetMean());
		   func->SetParameter(2,0.1*resp2->GetRMS());
		   func->SetParameter(3,100000);
		   func->SetParameter(4,resp2->GetMean());
		   func->SetParameter(5,5000);
		   func->SetParameter(6,0.5*resp2->GetMean());
		   func->SetParameter(7,0.3*resp2->GetRMS());

		 }
	       else
		 {
              func->SetParameter(0,1000); // set starting values                                                                                                                                            
              func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                             
              func->SetParameter(2,resp2->GetRMS());
              func->SetParameter(3,10000);
              func->SetParameter(4,resp2->GetMean());
              func->SetParameter(5,5000);
              func->SetParameter(6,resp2->GetMean()-2);
              func->SetParameter(7,resp2->GetRMS()-2);
		 }
              func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
              float r1 = resp2->GetMean()- 4*resp2->GetRMS();
              float r2 = resp2->GetMean()+4*resp2->GetRMS();
              if(r1<0){
                func->SetRange(0,r2);}
              else if(r1>0){
                func->SetRange(r1,r2);}
              resp2->Fit("fit_func","ME");
              resp2->GetFunction("fit_func")->SetLineColor(kBlack);
              TF1* f3 = resp2->GetFunction("fit_func");
              TF1 *f_l =  (TF1*)f3->Clone("f3");
              constant1[iModule]= f_l->GetParameter(0);
              constant2[iModule]= f_l->GetParameter(3);
              constant3[iModule]=f_l->GetParameter(5);
	       Mean_G[iModule]=f_l->GetParameter(1);
              Mean_P[iModule]=f_l->GetParameter(4);
              Sigma_G[iModule]=f_l->GetParameter(2);
              Mean_l[iModule]=f_l->GetParameter(6);
              Sigma_l[iModule]=f_l->GetParameter(7);
              file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
              f_l->SetRange(0, 400);
              resp2->Draw("");
             f_l->Draw("Lsame");
             auto legend = new TLegend(0.75,0.25,0.9,0.32);
             legend->SetHeader("","C");
             legend->AddEntry(resp2,"orig","l");
             legend->AddEntry(f_l,"fit funct","l");
             gStyle->SetLegendTextSize(0.03);
             legend->Draw("sames");
              Canvas_n2->Modified();
              Canvas_n2->cd();
              Canvas_n2->SetSelected(Canvas_n2);
              Canvas_n2->SaveAs(full_path2);
            }

    
      }
	   if((map_vec[iModule].u<=5 &&(map_vec[iModule].v==6||map_vec[iModule].v==7||map_vec[iModule].v==8))||(map_vec[iModule].v<=5 &&(map_vec[iModule].u==6||map_vec[iModule].u==7||map_vec[iModule].u==8)))
	    {
	      if(map_vec[iModule].layer<=24)
	      	// if(map_vec[iModule].layer<=0)
	      	{
	      	  count++;
	      	  TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      	  Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      	  Canvas_n2->SetFillColor(0);
	      	  Canvas_n2->SetBorderMode(0);
	      	  Canvas_n2->SetBorderSize(2);
	      	  gStyle->SetOptFit(111111);
	      	  sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
		  
	      	  TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      	  //resp2->SetName("");
	      	  gStyle->SetOptFit(1);
	      	  gPad->SetLogy();
	      	  resp2->GetXaxis()->SetTitle("number of hits");
	      	  resp2->GetXaxis()->SetTitleSize(0.05);
	      	  //resp2->GetXaxis()->SetLimits(0,400);
	      	  resp2->SetLineColorAlpha(kRed,1);
	      	  resp2->SetMarkerColor(kRed);
	      	  resp2->SetLineWidth(2);

	      	  TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
	      
	      	  func->SetParameter(0,1000); // set starting values
	      	  func-> SetParameter(1,resp2->GetMean()); // set starting values
	      	  func->SetParameter(2,resp2->GetRMS());
	      	  func->SetParameter(3,10000);
	      	  func->SetParameter(4,resp2->GetMean());
	      	  func->SetParameter(5,5000);
	      	  func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
	      	  //func->SetParameter(6,resp2->GetMean());
	      	  func->SetParameter(7,resp2->GetRMS());
	      	  func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      	  float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	      	  float r2 = resp2->GetMean()+4*resp2->GetRMS();
		  
	      	  if(r1<0){
	      	    func->SetRange(0,r2);}
	      	  else if(r1>0){
	      	    func->SetRange(r1,r2);}
	      	  resp2->Fit("fit_func","ME");
	      	  resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      	  TF1* f3 = resp2->GetFunction("fit_func");
	      	  TF1 *f_l =  (TF1*)f3->Clone("f3");
	      	  constant1[iModule]= f_l->GetParameter(0);
	      	  constant2[iModule]= f_l->GetParameter(3);
	      	  constant3[iModule]=f_l->GetParameter(5);
	      	  Mean_G[iModule]=f_l->GetParameter(1);
	      	  Mean_P[iModule]=f_l->GetParameter(4);
	      	  Sigma_G[iModule]=f_l->GetParameter(2);
	      	  Mean_l[iModule]=f_l->GetParameter(6);
	      	  Sigma_l[iModule]=f_l->GetParameter(7);
	      	  file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      	  f_l->SetRange(0, 400);
	      	  resp2->Draw("");
	      	 f_l->Draw("Lsame");
	      	 auto legend = new TLegend(0.75,0.25,0.9,0.32);
	      	 legend->SetHeader("","C");
	      	 legend->AddEntry(resp2,"orig","l");
	      	 legend->AddEntry(f_l,"fit funct","l");
	      	 gStyle->SetLegendTextSize(0.03);
	      	 legend->Draw("sames");
	      	  Canvas_n2->Modified();
	      	  Canvas_n2->cd();
	      	  Canvas_n2->SetSelected(Canvas_n2);
	      	  Canvas_n2->SaveAs(full_path2);
	      	}

	      
	 else if(map_vec[iModule].layer>=25 &&map_vec[iModule].layer<=30)
	      	{
	      	  if((map_vec[iModule].u<=5 &&map_vec[iModule].v==8)||(map_vec[iModule].v<=5 &&map_vec[iModule].u==8))
	      	    {
	 	      count++;
	      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      	      Canvas_n2->SetFillColor(0);
	      	      Canvas_n2->SetBorderMode(0);
	      	      Canvas_n2->SetBorderSize(2);
	      	      gStyle->SetOptFit(111111);
	      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

	      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      	      //resp2->SetName("");
	      	      gStyle->SetOptFit(1);
	      	      gPad->SetLogy();
	      	      resp2->GetXaxis()->SetTitle("number of hits");
	      	      resp2->GetXaxis()->SetTitleSize(0.05);
	      	      //resp2->GetXaxis()->SetLimits(0,400);
	      	      resp2->SetLineColorAlpha(kRed,1);
	      	      resp2->SetMarkerColor(kRed);
	      	      resp2->SetLineWidth(2);
	      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");

	      	      func->SetParameter(0,1000); // set starting values
	      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
	      	      func->SetParameter(2,resp2->GetRMS());
	      	      func->SetParameter(3,10000);
	      	      func->SetParameter(4,resp2->GetMean());
	      	      func->SetParameter(5,1500);
	      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
	      	      //func->SetParameter(6,resp2->GetMean());
	      	      func->SetParameter(7,resp2->GetRMS());
	      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	 	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	 	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
		      
	      	      if(r1<0){
	      		func->SetRange(0,r2);}
	      	      else if(r1>0){
	      		func->SetRange(r1,r2);}
	      	      resp2->Fit("fit_func","ME");
	      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      	      TF1* f3 = resp2->GetFunction("fit_func");
	      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
	 	      constant1[iModule]= f_l->GetParameter(0);
	 	      constant2[iModule]= f_l->GetParameter(3);
	 	      constant3[iModule]=f_l->GetParameter(5);
	 	      Mean_G[iModule]=f_l->GetParameter(1);
	 	      Mean_P[iModule]=f_l->GetParameter(4);
	 	      Sigma_G[iModule]=f_l->GetParameter(2);
	 	      Mean_l[iModule]=f_l->GetParameter(6);
	 	      Sigma_l[iModule]=f_l->GetParameter(7);
	 	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      	      f_l->SetRange(0, 400);
	      	      resp2->Draw("");
	      	     f_l->Draw("Lsame");
	 	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
	 	     legend->SetHeader("","C");
	 	     legend->AddEntry(resp2,"orig","l");
	 	     legend->AddEntry(f_l,"fit funct","l");
	 	     gStyle->SetLegendTextSize(0.03);
	 	     legend->Draw("sames");
	      	      Canvas_n2->Modified();
	      	      Canvas_n2->cd();
	      	      Canvas_n2->SetSelected(Canvas_n2);
	      	      Canvas_n2->SaveAs(full_path2);
		      
	      	    }
		
	      	  else
	      	    {
	 	      count++;
	      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      	      Canvas_n2->SetFillColor(0);
	      	      Canvas_n2->SetBorderMode(0);
	      	      Canvas_n2->SetBorderSize(2);
	      	      gStyle->SetOptFit(111111);
	      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

	      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      	      //resp2->SetName("");
	      	      gStyle->SetOptFit(1);
	      	      gPad->SetLogy();
	      	      resp2->GetXaxis()->SetTitle("number of hits");
	      	      resp2->GetXaxis()->SetTitleSize(0.05);
	      	      //resp2->GetXaxis()->SetLimits(0,400);
	      	      resp2->SetLineColorAlpha(kRed,1);
	      	      resp2->SetMarkerColor(kRed);
	      	      resp2->SetLineWidth(2);
	      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
		      if(map_vec[iModule].layer==26 && map_vec[iModule].u ==6 && map_vec[iModule].v==11)
			{
			  func->SetParameter(0,1000); // set starting values                                                                                                                             
                      func-> SetParameter(1,resp2->GetMean()-2); // set starting values                                                                                                                  
                      func->SetParameter(2,resp2->GetRMS());
                      func->SetParameter(3,10000);
                      func->SetParameter(4,resp2->GetMean());
                      func->SetParameter(5,5000);
                      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());                                                                                                                   
                      func->SetParameter(7,resp2->GetRMS());

			}
		      else
			{
	      	      func->SetParameter(0,1000); // set starting values
	      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
	      	      func->SetParameter(2,resp2->GetRMS());
	      	      func->SetParameter(3,10000);
	      	      func->SetParameter(4,resp2->GetMean());
	      	      func->SetParameter(5,700);
	      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
	      	      //func->SetParameter(6,resp2->GetMean());
	      	      func->SetParameter(7,resp2->GetRMS());
			}
	      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	 	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
		      
	      	      if(r1<0){
	      		func->SetRange(0,r2);}
	      	      else if(r1>0){
	      		func->SetRange(r1,r2);}
	      	      resp2->Fit("fit_func","ME");
	      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      	      TF1* f3 = resp2->GetFunction("fit_func");
	      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
	 	      constant1[iModule]= f_l->GetParameter(0);
	 	      constant2[iModule]= f_l->GetParameter(3);
	 	      constant3[iModule]=f_l->GetParameter(5);
	 	      Mean_G[iModule]=f_l->GetParameter(1);
	 	      Mean_P[iModule]=f_l->GetParameter(4);
	 	      Sigma_G[iModule]=f_l->GetParameter(2);
	 	      Mean_l[iModule]=f_l->GetParameter(6);
	 	      Sigma_l[iModule]=f_l->GetParameter(7);
	 	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      	      f_l->SetRange(0, 400);
	      	      resp2->Draw("");
	      	     f_l->Draw("Lsame");
	 	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
	 	     legend->SetHeader("","C");
	 	     legend->AddEntry(resp2,"orig","l");
	 	     legend->AddEntry(f_l,"fit funct","l");
	 	     gStyle->SetLegendTextSize(0.03);
	 	     legend->Draw("sames");
	      	      Canvas_n2->Modified();
	      	      Canvas_n2->cd();
	      	      Canvas_n2->SetSelected(Canvas_n2);
	      	      Canvas_n2->SaveAs(full_path2);
		      
	      	    }
	      	}
	      else if(map_vec[iModule].layer>=31 &&map_vec[iModule].layer<=36)
	      	{
	      	  // if((map_vec[iModule].u<=5 &&(map_vec[iModule].v==8||map_vec[iModule].v==7))||(map_vec[iModule].v<=5 &&(map_vec[iModule].u==8||map_vec[iModule].u==7)))
	      	  //   {
	      	  count++;
	      	  TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      	  Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      	  Canvas_n2->SetFillColor(0);
	      	  Canvas_n2->SetBorderMode(0);
	      	  Canvas_n2->SetBorderSize(2);
		  
	      	  gStyle->SetOptFit(111111);
	      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

	      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      	      //resp2->SetName("");
	      	      gStyle->SetOptFit(1);
	      	      gPad->SetLogy();
	      	      resp2->GetXaxis()->SetTitle("number of hits");
	      	      resp2->GetXaxis()->SetTitleSize(0.05);
	      	      //resp2->GetXaxis()->SetLimits(0,400);
	      	      resp2->SetLineColorAlpha(kRed,1);
	      	      resp2->SetMarkerColor(kRed);
	      	      resp2->SetLineWidth(2);
	      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
		      
	      	      func->SetParameter(0,1000); // set starting values
	      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
	      	      func->SetParameter(2,resp2->GetRMS());
	      	      func->SetParameter(3,10000);
	      	      func->SetParameter(4,resp2->GetMean());
	      	      func->SetParameter(5,1550);
	      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
	      	      //func->SetParameter(6,resp2->GetMean());
	      	      func->SetParameter(7,resp2->GetRMS());
	      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	      	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
		      
	      	      if(r1<0){
	      		func->SetRange(0,r2);}
	      	      else if(r1>0){
	      		func->SetRange(r1,r2);}
	      	      resp2->Fit("fit_func","ME");
	      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      	      TF1* f3 = resp2->GetFunction("fit_func");
	      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
	      	      constant1[iModule]= f_l->GetParameter(0);
	      	      constant2[iModule]= f_l->GetParameter(3);
	      	      constant3[iModule]=f_l->GetParameter(5);
	      	      Mean_G[iModule]=f_l->GetParameter(1);
	      	      Mean_P[iModule]=f_l->GetParameter(4);
	      	      Sigma_G[iModule]=f_l->GetParameter(2);
	      	      Mean_l[iModule]=f_l->GetParameter(6);
	      	      Sigma_l[iModule]=f_l->GetParameter(7);
	      	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      	      f_l->SetRange(0, 400);
	      	      resp2->Draw("");
	      	     f_l->Draw("Lsame");
	      	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
	      	     legend->SetHeader("","C");
	      	     legend->AddEntry(resp2,"orig","l");
	      	     legend->AddEntry(f_l,"fit funct","l");
	      	     gStyle->SetLegendTextSize(0.03);
	      	     legend->Draw("sames");
	      	      Canvas_n2->Modified();
	      	      Canvas_n2->cd();
	      	      Canvas_n2->SetSelected(Canvas_n2);
	      	      Canvas_n2->SaveAs(full_path2);
	      	}    
	    
	     else if(map_vec[iModule].layer>=37 &&map_vec[iModule].layer<=43)
	      	    {
	     	      count++;
	      	      TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      	      Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      	      Canvas_n2->SetFillColor(0);
	      	      Canvas_n2->SetBorderMode(0);
	      	      Canvas_n2->SetBorderSize(2);
		      
	      	      gStyle->SetOptFit(111111);
	      	      sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

	      	      TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      	      //resp2->SetName("");
	      	      gStyle->SetOptFit(1);
	      	      gPad->SetLogy();
	      	      resp2->GetXaxis()->SetTitle("number of hits");
	      	      resp2->GetXaxis()->SetTitleSize(0.05);
	      	      //resp2->GetXaxis()->SetLimits(0,400);
	      	      resp2->SetLineColorAlpha(kRed,1);
	      	      resp2->SetMarkerColor(kRed);
	      	      resp2->SetLineWidth(2);
	      	      TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
		      if(map_vec[iModule].layer == 37 && map_vec[iModule].u == 9 && map_vec[iModule].v == 8)
			{
			   func->SetParameter(0,100); // set starting values                                                                                                                            
			   func-> SetParameter(1,resp2->GetMean()); // set starting values                                                                                                                    
			   func->SetParameter(2,resp2->GetRMS());
			   func->SetParameter(3,10000);
			   func->SetParameter(4,resp2->GetMean());
			   func->SetParameter(5,4000);
			   func->SetParameter(6,resp2->GetMean()-1);//
			   func->SetParameter(7,resp2->GetRMS());
			}
		      else if ((map_vec[iModule].layer==41  && map_vec[iModule].u==5 && map_vec[iModule].v==6) ||(map_vec[iModule].layer==42  && map_vec[iModule].u==3 && map_vec[iModule].v==7))
                {
                      func->SetParameter(0,250);
                      func-> SetParameter(1,0.5*resp2->GetMean());
                      func->SetParameter(2,0.5*resp2->GetRMS());
                      func->SetParameter(3,100000);
                      func->SetParameter(4,resp2->GetMean());
                      func->SetParameter(5,200000);
                      func->SetParameter(6,0.3*resp2->GetMean());
                      func->SetParameter(7,resp2->GetRMS());

                }

		      else if ((map_vec[iModule].u==5 && map_vec[iModule].v ==8 && map_vec[iModule].layer==37)|| (map_vec[iModule].layer == 39 && map_vec[iModule].u == 7 && map_vec[iModule].v == 3))
                 {
                   func->SetParameter(0,100);
                   func-> SetParameter(1,0.05*resp2->GetMean());
                   func->SetParameter(2,0.01*resp2->GetRMS());
                   func->SetParameter(3,100000);
                   func->SetParameter(4,resp2->GetMean());
                   func->SetParameter(5,5000);
                   func->SetParameter(6,0.05*resp2->GetMean());
                   func->SetParameter(7,0.03*resp2->GetRMS());

                 }
		      else
			{
	      	      func->SetParameter(0,1000); // set starting values
	      	      func-> SetParameter(1,resp2->GetMean()); // set starting values
	      	      func->SetParameter(2,resp2->GetRMS());
	      	      func->SetParameter(3,10000);
	      	      func->SetParameter(4,resp2->GetMean());
	      	      func->SetParameter(5,4000);
	      	      func->SetParameter(6,resp2->GetMean());//-0.5*resp2->GetMean());
			
	      	      //func->SetParameter(6,resp2->GetMean());
	      	      func->SetParameter(7,resp2->GetRMS());
			}
	      	      func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      	      float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	     	      float r2 = resp2->GetMean()+4*resp2->GetRMS();
		      
	      	      if(r1<0){
	      	  	func->SetRange(0,r2);}
	      	      else if(r1>0){
	      	  	func->SetRange(r1,r2);}
	      	      resp2->Fit("fit_func","ME");
	      	      resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      	      TF1* f3 = resp2->GetFunction("fit_func");
	      	      TF1 *f_l =  (TF1*)f3->Clone("f3");
	     	      constant1[iModule]= f_l->GetParameter(0);
	     	      constant2[iModule]= f_l->GetParameter(3);
	     	      constant3[iModule]=f_l->GetParameter(5);
	     	      Mean_G[iModule]=f_l->GetParameter(1);
	     	      Mean_P[iModule]=f_l->GetParameter(4);
	     	      Sigma_G[iModule]=f_l->GetParameter(2);
	     	      Mean_l[iModule]=f_l->GetParameter(6);
	     	      Sigma_l[iModule]=f_l->GetParameter(7);
	     	      file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      	      f_l->SetRange(0, 400);
	      	      resp2->Draw("");
	      	     f_l->Draw("Lsame");
	     	     auto legend = new TLegend(0.75,0.25,0.9,0.32);
	     	     legend->SetHeader("","C");
	     	     legend->AddEntry(resp2,"orig","l");
	     	     legend->AddEntry(f_l,"fit funct","l");
	     	     gStyle->SetLegendTextSize(0.03);
	     	     legend->Draw("sames");
	      	      Canvas_n2->Modified();
	      	      Canvas_n2->cd();
	      	      Canvas_n2->SetSelected(Canvas_n2);
	      	      Canvas_n2->SaveAs(full_path2);
		      
	      	    }
	      else if (map_vec[iModule].layer>=44 &&map_vec[iModule].layer<=47)
	      	{
	      	  count++;
	      	  TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	      	  Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	      	  Canvas_n2->SetFillColor(0);
	      	  Canvas_n2->SetBorderMode(0);
	      	  Canvas_n2->SetBorderSize(2);

	      	  gStyle->SetOptFit(111111);
	      	  sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

	      	  TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
	      	  //resp2->SetName("");
	      	  gStyle->SetOptFit(1);
	      	  gPad->SetLogy();
	      	  resp2->GetXaxis()->SetTitle("number of hits");
	      	  resp2->GetXaxis()->SetTitleSize(0.05);
	      	  //resp2->GetXaxis()->SetLimits(0,400);
	      	  resp2->SetLineColorAlpha(kRed,1);
	      	  resp2->SetMarkerColor(kRed);
	      	  resp2->SetLineWidth(2);
		  
	      	  TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");

	      	  func->SetParameter(0,1000); // set starting values
	      	  func-> SetParameter(1,resp2->GetMean()); // set starting values
	      	  func->SetParameter(2,resp2->GetRMS());
	      	  func->SetParameter(3,6000);
	      	  func->SetParameter(4,resp2->GetMean());
	      	  func->SetParameter(5,1500);
	      	  func->SetParameter(6,resp2->GetMean());
	      	  func->SetParameter(7,resp2->GetRMS());
	      	  func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");
	      	  //float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	      	  float r1 = resp2->GetMean()- 4*resp2->GetRMS();
	      	  float r2 = resp2->GetMean()+4*resp2->GetRMS();
		  
	      	  if(r1<0){
	      	    func->SetRange(0,r2);}
	      	  else if(r1>0){
	      	    func->SetRange(r1,r2);}
	      	  resp2->Fit("fit_func","ME");
	      	  resp2->GetFunction("fit_func")->SetLineColor(kBlack);
	      	  TF1* f3 = resp2->GetFunction("fit_func");
	      	  TF1 *f_l =  (TF1*)f3->Clone("f3");
	      	  constant1[iModule]= f_l->GetParameter(0);
	      	  constant2[iModule]= f_l->GetParameter(3);
	      	  constant3[iModule]=f_l->GetParameter(5);
	      	  Mean_G[iModule]=f_l->GetParameter(1);
	      	  Mean_P[iModule]=f_l->GetParameter(4);
	      	  Sigma_G[iModule]=f_l->GetParameter(2);
	      	  Mean_l[iModule]=f_l->GetParameter(6);
	      	  Sigma_l[iModule]=f_l->GetParameter(7);
	      	  file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
	      	  f_l->SetRange(0, 400);
	      	  resp2->Draw("");
	      	 f_l->Draw("Lsame");
	      	 auto legend = new TLegend(0.75,0.25,0.9,0.32);
	      	 legend->SetHeader("","C");
	      	 legend->AddEntry(resp2,"orig","l");
	      	 legend->AddEntry(f_l,"fit funct","l");
	      	 gStyle->SetLegendTextSize(0.03);
	      	 legend->Draw("sames");
	      	  Canvas_n2->Modified();
	      	  Canvas_n2->cd();
	      	  Canvas_n2->SetSelected(Canvas_n2);
	      	  Canvas_n2->SaveAs(full_path2);

		  
	      	}
	      else if (map_vec[iModule].layer>=48)// &&map_vec[iModule].layer<=47)
		{
		  count++;
		  TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
		  Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
		  Canvas_n2->SetFillColor(0);
		  Canvas_n2->SetBorderMode(0);
		  Canvas_n2->SetBorderSize(2);

		  gStyle->SetOptFit(111111);
		  sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);

		  TH1F *resp2 = (TH1F*)inputfile->Get(hist_name);
		  //resp2->SetName("");
		  gStyle->SetOptFit(1);
		  gPad->SetLogy();
		  resp2->GetXaxis()->SetTitle("number of hits");
		  resp2->GetXaxis()->SetTitleSize(0.05);
		  //resp2->GetXaxis()->SetLimits(0,400);
		  resp2->SetLineColorAlpha(kRed,1);
		  resp2->SetMarkerColor(kRed);
		  resp2->SetLineWidth(2);

		  TF1* func = new TF1("fit_func","([0]*TMath::Gaus(x,[1],[2])) + [3]*TMath::Poisson(x,[4])+[5]*TMath::Landau(x,[6],[7])");
		  if(map_vec[iModule].layer==48 && map_vec[iModule].u == 7 && map_vec[iModule].v==4)
		    {
		      func->SetParameter(0,1000); // set starting values                                                                                                                                 
		      func-> SetParameter(1,resp2->GetMean()-1); // set starting values                                                                                                                         
		      func->SetParameter(2,resp2->GetRMS());
		      func->SetParameter(3,5000);
		      func->SetParameter(4,resp2->GetMean());
		      func->SetParameter(5,5050);
		      func->SetParameter(6,resp2->GetMean());
		      func->SetParameter(7,resp2->GetRMS());

		    }
		  else
		    {
		  func->SetParameter(0,100); // set starting values
		  func-> SetParameter(1,resp2->GetMean()); // set starting values
		  func->SetParameter(2,resp2->GetRMS());
		  func->SetParameter(3,5000);
		  func->SetParameter(4,resp2->GetMean());
		  func->SetParameter(5,5050);
		  func->SetParameter(6,resp2->GetMean());
		  func->SetParameter(7,resp2->GetRMS());
		    }
		  func->SetParNames("Constant1","Mean_G","Sigma_G","Constant2","Mean_P","Constant3","Mean_L","Sigma_L");

		  //float r1 = resp2->GetMean()- 4*resp2->GetRMS();
		  float r1 = resp2->GetMean()- 6*resp2->GetRMS();
		  float r2 = resp2->GetMean()+20*resp2->GetRMS();
		  if(r1<0)
		 {
		    func->SetRange(0,r2);}
		  else if(r1>0){
		    func->SetRange(r1,r2);}
		  resp2->Fit("fit_func","RME");
		  resp2->GetFunction("fit_func")->SetLineColor(kBlack);
		  TF1* f3 = resp2->GetFunction("fit_func");
		  TF1 *f_l =  (TF1*)f3->Clone("f3");
		  constant1[iModule]= f_l->GetParameter(0);
		  constant2[iModule]= f_l->GetParameter(3);
		  constant3[iModule]=f_l->GetParameter(5);
		  Mean_G[iModule]=f_l->GetParameter(1);
		  Mean_P[iModule]=f_l->GetParameter(4);
		  Sigma_G[iModule]=f_l->GetParameter(2);
		  Mean_l[iModule]=f_l->GetParameter(6);
		  Sigma_l[iModule]=f_l->GetParameter(7);
		  file<<iModule<<"\t"<<map_vec[iModule].layer<<"\t"<<map_vec[iModule].u<<"\t"<<map_vec[iModule].v<<"\t"<<map_vec[iModule].type<<"\t"<<constant1[iModule]<<"\t"<<Mean_G[iModule]<< "\t"<<Sigma_G[iModule]<<"\t"<<constant2[iModule]<<"\t"<<Mean_P[iModule]<<"\t"<<constant3[iModule]<<"\t"<<Mean_l[iModule]<<"\t"<<Sigma_l[iModule]<<"\n";
		  
		  f_l->SetRange(0, 400);
		  resp2->Draw("");
		  f_l->Draw("Lsame");
		  auto legend = new TLegend(0.75,0.25,0.9,0.32);
		  legend->SetHeader("","C");
		  legend->AddEntry(resp2,"orig","l");
		  legend->AddEntry(f_l,"fit funct","l");
		  gStyle->SetLegendTextSize(0.03);
		  legend->Draw("sames");

		  Canvas_n2->Modified();
		  Canvas_n2->cd();
		  Canvas_n2->SetSelected(Canvas_n2);
		  Canvas_n2->SaveAs(full_path2);
		  
		}
		
    
	    }
	      
	       }
	   
      //    }	    
  f->Close();
  file.close();
	
    
  cout<<"total"<<count<<endl;
  
}
  
