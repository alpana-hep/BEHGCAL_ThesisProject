#include <stdio.h>
#include<fstream>
#include <vector>
#include <string>
#include <map>
#include<iostream>
using namespace std;
double calculateError(float x, float y, float ex, float ey,float cov )
{
  return (x/y)*(sqrt(((ex/x)*(ex/x))+((ey/y)*(ey/y))-(2*cov/x*y)));
    }
void overlayPlots_v1(string filename, string filename1, string path, string histname, string tag_name, string tag_name1, int xmin, int xmax, int ymin, int ymax)//, string xaxis_label, string yaxis_label)
{
  char* hname = new char[200];
 char* hist_name  = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
char* full_path = new char[2000];
  char* full_path1 = new char[2000];
  char* full_path2 = new char[2000];
  char* path2 = new char[2000];
  char* title= new char[2000];
char* title1= new char[2000];char* title2= new char[2000];
  char* full_path3 = new char[2000];
  char* full_path4 = new char[2000];
  char* full_path5 = new char[2000];
  char* full_path6 = new char[2000];
  char* full_path7 = new char[2000];
  char* full_path8 = new char[2000];
  char* full_path9 = new char[2000];
  char* full_path10 = new char[2000];
  char* full_path11= new char[2000];
  char* tag = new char[2000];
  char* tag1 = new char[2000];
  sprintf(tag,"%s",tag_name.c_str());
  sprintf(tag1,"%s",tag_name1.c_str());
  sprintf(hname,"%s",filename.c_str());
  sprintf(path2,"%s",path.c_str());
  sprintf(hist_name1,"%s",histname.c_str());
 
  TFile * inputfile = new TFile(hname,"READ");
  char* reso_name=new char[1000];
  char* resp_name=new char[1000];
  char* name=new char[1000];
  char* name1=new char[1000];
  char* name2=new char[1000];


  const char *data[4] = {"", "Valid", "Train",
                             "Tbdata" };
  const char *data3[4]={"Simulation","Simulation","Simulation","Data"};
const char *Energy[8]={"20","50","80","100","120","200","250","300"};
int Elist[8]={20,50,80,100,120,200,250,300};
 // int Elist[85] =  {10, 14 , 18 , 22 , 26 , 30,  34 , 38 , 42,  46,  50,  54,  58,  62,  66,  70,  74,  78,
 //                     82,  86,  90,  94,  98, 102, 106, 110, 114 ,118, 122, 126, 130, 134, 138, 142 ,146, 150,
 //                     154 ,158 ,162 ,166 ,170 ,174 ,178, 182, 186, 190 ,194 ,198, 202, 206, 210, 214 ,218, 222,
 //                     226, 230, 234 ,238, 242 ,246 ,250, 254, 258 ,262 ,266, 270, 274, 278, 282, 286, 290, 294,
 //                     298, 302 ,306, 310, 314, 318 ,322, 326, 330, 334 ,338 ,342 ,346};
 
 //float xmin=0, xmax=0,
 float xhigh=0;
 float mean=0, sigma=0, mean_err=0,sigma_err=0,Err_resolution=0, Err_response=0,resolution=0, response=0;
 int rebin=4;
 TF1* fit_func_data = new TF1("fit_func_data","gaus");
 for(int i_data=0; i_data<3; i_data++)
   {
     for(int i_en=0;i_en<8;i_en++)
       {
	 if(Elist[i_en]<=80)
	   xhigh= 5.0*Elist[i_en];
	 else
	   xhigh= 2.5*Elist[i_en];
	 //      cout<<xhigh<<endl;
	 if(i_data==0)
	   sprintf(hist_name,"%s_TrueEn_%d",hist_name1,Elist[i_en]);//,Elist[i_en]+4);
	 else
	   sprintf(hist_name,"%s_%s_TrueEn_%d",hist_name1,data[i_data],Elist[i_en]);
	 cout<<hist_name<<endl;
     TH2F* resp= (TH2F*)inputfile->Get(hist_name);
     //sprintf(hist_name,"h2d_fracLeakVsChi2_categ4_TrueEn_%d",Elist[i_en]);
     TH2F* resp1= (TH2F*)inputfile->Get(hist_name);
     resp->RebinX(2);
     resp->RebinY(2);
     //resp1->Rebin2D(6);
     Double_t factor = 1.;
     TCanvas *Canvas_n2 = new TCanvas(hist_name, hist_name,900,700);
     //Canvas_n2->Range(-60.25,-0.625,562.25,0.625);
     Canvas_n2->SetLeftMargin(0.14);
     Canvas_n2->SetTopMargin(0.034);
     Canvas_n2->SetBottomMargin(0.12);
     Canvas_n2->SetRightMargin(0.11);
     
     Canvas_n2->SetFillColor(0);
     Canvas_n2->SetBorderMode(0);
     Canvas_n2->SetBorderSize(2);

     // TPaveStats *ptstats = new TPaveStats(0.75,0.75,0.9,0.9,"brNDC");
     // ptstats->SetBorderSize(1);
     // ptstats->SetFillColor(0);
     // ptstats->SetLineColor(kRed);
     // ptstats->SetTextAlign(12);
     // ptstats->SetTextColor(kRed);
     // ptstats->SetTextFont(42);
     // ptstats->SetOptStat(1111);
     // ptstats->SetOptFit(1);
     // resp->GetListOfFunctions()->Add(ptstats);
     // ptstats->SetParent(resp);
     
     // TPaveStats *ptstats1 = new TPaveStats(0.75,0.6,0.9,0.75,"brNDC");
     // ptstats1->SetBorderSize(1);
     // ptstats1->SetFillColor(0);
     // ptstats1->SetLineColor(kBlue);
     // ptstats1->SetTextAlign(12);
     // ptstats1->SetTextColor(kBlue);
     // ptstats1->SetTextFont(42);
     // ptstats1->SetOptStat(1111);
     // ptstats1->SetOptFit(1);
     // resp1->GetListOfFunctions()->Add(ptstats1);
     // ptstats1->SetParent(resp1);

     // TPaveStats *ptstats2 = new TPaveStats(0.75,0.45,0.9,0.6,"brNDC");
     // ptstats2->SetBorderSize(1);
     // ptstats2->SetFillColor(0);
     // ptstats2->SetLineColor(kBlack);
     // ptstats2->SetTextAlign(12);
     // ptstats2->SetTextColor(kBlack);
     // ptstats2->SetTextFont(42);
     // ptstats2->SetOptStat(1111);
     // ptstats2->SetOptFit(1);
     // resp2->GetListOfFunctions()->Add(ptstats2);
     // ptstats2->SetParent(resp2);
     
     sprintf(full_path,"%s/%s_%s_TrueEn_%03d_2d.png",path2,hist_name1,data[i_data],Elist[i_en]);
     sprintf(full_path1,"%s/%s_%s_TrueEn_%03d_2d.pdf",path2,hist_name1,data[i_data],Elist[i_en]);
     sprintf(title,"%s",tag);
     resp->GetYaxis()->SetTitle(title);//"F^{Longi}_{Leak} = E^{Longi}_{Leak}/E_{True}");//_{GNN}");
     sprintf(name,"%s",tag1);//GNN-%s}",tag);
     resp->GetXaxis()->SetTitle(name);//#chi^{2}}");
     sprintf(title,"");//(%d GeV)",Elist[i_en]);//,data[i_data]);

     resp->GetXaxis()->SetTitleSize(00.05);
     resp->GetXaxis()->SetLabelSize(0.04);
     resp->GetYaxis()->SetLabelSize(0.04);
     resp->GetYaxis()->SetTitleSize(00.05);
     resp->GetYaxis()->SetTitleOffset(1.25);
     //     resp->SetMaximum(20);
     // resp->SetLineColor(kRed);
     // resp1->SetLineColor(kBlue);
     //         resp2->SetLineColor(kBlack);

     // resp->SetLineWidth(2);
     // resp1->SetLineWidth(2);
     // // resp2->SetLineWidth(2);
     // resp->SetName(name);
     resp->SetTitle(title);
     // resp1->SetName(name1);
     // resp1->SetTitle(title);
     // resp2->SetName(name2);
     // resp2->SetTitle(title);
     gStyle->SetOptStat(0);
     xmax = xmax*Elist[i_en];
     ymax= ymax*Elist[i_en];
     resp->GetXaxis()->SetRangeUser(xmin,xmax);//xmax*Elist[i_en]);
     resp->GetYaxis()->SetRangeUser(ymin,70+Elist[i_en]);//Elist[i_en]+40);//xhigh);
     // resp1->GetXaxis()->SetRangeUser(0,xhigh);
     // resp1->GetYaxis()->SetRangeUser(0,xhigh);

     //       resp->GetYaxis()->SetRangeUser(0,Elist[i_en]+50);

     //     resp1->GetXaxis()->SetRangeUser(-2,2);//0,xhigh);
     // resp2->GetXaxis()->SetRangeUser(0,xhigh);
     auto legend = new TLegend(0.42,0.78,0.83,0.89);
     legend->SetLineColor(kWhite);
     char* lhead = new char[100];
     sprintf(lhead,"%s : #bf{%d GeV #pi^{-}}",data3[i_data],Elist[i_en]);
     legend->SetHeader(lhead);//SetHeader(lhead);
     // TLegendEntry* l1 = legend->AddEntry(resp,lhead);//,"l");
     // l1->SetTextColor(kRed);
     auto legend1 = new TLegend(0.42,0.8,0.83,0.89);
     legend1->SetLineColor(kWhite);
     //     char* lhead = new char[100];
     sprintf(lhead,"%s : #bf{%d GeV #pi^{-}}",data3[i_data],Elist[i_en]);
     legend1->SetHeader(lhead);
     sprintf(lhead,"#chi^{2} method");//DRN (E,x,y,z)");
     TLegendEntry* l2 = legend->AddEntry(resp,lhead,"");
     l2->SetTextColor(kRed);

     gStyle->SetLegendTextSize(0.047);
     //legend->SetHeader("","C");
     // legend->AddEntry(resp,"Chi2","l");
     // legend->AddEntry(resp1,"Model","l");
     //    legend->AddEntry(resp2,"TB Data","l");
     // if(i_en==1 || i_en==5 || i_en==4)
     //   {
     // 	 //	 resp2->Draw("HIST");
     // 	 resp1->Draw("HIST");
     // 	 resp->Draw("HIST sames");
     //   }
     // else
     //   {
     // 	 resp1->Draw("HIST");
     // 	 //resp2->Draw("HIST sames");
     //     resp->Draw("HIST sames");
     //   }
     // legend->Draw("sames");
     gStyle->SetOptStat(0);
     resp1->SetLineColor(kRed);
     resp1->SetLineWidth(3);
     resp->Draw("colz");
     legend->Draw("sames");    
     //     resp1->Draw("box sames");
     Canvas_n2->Modified();
     Canvas_n2->cd();
     Canvas_n2->SetSelected(Canvas_n2);
     Canvas_n2->SaveAs(full_path);
     Canvas_n2->SaveAs(full_path1);

       }
   }
}
