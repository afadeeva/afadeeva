#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include <iostream>
#include "TH2.h"
#include "TChain.h"
#include <vector>

//using namespace std;

void comp_per_crate_and_channel(){
  
  TChain* chchain = new TChain("_compress_ch");
  chchain->Add("/a/data/amsterdam/afadeeva/CompOut/compWF_may17_perch_all.root"); //CompOut/compWF_mdat3_mdat5_perch_ubu_total.root"); //compWF_may17_perch_all.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_current_det_ubu.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_current_det_ubu_1.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_1.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_2.root");
  //compWF_mdat3_perch_dfltnn_ubu.root");
  ///a/data/riverside/afadeeva/OutSN/huff_dat_pr7_pst8_bt2_vt2_un_b_up_25_15_30.root");

  TH2D *hist_ch = new TH2D("hist_ch","Data Reduction vs. Ch",8256,1,8256,8256,0,1);
  TH2D *hist_cr = new TH2D("hist_cr","Data Reduction vs. Crate",9,1,10,100,0,1);
  TH1D *prof_cr = new TH1D("prof","Avg. Compression per Crate",9,1,10);

  std::ifstream ch_map("20170213190344_map.txt");
  int crate, board, cn, ch;
  std::string plane;

  std::vector<int> crate_v,ch_v;

  while(ch_map.good()){ 
    ch_map >> crate >> board >> cn >> plane >> ch; 
    crate_v.push_back(crate);

    if(plane.compare("U") == 0){ch_v.push_back(ch );}
    else if(plane.compare("V") == 0){ch_v.push_back( ch + 2400 );}
    else if(plane.compare("Y") == 0){ch_v.push_back( ch + 4800 );}/////
    
  }
  
  Double_t _ch_compression_huff;   Int_t _ch; 
  chchain->SetBranchAddress("_ch_compression_huff",&_ch_compression_huff);
  chchain->SetBranchAddress("_ch",&_ch);

  Long64_t chentries = chchain->GetEntries();
  cout << "amt of entries= " << chentries <<endl;
  Long64_t nb = 0, cb = 0 , cbytes = 0, nbytes = 0 ;
   
  int e = chentries/8256;//amount of events
  int x = 26;//target minimum data reduction factor
  std::cout << e << " events" << std::endl;

  for (Long64_t j=0; j<chentries; j++) { // chentries = 7768896 //loop over all channels and all events
    Long64_t centry = chchain->LoadTree(j);
    if (centry <0 ) break;
    cb = chchain->GetEntry(j); cbytes += cb;
        
    for(int i=0; i<8256; i++){
      if(_ch==ch_v[i]){

	hist_cr->Fill(crate_v[i],(_ch_compression_huff));
        hist_ch->Fill(ch,_ch_compression_huff);
      }}
  }

  TH1D *cratehist[9];
  char histoname[500];

      int non_zero_bins = 0;
      double integral_cr = 0;
      double denom_cr = 0;

      double integral_ch = 0;
      double denom_ch = 0;
  for( int jj=0; jj < hist_cr->GetNbinsX(); jj++){
    sprintf(histoname,"cratehist%i",(jj+1));

    int ybins = hist_cr->GetNbinsY();

    cratehist[jj] = new TH1D(histoname,Form("Crate %i",(jj+1)),ybins,0,1);
    
     integral_cr = 0;//reset numerator and denominator between crates
     denom_cr = 0;

     integral_ch = 0;
     denom_ch = 0;

    for( int ii=0; ii < ybins; ii++){
      cratehist[jj]->SetBinContent(ii+1,hist_cr->GetBinContent(jj+1,ii+1));

      double x_coor = hist_cr->GetXaxis()->GetBinCenter(jj+1);
      double y_coor = hist_cr->GetYaxis()->GetBinCenter(ii+1);

      integral_cr += (hist_cr->GetBinContent(jj+1,ii+1))*y_coor;//sum of bin contents*compression value
      denom_cr += (hist_cr->GetBinContent(jj+1,ii+1));//sum of bin contents
      // divide by sum of bin content

      integral_ch += (hist_ch->GetBinContent(jj+1,ii+1))*y_coor;//sum of bin contents*compression value
      denom_ch += (hist_ch->GetBinContent(jj+1,ii+1));//sum of bin contents

    }
   
   	double mean_cr = integral_cr / denom_cr ;
   	prof_cr->SetBinContent(jj+1,(1/mean_cr));
    }

 TCanvas *cob = new TCanvas("cob","comp per channel v channel",900,500);
 TCanvas *gary = new TCanvas("gary","profile",900,500);
 TCanvas *steve = new TCanvas("steve","percent",500,500);
 TCanvas *lewis = new TCanvas("lewis","Crates",900,500); 
 TCanvas *gregg[9]; 
 TCanvas *prince[9];

 gary->cd();
 prof_cr->SetXTitle("Crate");
 prof_cr->SetYTitle("Data Reduction Factor");
 prof_cr->Draw("E1");

 cob->cd();
 hist_cr->SetXTitle("Crate");
 hist_cr->SetYTitle("Compression");
 hist_cr->Draw("colz");

 lewis->cd();
 //prof->Draw("E1");
// printf("Crate No\tMean\tStdev\n");

  auto legend = new TLegend(0.1,0.7,0.2,0.5);
  legend->SetHeader("Legend");

 for (int i=0; i<9; i++){
   TLegendEntry* l = legend->AddEntry(Form("cratehist[%i]",i),Form("Crate %i",(i+1)),"");
   l->SetTextColor(i+1); 

   if (i==0){
      cratehist[i]->SetMarkerStyle(42);
      cratehist[i]->SetXTitle("Compression");
      cratehist[i]->SetYTitle("Amt. of Events");
      cratehist[i]->Draw();
//     printf("%i\t%f\t%f\n",i,cratehist[i]->GetMean(),cratehist[i]->GetStdev());
   }

   else{
     cratehist[i]->SetMarkerColor(i+1);
     cratehist[i]->SetLineColor(i+1);
     cratehist[i]->SetMarkerStyle(i+20);
//     printf("%i\t%f\t%f\n",i,cratehist[i]->GetMean(),cratehist[i]->GetStdev());
     cratehist[i]->Draw("same"); 
     cratehist[i]->SetXTitle("Compression Factor");
     cratehist[i]->SetYTitle("Amt. of Events");

    legend->Draw();
   }}

for(int j=0; j<9; j++){
 gregg[j] = new TCanvas(Form("Crate %d",j));
 gregg[j]->cd();
 cratehist[j]->Draw();
 }
 

}



