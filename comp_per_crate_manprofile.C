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

using namespace std;

void comp_per_crate_manprofile(){

  TChain* chchain = new TChain("_compress_ch");
  chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu.root");
  chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_1.root");
  chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_2.root");
//compWF_mdat3_perch_dfltnn_ubu.root");
///a/data/riverside/afadeeva/OutSN/huff_dat_pr7_pst8_bt2_vt2_un_b_up_25_15_30.root");

  TH2D *hist = new TH2D("hist","Data Reduction vs. Crate",9,1,10,501,0,500);
  TH2D *hist2 = new TH2D("hist2","Percent of events not achieving compression factor vs. Crate",9,1,10,101,0,100);
  TH2D *prof = new TH2D("prof","Manual Profile imitation",9,1,10,101,0,100);

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

  Double_t _ch_compression;   Int_t _ch;
  chchain->SetBranchAddress("_ch_compression",&_ch_compression);
  chchain->SetBranchAddress("_ch",&_ch);
      
  Long64_t chentries = chchain->GetEntries();
  cout << "amt of entries= " << chentries <<endl;
  Long64_t nb = 0, cb = 0 , cbytes = 0, nbytes = 0 ;

  int x = 26;

for (Long64_t j=0; hentrs; j++) { // chentries = 7768896
   Long64_t centry = chchain->LoadTree(j);
   if (centry <0 ) break;
   cb = chchain->GetEntry(j); cbytes += cb;

  // cout << "channel " << _ch << " compression for this event " << _ch_compression << endl;

   for(int i=0; i<8256; i++){
     if(_ch==ch_v[i]){
     hist->Fill(crate_v[i],(1/_ch_compression));
     
     //std::cout << "channel " << _ch << " crate " << crate_v[i] << std::endl;
   }}
}

for( int jj=0; jj < hist->GetNbinsX(); jj++){
	double this_xbin = 0;
        int ybins = hist->GetNbinsY();

	for( int ii=0; ii < ybins; ii++){
	   this_xbin += hist->GetBinContent(jj,ii);  	
	}

double mean = this_xbin / ybins;
prof->Fill(jj,mean);

}



for(int l=1; l<10; l++){
  Double_t  not_achieving = hist->Integral(l,l,0,x,"");
  Double_t  achieving = hist->Integral(l,l,x,500,"");

  Double_t percent = not_achieving/(not_achieving+achieving);

  hist2->Fill(l,100*percent);

}

  TProfile *px = hist->ProfileX();

 TCanvas *cob = new TCanvas("cob","comp per channel v channel",900,500);
 TCanvas *gary = new TCanvas("gary","profile",900,500);
 TCanvas *steve = new TCanvas("steve","percent",500,500);
 TCanvas *lewis = new TCanvas("lewis","test manual profile",900,500); 

 gary->cd();
 px->SetXTitle("Crate");
 px->SetYTitle("Data Reduction Factor");
 px->Draw();
  
 cob->cd();
 hist->SetXTitle("Crate");
 hist->SetYTitle("Data Reduction Factor");
 hist->Draw("colz");

 steve->cd();
 hist2->SetXTitle("Crate");
 hist2->SetYTitle(Form("Percent of events not achieving reduction factor of %d",x));
 hist2->Draw("L");
 //cob->SaveAs("/a/data/riverside/afadeeva/ch_comp_canvas_perch.pdf");
  
 lewis->cd();
 prof->Draw("E1");

}
