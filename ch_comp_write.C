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

void ch_comp_write(){

  TChain* chchain = new TChain("_compress_ch");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_Apr1_bbb_5_first1000ev.root"); //read in file
   chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_Apr1_bbb_5.root");
  TH2D *hist = new TH2D("hist","Data Reduction vs. Channel",8257,0,8256,301,0,300);
  TH2D *hist2 = new TH2D("hist2","Percent of Events not achieving compression factor vs. Channel",8257,0,8256,301,0,100);
 
  Double_t _ch_compression;   Int_t _ch;
  chchain->SetBranchAddress("_ch_compression_huff",&_ch_compression);
  chchain->SetBranchAddress("_ch",&_ch);
      
        Long64_t chentries = chchain->GetEntries();
	cout << "amt of entries= " << chentries <<endl;
        Long64_t nb = 0, cb = 0 , cbytes = 0, nbytes = 0 ;

      int x = 40;

      for (Long64_t j=0; j< chentries; j++) { 
      	      Long64_t centry = chchain->LoadTree(j);
                    if (centry <0 ) break;
                   cb = chchain->GetEntry(j); cbytes += cb;

		   cout << "channel " << _ch << " compression for this event " << _ch_compression << endl;
		   
		   hist->Fill(_ch,(1/_ch_compression));

  }

for(int l=0; l<8256; l++){
  //maxbin  = hist->GetYaxis()->FindBin(ymax);

  Double_t not_achieving = hist->Integral(l,l,0,x,"");
  Double_t achieving = hist->Integral(l,l,x,300,"");
  
  Double_t percent =  not_achieving/(not_achieving+achieving);

  hist2->Fill(l,100*percent);

}

  TProfile *px = hist->ProfileX();

ofstream oneval;
oneval.open("ch_comp_perch.txt");//compression value listed per channel

ofstream bad_channels;
bad_channels.open("bad_channels_perch.txt");//list of channels not achieving desired compression factor

Double_t comp = 0.0;
for(int a = 0; a< (px->GetNbinsX()); a++){
  comp = px->GetBinContent(a); 
  cout << "channel " << a << " profile val " << comp << endl;
  oneval << a << " " << (1/comp) << endl;//makes two columns: channel number : data reduction factor

  if (comp > 0.0125) { //desired compression factor (note: data reduction factor of 80 = compression factor of 0.0125)
  bad_channels << a << endl;
  }

}

oneval.close();

//Plotting
 TCanvas *cob = new TCanvas("cob","comp per channel v channel (per channel threshold setting)",1250,500);
 TCanvas *gary = new TCanvas("gary","profile",1250,500);
 TCanvas *steve = new TCanvas("steve","percent",1250,500);

 gary->cd();
 px->SetXTitle("Channel");
 px->SetYTitle("Data Reduction Factor");
 px->Draw();
  
 cob->cd();
 hist->SetXTitle("Channel");
 hist->SetYTitle("Data Reduction Factor");
 hist->Draw("colz");

 steve->cd();
 hist2->SetYTitle("Percent of Events Not Reaching Data Reduction Factor 40");
 hist2->SetXTitle("Channel");
 hist2->Draw();

 //cob->SaveAs("/a/data/riverside/afadeeva/ch_comp_canvas_perch.pdf");
}
