const int n_pl = 4;
bool logx = false;
TString pls[n_pl] = {"FTFP_BERT_EMN","QGSP_FTFP_BERT_EMN","FTFP_BERT_EMM","QGSP_BERT"};
TString dRs[9] = {"dR < 0.56cm","dR < 1.0cm","dR < 2.0cm","dR < 3.0cm","dR < 5.0cm","dR < 8.0cm","dR < 12.0cm","dR < 18.0cm","dR < 20.0cm"};
const char* legend_text[5]={"n10+=10","n20+=10","n30+=10","nHits+=10","Scale-Off"};
TString legend_text1[3]={"CMSSW(110k)","Correlated pseudo(1M)","Uncorrelated pseudo(1M)"};//,"200kBX","400kBX","600kBX","800kBX","1000kBX","1500kBX"};
TString legend_text2[2]={"Geometry D86","Geometry D49"};
int line_width[12] = {3,3,3,3,3,2,2,2,2,2,2,2};
int line_style[12] = {1,1,1,3,5,1,1,1,1,1,1,1};
// /int line_color[n_pl+1] = {kBlack, kRed, kGreen+2, kBlue, kRed};
int line_color[9] = {kViolet+1,kRed+1,kGreen+2,kCyan + 1 ,kGray+2,kRed, kBlue + 2 , kCyan + 1 , kGreen + 3 };
//int line_color[11] = {kPink+1, kRed, kBlue,kGray+1 , kGreen+2, kMagenta, kYellow + 2 , kCyan+3,  kBlue + 2 ,kRed+2,kGreen + 3 };
void setLastBinAsOverFlow(TH1F*);
std::map<int,int> rebin_;
std::map<int,float> x_max;
std::map<int,float> x_min;
void rebin_init_layer_pi(){
  rebin_.insert(pair<int, int>(20,2));
  rebin_.insert(pair<int, int>(50,2));
  rebin_.insert(pair<int, int>(100,2));
  rebin_.insert(pair<int, int>(200,4));
    
  x_max.insert(pair<int, int>(20,2500.0));
  x_max.insert(pair<int, int>(50,6000.0));
  x_max.insert(pair<int, int>(100,12000.0));
  x_max.insert(pair<int, int>(200,22000.0));
  
  x_min.insert(pair<int, int>(20,0.0));
  x_min.insert(pair<int, int>(50,0.0));
  x_min.insert(pair<int, int>(100,0.0));
  x_min.insert(pair<int, int>(200,0.0));
  x_min.insert(pair<int, int>(300,0.0));
  
}
void setLastBinAsOverFlow(TH1F* h_hist){
  double lastBinCt =h_hist->GetBinContent(h_hist->GetNbinsX()),overflCt =h_hist->GetBinContent(h_hist->GetNbinsX()+1);
  double lastBinErr=h_hist->GetBinError(h_hist->GetNbinsX()),  overflErr=h_hist->GetBinError(h_hist->GetNbinsX()+1);
  if(lastBinCt!=0 && overflCt!=0)
    lastBinErr = (lastBinCt+overflCt)* (sqrt( ((lastBinErr/lastBinCt)*(lastBinErr/lastBinCt)) + ((overflErr/overflCt)*(overflErr/overflCt)) ) );

  else if(lastBinCt==0 && overflCt!=0)
    lastBinErr = overflErr;
  else if(lastBinCt!=0 && overflCt==0)
    lastBinErr = lastBinErr;
  else lastBinErr=0;

  lastBinCt = lastBinCt+overflCt;
  h_hist->SetBinContent(h_hist->GetNbinsX(),lastBinCt);
  h_hist->SetBinError(h_hist->GetNbinsX(),lastBinErr);

}

void generate_1Dplot(vector<TH1F*> hist, char const *tag_name="", int energy=-1, char const *leg_head="",
		     bool normalize=false, bool log_flag=true, bool DoRebin=false, bool save_canvas=true, char const *title="",vector<string>legend_txt={""}){  
  

  TCanvas *c = new TCanvas(tag_name, tag_name, 900, 750);
  c->SetRightMargin(0.025);
  c->SetLeftMargin(0.14);
  c->SetTopMargin(0.02);
  c->SetBottomMargin(0.12);
  gStyle->SetOptStat(1111111);
  //   gStyle->SetOptStat(0);
  //double pvt_x_min = 0.6;
  double pvt_x_min = 0.75;
  double pvt_x_max = 0.99;
  double pvt_y_min = 0.9;
  //double pvt_dely = 0.18;
  double pvt_dely = 0.15;
  gStyle->SetOptStat(0);

 //gStyle->SetOptFit(0);
  vector<TString> legName;
  //TLegend *legend = new TLegend(0.65,0.95,0.99,0.75);
  std::string leg_head_str = leg_head;
  double x = 0.15;
  double y = 0.90;
  TLegend *legend;
  //legend = new TLegend(0.60,0.88,0.98,0.72);  
  //    legend = new TLegend(0.75,0.75,0.95,0.95);  
  legend = new TLegend(0.55,0.78,0.97,0.97);
 
  legend->SetTextSize(0.028); 
  gStyle->SetLegendTextSize(0.028);
  legend->SetLineColor(kWhite);
  char* lhead = new char[100];

  sprintf(lhead,"#bf{%s} ",title);//legend_text[0]);//, tag[i]);                                                                         
  legend->SetNColumns(2);
    legend->SetHeader(lhead);
    char* text_name = new char[10000];
  TLegendEntry* leg_entry[11];
  float x_label_size = 0.025;
  double ymin = 100000.0;
  double ymax = 0.0;
  cout<<" hist.size() = "<<hist.size()<<endl;


  for(int i = 0; i < (int)hist.size(); i++) {
  	
        normalize = true;
    if(normalize) {
      //hist.at(i)->Scale(1.0/hist.at(i)->Integral());
      //hist.at(i)->GetYaxis()->SetTitle("Normalized");
    }
    else {
      hist.at(i)->GetYaxis()->SetTitle("Entries");
    }
    
    hist.at(i)->SetLineWidth(line_width[i]);
    
    hist.at(i)->SetLineStyle(line_style[i]);
    hist.at(i)->SetLineColor(line_color[i]);
    hist.at(i)->SetTitle(" ");
    
    hist.at(i)->GetXaxis()->SetTitleSize(0.04);
    hist.at(i)->GetXaxis()->SetLabelSize(x_label_size);
    hist.at(i)->GetXaxis()->SetLabelSize(0.030);
    hist.at(i)->GetYaxis()->SetLabelSize(0.030);
    hist.at(i)->GetYaxis()->SetTitleOffset(1.45);
    if(DoRebin) {
     hist.at(i)->Rebin(4);
    }
    /* hist.at(i)->GetXaxis()->SetRangeUser(x_min[energy],x_max[energy]); */
    legName.push_back(hist.at(i)->GetName());
    leg_entry[i] = legend->AddEntry(hist.at(i),legend_text[i],"l");
    leg_entry[i]->SetTextColor(hist.at(i)->GetLineColor());
    sprintf(text_name,"%.03f %0.1f",hist.at(i)->GetMean(),hist.at(i)->GetRMS());
    leg_entry[i+(int)hist.size()] = legend->AddEntry(hist.at(i),legend_txt[i].c_str(),"");
    leg_entry[i+(int)hist.size()]->SetTextColor(hist.at(i)->GetLineColor());

    if(hist.at(i)->GetMaximum() > ymax) ymax = hist.at(i)->GetMaximum();
    if(hist.at(i)->GetMinimum() < ymin) ymin = hist.at(i)->GetMinimum();
    setLastBinAsOverFlow(hist.at(i));
    
    

  }
  if(ymin == 0.0) ymin = 1e-1;
  if(ymin<0.0) ymin = 1e-2;
  for(int i = 0; i < (int)hist.size(); i++) {
    if(!normalize) hist.at(i)->GetYaxis()->SetRangeUser(ymin,ymax*10);
    else
      {  hist.at(i)->GetYaxis()->SetRangeUser(ymin,ymax*10.0);
	//	hist.at(i)->GetXaxis()->SetRangeUser(0,xmax_[i]);
	
      }
  	if(!i) hist.at(i)->Draw("hist");
    else   hist.at(i)->Draw("hist sames");

  }
  
    legend->Draw();



  if(log_flag) {
      gPad->SetLogy();
    }
  if(logx)
    gPad->SetLogx();

  
  gPad->Update();

 
  TLatex* textOnTop = new TLatex();
  textOnTop->SetTextSize(0.035);
  //  textOnTop->DrawLatexNDC(0.12,0.96,"CMS #it{#bf{Preliminary}}");
  
  char* en_lat = new char[500];
  textOnTop->SetTextSize(0.035);
  int inlumi=137;
  // sprintf(en_lat,"#bf{%0.1d fb^{-1} (13 TeV)}",inlumi);
  //textOnTop->DrawLatexNDC(0.7,0.96,en_lat);

 

  gPad->Modified();
  char* canvas_name = new char[1000];
  if(save_canvas) {
    sprintf(canvas_name,"/eos/user/k/kalpana/www/folder/HGCAL_TDAQ/Plots/May2023_EmulStudies_/%s.png",tag_name);
    c->SaveAs(canvas_name);
    sprintf(canvas_name,"/eos/user/k/kalpana/www/folder/HGCAL_TDAQ/Plots/May2023_EmulStudies_/%s.pdf",tag_name);
    c->SaveAs(canvas_name);

  }
  
}
const int nfiles=100,nBG=6;                                                                                                                                                              
TFile *f[nfiles];

TFile *f1[nfiles];

TFile *f2[nfiles];

TFile *f3[nfiles];

TFile *f4[nfiles];

TFile *f5[nfiles];

void v1_2Dplots_EcondvsTruncation(int study,string outfile,int layer)
{
  char* hname = new char[200];
  char* hist_name  = new char[200];
  char* hist_name1 = new char[200];
  char* hist_name2 = new char[200];
  char* hist_name3 = new char[200];
  char* hist_name4 = new char[200];
  char* hist_name5 = new char[200];
  char* hist_name6 = new char[200];

  char* full_path = new char[2000];
  char* full_path1 = new char[2000];
  char* full_path2 = new char[2000];
  char* path2 = new char[2000];
  char* title= new char[2000];
  char* full_path3 = new char[2000];
  char* full_path4 = new char[2000];
  char* full_path5 = new char[2000];
  char* full_path6 = new char[2000];
  char* full_path7 = new char[2000];
  char* full_path8 = new char[2000];
  char* full_path9 = new char[2000];
  char* full_path10 = new char[2000];
  char* full_path11= new char[2000];
  char *leg_head = new char[200];

  //reading mapping file
  std::ifstream mapping_file;
  mapping_file.open("./updated_luv_ncells_map_cmssw_D86_V10.txt",ios::in);
  struct modulemap {
    unsigned int  layer, u, v, type,luv;
    int nCells,nHalfHgcRocs;
  };
  std::vector <modulemap> map_vec;
  std::vector<int> map_layer;
  std::vector<int> map_wafer_u;
  std::vector<int> map_wafer_v;
  std::vector<int> map_type;
  std::vector<int> map_nCells;
  std::vector<int> map_nHalfHgcRocs;
  std::vector<int> map_LUV;
  if(!mapping_file.is_open())
    {
      std::cout << " file not opened" << std::endl;
    }
  else
    { map_vec.clear();
      map_vec.clear();
      map_layer.clear();
      map_wafer_u.clear();
      map_wafer_v.clear();
      map_type.clear();
      map_nCells.clear();
      map_nHalfHgcRocs.clear();
      map_LUV.clear();
      
      while (!mapping_file.eof()) {
	unsigned int  layer, u, v, type,luv;
	int nCells,nHalfHgcRocs;
        mapping_file >> layer >> u >> v >> luv >> type  >> nCells >> nHalfHgcRocs ;
        if (mapping_file.eof()) break;
        map_layer.push_back(layer);
        map_wafer_u.push_back(u);
        map_wafer_v.push_back(v);
	map_type.push_back(type);
	map_LUV.push_bakc(luv);
	map_nHalfHgcRocs.push_back(nHalfHgcRocs);
	map_nCells.push_back(nCells);
	
      }
      mapping_file.close();
    }
  cout<<"map_layer.size() "<<map_layer.size()<<"\t"<<map_layer[4499]<<endl;

  
  gr_name ={"TruncatVsLuv_EcondWithNZ","TruncatVsLuv_EcondWithNZ_withElink1","TruncatVsLuv_EcondWithNZ_withElink2","TruncatVsLuv_EcondWithNZ_withElink3","TruncatVsLuv_EcondWithNZ_withElink4","TruncatVsLuv_EcondWithNZ_withElink5","TruncatVsLuv_EcondWithNZ_withElink6","AvePacketSizeVsLuv_EcondWithNZ","AvePacketSizeVsLuv_EcondWithNZ_withElink1","AvePacketSizeVsLuv_EcondWithNZ_withElink2","AvePacketSizeVsLuv_EcondWithNZ_withElink3","AvePacketSizeVsLuv_EcondWithNZ_withElink4","AvePacketSizeVsLuv_EcondWithNZ_withElink5","AvePacketSizeVsLuv_EcondWithNZ_withElink6","elinkVsLuv_EcondWithNZ","AveTotalHitsVsLuv_EcondWithNZ","elink_EcondWithNZ","AvePacketSize_EcondWithNZ","AveTotalHits_EcondWithNZ","AveDataRatesPerElink_EcondWithNZ"};

  xlabel ={"Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Number of elinks","Average packet size","Average total hits","Average data rates per elink"};
  ylabel={"frac of Truncation","frac of Truncation (Nelinks==1)","frac of Truncation (Nelinks==2)","frac of Truncation (Nelinks==3)","frac of Truncation (Nelinks==4)","frac of Truncation (Nelinks==5)","frac of Truncation (Nelinks==6)","Average packet size","Average packet size (Nelinks==1)","Average packet size (Nelinks==2)","Average packet size (Nelinks==3)","Average packet size (Nelinks==4)","Average packet size (Nelinks==5)","Average packet size (Nelinks==6)","number of elinks","Average total hits","No of ECON-Ds","No of ECON-Ds","No of ECON-Ds","No of ECON-Ds"}; 
  vector<string> filetag={"MultiBy1","MultiBy11","MultiBy12","MultiBy13","MultiBy14","MultiBy15","MultiBy16","MultiBy17","MultiBy18","MultiBy20"}; 
  //xValues={0,0,4,5,7,9,28,30,32,34,36};
  const char* baseline[7]={"EcondAsic_FractionTruncated","EcondAsic_MaxBuffer","DroppedPackets","EcondAsic_FractionTruncatedLuv","EcondAsic_AveragePacketWordsLuv","EcondAsic_AverageTotalHitsLuv","EcondAsic_NumberOfElinksLuv"};//{"EcondAsic_AveragePacketWords","EcondAsic_AveragePacketBits","EcondAsic_LinkPacketOccupancy","EcondAsic_MaxBuffer","EcondAsic_FractionEmptyBuffer","EcondAsic_FractionLowBuffer","EcondAsic_FractionMediumBuffer","EcondAsic_FractionHighBuffer","EcondAsic_FractionTruncated","EventSize","DroppedPackets","EcondAsic_AverageN10Hits","EcondAsic_AverageN20Hits","EcondAsic_AverageN30Hits","EcondAsic_AverageTotalHits"};
  const char* data_type[3]={"CMSSW (Scale-Layer-34)","Correlate_Pseudo(Scale-Layer-34)","Uncorrelate_Pseudo    mean stddev"};//
  //  const char* filetag[5]={"n10+=0.1*nCells","n20+=0.1*nCells","n30+=0.1*nCells","Scale-Off","Scale-Off"};//"Scale-1000","Scale-1100","Scale-1200","Scale-2000"};//{"10kBX","100kBX","400kBX","1000kBX","1500kBX"};//{"10kBX","50kBX","100kBX","200kBX","400kBX","600kBX","800kBX","1000kBX","1500kBX"};
  const char* geo[2]={"D49","D86"};
  const char* ip_data[3]={"CMSSW","Correlate_Psuedo","UnCorrelate_Pseudo"};
    char* name = new char[2000];
    char* xtitle = new char[200];
    char* ytitle = new char[200];

      for(int ig=0;ig<2;ig++)
	{
	  vector<TH1F*> hist_list;
	  for(int i_cut=3; i_cut<n;i_cut++)
	    {
	      sprintf(hname,"EconDs_withNZtrucation_geometry_%s_Hits_%s_%s.txt",geo[ig],filetag[i_cut].c_str(),outfile.c_str());
	      std::ifstream file_;
              file_.open(hname,ios::in);
	      struct luv_map {
		unsigned int  luv, elinks;
		float aveHits, aveData, Fractrunc;
	      };
	      std::vector <luv_map> luv_vec;
	      std::vector<int> map_luv;
	      std::vector<int> map_elinks;
	      std::vector<float> map_aveHits;
	      std::vector<float> map_aveData;
	      std::vector<float>map_Fractrunc;
	      if(!file_.is_open())
		{
		  std::cout << " file not opened" << std::endl;
		}
	      else
		{ luv_vec.clear();
		  map_luv.clear();
		  map_elinks.clear();
		  map_aveHits.clear();
		  map_aveData.clear();
		  map_Fractrunc.clear();
		  while (!file_.eof()) {
		    unsigned int  luv, elinks;
		    float aveHits, aveData, Fractrunc;
		    file_ >> luv >> Fractrunc >> elinks >> aveHits >> aveData ;
		    if (file_.eof()) break;
		    map_luv.push_back(luv);
		    map_elinks.push_back(elinks);
		    map_aveHits.push_back(aveHits);
		    map_aveData.push_back(aveData);
		    map_Fractrunc.push_back(Fractrunc);

		  }
		  file_.close();
		}
	      cout<<"map_vec.size() "<<map_luv.size()<<"\t"<<map_layer[4499]<<endl;

	      
	      sprintf(name,"%s_%s_scaling_%s",gr_name[16].c_str(),ip_data[ig],filetag[i_cut].c_str());
	      TH1F* toSave1 = new TH1F(name,"",10,0,10);
	      sprintf(xtitle,"%s",xlabel[16].c_str());
	      sprintf(ytitle,"%s",ylabel[16].c_str());
	      toSave1->GetXaxis()->SetTitle(xtitle);
	      toSave1->GetYaxis()->SetTitle(ytitle);

	      sprintf(name,"%s_%s_scaling_%s",gr_name[17].c_str(),ip_data[ig],filetag[i_cut].c_str());
	      TH1F* toSave2 = new TH1F(name,"",300,0,300);
	      sprintf(xtitle,"%s",xlabel[17].c_str());
	      sprintf(ytitle,"%s",ylabel[17].c_str());
	      toSave2->GetXaxis()->SetTitle(xtitle);
	      toSave2->GetYaxis()->SetTitle(ytitle);
	      sprintf(name,"%s_%s_scaling_%s",gr_name[18].c_str(),ip_data[ig],filetag[i_cut].c_str());
	      TH1F* toSave3 = new TH1F(name,"",500,0,500);
	      sprintf(xtitle,"%s",xlabel[18].c_str());
	      sprintf(ytitle,"%s",ylabel[18].c_str());
	      toSave3->GetXaxis()->SetTitle(xtitle);
	      toSave3->GetYaxis()->SetTitle(ytitle);

	      sprintf(name,"%s_%s_scaling_%s",gr_name[19].c_str(),ip_data[ig],filetag[i_cut].c_str());
              TH1F* toSave4 = new TH1F(name,"",500,0,500);
              sprintf(xtitle,"%s",xlabel[19].c_str());
              sprintf(ytitle,"%s",ylabel[19].c_str());
              toSave4->GetXaxis()->SetTitle(xtitle);
              toSave4->GetYaxis()->SetTitle(ytitle);

	      if(study==11 && i_cut==9) continue;
	      
	    }
	}	  //Nevents++;
	  	    //		  else if (i_cut==1 && resp->GetBinContent(ibin)>2000
		    //cout<<resp->GetBinContent(ibin)<<"\t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
		  //->GetXaxis()->GetBinCenter(h1->GetXaxis()->FindFixBin(somex)); 
	      //resp->Scale(double(resp1->Integral()/resp->Integral()));
	      //	      cout<<i_file<<"\t"<<ig<<"\t"<<i_cut<<"\t"<<double(resp->Integral())<<"\t"<<resp->GetNbinsX()<<endl;
	      //hist_list.push_back(resp);
	  
	  //	}
	    // int energy=-1;
      // sprintf(full_path,"Allin1_%sD86_%s",ip_data[ig],baseline[i_file]);//,filetag[i_cut]);
	  //  generate_1Dplot(hist_list,full_path,energy,leg_head,false,true,false,true,data_type[ig]);
	


}





