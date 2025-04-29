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

// Double_t g2(Double_t x, Double_t p0, Double_t p1)
// {
  
//   Double_t function = exp(p0+p1*x);
//   return function;
// }

// TCanvas* Fit_to_hist(TH1F* hh)
// {
//   TCanvas* cc = new TCanvas("canvas_name","canvas_title",800,600);
//   hh->Fit("fit_func");
//   return(cc);
// }
// TCanvas* Fit_to_hist1(TH1F* hh)
// {
//   TCanvas* cc = new TCanvas("canvas_name","canvas_title",800,600);
//   hh->Fit("fitt");
//   return(cc);
// }


void fit_function(string filename)
{
     //  TCanvas* Fit_to_hist(TH1F*);
  //TCanvas* Fit_to_hist1(TH1F*);
   //  TF1* ff = new TF1("fit_func","pol 1",0,2000);
  //  TMultiGraph*mg = new TMultiGraph();//650
  TF1* ff = new TF1("fit_func","expo",300,730); //layer 1 to 3// Define the fit function here. //420===400,730===700
  TF1* ff1 = new TF1("fitt","expo",760,840);// 1 to 3
  // Define the fit function here.
  TF1* ff2 = new TF1("fit_func2","expo",300,735);//4 to 8
  TF1* ffi2 = new TF1("fitt2","expo",760,840); // 4 to 8
  TF1* ff3 = new TF1("fit_func3","expo",300,730);//9 to 12
  TF1* ffi3 = new TF1("fitt3","expo",760,840); // 9 to 12
  TF1* ff4 = new TF1("fit_func4","expo",300,730);//13 to 18
  TF1* ffi4 = new TF1("fitt4","expo",760,840); //13 to 18
  TF1* ff5 = new TF1("fit_func5","expo",300,730);//19 to 24
  TF1* ffi5 = new TF1("fitt5","expo",760,840); //19 to 24   
  TF1* ff6 = new TF1("fit_func6","expo",300,730);//24 to 28
  TF1* ffi6 = new TF1("fitt6","expo",760,840); //24 to 28         
  TF1* fhdm = new TF1("fhdm","expo",300,700);//29
  TF1* fldm = new TF1("fldm","expo", 695,775);//layer29
  
  TF1* ff7 = new TF1("fit_func7","expo",310,680);//30
  TF1* ffi7 = new TF1("fitt7","expo",670,765);//30
  
  TF1* fHdm = new TF1("fHdm","expo",310,635);//layer 31 
  TF1* fLdm = new TF1("fLdm","expo", 660,800);  
  TF1* FHdm = new TF1("FHdm","expo",420,490);
  TF1* FLdm = new TF1("FLdm","expo", 507,780);
  // TF1* ff = new TF1("fit_func","expo",410,730); //layer 1 to 3// Define the fit function here. //420===400,730===700
  // TF1* ff1 = new TF1("fitt","expo",760,950);// 1 to 3
  // // Define the fit function here.
  // TF1* ff2 = new TF1("fit_func2","expo",440,735);//4 to 8
  // TF1* ffi2 = new TF1("fitt2","expo",760,890); // 4 to 8
  // TF1* ff3 = new TF1("fit_func3","expo",410,730);//9 to 12
  // TF1* ffi3 = new TF1("fitt3","expo",760,880); // 9 to 12
  // TF1* ff4 = new TF1("fit_func4","expo",410,730);//13 to 18
  // TF1* ffi4 = new TF1("fitt4","expo",760,880); //13 to 18
  // TF1* ff5 = new TF1("fit_func5","expo",410,730);//19 to 24
  // TF1* ffi5 = new TF1("fitt5","expo",760,890); //19 to 24
  // TF1* ff6 = new TF1("fit_func6","expo",410,730);//24 to 28
  // TF1* ffi6 = new TF1("fitt6","expo",760,875); //24 to 28
  // TF1* fhdm = new TF1("fhdm","expo",485,700);//29
  // TF1* fldm = new TF1("fldm","expo", 695,825);//layer29

  // TF1* ff7 = new TF1("fit_func7","expo",485,680);//30
  // TF1* ffi7 = new TF1("fitt7","expo",670,790);//30

  // TF1* fHdm = new TF1("fHdm","expo",535,635);//layer 31
  // TF1* fLdm = new TF1("fLdm","expo", 670,790);

  // TF1* FHdm = new TF1("FHdm","expo",420,490);
  // TF1* FLdm = new TF1("FLdm","expo", 507,780);

  
  //  ff->SetParameters(0,3);
  //ff->SetParameters(1,4);
  //  TFitResultPtr Fit("ff","E","",0,1400);
  char* hname = new char[2000];
  char* Name = new char[50];
  // TCanvas* Fit_to_hist(TH1F* hh)
  // {
  //   TCanvas* cc = new TCanvas("canvas_name","canvas_title",800,600);
  //   hh->Fit("fit_func");
  //   return(cc);
  // }
  std::ofstream file;
  file.open("function_parameters_without_noise_0208_v0.txt",ios::out); //output file that contains fitted function parameteres
  
  //  TFile* f = new TFile("fited_Andrew_Studies.root","Recreate"); //not useful right now, added to have an o/p file containing all graphs
  sprintf(hname,"%s",filename.c_str()); // reading the file name from the working command
  TFile * inputfile = new TFile(hname,"READ");// read the root file given from the input
  char* hist_name = new char[200];
  char* hist_name1 = new char[200];
  char* path2 = new char[2000];
  char* full_path2 = new char[2000];
  float hp0[39]={};
  float hp1[39]={};
  float hp2[39]={};
  float hp3[39]={};
  float lp2[39]={};
  float lp3[39]={};
  
  float lp0[39]={};
  float lp1[39]={};
  float x[20]={};
  float y[20]={};
  sprintf(path2,"results");//Andrew_studies/correction/with_logy/");//my_studies/correction/");                //path to save files
  
  //loop for graphs of the root file
  for(int i = 1;i<37;i++)
    {
      sprintf(hist_name,"average_vs_r_hdm_in_layer%02d",i);
      sprintf(hist_name1,"average_vs_r_ldm_in_layer%02d",i);

      sprintf(full_path2,"%s/average_vs_r_in_layer%02d.png",path2,i);
      TCanvas *Canvas_1_n2 = new TCanvas(hist_name, hist_name,65,52,725,527);
      Canvas_1_n2->Range(-60.25,-0.625,562.25,0.625);
      Canvas_1_n2->SetFillColor(0);
      Canvas_1_n2->SetBorderMode(0);
      Canvas_1_n2->SetBorderSize(2);
      Canvas_1_n2->SetFrameBorderMode(0);
      Canvas_1_n2->SetFrameBorderMode(0);
      gStyle->SetOptFit(111);
      TMultiGraph*mg = new TMultiGraph();    
      TGraphErrors *resp = (TGraphErrors*)inputfile->Get(hist_name);
      TGraphErrors *resp1 = (TGraphErrors*)inputfile->Get(hist_name1);
      //cout<<"........."<<endl;
      resp->SetName("HDM");
      resp1->SetName("LDM");
      TPaveStats *ptstats0 = new TPaveStats(0.75,0.75,0.99,0.99,"brNDC");
      ptstats0->SetBorderSize(1);
      ptstats0->SetFillColor(0);
      ptstats0->SetLineColor(kRed);
      ptstats0->SetTextAlign(12);
      ptstats0->SetTextColor(kRed);
      ptstats0->SetTextFont(42);
      resp->GetListOfFunctions()->Add(ptstats0);
      ptstats0->SetParent(resp);


      TPaveStats *ptstats1 = new TPaveStats(0.75,0.5,0.99,0.75,"brNDC");
      ptstats1->SetBorderSize(1);
      ptstats1->SetFillColor(0);
      ptstats1->SetLineColor(kBlue);
      ptstats1->SetTextAlign(12);
      ptstats1->SetTextColor(kBlue);
      ptstats1->SetTextFont(42);
      resp1->GetListOfFunctions()->Add(ptstats1);
      ptstats1->SetParent(resp1);
      
      resp->SetMarkerStyle( kFullSquare);
      resp1->SetMarkerStyle( kFullCircle);
      resp1->SetMarkerColorAlpha(kGreen+2,0.1);
      resp->SetMarkerColorAlpha(kBlack,0.1);
      
      
      //gPad->SetLogy();
      //      f->WriteTObject(resp->Fit("fit_func"));
     
     
      if(i<4){
	
	resp->Fit("fit_func","R");
	resp1->Fit("fitt","R");
	resp1->GetFunction("fitt")->SetLineColor(kBlue);
	resp->GetFunction("fit_func")->SetLineColor(kRed);
	TF1* f3 = resp->GetFunction("fit_func");
	TF1* f2 = resp1->GetFunction("fitt");
	hp0[i]= f3->GetParameter(0);
	hp1[i]= f3->GetParameter(1);
	lp0[i]= f2->GetParameter(0);
	lp1[i]= f2->GetParameter(1);
	//hp2[i]= f3->GetParameter(2);
	//hp3[i]= f3->GetParameter(3);
	//lp2[i]= f2->GetParameter(2);
	//lp3[i]= f2->GetParameter(3);
	//file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	sprintf(Name,"Layer%02d",i);
	mg->SetTitle(Name);
	mg->GetXaxis()->SetTitle("distance from the beamline");
	mg->GetYaxis()->SetTitle("average hits");
	
	mg->Add(resp);
	mg->Add(resp1);
	mg->Draw("ap");
	// Define the fit function here.
	TF1 *f_l =  (TF1*)f3->Clone("f2");
	TF1 *f_h = (TF1*)f2->Clone("f3");
	f_l->SetLineStyle(2);
	f_h->SetLineStyle(2);
      
	f_l->SetRange(0, 1400); 
	f_h->SetRange(0, 1400);
	f_l->Draw("Lsame");
	f_h->Draw("Lsame");
     
	auto legend = new TLegend(0.75,0.35,0.99,0.5);
	legend->SetHeader("","C");
	legend->AddEntry(resp, resp->GetName(),"p");
	legend->AddEntry(resp1, resp1->GetName(),"p");
	legend->AddEntry(f3,"Hdm fit", "L");
	legend->AddEntry(f2,"Ldm fit", "L");
      
	gStyle->SetLegendTextSize(0.03);
	legend->Draw();
	
	Canvas_1_n2->Modified();
	Canvas_1_n2->cd();
	Canvas_1_n2->SetSelected(Canvas_1_n2);
	

	 Canvas_1_n2->SaveAs(full_path2);
	//f->cd();
	//      f->Write();
	//      f->WriteTObject(resp->Fit("fit_func"));
	//f->WriteTObject(resp1);
	
	//	}
      }
      else if(i>=4 &&i<=7)
	{
	  resp->Fit("fit_func2","R");
	  resp1->Fit("fitt2","R");
	  resp1->GetFunction("fitt2")->SetLineColor(kBlue);
	  resp->GetFunction("fit_func2")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fit_func2");
	  TF1* f2 = resp1->GetFunction("fitt2");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");

	  
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if(i>=8&&i<=15)
	{
	  resp->Fit("fit_func3","R");
	  resp1->Fit("fitt3","R");
	  resp1->GetFunction("fitt3")->SetLineColor(kBlue);
	  resp->GetFunction("fit_func3")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fit_func3");
	  TF1* f2 = resp1->GetFunction("fitt3");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  // hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  // file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");

	  
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if (i>15&&i<19)
	{
	  resp->Fit("fit_func4","R");
	  resp1->Fit("fitt4","R");
	  resp1->GetFunction("fitt4")->SetLineColor(kBlue);
	  resp->GetFunction("fit_func4")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fit_func4");
	  TF1* f2 = resp1->GetFunction("fitt4");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");


	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if(i>=19 && i<25)
	{
	  resp->Fit("fit_func5","R");
	  resp1->Fit("fitt5","R");
	  resp1->GetFunction("fitt5")->SetLineColor(kBlue);
	  resp->GetFunction("fit_func5")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fit_func5");
	  TF1* f2 = resp1->GetFunction("fitt5");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  // hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if(i>=25 && i<29)
	{
	  resp->Fit("fit_func6","R");
	  resp1->Fit("fitt6","R");
	  resp1->GetFunction("fitt6")->SetLineColor(kBlue);
	  resp->GetFunction("fit_func6")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fit_func6");
	  TF1* f2 = resp1->GetFunction("fitt6");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if (i==29)
	
	{
	  resp->Fit("fhdm","R");
	  resp1->Fit("fldm","R");
	  resp1->GetFunction("fldm")->SetLineColor(kBlue);
	  resp->GetFunction("fhdm")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fhdm");
	  TF1* f2 = resp1->GetFunction("fldm");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n";
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	  
	}
      else if(i==30)
	{
	  resp->Fit("fit_func7","R");
	  resp1->Fit("fitt7","R");
	  resp1->GetFunction("fitt7")->SetLineColor(kBlue);
	  resp->GetFunction("fit_func7")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fit_func7");
	  TF1* f2 = resp1->GetFunction("fitt7");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n";
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");
	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
	  
      else if (i==31)// && i<35)
	{
	  resp->Fit("fHdm","R");
	  resp1->Fit("fLdm","R");
	  resp1->GetFunction("fLdm")->SetLineColor(kBlue);
	  resp->GetFunction("fHdm")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fHdm");
	  TF1* f2 = resp1->GetFunction("fLdm");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n";
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	  
	  
	}
      else if (i==32)// && i<35)
	{
	  fHdm->SetRange(320,590);
	  fLdm->SetRange(630,760);
	  resp->Fit("fHdm","R");
	  resp1->Fit("fLdm","R");
	  resp1->GetFunction("fLdm")->SetLineColor(kBlue);
	  resp->GetFunction("fHdm")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fHdm");
	  TF1* f2 = resp1->GetFunction("fLdm");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n";
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if (i==33 ||i==34) //many desripancies
	{
	  fHdm->SetRange(300,540);
	  fLdm->SetRange(585,700);
	  resp->Fit("fHdm","R");
	  resp1->Fit("fLdm","R");
	  resp1->GetFunction("fLdm")->SetLineColor(kBlue);
	  resp->GetFunction("fHdm")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fHdm");
	  TF1* f2 = resp1->GetFunction("fLdm");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  // hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n";
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      else if(i==35)
	{
	  fHdm->SetRange(300,540);
	  fLdm->SetRange(525,760);
	  resp->Fit("fHdm","R");
	  resp1->Fit("fLdm","R");
	  resp1->GetFunction("fLdm")->SetLineColor(kBlue);
	  resp->GetFunction("fHdm")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("fHdm");
	  TF1* f2 = resp1->GetFunction("fLdm");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  // hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //lp3[i]= f2->GetParameter(3);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //	  file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n";
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);
	}
      
      else
	{
	  FHdm->SetRange(300,590);
	  FLdm->SetRange(500,825);
	  resp->Fit("FHdm","R");
	  resp1->Fit("FLdm","R");
	  resp1->GetFunction("FLdm")->SetLineColor(kBlue);
	  resp->GetFunction("FHdm")->SetLineColor(kRed);
	  TF1* f3 = resp->GetFunction("FHdm");
	  TF1* f2 = resp1->GetFunction("FLdm");
	  hp0[i]= f3->GetParameter(0);
	  hp1[i]= f3->GetParameter(1);
	  //hp2[i]= f3->GetParameter(2);
	  //hp3[i]= f3->GetParameter(3);
	  //lp2[i]= f2->GetParameter(2);
	  //	  lp3[i]= f2->GetParameter(3);
	  lp0[i]= f2->GetParameter(0);
	  lp1[i]= f2->GetParameter(1);
	  //file<<i<<"\t"<<hp0[i]<<"\t"<<hp1[i]<<"\t"<<hp2[i]<<"\t"<<hp3[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\t"<<lp2[i]<<"\t"<<lp3[i]<<"\n";
	  file<<i<<"\t"<<hp0[i]<<"\t"<<"\t"<<hp1[i]<<"\t"<<lp0[i]<<"\t"<<lp1[i]<<"\n"; 
	  sprintf(Name,"Layer%02d",i);
	  mg->SetTitle(Name);
	  mg->GetXaxis()->SetTitle("distance from the beamline");
	  mg->GetYaxis()->SetTitle("average hits");
	  
	  mg->Add(resp);
	  mg->Add(resp1);
	  mg->Draw("ap");
	  // Define the fit function here.
	  TF1 *f_l =  (TF1*)f3->Clone("f2");
	  TF1 *f_h = (TF1*)f2->Clone("f3");
	  f_l->SetLineStyle(2);
	  f_h->SetLineStyle(2);
	  f_l->SetRange(0, 1400);
	  f_h->SetRange(0, 1400);
	  f_l->Draw("Lsame");
	  f_h->Draw("Lsame");
	  auto legend = new TLegend(0.75,0.35,0.99,0.5);
	  legend->SetHeader("","C");
	  legend->AddEntry(resp, resp->GetName(),"p");
	  legend->AddEntry(resp1, resp1->GetName(),"p");
	  legend->AddEntry(f3,"Hdm fit", "L");
	  legend->AddEntry(f2,"Ldm fit", "L");

	  gStyle->SetLegendTextSize(0.03);
	  legend->Draw();

	  Canvas_1_n2->Modified();
	  Canvas_1_n2->cd();
	  Canvas_1_n2->SetSelected(Canvas_1_n2);


	  Canvas_1_n2->SaveAs(full_path2);


	
      
	}
    }
  
  
  //  f->Close();
}
      
  
  
