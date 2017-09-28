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
  chchain->Add("/a/data/amsterdam/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_total.root");//read in file

  TH2D *hist = new TH2D("hist","Data Reduction vs. Crate",9,1,10,10000,0,1);//histogram with one compression entry per crate
  TH1D *manprof = new TH1D("manprof","Avg. Compression per Crate",9,1,10);//manual profile of hist
  TH2D *hist_ch = new TH2D("hist_ch","Avg. Compression per Crate",8256,0,8256,10000,0,1);//comp v channel


//mapping of channel per crate
  std::ifstream ch_map("20170213190344_map.txt");//input file that lists the crate for every channel (and other things)
  int crate, board, cn, ch;
  std::string plane;

  std::vector<int> crate_v,ch_v;

  while(ch_map.good()){ 
    ch_map >> crate >> board >> cn >> plane >> ch; 
    crate_v.push_back(crate);//fill vector with crate values 

    //now make a corresponding channel vector; now entry crate_v[x] corresponds to entry ch_v[x]
    //the channels start from 0 on every plane, but for our purposes channels are numbered continuously from 0-8256
    //therefore, we identify the V plane and add 2400 for the channel number, and add 4800 for the Y plane
    if(plane.compare("U") == 0){ch_v.push_back(ch );}
    else if(plane.compare("V") == 0){ch_v.push_back( ch + 2400 );}
    else if(plane.compare("Y") == 0){ch_v.push_back( ch + 4800 );}/////
    
  }
  
//read in vairables:
  Double_t _ch_compression_huff;   Int_t _ch; 
  chchain->SetBranchAddress("_ch_compression_huff",&_ch_compression_huff);
  chchain->SetBranchAddress("_ch",&_ch);

  Long64_t chentries = chchain->GetEntries();
  cout << "amt of entries= " << chentries <<endl;
  Long64_t nb = 0, cb = 0 , cbytes = 0, nbytes = 0 ;
   
  int e = chentries/8256;//amount of events
  std::cout << e << " events" << std::endl;

  for (Long64_t j=0; j<chentries; j++) { // loop over all channels and all events, to do a quick run modify to j<(desired_amt_of_events)
    Long64_t centry = chchain->LoadTree(j);
    if (centry <0 ) break;
    cb = chchain->GetEntry(j); cbytes += cb;
        
    //    cout << "channel " << _ch << " compression for this event " << _ch_compression << endl;

      hist_ch->Fill(_ch,(_ch_compression_huff));//fill this histo with x = channel and y = compression for that channel

    //sorting channels from crates
    for(int i=0; i<8256; i++){ //loop over all channels
      if(_ch==ch_v[i]){ //if the channel number matches an entry in the channel vector
	hist->Fill(crate_v[i],(_ch_compression_huff));//fill histogram with x = the crate number and y = compression for that channel
        //std::cout << "ch " << _ch << " crate: " << crate_v[i] << std::endl;
      }}
      
}
//now to make a manual profile of the first two histograms:
      int non_zero_bins = 0; //amt of non-zero bins
      double integral = 0; //sum of values in bin
      double denom = 0; //amount of entries in bin
  for( int jj=0; jj < hist->GetNbinsX(); jj++){ //loop over all X bins in hist
    int ybins = hist->GetNbinsY();//get y bins from hist

    //reset these variables to 0 in every loop
    //non_zero_bins = 0;
     integral = 0; 
     denom = 0;
    for( int ii=0; ii < ybins; ii++){//loop over all bins
      double x_coor = hist->GetXaxis()->GetBinCenter(jj+1);//the value of the bin
      double y_coor = hist->GetYaxis()->GetBinCenter(ii+1);

      integral += (hist->GetBinContent(jj+1,ii+1))*y_coor;//sum of bin contents*compression value
      denom += (hist->GetBinContent(jj+1,ii+1));//sum of bin contents
    }
    
   
   if((jj+1)==1){
   	double mean = integral / denom; // mean = sum / amt of entries
   	std::cout << 1/mean << std::endl;
	//std::cout << "denom " << denom << " " <<((double)e*672)<<std::endl; //this is to check that the denominator is, in fact, the amount of entries * amoutn of channels in that crate
   	manprof->SetBinContent(jj+1,(1/mean));
    }

   else if((jj+1)==9){
      double mean = integral / denom;
      std::cout << 1/mean << std::endl;
	//std::cout << "denom " << denom << " " <<((double)e*864)<<std::endl;
      manprof->SetBinContent(jj+1,(1/mean));
    }
   else {
      double mean = integral / denom;
      std::cout << 1/mean << std::endl;
	//std::cout << "denom " << denom << " " <<((double)e*960)<<std::endl;
      manprof->SetBinContent(jj+1,(1/mean));
    }}

//propagating error
 int sqt_n_1 = sqrt((e*672));
 int sqt_n_9 = sqrt((e*864));
 int sqt_n = sqrt((e*960));

 int std_err = hist->GetStdDev();

//making a profile of hist using the built in ROOT function: only one line per histogram! 
 TProfile *prof = hist->ProfileX();
 TProfile *prof_ch = hist_ch->ProfileX();

//Plotting:

 TCanvas *bob = new TCanvas("bob","Compression vs. Crate",800,800);//histogram with one compression entry per crate
 bob->Divide(2,2);

 bob->cd(1);
 //manprof->SetLineColor(3);
 manprof->Draw();

 bob->cd(2);
 prof->Draw();

 bob->cd(3);
 hist_ch->Draw();

 bob->cd(4);
 prof_ch->Draw();

}

