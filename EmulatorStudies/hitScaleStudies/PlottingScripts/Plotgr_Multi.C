const int n_pl = 4;
bool logx = false;
TString pls[n_pl] = {"FTFP_BERT_EMN","QGSP_FTFP_BERT_EMN","FTFP_BERT_EMM","QGSP_BERT"};
TString dRs[9] = {"dR < 0.56cm","dR < 1.0cm","dR < 2.0cm","dR < 3.0cm","dR < 5.0cm","dR < 8.0cm","dR < 12.0cm","dR < 18.0cm","dR < 20.0cm"};
//TString legend_text[11] = {"No cuts","skimmed","lep-veto","isotrk-veto","Pho-Pt>20","Njets>=2","Dphi-cut","MET>100","MET>250","ST>300","Pho-pt>100"};
//TString legend_text[4] = {"SR","CR","pMSSM_MCMC_106_19786","pMSSM_MCMC_473_54451"};//{"No cuts","skimmed","lep-veto","isotrk-veto","Dphi-cut","MET>250","ST>300","Pho-pt>100"}; 
//const char* baseline[9]={"EcondAsic_AveragePacketWords","EcondAsic_AveragePacketBits","EcondAsic_LinkPacketOccupancy","EcondAsic_MaxBuf \fer","EcondAsic_FractionEmptyBuffer","EcondAsic_FractionLowBuffer","EcondAsic_FractionMediumBuffer","EcondAsic_FractionHighBuffer","Econd\Asic_FractionTruncated"};
const char* legend_text[9]={"10kBX","100kBX","400kBX","1000kBX","1500kBX"};//{"10kBX","50kBX","100kBX","200kBX","400kBX","600kBX","800kBX","1000kBX","1500kBX"};
//TString legend_text1[3]={"CMSSW(110k)","Correlated pseudo(1M)","Uncorrelated pseudo(1M)"};//,"200kBX","400kBX","600kBX","800kBX","1000kBX","1500kBX"};
TString legend_text2[2]={"Geometry D86","Geometry D49"};

const char* data_type[3]={"CMSSW                  evt   mean   stddev","Correlate_Pseudo evt   mean stddev","Uncorrelate_Pseudo evt   mean stddev"};//                                                                                                                          
const char* filetag[5]={"BX1M","BX2M","BX3M","BX4M","BX5M"};//{"10kBX","50kBX","100kBX","200kBX","400kBX","600kBX","800kBX","1000kBX","\
//1500kBX"};                                                                                                                            
  // const char* geo[2]={"D49","D86"};
   const char* legend_text1[3]={"CMSSW","Correlate_Psuedo","UnCorrelate_Pseudo"};
  // const char* trig_rate[25]={"600kHz","625kHz","650kHz","675kHz","700kHz","725kHz","750kHz","775kHz","800kHz","825kHz","850kHz","875kHz","900kHz","925kHz","950kHz","975kHz","1000kHz","1025kHz","1050kHz","1075kHz","1100kHz","1125kHz","1150kHz","1175kHz","1200kHz"};
//const char* legend_text1[8]={"600kHz","750kHz","800kHz","850kHz","950kHz","1025kHz","1125kHz","1175kHz",};

int line_width[12] = {2,3,2,3,3,2,2,2,2,2,2,2};
int line_style[12] = {1,7,1,7,7,1,1,1,1,1,1,1};
// /int line_color[n_pl+1] = {kBlack, kRed, kGreen+2, kBlue, kRed};
int line_color[9] = {kViolet+2,kRed+2,kGreen+2,kBlue,kRed,kBlue, kRed+3 , kYellow+2, kGray+2};
float point_size[10] = {1.5,1.5,1.5,1.5,1.4,1.3,1.4,1.5,1.4,1.5};
int marker[12] = {kFullTriangleDown,21,kFullCircle,23,29,33,kFullCircle,kOpenSquare,kOpenTriangleUp,kFullTriangleDown,kOpenTriangleUp,};
//float xrange_max[5]={
//int point_col[5] = {kBlue, kBlue+1, kGreen+2, kBlue, kRed};
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



void generate_1Dplot_v1(vector<TH1F*> hist,  char const *tag_name="", int energy=-1, char const *leg_head="",
			bool normalize=false, bool log_flag=true, bool DoRebin=false, bool save_canvas=true, char const *title="",char const *title1="", char const *tag="", vector<string>legend_txt={""}){


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
  TLegend *legend1 = new TLegend(0.65,0.73,0.96,0.62);                                                                                                                                                                     
  //    legend = new TLegend(0.75,0.75,0.95,0.95);                                                                                                                                                                 
  legend = new TLegend(0.65,0.75,0.96,0.97);

  legend->SetTextSize(0.035);
  gStyle->SetLegendTextSize(0.03);
  legend->SetLineColor(kWhite);
  char* lhead = new char[100];
  legend1->SetLineColor(kWhite);

  sprintf(lhead,"#bf{%s: 1M BX} ",tag);//legend_text[0]);//, tag[i]);                                                                                                                                              
  //  legend->SetNColumns(2);                                                                                                                                                                                      
  legend->SetHeader(lhead);
  char* text_name = new char[10000];
  TLegendEntry* leg_entry[20];
  float x_label_size = 0.025;
  double ymin = 100000.0;
  double ymax = 0.0;
  cout<<" hist.size() = "<<hist.size()<<endl;

  sprintf(lhead,"#bf{%s}",leg_head);
  legend1->SetHeader(lhead);

  for(int i = 0; i < (int)hist.size(); i++) {
      
    normalize = true;
    // if(normalize) {
    // //hist.at(i)->Scale(1.0/hist.at(i)->Integral());
    // //hist.at(i)->GetYaxis()->SetTitle("Normalized");
    // }
    // else {
    // hist.at(i)->GetYaxis()->SetTitle("Entries");
    // }
    
    hist.at(i)->SetLineWidth(line_width[i]);
    
    hist.at(i)->SetLineStyle(line_style[i]);
    hist.at(i)->SetLineColor(line_color[i]);
    hist.at(i)->SetTitle(" ");
    
    hist.at(i)->GetXaxis()->SetTitleSize(0.04);
    hist.at(i)->GetXaxis()->SetTitle(title);
    hist.at(i)->GetYaxis()->SetTitle(title1);

    hist.at(i)->GetXaxis()->SetLabelSize(x_label_size);
    hist.at(i)->GetXaxis()->SetLabelSize(0.030);
    hist.at(i)->GetYaxis()->SetLabelSize(0.030);
    hist.at(i)->GetYaxis()->SetTitleOffset(1.45);
    // if(DoRebin) {
    //   hist.at(i)->Rebin(4);
    // }
    /* hist.at(i)->GetXaxis()->SetRangeUser(x_min[energy],x_max[energy]); */
    legName.push_back(hist.at(i)->GetName());
    // leg_entry[i] = legend->AddEntry(hist.at(i),legend_text[i],"l");
    // leg_entry[i]->SetTextColor(hist.at(i)->GetLineColor());
    sprintf(text_name,"%.03f %0.1f",hist.at(i)->GetMean(),hist.at(i)->GetRMS());
    leg_entry[i+(int)hist.size()] = legend->AddEntry(hist.at(i),legend_txt[i].c_str(),"l");
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
	//hist.at(i)->GetXaxis()->SetRangeUser(0,xmax_[i]);
	  
      }
    if(!i) hist.at(i)->Draw("hist");
    else   hist.at(i)->Draw("hist sames");

  }
  
  legend->Draw("same");
  gStyle->SetLegendTextSize(0.04);

  legend1->Draw("same");


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
    sprintf(canvas_name,"%s.png",tag_name);
    c->SaveAs(canvas_name);
    sprintf(canvas_name,"%s.pdf",tag_name);
    c->SaveAs(canvas_name);

  }
  
}


void generate_1Dplot(vector<TGraph*> hist, char const *tag_name="", int energy=-1, char const *leg_head="",
		     bool normalize=false, bool log_flag=true, bool DoRebin=false, bool save_canvas=true, char const *title="",char const *title1="", char const *tag="", vector<string>legend_txt={""}, int xmin_=-1, int xmax_=-1){  
  

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
  TLegend *legend; //(0.65,0.73,0.96,0.62)
  TLegend *legend1 = new TLegend(0.6,0.8,0.45,0.9);                                                                                            
 legend = new TLegend(0.65,0.75,0.96,0.97);
 
  legend->SetTextSize(0.035); 
  gStyle->SetLegendTextSize(0.03);
  legend->SetLineColor(kWhite);
  char* lhead = new char[100];

  sprintf(lhead,"#bf{%s: 1M BX} ",tag);//legend_text[0]);//, tag[i]);                                                                         
  //  legend->SetNColumns(2);
  legend->SetHeader(lhead);
    char* text_name = new char[10000];
  TLegendEntry* leg_entry[20];
  sprintf(lhead,"#bf{%s}",leg_head);
  legend1->SetHeader(lhead);
  legend1->SetLineColor(kWhite);

  float x_label_size = 0.025;
  double ymin = 100000.0;
  double ymax = 0.0;
  cout<<" hist.size() = "<<hist.size()<<endl;
  TMultiGraph* mg = new TMultiGraph();

  for(int i = 0; i < (int)hist.size(); i++) {
  	
    //        normalize = true;
    // if(normalize) {
    //   //hist.at(i)->Scale(1.0/hist.at(i)->Integral());
    //   //hist.at(i)->GetYaxis()->SetTitle("Normalized");
    // }
    // else {
    //   hist.at(i)->GetYaxis()->SetTitle("Entries");
    // }
    
    hist.at(i)->SetLineWidth(line_width[i]);
    
    hist.at(i)->SetLineStyle(line_style[i]);
    hist.at(i)->SetLineColor(line_color[i]);
    hist.at(i)->SetTitle(" ");
    
    // hist.at(i)->GetXaxis()->SetTitleSize(0.04);
    // hist.at(i)->GetXaxis()->SetLabelSize(x_label_size);
    // hist.at(i)->GetXaxis()->SetLabelSize(0.030);
    // hist.at(i)->GetYaxis()->SetLabelSize(0.030);
    // hist.at(i)->GetYaxis()->SetTitleOffset(1.45);
    // hist.at(i)->GetXaxis()->SetTitle("Trigger Rate");
    // hist.at(i)->GetYaxis()->SetTitleOffset(1.6);
    // hist.at(i)->GetYaxis()->SetTitle(title1);
    hist.at(i)->SetMarkerColorAlpha(line_color[i], 0.95);
    hist.at(i)->SetMarkerSize(1.5);
    hist.at(i)->SetMarkerStyle(marker[i]);
    //    hist.at(i)->GetYaxis()->SetTitleSize(0.05);
    // if(i<3)
    //   hist.at(i)->SetMarkerSize(2);

    // double xmax = TMath::MaxElement(hist.at(0)->GetN(),hist.at(0)->GetX());
    // TAxis *axis51=  hist.at(i)->GetXaxis();
    // axis51->SetRangeUser(0,12);

    //    hist.at(i)->GetXaxis()->SetRangeUser(0,xmax); 
    legName.push_back(hist.at(i)->GetName());
    leg_entry[i] = legend->AddEntry(hist.at(i),legend_txt[i].c_str(),"p");
    leg_entry[i]->SetTextColor(hist.at(i)->GetLineColor());
    // sprintf(text_name,"%.03f %0.1f",hist.at(i)->GetMean(),hist.at(i)->GetRMS());
    // leg_entry[i+(int)hist.size()] = legend->AddEntry(hist.at(i),text_name,"");
    // leg_entry[i+(int)hist.size()]->SetTextColor(hist.at(i)->GetLineColor());

    if(hist.at(i)->GetMaximum() > ymax) ymax = hist.at(i)->GetMaximum();
    if(hist.at(i)->GetMinimum() < ymin) ymin = hist.at(i)->GetMinimum();
    // setLastBinAsOverFlow(hist.at(i));
    //    hist.at(i)->SetMarkerSize(point_size[i]);                                                                                                  
    //hist.at(i)->SetMarkerStyle(marker[i]);

    mg->Add(hist.at(i));

  }
  // if(ymin == 0.0) ymin = 1e-1;
  // if(ymin<0.0) ymin = 1e-2;
  // for(int i = 0; i < (int)hist.size(); i++) {
  //   if(!normalize) hist.at(i)->GetYaxis()->SetRangeUser(ymin,0.5*ymax*100);
  //   else
  //     {  hist.at(i)->GetYaxis()->SetRangeUser(ymin,0.5*ymax*100);
  // 	//	hist.at(i)->GetXaxis()->SetRangeUser(0,xmax_[i]);
	
  //     }
  // 	if(!i) hist.at(i)->Draw("hist");
  //   else   hist.at(i)->Draw("hist sames");

  // }
 
      
  mg->GetXaxis()->SetTitle(title);//"Trigger Rate (kHz)");
  mg->GetYaxis()->SetTitle(title1);
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetYaxis()->SetTitleSize(0.05);
    mg->GetYaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetTitleOffset(1.1);
    mg->GetYaxis()->SetTitleOffset(1.45);
    //    mg->SetMarkerSize(1.7);
    cout<<"hist.at(1)->GetMaximum()  "<<hist.at(0)->GetHistogram()->GetMaximum()<<"\t"<<ymax<<endl;
    double xmax1 =0;
    //xmax1= TMath::MaxElement(hist.at(0)->GetN(),hist.at(0)->GetX()); 
    cout<<"hist.at(1)->GetMaximum()  "<<hist.at(0)->GetHistogram()->GetMaximum()<<"\t"<<ymax<<endl;

    //    cout<<xmax<<endl;
       if(hist.at(hist.size()-1)->GetHistogram()->GetMaximum()<1.1)
	 xmax1 = 1.05;//*hist.at(hist.size()-1)->GetHistogram()->GetMaximum();
       // else if(hist.at(hist.size()-1)->GetHistogram()->GetMaximum()==1.1) 
       // 	 xmax1=1.2;
       else if(hist.at(hist.size()-1)->GetHistogram()->GetMaximum()>0.9 && hist.at(hist.size()-1)->GetHistogram()->GetMaximum()<10)
	 xmax1 = hist.at(hist.size()-1)->GetHistogram()->GetMaximum()+10;
       else
	 xmax1=hist.at(hist.size()-1)->GetHistogram()->GetMaximum()+200;
       cout<<xmax1<<" xmax1"<<"\t"<<hist.at(hist.size()-1)->GetHistogram()->GetMaximum()<<endl;
       mg->SetMaximum(xmax1);//hist.at(hist.size()-1)->GetHistogram()->GetMaximum()+xmax1);//.5*hist.at(hist.size()-1)->GetHistogram()->GetMaximum());//.0*hist.at(1)->GetMaximum());
       

       //mg->Draw("AP");

    TAxis *axis51=  mg->GetXaxis();
    //    axis51->SetRangeUser(xmin_,xmax_);
       c->SetGrid();
    gPad->Update();
    gPad->Modified();
     mg->Draw("AP");
    legend->Draw();
    legend1->Draw();
    gPad->Modified();
    gPad->Update();
    c->Modified();
  // if(log_flag) {
  //     gPad->SetLogy();
  //   }
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
    sprintf(canvas_name,"%s.png",tag_name);
    c->SaveAs(canvas_name);
    sprintf(canvas_name,"%s.pdf",tag_name);
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

void Plotgr_Multi(int flag)
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
  char *fname=new char[1000];
  char* tag=new char[1000];
  char* tag1=new char[1000];
  int n=0;
  vector<string>xlabel;
  vector<string>ylabel;
  vector<double>xValues;
  vector<int>YValues;
  vector<string>gr_name;
  vector<string> leg_text_v1;

  if(flag ==0){
    n=3;
    xlabel ={"F","F","F","F","F","F","F","F","F","F","F","F","F","F","F","F"};//+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells","+=(#bf{F%})nCells",};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","frac-Events with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    xValues={0,1,2,3,4,5,6,7,8,9,10};
  f[0] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_1_23April2023.root");
  f[1] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_2_23April2023.root");
  f[2] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_3_23April2023.root");
  f[3] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_4_23April2023.root");
  leg_text_v1={"n10+=(#bf{F%})nCells","n20+=(#bf{F%})nCells","n30+=(#bf{F%})nCells","nHits+=(#bf{F%})nCells"};
  sprintf(full_path1,"Adding_fracnCells");
  }
  if(flag ==1){
    n=4;
    xlabel ={"F","F","F","F","F","F","F","F","F","F","F","F","F","F"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","frac-Events with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    xValues={0,1,2,3,4,5,6,7,8,9,10};
    f[0] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_51_23April2023.root");
    f[1] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_52_23April2023.root");
    f[2] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_53_23April2023.root");
    f[3] = new TFile("Out_ImpactOfScaling_V1HitAdding_fracNCells_case_5_23April2023.root");
    leg_text_v1={"n10+=(#bf{F})","n20+=(#bf{F})","n30+=(#bf{F})","nHits+=(#bf{F})"};
    sprintf(full_path1,"Adding_ConstantNumbers");
  }
  if(flag ==2){
    n =4;
    xlabel ={"F","F","F","F","F","F","F","F","F","F","F","F"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","frac-Events with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    xValues={1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2};

    f[0] = new TFile("Out_withEmuRuns_1MBX_graph_Multi_case_1_4July2023.root");
    f[1] = new TFile("Out_withEmuRuns_1MBX_graph_Multi_case_2_4July2023.root");
    f[2] = new TFile("Out_withEmuRuns_1MBX_graph_Multi_case_3_4July2023.root");
    f[3] = new TFile("Out_withEmuRuns_1MBX_graph_Multi_case_4_4July2023.root");
    leg_text_v1={"n10*=(#bf{F})","n20*=(#bf{F})","n30*=(#bf{F})","nHits*=(#bf{F})"};
    sprintf(full_path1,"/eos/user/k/kalpana/www/folder/HGCAL_TDAQ/Plots/July2023_EmulStudies_w1MBX/Scaling_HitsWise/EconDsCheck/");

  }
  if(flag ==3){
    n =3;
    f[0] = new TFile("Out_ImpactOfScaling_HitAdding_fracNCells_case_4_23April2023.root");
    f[1] = new TFile("Out_ImpactOfScaling_HitAdding_fracNCells_case_5_23April2023.root");
    f[2] = new TFile("Out_ImpactOfScaling_HitScaling_ConstNum_23April2023.root");
    //f[3] = new TFile("Out_ImpactOfScaling_HitScaling_ConstNum_23April2023.root");
  }
  if(flag==4){
    n =3;
    xlabel ={"Layer number","Layer number","Layer number","Layer number","Layer number","Layer number","Layer number","Layer number","Layer number","Layer number","Layer number","Layer number"};//"None","Layer-4","Layer-5","Layer-7","Layer-9","Layer-28","Layer-30","Layer-32","Layer-34","Layer-36"};
    ylabel={"N-Econds with truncation>0","N-Econds with max buffer>1000","frac-Events with dropped packets>0","frac-Events with dropped packets>50","frac-Events with dropped packets>100","frac-Events with dropped packets>200","Events with dropped packets>0","Events with dropped packets>50","Events with dropped packets>100","Events with dropped packets>200","frac-Events with dropped packets>500","frac-Events with dropped packets>800"};
    xValues={0,4,5,7,9,28,30,32,34,36};//{0,1,2,3,4,5,6,7,8,9,10};
    leg_text_v1={"n10+=(#bf{10%})nCells","n20+=(#bf{10%})nCells","n30+=(#bf{10%})nCells","nHits+=(#bf{10%})nCells"};
    f[0] = new TFile("Out_withEmuRuns_1MBX_ImpactOfScaling_Layerwise_scaling_case_1_3July2023.root");
    f[1] = new TFile("Out_withEmuRuns_1MBX_ImpactOfScaling_Layerwise_scaling_case_2_3July2023.root");
    f[2] = new TFile("Out_withEmuRuns_1MBX_ImpactOfScaling_Layerwise_scaling_case_3_3July2023.root");
    f[3] = new TFile("Out_withEmuRuns_1MBX_ImpactOfScaling_Layerwise_scaling_case_4_3July2023.root");
    sprintf(full_path1,"LayerWiseTesting");
  }

  if(flag==5){
    n =3;
    leg_text_v1={"n20+=(#bf{10%})nCells","n30+=(#bf{10%})nCells","nHits+=(#bf{10%})nCells"};

    f[0] = new TFile("Out_withEmuRuns_1MBX_graph_LayerWiseScaling_case_1_4July2023.root");
    f[1] = new TFile("Out_withEmuRuns_1MBX_graph_LayerWiseScaling_case_2_4July2023.root");
    f[2] = new TFile("Out_withEmuRuns_1MBX_graph_LayerWiseScaling_case_3_4July2023.root");
    f[3] = new TFile("Out_withEmuRuns_1MBX_graph_LayerWiseScaling_case_4_4July2023.root");
    sprintf(full_path1,"LayerWiseTesting/EconDsCheck");

  }
  // gr_name ={"TruncatVsLuv_EcondWithNZ","TruncatVsLuv_EcondWithNZ_withElink1","TruncatVsLuv_EcondWithNZ_withElink2","TruncatVsLuv_EcondWithNZ_withElink3","TruncatVsLuv_EcondWithNZ_withElink4","TruncatVsLuv_EcondWithNZ_withElink5","TruncatVsLuv_EcondWithNZ_withElink6","AvePacketSizeVsLuv_EcondWithNZ","AvePacketSizeVsLuv_EcondWithNZ_withElink1","AvePacketSizeVsLuv_EcondWithNZ_withElink2","AvePacketSizeVsLuv_EcondWithNZ_withElink3","AvePacketSizeVsLuv_EcondWithNZ_withElink4","AvePacketSizeVsLuv_EcondWithNZ_withElink5","AvePacketSizeVsLuv_EcondWithNZ_withElink6","elinkVsLuv_EcondWithNZ","AveTotalHitsVsLuv_EcondWithNZ","elink_EcondWithNZ","AvePacketSize_EcondWithNZ","AveTotalHits_EcondWithNZ"};

 xlabel ={"Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Luv","Number of elinks","Average packet siz\
e","Average total hits"};
  ylabel={"frac of Truncation","frac of Truncation (Nelinks==1)","frac of Truncation (Nelinks==2)","frac of Truncation (Nelinks==3)","frac of Truncation (Nelinks==4)","frac of Truncation (Nelinks==5)","frac of Truncation (Nelinks==6)","Average packet size","Average packet size (Nelinks==1)","Average packet size (Nelinks==2)","Average packet size (Nelinks==3)","Average packet size (Nelinks==4)","Average packet size (Nelinks==5)","Average packet size (Nelinks==6)","number of elinks","Average total hits","No of ECON-Ds","No of ECON-Ds","No of ECON-Ds"};
  vector<string> filetag={"MultiBy1","MultiBy11","MultiBy12","MultiBy13","MultiBy14","MultiBy15","MultiBy16","MultiBy17","MultiBy18","MultiBy20"} ;
  xValues={1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2};


  sprintf(path2,"/eos/cms/store/group/dpg_hgcal/comm_hgcal/kalpana/Hgcal_BEDaq_PseudoNtuples/OutRootFiles_EmulatorRuns/");
  //  xValues={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47}; 
  vector<int>xmin_={1,1,12, 268, 524, 780, 1036, 1292, 1548, 1804, 2060, 2316, 2572, 2828, 3084, 3340, 3596, 3852, 4108, 4364, 4620, 4876, 5132, 5388, 5644, 5900, 6156, 6412, 6668, 6924, 7180, 7436, 7692, 7948, 8204, 8460, 8716, 8972, 9228, 9484, 9740, 9996, 10252, 10508, 10764, 11020, 11276, 11532};
  vector<int>xmax_ ={755, 1011, 1267, 1523, 1779, 2035, 2291, 2547, 2803, 3059, 3315, 3571, 3827, 4083, 4339, 4595, 4851, 5107, 5363, 5619, 5875, 6131, 6387, 6643, 6899, 7155, 7411, 7667, 7923, 8179, 8435, 8691, 8947, 9203, 9459, 9715, 9971, 10227, 10483, 10739, 10995, 11251, 11507, 11763, 12019, 12275, 12531, 12787  };
   gr_name ={"NecondWNZtrunca_vsScaling","NecondWmaxBuffer_1000_vsScaling","fracEventswithNZDroppedPackets_vsScaling","fracEventswith50DroppedPackets_vsScaling","fracEventswith100DroppedPackets_vsScaling","fracEventswith200DroppedPackets_vsScaling","EventswithNZDroppedPackets_vsScaling","Eventswith50DroppedPackets_vsScaling","Eventswith100DroppedPackets_vsScaling","Eventswith200DroppedPackets_vsScaling","fracEventswith500DroppedPackets_vsScaling","fracEventswith800DroppedPackets_vsScaling"};


  //  const char* baseline[8]={"sigmaVsScale_EventSize_TrigRate750kHz","meanVsScale_EventSize_TrigRate_750kHz","ratioVsScale_EventSize_TrigRate_750kHz","MeanVsSigma_EventSize_TrigRate_750kHz","sigmaVsScale_EcondAsic_MaxBuffer_TrigRate750kHz","meanVsScale_EcondAsic_MaxBuffer_TrigRate_750kHz","ratioVsScale_EcondAsic_MaxBuffer_TrigRate_750kHz","MeanVsSigma_EcondAsic_MaxBuffer_TrigRate_750kHz"};//,"EcondAsic_AveragePacketWords","EcondAsic_AveragePacketBits","EcondAsic_LinkPacketOccupancy","EcondAsic_FractionEmptyBuffer","EcondAsic_FractionLowBuffer","EcondAsic_FractionMediumBuffer","EcondAsic_FractionHighBuffer","EcondAsic_FractionTruncated","DroppedPackets"};
  // const char* Xaxis[8]={"Scale factor","Scale factor","Scale factor","Mean","Scale factor","Scale factor","Scale factor","Mean"};
  // const char* Yaxis[8]={"RMS","Mean","RMS/Mean","RMS","RMS","Mean","RMS/Mean","RMS"};//"Scale factor","Scale factor","Scale factor","Mean"};
  
  // const char* baseline1[8]={"EventSize","EventSize","EventSize","EventSize","MaxBuffer","MaxBuffer","MaxBuffer","MaxBuffer"};//"EcondAsic_LinkPacketOccupancy","EcondAsic_MaxBuffer","EcondAsic_FractionEmptyBuffer","EventSize","DroppedPackets"};//,"EcondAsic_FractionTruncated"};
  const char* data_type[3]={"CMSSW","Correlate_Pseudo","Uncorrelate_Pseudo evt   mean stddev"};//
  //  const char* filetag[5]={"10kBX","100kBX","400kBX","1000kBX","1500kBX"};//,"600kBX","800kBX","1000kBX","1500kBX"};
  const char* geo[2]={"D49","D86"};
  const char* ip_data[3]={"CMSSW","Correlate_Psuedo","UnCorrelate_Pseudo"};
  const char* trig_rate1[23]={"600kHz","625kHz","650kHz","675kHz","700kHz","750kHz","775kHz","800kHz","825kHz","850kHz","875kHz","900kHz","925kHz","975kHz","1000kHz","1025kHz","1050kHz","1075kHz","1100kHz","1125kHz","1150kHz","1175kHz","1200kHz"};
  //  const char* trig_rate1[8]={"600kHz","750kHz","800kHz","850kHz","950kHz","1025kHz","1125kHz","1175kHz",};
  char *outfile = new char[10000];
  int TrigRate[23]={600,625,650,675,700,750,775,800,825,850,875,900,925,975,1000,1025,1050,1075,1100,1125,1150,1175,1200};
  //  vector<string> leg_text_v1;
  // sprintf(outfile,"./out_graph_emuRuns_BX_vs_TrigRate_D86.root"); 
  // TFile* fout = new TFile(outfile);//,"RECREATE");
  // float xrange_max[8]={2200,2200,2200,700,2200,2200,2200,700};
  // float xrange_min[8]={700,700,700,200,700,700,700,200};
  // float yrange_min[8] ={0,200,0,0,0,0,0.2,0};
  // float yrange_max[8]={100,700,0.015,100,1000,700,5,1000};
  for (int ig=1;ig<2;ig++)
    {
      for(int il=3;il<10;il++){
	
	//if(ig==1 && (il ==14 || il==24 || il ==43)) continue;// || il ==43) continue;                   
	//	if(il==27 || il==5) continue;
	//	if((il==6 || il==27 || il==7 || il==5))continue;
	//if((il==27 || il==5 || il==8 || il==9) && ig==1)continue;
	//if(study==4 && layer==1 && i_cut==6)continue;
	// if(study==4 && layer==1 && ig==1 && i_cut==28)continue;
	//	if(study==4 && layer==0 && ig==1 && i_cut==7) continue;
	//	if(study==4 && layer==0 && (i_cut==7 || i_cut==6)) continue;
	//	if(study==4 && layer==0 && ig==1 && (i_cut==5 || i_cut==8))continue;
	//	if(study==3 && layer==0 && ig==1 && i_cut==9)continue;

	for(int i_cut=0; i_cut<int(gr_name.size());i_cut++)
	{ 
	  vector<TGraph*> hist_list;
	  sprintf(tag,"%s_%s_scaling_%s",gr_name[i_cut].c_str(),ip_data[ig],filetag[il].c_str());
	  cout<<tag<<endl;
	  for (int ifile=0;ifile<4;ifile++){
	    cout<<"file name "<<f[ifile]->GetName()<<"\t"<<ifile<<endl;
	    TGraph* resp = (TGraph*)f[ifile]->Get(tag);		  
	    //cout<<resp->GetMinimum()<<endl;
	    resp->GetXaxis()->SetTitle(xlabel[i_cut].c_str());
	    resp->GetYaxis()->SetTitle(ylabel[i_cut].c_str());
	    resp->GetXaxis()->SetRange(xmin_[il],xmax_[il]);
	    //resp->SetMarkerSize(1.6);
	    cout<<resp->GetMinimum()<<endl;

	    hist_list.push_back(resp);
	  }
	    int energy=-1;
	    sprintf(full_path,"%s/%s_%s_%s",full_path1,filetag[il].c_str(),ip_data[ig],gr_name[i_cut].c_str());	      
	    sprintf(leg_head,"Scale factor: %.2f",xValues[il]);
	    //    	      sprintf(tag,"%s %s %s",geo[ig],ip_data[id],filetag[ibx]);
	    generate_1Dplot(hist_list,full_path,energy,leg_head,false,true,false,true,xlabel[i_cut].c_str(),ylabel[i_cut].c_str(),ip_data[ig],leg_text_v1,xmin_[il],xmax_[il]);	  
	}
      //      vector<TH1F*>hist_list1;
      for(int i_cut=16; i_cut<19;i_cut++)
        {
	  vector<TH1F*> hist_list1;
          sprintf(tag,"%s_%s_scaling_%s",gr_name[i_cut].c_str(),ip_data[ig],filetag[il].c_str());
	  cout<<tag<<endl;
          for (int ifile=0;ifile<4;ifile++){
            cout<<"file name "<<f[ifile]->GetName()<<"\t"<<ifile<<endl;
            TH1F* resp = (TH1F*)f[ifile]->Get(tag);
            //cout<<resp->GetMinimum()<<endl;                                                                                                     
            resp->GetXaxis()->SetTitle(xlabel[i_cut].c_str());
            resp->GetYaxis()->SetTitle(ylabel[i_cut].c_str());
            //cout<<resp->GetMinimum()<<endl;
	    hist_list1.push_back(resp);
          }
	  int energy=-1;
	  sprintf(full_path,"%s/%s_%s_%s",full_path1,filetag[il].c_str(),ip_data[ig],gr_name[i_cut].c_str());
	  sprintf(leg_head,"Scale factor: %.2f",xValues[il]);
	  //                sprintf(tag,"%s %s %s",geo[ig],ip_data[id],filetag[ibx]);                                                           
	  generate_1Dplot_v1(hist_list1,full_path,energy,leg_head,false,true,false,true,xlabel[i_cut].c_str(),ylabel[i_cut].c_str(),ip_data[ig],leg_text_v1);

      }
    }
    	
    
  //    }
      
//  fout->Close();
    }
}



