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

void comp_per_crate_v6(){
  
  TChain* chchain = new TChain("_compress_ch");
  chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_total.root"); //compWF_may17_perch_all.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_current_det_ubu.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_current_det_ubu_1.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_1.root");
  //chchain->Add("/a/data/westside/afadeeva/CompOut/compWF_mdat3_mdat5_perch_ubu_2.root");
  //compWF_mdat3_perch_dfltnn_ubu.root");
  ///a/data/riverside/afadeeva/OutSN/huff_dat_pr7_pst8_bt2_vt2_un_b_up_25_15_30.root");

  TH2D *hist = new TH2D("hist","Data Reduction vs. Crate",9,1,10,100,0,1);
  TH2D *induct = new TH2D("induct","Induction Plane: Compression vs. Crate",9,1,10,100,0,1);
  TH2D *collect = new TH2D("collect","Collection Plane: Compression vs. Crate",9,1,10,100,0,1);
  TH1D *prof = new TH1D("prof","Avg. Compression per Crate",9,1,10);

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
        
    //    cout << "channel " << _ch << " compression for this event " << _ch_compression << endl;
   // printf("channel %i\t compression for this channel %8.5f\n",_ch,_ch_compression);
    for(int i=0; i<8256; i++){
      if(_ch==ch_v[i]){
	hist->Fill(crate_v[i],(_ch_compression_huff));
	//	std::cout << "  I found it! channel " << _ch << " crate " << crate_v[i] << std::endl;
	//printf("I found it! channel %i\t crate %i\n",ch_v[i],crate_v[i]);

        if( 0 <= _ch && _ch <= 4800){ induct->Fill(crate_v[i],(_ch_compression_huff));	}
        else { collect->Fill(crate_v[i],(_ch_compression_huff));

      }}
    

  }}
TH1D *in_crate[9];
TH1D *col_crate[9];

  char iname[500];
  char jname[500];

  for( int a=0; a<9; a++){

        sprintf(iname,"in_crate%i",a);
        sprintf(jname,"col_crate%i",a);

        in_crate[a] = new TH1D(iname,Form("Crate %i",a+1),100,0,1);
        col_crate[a] = new TH1D(jname,Form("Crate %i",a+1),100,0,1);  
  }                                              
 
  TH1D *cratehist[9];
  char histoname[500];

      int non_zero_bins = 0;
      double integral = 0;
      double denom = 0;
  for( int jj=0; jj < hist->GetNbinsX(); jj++){
    sprintf(histoname,"cratehist%i",(jj+1));

    int ybins = hist->GetNbinsY();

    cratehist[jj] = new TH1D(histoname,Form("Crate %i",(jj+1)),ybins,0,1);
    
    //non_zero_bins = 0;
     integral = 0;
     denom = 0;
    for( int ii=0; ii < ybins; ii++){
      cratehist[jj]->SetBinContent(ii+1,hist->GetBinContent(jj+1,ii+1));
      //      this_xbin += hist->GetBinContent(jj,ii);  	
      in_crate[jj]->SetBinContent(ii+1,induct->GetBinContent(jj+1,ii+1));
      col_crate[jj]->SetBinContent(ii,1,collect->GetBinContent(jj+1,ii+1));
      
      //integral += (hist->GetYaxis()->GetBinCenter(hist->GetXaxis(ii+1))) * (hist->GetBin(jj+1,ii+1));
      double x_coor = hist->GetXaxis()->GetBinCenter(jj+1);
      double y_coor = hist->GetYaxis()->GetBinCenter(ii+1);

      integral += (hist->GetBinContent(jj+1,ii+1))*y_coor;//sum of bin contents*compression value
      denom += (hist->GetBinContent(jj+1,ii+1));//sum of bin contents
      // divide by sum of bin content
/*
      //count how many non-zero bins in 1D hist
      if((hist->GetBinContent(jj+1,ii+1))!=0){
       non_zero_bins += 1;
	}
*/
    }
    
   // double integral = hist->Integral(jj+1,jj+1,0,ybins);//(first x, last x, first y, last y)
   
   if((jj+1)==1){
   	//double mean = integral / (double)non_zero_bins 
   	// double mean = cratehist[jj]->GetMean();//takes mean of zero bins also
   	double mean = integral / ((double)e * 672.);
   	std::cout << mean << std::endl;
	//std::cout << "denom " << denom << " " <<((double)e*672)<<std::endl;
   	prof->SetBinContent(jj+1,(1/mean));
    }

   else if((jj+1)==9){
      double mean = integral / ((double)e * 864.) ;
      std::cout << mean << std::endl;
	//std::cout << "denom " << denom << " " <<((double)e*864)<<std::endl;
      prof->SetBinContent(jj+1,(1/mean));
    }
   else {
      double mean = integral / ((double)e * 960.) ;
      std::cout << mean << std::endl;
	//std::cout << "denom " << denom << " " <<((double)e*960)<<std::endl;
      prof->SetBinContent(jj+1,(1/mean));
    }
  }

 prof->SetLineColor(3);
 prof->Draw("E1 same");
}
