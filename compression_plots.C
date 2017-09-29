
#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include <iostream>
#include "TH1.h"
#include "TChain.h"

using namespace std;

void compression_plots(){

  TChain* compchain = new TChain("_compress_tree");
  compchain->Add("/a/data/riverside/afadeeva/OutSN/huffman_test_831_bt2_tt2_30_un_un_up.root");
///a/data/riverside/afadeeva/OutSN/SNoutput_ana.root"); // these output files were created from: /a/data/bleeker/dcaratelli/michel/DATA

  //  TChain* chchain = new TChain("_compress_ch");
  //  chchain->Add("/a/data/riverside/afadeeva/OutSN/SNoutput_ana_dat_20.root");

   TH1D *comp = new TH1D("comp","_compression",100,0,0.1);
   TH1D *compU = new TH1D("compU","_compressionU",100,0,0.1);
   TH1D *compV = new TH1D("compV","_compressionV",100,0,0.1);
   TH1D *compY = new TH1D("compY","_compressionY",100,0,0.05);
   //   TH2D *hist = new TH2D("hist","Comp_v_Ch",16000,-1,8300,10000,0,1.2);
   //   TH2D *avg_compvch = new TH2D("avg_compvch","Avg Comp v. Ch",16600,-1,8300,100,0,1.2);

  Double_t _compression, _compressionU, _compressionV, _compressionY, _ch_compression;
  //  Int_t _ch;
  compchain->SetBranchAddress("_compression",&_compression);
  compchain->SetBranchAddress("_compressionU",&_compressionU);
  compchain->SetBranchAddress("_compressionV",&_compressionV);
  compchain->SetBranchAddress("_compressionY",&_compressionY);
  //  chchain->SetBranchAddress("_ch_compression",&_ch_compression);
  //  chchain->SetBranchAddress("_ch",&_ch);
      
   Long64_t nentries = compchain->GetEntries();

   //   Long64_t cchentries = chchain->GetEntries("_ch_compression");
   //   Long64_t chentries = chchain->GetEntries("_ch");
 
   Long64_t nb = 0, cb = 0 , cbytes = 0, nbytes = 0 ;

      for (Long64_t i=0; i<nentries ; i++) {
	      Long64_t entry = compchain->LoadTree(i);
              if (entry <0 ) break;

             nb = compchain->GetEntry(i); nbytes += nb;

	     //             cout << "compression = " << _compression << endl;
      cout << _compression << endl;
      comp->Fill(_compression);
      compU->Fill(_compressionU);
      compV->Fill(_compressionV);
      compY->Fill(_compressionY);
      }

      //      for (Long64_t j=0; j<chentries ; j++) { // chentries 
      //	      Long64_t centry = chchain->LoadTree(j);
      //              if (centry <0 ) break;
      //             cb = chchain->GetEntry(j); cbytes += cb;
      //		}
   TCanvas *cans = new TCanvas("cans","raw data through SN compression algo",800,800);
   cans->Divide(2,2);

   cans->cd(1);
   comp->SetXTitle("compression factor");
   comp->Draw();
   //_compress_tree->Draw("_compression");

   cans->cd(2);
   compU->SetXTitle("compression factor U");
   compU->Draw();

   cans->cd(3);
   compV->SetXTitle("compression factor V");
   compV->Draw();

   cans->cd(4);
   compY->SetXTitle("compression factor Y");
   compY->Draw();
}

