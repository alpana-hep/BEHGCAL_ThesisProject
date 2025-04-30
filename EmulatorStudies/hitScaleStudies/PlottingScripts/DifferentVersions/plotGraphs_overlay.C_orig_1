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

void plotGraphs_overlay(int study,string outfile,int layer)
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
  sprintf(full_path10,"Out_ImpactOfScaling_%s_23April2023.root",outfile.c_str());
  TFile* fout = new TFile(full_path10,"RECREATE");
  vector<string>xlabel;
  vector<string>ylabel;
  vector<double>xValues;
  vector<int>YValues;
  vector<string>gr_name;
  vector<string> leg_text_v1;

  int n=0,nmin=0;
  if(study==0){
    n=10;
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};//,"Econds with truncation>0","Econds with max buffer>2000"};
    //xValues={0,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};
    xValues={1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2};
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    leg_text_v1={"Scale-Off","nHits*=1.1","nHits*=1.2","nHits*=1.3","nHits*=1.4","nHits*=1.5","nHits*=1.6","nHits*=1.7","nHits*=1.8","nHits*=2"};
  f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
  f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1100.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1200.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1300.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1400.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1500.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1600.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1700.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_1800.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_2000.root");
    
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1100.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1200.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1300.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1400.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1500.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1600.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1700.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_1800.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_2000.root");
  }
  if(study==11){
    n=10;
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};                                                                                                                                       
    xValues={1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2};
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    leg_text_v1={"Scale-Off","n10*=1.1","n10*=1.2","n10*=1.3","n10*=1.4","n10*=1.5","n10*=1.6","n10*=1.7","n10*=1.8","n10*=2"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_11_case_1.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_12_case_1.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_13_case_1.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_14_case_1.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_15_case_1.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_16_case_1.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_17_case_1.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_18_case_1.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_20_case_1.root");

    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_11_case_1.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_12_case_1.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_13_case_1.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_14_case_1.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_15_case_1.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_16_case_1.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_17_case_1.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_18_case_1.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_20_case_1.root");
  }

  if(study==12){
    n=10;
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    xValues={1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2};
    leg_text_v1={"Scale-Off","n20*=1.1","n20*=1.2","n20*=1.3","n20*=1.4","n20*=1.5","n20*=1.6","n20*=1.7","n20*=1.8","n20*=2"};
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_11_case_2.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_12_case_2.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_13_case_2.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_14_case_2.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_15_case_2.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_16_case_2.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_17_case_2.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_18_case_2.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_20_case_2.root");

    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_11_case_2.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_12_case_2.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_13_case_2.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_14_case_2.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_15_case_2.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_16_case_2.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_17_case_2.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_18_case_2.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_20_case_2.root");
  }

  if(study==13){
    n=10;
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    xValues={1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2};
    leg_text_v1={"Scale-Off","n30*=1.1","n30*=1.2","n30*=1.3","n30*=1.4","n30*=1.5","n30*=1.6","n30*=1.7","n30*=1.8","n30*=2"};
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_11_case_3.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_12_case_3.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_13_case_3.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_14_case_3.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_15_case_3.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_16_case_3.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_17_case_3.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_18_case_3.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScale_20_case_3.root");

    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_11_case_3.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_12_case_3.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_13_case_3.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_14_case_3.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_15_case_3.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_16_case_3.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_17_case_3.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_18_case_3.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScale_20_case_3.root");
  }



  if(study==1){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};//,"Econds with truncation>0","Econds with max buffer>2000"};      
    n=11;                                                                                                                                  
    xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};
    leg_text_v1={"Scale-Off","n10+==1%nCells","n10+==2%nCells","n10+==1.2%nCells","n10+==1.3%nCells","n10+==1.4%nCells","n10+==1.5%nCells","n10+==1.6%nCells","n10+==1.7%nCells","n10+==1.8%nCells","n10+==2"};
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_1.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_1.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_1.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_1.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_1.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_1.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_1.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_1.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_1.root");
    f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_1.root");
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_1.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_1.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_1.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_1.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_1.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_1.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_1.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_1.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_1.root");
    f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_1.root");
  } 
  if(study==2){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};//,"Econds with truncation>0","Econds with max buffer>2000"};                                                                                                                                       
    n=11;
    xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};                                                                                                                          
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
 f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_2.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_2.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_2.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_2.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_2.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_2.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_2.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_2.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_2.root");
    f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_2.root");
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_2.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_2.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_2.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_2.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_2.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_2.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_2.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_2.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_2.root");
    f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_2.root");
  }
  if(study==3){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};//,"Econds with truncation>0","Econds with max buffer>2000"};                                                                                                                                        
    n=11;
  xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};                                                                                                                           
  gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
  f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
  f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_3.root");
  f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_3.root");
  f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_3.root");
  f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_3.root");
  f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_3.root");
  f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_3.root");
  f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_3.root");
  f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_3.root");
  f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_3.root");
  f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_3.root");
  f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
  f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_3.root");
  f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_3.root");
  f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_3.root");
  f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_3.root");
  f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_3.root");
  f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_3.root");
  f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_3.root");
  f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_3.root");
  f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_3.root");
  f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_3.root");
}
  if(study==4){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};//,"Econds with truncation>0","Econds with max buffer>2000"};                                                                                                                                        
    n=11;
    xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};                                                                                  
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_4.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_4.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_4.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_4.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_4.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_4.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_4.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_4.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_4.root");
    f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_4.root");
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_4.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_4.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_4.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_4.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_4.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_4.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_4.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_4.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_4.root");
    f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_4.root");
  }
  if(study==5){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};                                                                                                                                        
    n=11; 
xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};                                                                                                                           
gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fra\
cEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_5.root");
f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_5.root");
f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_5.root");
f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_5.root");
f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_5.root");
f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_5.root");
f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_5.root");
f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_5.root");
f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_5.root");
f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_5.root");
f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_5.root");
f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_5.root");
f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_5.root");
f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_5.root");
f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_5.root");
f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_5.root");
f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_5.root");
f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_5.root");
f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_5.root");
f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_5.root");
}

  if(study==51){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    n=11;
    xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};                                                                                                                               
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_51.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_51.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_51.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_51.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_51.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_51.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_51.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_51.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_51.root");
    f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_51.root");
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_51.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_51.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_51.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_51.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_51.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_51.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_51.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_51.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_51.root");
    f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_51.root");
  }

  if(study==52){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    n=11;
    xValues={0,1,2,3,4,5,6,7,8,9,10};//0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.2};                                                                                                                               
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_52.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_52.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_52.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_52.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_52.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_52.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_52.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_52.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_52.root");
    f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_52.root");
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_52.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_52.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_52.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_52.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_52.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_52.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_52.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_52.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_52.root");
    f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_52.root");
  }

  if(study==53){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    n=11;
    xValues={0,1,2,3,4,5,6,7,8,9,10};                                                                                                                                                                                                                 
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    f[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_1per_case_53.root");
    f[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_2per_case_53.root");
    f[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_3per_case_53.root");
    f[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_4per_case_53.root");
    f[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_5per_case_53.root");
    f[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_6per_case_53.root");
    f[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_7per_case_53.root");
    f[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_8per_case_53.root");
    f[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_9per_case_53.root");
    f[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_hitScaleAdditive_10per_case_53.root");
    f1[0] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    f1[1] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_1per_case_53.root");
    f1[2] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_2per_case_53.root");
    f1[3] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_3per_case_53.root");
    f1[4] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_4per_case_53.root");
    f1[5] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_5per_case_53.root");
    f1[6] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_6per_case_53.root");
    f1[7] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_7per_case_53.root");
    f1[8] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_8per_case_53.root");
    f1[9] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_9per_case_53.root");
    f1[10] = new TFile("./Out_EmulatorRuns/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_hitScaleAdditive_10per_case_53.root");
  }
  if(study==1 && layer==1){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"}; 
    n=11;    xValues={0,0,4,5,7,9,28,30,32,34,36};                                                                                                                                                                  nmin=1;
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    //    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_1_10per_case_1.root");
    f[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_1.root");
    f[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_1.root");
    f[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_1.root");
    f[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_1.root");
    f[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_1.root");
    f[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_1.root");
    f[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_1.root");
    f[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_1.root");
    f[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_1.root");
    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    //    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_1_10per_case_1.root");
    f1[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_1.root");
    f1[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_1.root");
    f1[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_1.root");
    f1[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_1.root");
    f1[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_1.root");
    f1[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_1.root");
    f1[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_1.root");
    f1[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_1.root");
    f1[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_1.root");
  }

  if(study==2 && layer==1){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"}; nmin=1;
n=11;    xValues={0,0,4,5,7,9,28,30,32,34,36};                                                                                                                                                          
 gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"}; nmin=1;
    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    //    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_1_10per_case_2.root");
    f[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_2.root");
    f[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_2.root");
    f[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_2.root");
    f[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_2.root");
    f[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_2.root");
    f[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_2.root");
    f[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_2.root");
    f[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_2.root");
    f[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_2.root");
    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    //    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_1_10per_case_2.root");
    f1[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_2.root");
    f1[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_2.root");
    f1[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_2.root");
    f1[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_2.root");
    f1[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_2.root");
    f1[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_2.root");
    f1[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_2.root");
    f1[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_2.root");
    f1[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_2.root");
  }

  if(study==3 && layer==1){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"}; n=11; nmin=1;   xValues={0,0,4,5,7,9,28,30,32,34,36};                                                                                                                                                      
    gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScali\
ng","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    //    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_1_9per_case_3.root");
    f[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_3.root");
    f[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_3.root");
    f[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_3.root");
    f[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_3.root");
    f[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_3.root");
    f[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_3.root");
    f[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_3.root");
    f[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_3.root");
    f[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_3.root");
    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    //    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_1_9per_case_3.root");
    f1[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_3.root");
    f1[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_3.root");
    f1[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_3.root");
    f1[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_3.root");
    f1[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_3.root");
    f1[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_3.root");
    f1[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_3.root");
    f1[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_3.root");
    f1[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_3.root");
  }

  if(study==4 && layer==1){
    xlabel ={"Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor","Scale factor"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","NEvents with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"}; n=11;    xValues={0,0,4,5,7,9,28,30,32,34,36};             nmin=1;
                                                                                                                                         gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};
    f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig.root");
    //f[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_1_10per_case_4.root");
    f[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_4.root");
    f[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_4.root");
    f[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_4.root");
    f[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_4.root");
    f[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_4.root");
    f[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_4.root");
    f[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_4.root");
    f[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_4.root");
    f[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_CMSSW_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_4.root");
    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig.root");
    //    f1[1] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_1_10per_case_4.root");
    f1[2] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_4_10per_case_4.root");
    f1[3] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_5_10per_case_4.root");
    f1[4] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_7_10per_case_4.root");
    f1[5] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_9_10per_case_4.root");
    f1[6] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_28_10per_case_4.root");
    f1[7] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_30_10per_case_4.root");
    f1[8] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_32_10per_case_4.root");
    f1[9] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_34_10per_case_4.root");
    f1[10] = new TFile("/eos/user/k/kalpana/Out_EmuLatorRuns_April2023/D86/Out_emuRuns_D86_Correlate_Psuedo_BX_100k_defaultConfig_AddToEcondD_Layer_36_10per_case_4.root");
  }

  const char* baseline[5]={"EcondAsic_FractionTruncated","EcondAsic_MaxBuffer","DroppedPackets","EcondAsic_MaxBufferLuv","EcondAsic_FractionTruncatedLuv",};//{"EcondAsic_AveragePacketWords","EcondAsic_AveragePacketBits","EcondAsic_LinkPacketOccupancy","EcondAsic_MaxBuffer","EcondAsic_FractionEmptyBuffer","EcondAsic_FractionLowBuffer","EcondAsic_FractionMediumBuffer","EcondAsic_FractionHighBuffer","EcondAsic_FractionTruncated","EventSize","DroppedPackets","EcondAsic_AverageN10Hits","EcondAsic_AverageN20Hits","EcondAsic_AverageN30Hits","EcondAsic_AverageTotalHits"};
  const char* data_type[3]={"CMSSW (Scale-Layer-34)","Correlate_Pseudo(Scale-Layer-34)","Uncorrelate_Pseudo    mean stddev"};//
  const char* filetag[5]={"n10+=0.1*nCells","n20+=0.1*nCells","n30+=0.1*nCells","Scale-Off","Scale-Off"};//"Scale-1000","Scale-1100","Scale-1200","Scale-2000"};//{"10kBX","100kBX","400kBX","1000kBX","1500kBX"};//{"10kBX","50kBX","100kBX","200kBX","400kBX","600kBX","800kBX","1000kBX","1500kBX"};
  const char* geo[2]={"D49","D86"};
  const char* ip_data[3]={"CMSSW","Correlate_Psuedo","UnCorrelate_Pseudo"};
    char* name = new char[2000];
    char* xtitle = new char[200];
    char* ytitle = new char[200];

  // for(int i_file=0; i_file<2;i_file++)
  //   {
      for(int ig=0;ig<2;ig++)
	{
	  vector<TGraph*> graph;
	  for(int ij=0;ij<xlabel.size();ij++){
	    TGraph* gr= new TGraph();
	    sprintf(name,"%s_%s_scaling",gr_name[ij].c_str(),ip_data[ig]);
	    gr->SetName(name);
	    sprintf(xtitle,"%s",xlabel[ij].c_str());
	    sprintf(ytitle,"%s",ylabel[ij].c_str());
	    gr->GetXaxis()->SetTitle(xtitle);
	    gr->GetYaxis()->SetTitle(ytitle);
	    graph.push_back(gr);
	  }
	  vector<TH1F*> hist_list;
	  for(int i_file=0;i_file<3;i_file++){
	    //cout<<i_file<<endl;
	  for(int i_cut=nmin; i_cut<n;i_cut++)
	    {
	      sprintf(hist_name,"%s",baseline[i_file]);
	      TH1F* resp = (TH1F*)f[i_cut]->Get(hist_name);
	      cout<<hist_name<<"\t"<<resp->Integral()<<"\t"<<"Entriess  "<<resp->GetEntries()<<"\t"<<f[i_cut]->GetName()<<"\t"<<ig<<"\t"<<resp->GetBinContent(resp->GetNbinsX())<<"\t"<<resp->GetBinContent(resp->GetNbinsX()+1)<<endl;
	      //TH1F* resp1 = (TH1F*)f[0]->Get(hist_name);
	      if(ig==0)
		resp = (TH1F*)f[i_cut]->Get(hist_name);
	      else if (ig==1)	     
		  resp = (TH1F*)f1[i_cut]->Get(hist_name);
	      
	      int NEconds=0,events=0, Nevents_50=0,Nevents_100=0,Nevents_200=0,Nevents_500=0,Nevents_800=0;
	      for(int ibin=0;ibin<resp->GetNbinsX();ibin++)
		{
		  if(i_file==0 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>0.000005)
		    {//		    { cout<<resp->GetBinContent(ibin)<<"\t temp \t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
		      NEconds+=resp->GetBinContent(ibin);
		      
		    }
		  if(i_file==1 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>1000)
		    {
		      NEconds+=resp->GetBinContent(ibin);  
		      //cout<<resp->GetBinContent(ibin)<<"\t temp \t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
		    }
		  if(i_file==2 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>10)
		    {
		      events+=resp->GetBinContent(ibin);
		      //cout<<resp->GetBinContent(ibin)<<"\t temp \t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
		    }
		  if(i_file==2 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>50)
                    {
		      Nevents_50+=resp->GetBinContent(ibin);
		      //  cout<<" >50 "<<"\t"<<resp->GetBinContent(ibin)<<"\t temp \t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
                    }
		  if(i_file==2 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>100)
                    {
                      Nevents_100+=resp->GetBinContent(ibin);
                      //cout<<" >100 "<<resp->GetBinContent(ibin)<<"\t temp \t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
                    }

		  if(i_file==2 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>200)
                    {
                      Nevents_200+=resp->GetBinContent(ibin);
                      //cout<<" >200  "<<resp->GetBinContent(ibin)<<"\t temp \t"<<ibin<<"\t"<<resp->GetXaxis()->GetBinCenter(ibin)<<endl;
                    }
		  if(i_file==2 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>500)
                    {
                      Nevents_500+=resp->GetBinContent(ibin);
		    }
		  if(i_file==2 && resp->GetBinContent(ibin)>0 && resp->GetXaxis()->GetBinCenter(ibin)>800)
                    {
                      Nevents_800+=resp->GetBinContent(ibin);

		    }
		}
	      cout<<"event before "<<events<<"\t"<<resp->GetBinContent(resp->GetNbinsX()+1)<<endl;
	      if(i_file==2){
	      events+=resp->GetBinContent(resp->GetNbinsX()+1);
	      Nevents_50+=resp->GetBinContent(resp->GetNbinsX()+1);
	      Nevents_100+=resp->GetBinContent(resp->GetNbinsX()+1);
	      Nevents_200+=resp->GetBinContent(resp->GetNbinsX()+1);
	      Nevents_500+=resp->GetBinContent(resp->GetNbinsX()+1);
              Nevents_800+=resp->GetBinContent(resp->GetNbinsX()+1);}
	      cout<<"NECond before "<<NEconds<<"\t"<<resp->GetBinContent(resp->GetNbinsX()+1)<<endl;
	      if(i_file==0 || i_file==1)
	      	NEconds+=resp->GetBinContent(resp->GetNbinsX()+1);
	      cout<<"NECond after "<<NEconds<<endl;
	      cout<<"event after "<<events<<"\t"<<resp->GetBinContent(resp->GetNbinsX()+1)<<endl;

	      if(i_file==0)
		graph[0]->SetPoint(i_cut,xValues[i_cut],NEconds);
	      if(i_file==1)
		graph[1]->SetPoint(i_cut,xValues[i_cut],NEconds); 
	      if(i_file==2)
		{
		  double frac = events/resp->GetEntries();
		  graph[2]->SetPoint(i_cut,xValues[i_cut],frac);
		  graph[6]->SetPoint(i_cut,xValues[i_cut],events);
		  frac = Nevents_50/resp->GetEntries();
		  graph[3]->SetPoint(i_cut,xValues[i_cut],frac);
		  graph[7]->SetPoint(i_cut,xValues[i_cut],Nevents_50);
		  frac = Nevents_100/resp->GetEntries();
		  graph[4]->SetPoint(i_cut,xValues[i_cut],frac);
		  graph[8]->SetPoint(i_cut,xValues[i_cut],Nevents_100);
		  frac = Nevents_200/resp->GetEntries();
		  graph[5]->SetPoint(i_cut,xValues[i_cut],frac);
		  graph[9]->SetPoint(i_cut,xValues[i_cut],Nevents_200);
		  graph[10]->SetPoint(i_cut,xValues[i_cut],Nevents_500/resp->GetEntries());
		  graph[11]->SetPoint(i_cut,xValues[i_cut],Nevents_800/resp->GetEntries());
		  
		}	  
	      cout<< ">50 integral "<<"\t"<<resp->Integral(50,1000)<<"\t"<<events<<"\t"<<Nevents_50<<"\t"<<Nevents_100<< "\t"<<Nevents_200<<endl;
		//}
		
		//		cout<<xValues[i_cut]<<"\t"<<frac<<"\t"<<Nevents_20<<"\t"<<resp->GetEntries()<<endl;}
	    }
	  }
	
	  fout->cd();
	  graph[0]->Write();
	  graph[1]->Write();
	  graph[2]->Write();
	  graph[3]->Write();
          graph[4]->Write();
          graph[5]->Write();
	  graph[6]->Write();
          graph[7]->Write();
          graph[8]->Write();
          graph[9]->Write();
	  graph[10]->Write();
          graph[11]->Write();

		  //Nevents++;
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

}





