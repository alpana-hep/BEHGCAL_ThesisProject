#include <stdio.h>
#include<fstream>
#include <vector>
#include <string>
#include <map>
#include<iostream>
using namespace std;


void overlayPlots_v1(string root_file_name,string root_file_name1,string map_file, string pathname, string tagname, string tagname1, string tagname2, string outfile)
{
  char* title= new char[200];
  //char* hname = new char[200];
  //char* hist_name = new char[200];
  char* stat_name = new char[200];
  char* name = new char[2000];
  char* path = new char[2000];
  char* full_path = new char[2000];
  char* path1 = new char[2000];
  char* full_path1 = new char[2000];
  char* path2 = new char[2000];
  char* full_path2 = new char[2000];
  char* full_path3 = new char[2000];

  //  char* plot_name = new char[200];
  char* old_rel = new char[200];
  char* new_rel = new char[200];
  char* old_release = new char[50];
  char* new_release = new char[50];
  char* hist = new char[200];
  sprintf(new_release,"%s.root",outfile.c_str());//RECREATE");
  TFile* fout = new TFile(new_release,"RECREATE");

  TFile* f = new TFile("data_per_elink_nt_EOL_correct.root","Recreate");
  TH1F* data_per_elink = new TH1F("data_per_elink","average econ data per elink",5066,0,5066);
  TH1F* Transformed_data_per_elink = new TH1F("Transformed_data_per_elink",";;transformed / original",5066,0,5066);
  TH1F* ratio_plot = new TH1F("ratio_plot","Pessimistics",5066,0,5066);
  ratio_plot->GetXaxis()->SetTitle("Econ-d's");
  ratio_plot->GetYaxis()->SetTitle("transformed/original");

  char* hname = new char[2000];
  char* hname1 = new char[2000];
  char* hname0 = new char[2000];
  char* hname2 = new char[2000];
  char* hname3 = new char[2000];
  char* hname4 = new char[2000];
  char* hist_name = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
  char* hist_name3 = new char[200];
  TGraph* gr = new TGraph();
  gr->SetTitle(" ");
  gr->SetName("N10_average");
  gr->GetXaxis()->SetTitle("Average occupancy (CMSSW)");
  gr->GetYaxis()->SetTitleOffset(1.4);
  gr->GetYaxis()->SetTitle("Average occupancy (Pseudo)");
  gr->SetMarkerColorAlpha(kBlue, 0.95);
  gr->SetMarkerSize(2);
  gr->SetMarkerStyle(8);
  sprintf(hname,"%s",root_file_name.c_str());
  sprintf(hname4,"%s",root_file_name1.c_str());
  
  sprintf(hname3,"%s.txt",outfile.c_str());
  std::ofstream file_3;
  file_3.open(hname3,ios::out);

  //  sprintf(hname0,"/home/alpana/work/HGCAL_BE_TDAQ/ECON_data/%s",root_file_name1.c_str());
  sprintf(path1,"./../%s/%s",pathname.c_str(), tagname.c_str());
  sprintf(path2,"%s",path1);//root_file_name.c_str());

  TFile * inputfile = new TFile(hname,"READ");
  TFile * inputfile1 = new TFile(hname4,"READ");

  sprintf(hname1,"%s",map_file.c_str());
  sprintf(hname2,"%s",tagname1.c_str());
  std::ofstream file;
  file.open(hname2,ios::out);

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
  if(!mapping_file.is_open())
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      map_vec.clear();
      while (!mapping_file.eof()) {
	unsigned int  layer, u, v, type,typee;
	char shape;
	int idxTpgLpgbt1, nTpgElinks1, idxTpgLpgbt2, nTpgElinks2;
	int  idxDaqLpgbt1, idxDaqLpgbt2;
	int nDaqElinks1, nDaqElinks2;
	unsigned nDaqLpgbt,nTpgLpgbt;
	float average_data_per_econ,Error_means,rms,rms_error;


	mapping_file >> layer >> u >> v >> type >>typee  >> idxTpgLpgbt1 >> idxTpgLpgbt2 ;
	if (mapping_file.eof()) break;
	modulemap mm;
	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee =typee;
	// mm.shape = shape;
	// mm.nDaqLpgbt=nDaqLpgbt;
	mm.idxDaqLpgbt1=idxDaqLpgbt1; mm.nDaqElinks1=nDaqElinks1;
	// mm.idxDaqLpgbt2=idxDaqLpgbt2; mm.nDaqElinks2= nDaqElinks2;
	// ;
	//      mm.nTpgLpgbt=nTpgLpgbt; mm.idxTpgLpgbt1=idxTpgLpgbt1; mm.nTpgElinks1=nTpgElinks1;
	//mm.idxTpgLpgbt2=idxTpgLpgbt2; mm.nTpgElinks2= nTpgElinks2;
	cout<<layer<<endl;
	//mm.average_data_per_econ = average_data_per_econ;
	map_vec.push_back(mm);
      }
      mapping_file.close();
    }      
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
      int length = (int(map_vec.size()));
      for(int iModule = 0; iModule<length;iModule++)
	{
	  average_per_elinks_Map map;
	  int luv =256*map_vec[iModule].layer+16*map_vec[iModule].u+map_vec[iModule].v;
	  cout<<luv<<endl;
	  int ncells = map_vec[iModule].idxTpgLpgbt2;
	  sprintf(hist_name,"orig_hits_%s_%d_%d_%d_%d",tagname2.c_str(),map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v)  ;
	  TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,65,65,725,725);
	  //	  Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
	  Canvas_n2->SetRightMargin(0.025);
	  Canvas_n2->SetLeftMargin(0.16);
	  Canvas_n2->SetTopMargin(0.034);
	  Canvas_n2->SetBottomMargin(0.12);
	  
	  Canvas_n2->SetFillColor(0);
	  Canvas_n2->SetBorderMode(0);
	  Canvas_n2->SetBorderSize(2);
	  gStyle->SetOptFit(111111);
	  sprintf(full_path2,"%s/Layer_%02d/Module_%d_%d_%d_%d.pdf",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);sprintf(full_path3,"%s/Layer_%02d/Module_%d_%d_%d_%d.png",path2,map_vec[iModule].layer,map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
	  TH1F *resp = (TH1F*)inputfile->Get(hist_name);
	  //cout<<hist_name<<endl;
	  sprintf(hist_name,"orig_hits_%s_%d_%d_%d_%d",tagname2.c_str(),map_vec[iModule].layer,map_vec[iModule].type,map_vec[iModule].u,map_vec[iModule].v);
	  
	  TH1F *resp1 = (TH1F*)inputfile1->Get(hist_name);
	  float factor=1.0;
	  resp1->Scale(factor/resp1->Integral());//resp1->GetEntries()/resp2->GetEntries());
	  resp->Scale(factor/resp->Integral());
	  resp1->GetXaxis()->SetTitle("Module occupancy");
	  resp->GetXaxis()->SetTitle("Module occupancy");
	  resp1->GetYaxis()->SetTitle("Normalized");
	  resp->GetYaxis()->SetTitle("Normalized");
	  gr->SetPoint(iModule,resp1->GetMean(),resp->GetMean());
	  if((resp->GetMean() -resp1->GetMean())>10)
	    cout<<hist_name<<endl;
	  resp1->GetXaxis()->SetTitleSize(0.05);
	  resp1->GetXaxis()->SetTitleOffset(0.99);
	  cout<<resp1->GetMean()<<endl;
	  resp1->SetLineColorAlpha(kRed,1);
	  resp1->SetMarkerColor(kRed);
	  resp->SetLineWidth(2);
	  resp->SetLineColorAlpha(kBlack,1);
	  resp->SetMarkerColor(kBlack);
	  resp1->SetLineWidth(2);
	  resp->GetXaxis()->SetTitleSize(0.05);
	  resp->GetXaxis()->SetTitleOffset(0.99);
	  float r2 = resp->GetMean()+10*resp1->GetRMS();
	  resp->GetXaxis()->SetRange(0,r2);
	  resp1->GetXaxis()->SetRange(0,r2);
	  auto legend = new TLegend(0.6,0.6,0.85,0.85);
	  gStyle->SetLegendTextSize(0.045);
	  
	  legend->SetLineColor(kWhite);
	  char* lhead = new char[100];
	  TLegendEntry* leg_entry = legend->AddEntry(resp,"Pseudo events","l");
	  leg_entry->SetTextColor(kBlack);
	  TLegendEntry* leg_entry1 = legend->AddEntry(resp1,"CMSSW events","l");
	  leg_entry1->SetTextColor(kRed);
	  
	  gStyle->SetOptStat(0);
	  gPad->SetLogy();
	  resp1->Draw("HIST");
	  resp->Draw("HIST sames");
	  legend->Draw("sames");
	  Canvas_n2->Modified();
	  Canvas_n2->cd();
	  Canvas_n2->SetSelected(Canvas_n2);
	  Canvas_n2->SaveAs(full_path2);
	  Canvas_n2->SaveAs(full_path3);
	}
      fout->cd();
      gr->Write();
      fout->Close();
	
	  
 
}
