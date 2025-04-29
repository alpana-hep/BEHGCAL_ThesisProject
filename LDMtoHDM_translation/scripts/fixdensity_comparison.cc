#include<iostream>
#include<fstream>
#include<stdio.h>
#include <vector>
#include <string>
#include <map>

using  namespace std;
float fit_func(float x, float p0, float p1) //define the expo function which will calculate average occupancies
{

  float function = exp(p0+p1*x);
  return function;
}
float fix_occupi(float x) //define the expo function which will calculate average occupancies
{

  float function = x;
  return function;
}
//fix density function is defined here
float fix_density(float ldm_number)
{
  float x ;
  x= ldm_number;
  float funct  = 432.0*(1.0-pow((192.0-x)/192.0,192.0/432.0));
  return funct;
}

void fixdensity_comparison(string data_file, string correct_file)
{
  char* fname = new char[2000];
  char* fname1 = new char[2000];
  //root file for outputs
  TFile* f = new TFile("fix_density_comparison_v0.root","Recreate");
  //text file for output
  std::ofstream file;
  file.open("fixdensity_comparison_v1.txt",ios::out);
  //reading the input files
  sprintf(fname,"%s",correct_file.c_str());
  sprintf(fname1,"%s",data_file.c_str());
  std::fstream fixdensity_file; //file containing fixdensity event by event corrections
  fixdensity_file.open(fname1,ios::in);
  std::fstream correction_file; //file containing modules needing corrections and corresponding predicted(hd nd ld)
  correction_file.open(fname,ios::in);
  

  //structures declared here to read and store i/o
  struct modulemap { //map for correct file
    unsigned int  layer, u, v, type,typee;
    char shape;
    float R,CMS_occup,HD_occup,LD_occup,mean_error;
  };
  std::vector <modulemap> map_vec;
  
  struct fixdensity{ //read fix density file
    unsigned int  layer, u, v,type,typee;
    char shape;
    
    float fix_average,rms,rms_error,mean_error;
  };
  std::vector <fixdensity> fix_vec;
  //to store output information
  struct output_map{
    unsigned int  layer, u, v, type;
    char shape;
    float R,CMS_occup,HD_occup,LD_occup;
    float fix_density,fix_pred,fix_occup,mean_error;
  };
  std::vector <output_map> out_vec;

  if(!correction_file.is_open())
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
	correction_file >> layer >> u >> v >> type >>typee >> shape
			>> R >> CMS_occup>>HD_occup>>LD_occup>>mean_error;

	if (correction_file.eof()) break;
	modulemap mm;
	mm.layer=layer; mm.u=u; mm.v=v; mm.type=type; mm.typee = typee;
	mm.shape=shape;
	mm.R=R;
	mm.mean_error=mean_error;
	mm.CMS_occup=CMS_occup;
	mm.LD_occup= LD_occup;
	mm.HD_occup=HD_occup;
	map_vec.push_back(mm);
	//      cout<<map_vec.size()<<endl;
      }
      correction_file.close();
    }
  //read fix density file
  
  if(!fixdensity_file.is_open())
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    {
      fix_vec.clear();
      while (true) {
	unsigned int  layer, u, v, type,typee;
	char shape;
	float fix_average,error_mean,rms,rms_error;
	fixdensity_file >> layer >> u >> v >> type >> typee >> shape
			>>fix_average >>error_mean;// >>rms >>rms_error;
			

	if (fixdensity_file.eof()) break;
	fixdensity mm;
	mm.layer=layer; mm.u=u; mm.v=v; //mm.type=type; mm.typee=typee;mm.shape=shape;
	 mm.fix_average = fix_average;
	 mm.mean_error=error_mean;
	 //	 cout<<error_mean<<endl;
	fix_vec.push_back(mm);
        //cout<<fix_vec.size()<<endl;
      }
      fixdensity_file.close();
    }
  float fix_predicted,fix_calculated,x,fix_occup;
  for(int iModule=0;iModule<(int(map_vec.size()));iModule++)
    {

      output_map mm;
      for(int ifix=0;ifix<(int(fix_vec.size()));ifix++)
	{
	  if(map_vec[iModule].layer==fix_vec[ifix].layer && map_vec[iModule].u==fix_vec[ifix].u && map_vec[iModule].v==fix_vec[ifix].v)
	    {
	      x= map_vec[iModule].CMS_occup;
	      fix_predicted= fix_density(x);
	      fix_occup = fix_occupi(x);
	      fix_calculated = fix_vec[ifix].fix_average;
	      //cout<<fix_predicted<<"\t"<<"eventbyevent"<<"\t"<<fix_calculated<<endl;
	      mm.layer=map_vec[iModule].layer; mm.u=map_vec[iModule].u; mm.v=map_vec[iModule].v;
	      mm.shape = map_vec[iModule].shape;
	      mm.type= map_vec[iModule].type;
	      mm.R = map_vec[iModule].R;
	      mm.CMS_occup= map_vec[iModule].CMS_occup;
	      mm.HD_occup= map_vec[iModule].HD_occup;
	      mm.LD_occup= map_vec[iModule].LD_occup;
	      mm.fix_density=fix_calculated;
	      mm.fix_pred=fix_predicted;//fix_predicted= calculated using formula for fix density
	      mm.fix_occup=fix_occup;
	      mm.mean_error= fix_vec[iModule].mean_error;
	      //cout<<fix_vec[iModule].mean_error<<endl;
	      out_vec.push_back(mm);
	      file << map_vec[iModule].layer <<"\t"<< map_vec[iModule].u <<"\t"<< map_vec[iModule].v <<"\t"<<map_vec[iModule].type <<"\t"<<map_vec[iModule].shape<<"\t"<<map_vec[iModule].R<<"\t"<<map_vec[iModule].CMS_occup<<"\t"<<"\t"<<map_vec[iModule].LD_occup<<"\t \t"<<fix_calculated<<"\t"<<fix_predicted<<"\t\t"<< map_vec[iModule].HD_occup<<"\n";
	      
	      break;
	    }
	  else
	    continue;
	}
    }

  //define variables to store values using Tgraph
  float Fl[168]={};
  float CMSSW[168]={};
  float Fh[168]={};
  float index[168]={};
  float r[168]={};
  float fix_pred[168]={};
  float fix_cal[168]={};
  float fix_occupa[168]={};
  float error[168]={};
  float ratio[168]={};
  for(int i=0;i<(int(out_vec.size()));i++)
    {
      index[i]=i;
      CMSSW[i]=out_vec[i].CMS_occup;
      Fl[i]=out_vec[i].LD_occup;
      Fh[i]=out_vec[i].HD_occup;
      fix_pred[i]=out_vec[i].fix_pred; //fix_pred= calculated using formula
      fix_cal[i]=out_vec[i].fix_density;//fix_cal= calculated event by event
      fix_occupa[i]=out_vec[i].fix_occup;
      error[i]=out_vec[i].mean_error;
      if(error[i]==0)
	{
	  ratio[i]=0;
	  continue;}
      else
	{
	  ratio[i]=(fix_pred[i]- Fh[i])/error[i];
	}
      }
  int n=(int(out_vec.size()));
  TGraph* gr = new TGraph(n,Fl,Fh);
      gr->SetMarkerStyle(8);
      gr->SetMarkerColor(kBlue);
      gr->SetName("ldm_hdm");
      //gr->Draw("AP");
      TF1* f2= new TF1("f2"," [0]*432.0*(1.0-pow((192.0-x)/192.0,192.0/432.0))",0,190);
      f2->SetParameter(0,1);
      f2->SetRange(0,190);
      //  f2->Draw("LSAME");
      gr->Write();
      //gr->Draw("APSAME");
      TGraph* gr1 = new TGraph(n,index,Fh);
      gr1->SetMarkerStyle(8);
      gr1->SetMarkerColor(kBlue);
      TGraph* gr2 = new TGraph(n,index,fix_pred);
      gr2->SetMarkerStyle(33);
      gr2->SetMarkerColor(kRed);
      TGraph* gr3 = new TGraph(n,index,fix_cal);
      gr3->SetMarkerStyle(kFullTriangleUp);
      gr3->SetMarkerColor(kBlack);
      TGraph* gr4 = new TGraph(n,index,fix_occupa);
      gr4->SetMarkerStyle(33);
      gr4->SetMarkerColor(kGreen);
      TCanvas *c = new TCanvas("c", "canvas", 800, 800);

      TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
      pad1->SetBottomMargin(0.01); // Upper and lower plot are joined
      //  pad1->SetGridx();         // Vertical grid
      pad1->Draw();             // Draw the upper pad: pad1
      pad1->cd();
      
      TMultiGraph* mg = new TMultiGraph();
      mg->SetName("comparison_fix_density");
       mg->GetXaxis()->SetTitle("index number");
      mg->GetYaxis()->SetTitle("average number of hits");

       mg->Add(gr1);
      mg->Add(gr2);
           mg->Add(gr3);
	   //   mg->Add(gr4);
      mg->Draw("ap");
      auto legend = new TLegend(0.7,0.7,0.9,0.9);
      legend->SetHeader("","C");
      legend->AddEntry(gr1,"HD prediction","p");
      legend->AddEntry(gr2,"fd cal using formula","p");
      legend->AddEntry(gr3,"fd cal event by event", "p");
      //    legend->AddEntry(gr4,"fixed number", "p");

      gStyle->SetLegendTextSize(0.02);
      legend->Draw();
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
      
      f->Close();
      

      //    }
  
}
