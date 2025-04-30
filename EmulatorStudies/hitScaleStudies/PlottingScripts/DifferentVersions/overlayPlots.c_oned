const int n_pl = 4;
bool logx = false;
TString pls[n_pl] = {"FTFP_BERT_EMN","QGSP_FTFP_BERT_EMN","FTFP_BERT_EMM","QGSP_BERT"};
TString dRs[9] = {"dR < 0.56cm","dR < 1.0cm","dR < 2.0cm","dR < 3.0cm","dR < 5.0cm","dR < 8.0cm","dR < 12.0cm","dR < 18.0cm","dR < 20.0cm"};
//TString legend_text[11] = {"No cuts","skimmed","lep-veto","isotrk-veto","Pho-Pt>20","Njets>=2","Dphi-cut","MET>100","MET>250","ST>300","Pho-pt>100"};
TString legend_text[4] = {"pMSSM_MCMC_86_7257","pMSSM_MCMC_70_90438","pMSSM_MCMC_106_19786","pMSSM_MCMC_473_54451"};//{"No cuts","skimmed","lep-veto","isotrk-veto","Dphi-cut","MET>250","ST>300","Pho-pt>100"}; 
int line_width[12] = {2,2,2,2,2,2,2,2,2,2,2,2};
int line_style[12] = {1,1,1,1,1,1,1,1,1,1,1,1};
// /int line_color[n_pl+1] = {kBlack, kRed, kGreen+2, kBlue, kRed};
int line_color[9] = {kBlack, kBlue, kRed, kGreen+2, kMagenta, kRed - 3, kBlue + 2 , kCyan + 1 , kGreen + 3 };
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
		     bool normalize=false, bool log_flag=true, bool DoRebin=false, bool save_canvas=true, char const *title=""){  
  

  TCanvas *c = new TCanvas(tag_name, tag_name, 700, 600);
  c->SetRightMargin(0.009);
  c->SetLeftMargin(0.11);
  c->SetTopMargin(0.05);

    gStyle->SetOptStat(1111111);
  //   gStyle->SetOptStat(0);
  //double pvt_x_min = 0.6;
  double pvt_x_min = 0.75;
  double pvt_x_max = 0.99;
  double pvt_y_min = 0.9;
  //double pvt_dely = 0.18;
  double pvt_dely = 0.15;
  //gStyle->SetOptStat(0);

  //gStyle->SetOptFit(0);
  vector<TString> legName;
  //TLegend *legend = new TLegend(0.65,0.95,0.99,0.75);
  std::string leg_head_str = leg_head;
  double x = 0.15;
  double y = 0.90;
  TLegend *legend;
  //legend = new TLegend(0.60,0.88,0.98,0.72);  
  legend = new TLegend(0.55,0.95,0.95,0.5);  
  legend->SetTextSize(0.03);
  

  TLegendEntry* leg_entry[11];
  float x_label_size = 0.025;
  double ymin = 100000.0;
  double ymax = 0.0;
  cout<<" hist.size() = "<<hist.size()<<endl;


  for(int i = 0; i < (int)hist.size(); i++) {
  	
    
    if(normalize) {
      	hist.at(i)->Scale(1.0/hist.at(i)->Integral());
      	hist.at(i)->GetYaxis()->SetTitle("Normalized");
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
    //    hist.at(i)->GetXaxis()->SetRange(0,1500);
    hist.at(i)->GetYaxis()->SetLabelSize(0.030);
    hist.at(i)->GetYaxis()->SetTitleOffset(1.45);
    if(DoRebin) {
     hist.at(i)->Rebin(4);
      //hist.at(i)->Rebin(1);
    }
    /* hist.at(i)->GetXaxis()->SetRangeUser(x_min[energy],x_max[energy]); */
    legName.push_back(hist.at(i)->GetName());
    leg_entry[i] = legend->AddEntry(hist.at(i),legend_text[i],"l");
    leg_entry[i]->SetTextColor(hist.at(i)->GetLineColor());
    if(hist.at(i)->GetMaximum() > ymax) ymax = hist.at(i)->GetMaximum();
    if(hist.at(i)->GetMinimum() < ymin) ymin = hist.at(i)->GetMinimum();
    setLastBinAsOverFlow(hist.at(i));
    
    

  }
  if(ymin == 0.0) ymin = 1e-2;
  if(ymin<0.0) ymin = 1e-1;
  //  if(ymax<=10) ymax=10;
  for(int i = 0; i < (int)hist.size(); i++) {
    if(!log_flag) hist.at(i)->GetYaxis()->SetRangeUser(0.0,ymax*1.3);
    else
      {  hist.at(i)->GetYaxis()->SetRangeUser(0.01,ymax*10.0);
	//	hist.at(i)->GetXaxis()->SetRangeUser(0,xmax_[i]);
	
      }
  	if(!i) hist.at(i)->Draw("hist");
    else   hist.at(i)->Draw("hist sames");

  }
  
  //   legend->Draw();



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
  //c->Print(canvas_name);
  
  if(save_canvas) {
    sprintf(canvas_name,"./Results/OverlayPlots/%s.png",tag_name);
    // c->SaveAs(canvas_name);
    
    //    sprintf(canvas_name,"./plots/%s.pdf",tag_name);
    c->SaveAs(canvas_name);
    
  }
  
}
const int nfiles=10,nBG=6;                                                                                                                                                              
TFile *f[nfiles];


void overlayPlots()
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
  //string filetag;//=new char[20000];                                                                                                                                                                   
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
  sprintf(path2,"Results/dPhi_METHadJets");
  // TFile *f[nfiles];                                                                                                                                                                                    
  /* f[0] = new TFile("Out_TTGJets_v18_Dphi_MetHadJets_corr_v2.root"); */
  /* f[1] = new TFile("Out_TTjets_v18_Dphi_MetHadJets_corr_v2.root");//ZG                                                                                                                                         */
  /* f[2]= new TFile("Out_ZJets_Gamma_v18_Dphi_MetHadJets_corr_v2.root"); */
  /* f[5] = new TFile("Out_GJets_DR_combined_v18_Dphi_MetHadJets_corr_v2.root"); */
  /* f[3] = new TFile("Out_WGJets_v18_Dphi_MetHadJets_corr_v2.root"); */
  /* f[4] = new TFile("Out_WJets_v18_Dphi_MetHadJets_corr_v2.root"); */
  /* f[6] = new TFile("OutT5bbbbZg_2350_10_Dphi_MetHadJets_corr_v2.root"); */
  /* f[7] = new TFile("OutT5bbbbZg_2350_50_Dphi_MetHadJets_corr_v2.root"); */
  /* f[8] = new TFile("OutT5bbbbZg_2350_200_Dphi_MetHadJets_corr_v2.root"); */
  /* f[9] = new TFile("OutT5bbbbZg_2350_1500_Dphi_MetHadJets_corr_v2.root"); */
  f[0] = new TFile("Out_pMSSM_MCMC_86_7257.root");
  f[1] = new TFile("Out_pMSSM_MCMC_70_90438.root");
  f[2] = new TFile("Out_pMSSM_MCMC_106_19786.root");
  f[3] = new TFile("Out_pMSSM_MCMC_473_54451.root");

  //  const char* baseline[11] = {"nocut","bkg_comp","veto-lep","veto-iso","Phot-pt_20","NhadJetsCut","dPhi_Met","Met_100","Met_250","st_300_Met250","pt_st_Met_250"};
  const char* baseline[9]= {"Nocut","bkg_comp","veto-lep","veto-iso","dPhi_Met","Met_250","st_300_Met250","pt_st_Met_250","nocut"};
  const char* baseline1[11]= {"Nocut","nocut_sam","nocut","Met_100","bkg_comp","veto-lep","veto-iso","dPhi_Met","Met_250","st_300_Met250","pt_st_Met_250"};
  const char* filetag[10]={"pMSSM_MCMC_86_7257","pMSSM_MCMC_70_90438","pMSSM_MCMC_106_19786","pMSSM_MCMC_473_54451","WJets","GJets","T5bbbbZG_10","T5bbbbZG_50","T5bbbbZG_200","T5bbbbZG_1500"};
   /* vector<TH1F*> hist_list_Njets; */
   /*    vector<TH1F*> hist_list_Bjets; */
   /*    vector<TH1F*> hist_list_MET; */
   /*    vector<TH1F*> hist_list_PhoPt; */
   /*    //vector<TH1F*> hist_list_Mt;                                                                                                                                    */
   /*    vector<TH1F*> hist_list_ST; */
   /*    vector<TH1F*> hist_list_HT; */

  for(int i_file=0; i_file<4;i_file++)
    {
      vector<TH1F*> hist_list_Njets;
      vector<TH1F*> hist_list_Bjets;
      vector<TH1F*> hist_list_MET;
      vector<TH1F*> hist_list_PhoPt;
      //vector<TH1F*> hist_list_Mt;
      vector<TH1F*> hist_list_ST;
      vector<TH1F*> hist_list_HT;
      for(int i_cut=0; i_cut<1;i_cut++)
	{
	  //if(i_cut>0 && i_cut!=6)continue;
	  if(i_file<6 )
	    { sprintf(hist_name,"h_NhadJets_%s",baseline1[i_cut]);
	      sprintf(hist_name1,"h_NBJets_%s",baseline1[i_cut]);
	      sprintf(hist_name2,"h_MET_%s",baseline1[i_cut]);
	      sprintf(hist_name3,"h_PhoPt_%s",baseline1[i_cut]);
	      //sprintf(hist_name4,"h_Mt_phoMET_%s",baseline1[i_cut]);
	      sprintf(hist_name5,"h_HT_%s",baseline1[i_cut]);
	      sprintf(hist_name6,"h_St_%s",baseline1[i_cut]);
	    }
	  else
	    {
	      sprintf(hist_name,"h_NhadJets_%s",baseline[i_cut]);
	      sprintf(hist_name1,"h_NBJets_%s",baseline[i_cut]);
	      sprintf(hist_name2,"h_MET_%s",baseline[i_cut]);
	      sprintf(hist_name3,"h_PhoPt_%s",baseline[i_cut]);
	      sprintf(hist_name4,"h_Mt_phoMET_%s",baseline[i_cut]);
	      sprintf(hist_name5,"h_dPhi_phoMet_%s",baseline[i_cut]);
	      sprintf(hist_name6,"h_St_%s",baseline[i_cut]);
	    }
	  TH1F* hist_Njets_temp = (TH1F*)f[i_file]->Get(hist_name);
	  TH1F* hist_Bjets_temp = (TH1F*)f[i_file]->Get(hist_name1);
	  TH1F* hist_MET_temp = (TH1F*)f[i_file]->Get(hist_name2);
	  TH1F* hist_phopt_temp = (TH1F*)f[i_file]->Get(hist_name3);
	  //	  TH1F* hist_Mt_temp = (TH1F*)f[i_file]->Get(hist_name4);
	  TH1F* hist_Ht_temp = (TH1F*)f[i_file]->Get(hist_name5);
	  TH1F* hist_ST_temp = (TH1F*)f[i_file]->Get(hist_name6);
	  hist_MET_temp->Rebin(4);
	  //hist_phopt_temp->Rebin(2);
	  hist_Ht_temp->Rebin(4);
	  hist_ST_temp->Rebin(4);
	  hist_Njets_temp->GetXaxis()->SetTitle("N_{jets}");
	  sprintf(title,"%s",f[i_file]);
	  //hist_Njets_temp->SetTitle("");
	  //	   hist_Bjets_temp->
	  hist_Bjets_temp->GetXaxis()->SetTitle("N_{B-jets}");
	  hist_MET_temp->GetXaxis()->SetTitle("p_{T}^{miss} (GeV)");
	  hist_phopt_temp->GetXaxis()->SetTitle("p_{T}^{#gamma} (GeV)");
	  //	  hist_Mt_temp->GetXaxis()->SetTitle("M_{T}^{#gamma & MET}");
	  hist_Ht_temp->GetXaxis()->SetTitle("HT");
	  hist_Ht_temp->GetXaxis()->SetRangeUser(0,1000);
	  hist_ST_temp->GetXaxis()->SetTitle("EnSum for jets");
	  hist_ST_temp->GetXaxis()->SetRangeUser(0,600);
	  if(i_file==0)
	    {
	      hist_Njets_temp->GetXaxis()->SetRangeUser(0,10);
	      hist_Bjets_temp->GetXaxis()->SetRangeUser(0,10);
	    }
	  else if(i_file==1)
            {
              hist_Njets_temp->GetXaxis()->SetRangeUser(0,10);
              hist_Bjets_temp->GetXaxis()->SetRangeUser(0,5);
            }

	  else
	    {
	      hist_Njets_temp->GetXaxis()->SetRangeUser(0,10);
              hist_Bjets_temp->GetXaxis()->SetRangeUser(0,6);

	    }
	  hist_MET_temp->GetXaxis()->SetRangeUser(0,1000);
	  hist_phopt_temp->GetXaxis()->SetRangeUser(0,140);
	  
	  hist_list_Njets.push_back(hist_Njets_temp);
	  hist_list_Bjets.push_back(hist_Bjets_temp);
	  hist_list_MET.push_back(hist_MET_temp);
	  hist_list_PhoPt.push_back(hist_phopt_temp);
	  hist_list_HT.push_back(hist_Ht_temp);
	  hist_list_ST.push_back(hist_ST_temp);
	  //hist_list_dPhi.push_back(hist_dPhi_temp);
	}
      // }
      //path to save the png file
      int energy=-1;
      //path to save the file
      /* sprintf(full_path6,"Overlay_dPhi_photon_MET_%s",filetag[i_file]); */
      /* generate_1Dplot(hist_list_dPhi,full_path6,energy,leg_head,false,true,false,true); */
      sprintf(full_path,"Overlay_NJets_%s_wlog",filetag[i_file]);
      generate_1Dplot(hist_list_Njets,full_path,energy,leg_head,false,true,false,true);
      sprintf(full_path1,"Overlay_N-BJets_%s_wlog",filetag[i_file]);
      generate_1Dplot(hist_list_Bjets,full_path1,energy,leg_head,false,true,false,true);
      sprintf(full_path2,"Overlay_MET_%s_wlog",filetag[i_file]);
      generate_1Dplot(hist_list_MET,full_path2,energy,leg_head,false,true,false,true);
      sprintf(full_path3,"Overlay_PhoPt_%s_wlog",filetag[i_file]);
      generate_1Dplot(hist_list_PhoPt,full_path3,energy,leg_head,false,true,false,true);
      sprintf(full_path4,"Overlay_Ht_%s_wlog",filetag[i_file]);
      generate_1Dplot(hist_list_HT,full_path4,energy,leg_head,false,true,false,true);
      sprintf(full_path5,"Overlay_ST_%s_wlog",filetag[i_file]);
      generate_1Dplot(hist_list_ST,full_path5,energy,leg_head,false,true,false,true);
	

      }
      

}





