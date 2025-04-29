#include<iostream>
#include<fstream>
#include<stdio.h>
#include <vector>
#include <string>
#include <map>
#include<iostream>
#include<fstream>

using namespace std;


void  ldm_hdm_correlation_graph( string filename)
{
  //output root file
  TFile *f = new TFile("correlation_expo_v0.root","recreate");
  char *fname= new char[1000];
  //reading the input file
  sprintf(fname,"%s",filename.c_str());
  std::fstream file_name;
  file_name.open(fname,ios::in);
  struct modulemap { //map for data_nt_elink.txt
    unsigned int  layer, u, v, type,typee;
    char shape;
    float R,CMS_occup,HD_occup,LD_occup,mean_error;
  };
  std::vector <modulemap> map_vec;
  if(!file_name.is_open())
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      map_vec.clear();
      while (true) {
	unsigned int  layer, u, v, type,typee;
	char shape;
	float R,CMS_occup,HD_occup,LD_occup,mean_error;
	file_name >> layer >> u >> v >> type >>typee >> shape
		  >> R >> CMS_occup>>HD_occup>>LD_occup >>mean_error;

	if (file_name.eof()) break;
	modulemap mm;
	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee = typee;
	mm.shape=shape;
	mm.R=R;
	mm.CMS_occup=CMS_occup;
	mm.LD_occup= LD_occup;
	mm.HD_occup=HD_occup;
	mm.mean_error=mean_error;
	map_vec.push_back(mm);
	//      cout<<map_vec.size()<<endl;
      }
      file_name.close();
    }
  cout<<(int(map_vec.size()))<<endl;
  //read the file and fill TGraphs
  float Fl[168]={};
  float C[168]={};
  float Fh[168]={};
  float index[168]={};
  float ratio[168]={};
  float error;
  float r[168]={};
  for(int iModule =0;iModule<(int(map_vec.size()));iModule++)
    {
      Fl[iModule]=map_vec[iModule].LD_occup;
      Fh[iModule]=map_vec[iModule].HD_occup;
      C[iModule]=map_vec[iModule].CMS_occup;
      error=map_vec[iModule].mean_error;
      index[iModule]=iModule;
      ratio[iModule]=  (C[iModule]-Fl[iModule])/error;
      r[iModule]=map_vec[iModule].R;
      //cout<<map_vec[iModule].layer<<map_vec[iModule].u<<map_vec[iModule].v<<"\t"<<map_vec[iModule].HD_occup<<"\t"<<map_vec[iModule].LD_occup<<"\t"<<map_vec[iModule].CMS_occup<<endl;
    }
  
  int n=(int(map_vec.size()));
  //  TCanvas *c = new TCanvas("c", "canvas", 800, 800);
  //  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  TGraph* gr = new TGraph(n,Fl,Fh);
  gr->SetMarkerStyle( kFullTriangleUp);
  //  gr->SetMarkerLineWidth(3);
  gr->SetMarkerColor(kBlack);
  gr->SetName("ldm_hdm");
  gr->GetXaxis()->SetTitle("average hits in a LDM");
  gr->GetYaxis()->SetTitle("average hits in a HDM");
  
  TAxis *axis = gr->GetXaxis();
  axis->SetLimits(0,199);                 // along X

  gr->GetHistogram()->SetMaximum(450);   // along
  gr->GetHistogram()->SetMinimum(0);
  //gr->Draw("AP");
  TF1* f2= new TF1("f2"," [0]*432.0*(1.0-pow((192.0-x)/192.0,192.0/432.0))",0,190);
  f2->SetParameter(0,1);
  f2->SetRange(0,190);
  f2->SetLineColor(kRed);
  TF1* f1= new TF1("f1","x",0,190);
  //  f1->SetParameter(0,1);
  f1->SetRange(0,200);
  f1->SetLineColor(kGreen);
  TF1* f3= new TF1("f3","[0]*((432*x)/192)",0,190);
  f3->SetParameter(0,1);
  f3->SetRange(0,200);
  f3->SetLineColor(kBlue);
  //f1->Draw("LSAME");
  //f3->Draw("LSAME");
  //f2->Draw("LSAME");
  auto l = new TLegend(0.4,0.65,0.65,0.9);
  l->SetHeader("","C");
  l->AddEntry(gr,"calculated values","p");
  l->AddEntry(f2,"fixed density","L");
  l->AddEntry(f3,"fixed occupancy", "L");
  l->AddEntry(f1,"fixed number", "L");
   auto l1 = new TLegend(0.4,0.35,0.65,0.6);
   l1->SetHeader("","C");
   l1->AddEntry(f3,"Pessimistic","L");
   l1->AddEntry(f1,"optimistic","L");
  
  gStyle->SetLegendTextSize(0.03);
  //   l->Draw("same");
  //l1->Draw("same");
  gr->Write();
  f1->Write();
  f2->Write();
  f3->Write();
  l->Write();
  //gr->Draw("APSAME");
    TCanvas *c = new TCanvas("c", "canvas", 800, 800);
  
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
  //  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();
  
  TGraph* gr1 = new TGraph(n,index,Fh);
  gr1->SetMarkerStyle(8);
  gr1->SetMarkerColor(kBlue);
  // gr1->SetLineWidth(3);
  TGraph* gr2 = new TGraph(n,index,Fl);
  gr2->SetMarkerStyle( kFullCircle);
  gr2->SetMarkerColor(kRed);
  gr2->SetLineWidth(5);
  TGraph* gr3 = new TGraph(n,index,C);
  gr3->SetMarkerStyle(kFullTriangleUp);
  gr3->SetMarkerColor(kBlack);
  TMultiGraph* mg = new TMultiGraph();
  mg->SetName("comparison");
  mg->GetXaxis()->SetTitle("index number");
  mg->GetYaxis()->SetTitle("average number of hits");
  TAxis *axis1 = mg->GetXaxis();
  axis1->SetLimits(0,170);                 // along X

  mg->GetHistogram()->SetMaximum(110);   // along
  mg->GetHistogram()->SetMinimum(0);
  
  mg->Add(gr1);
  mg->Add(gr2);
  mg->Add(gr3);
  
  mg->Draw("apsame");
  //  TLine *line = new TLine(0,0,170,0);
  //line->Draw("sames");
  auto legend = new TLegend(0.70,0.70,0.9,0.9);
  legend->SetHeader("","C");
  legend->AddEntry(gr1,"predicted HDM","p");
  legend->AddEntry(gr2,"predicted LDM","p");
  legend->AddEntry(gr3,"CMSSW", "p");
  //legend->AddEntry(f2,"Ldm fit", "L");

  gStyle->SetLegendTextSize(0.03);
  legend->Draw("sames");
  mg->Write();
  legend->Write();
  c->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0.03);
  pad2->SetBottomMargin(0.01);
  pad2->SetName("");
  //pad2->SetGridx(); // vertical grid
   pad2->Draw();
  pad2->cd();
  TGraph* gr5 = new TGraph(n,index,ratio);
  gr5->SetMarkerStyle( kFullSquare);
  gr5->SetMarkerSize(0.5);
  gr5->SetName("ratio");
  TAxis *axis5 = gr5->GetXaxis();
  axis5->SetLimits(0,170);
  axis5->SetLabelSize(0.1);
  
  TAxis *axis5y = gr5->GetYaxis();
  axis5y->SetLabelSize(0.1);
  axis5->SetTitle("index number(-->layers)");
  gr5->SetMarkerColor(kRed);
  gr5->Draw("apsame");
  TLine *line = new TLine(0,0,170,0);
  line->Draw("sames");
  TLine *line1 = new TLine(0,20,170,20);
  line1->SetLineColor(kBlue);
  line1->Draw("sames");
  TLine *line2 = new TLine(0,-20,170,-20);
  line2->SetLineColor(kBlue);
  line2->Draw("sames");
  gr5->Write();
  c->Write();
  f->Close();
}
