#include "TH1.h"
#include "TF1.h"
//#include TMath.h
#include <stdio.h>
#include <Math/Polynomial.h>

int MIP_fitter_min_V(){

TFile *f = new TFile("/path_to_input.root");  
f->ls();

ofstream file;
file.open("fit_parm_Vmin.txt");

int array[4] = {0,2400,4800,8256};

TH1D *h1;
TH2I *h2 = (TH2I*)f->Get("ch_ADC_min_all");
for (int n = 2401; n<4801; n++){
    h1 = h2->ProjectionY(Form("ch%d_ADC",n),n,n);

    int noise_peak_bin = h1->GetMaximumBin();
    double noise_peak = h1->GetBinCenter(noise_peak_bin);
    double noise_peak_max = 2*noise_peak;
    double noise_height = h1->GetBinContent(noise_peak_bin);

    double start = h1->GetBinCenter(h1->FindLastBinAbove(1));
    double tail = -70;

    Double_t par[6]={};
    double midpoint = (tail-noise_peak_max)/2. + noise_peak_max;

    TF1 *total = new TF1(Form("total_%d", n),"pol5",midpoint,3*noise_peak);
    h1->Fit(total,"RLFMQ+");

    total->GetParameters(&par[0]);
/*
    TCanvas *cans = new TCanvas(Form("cans_%d",n),"canvas",400,400);
    cans->cd();
    gStyle->SetOptFit(1);
    cans->SetLogy();
    h1->Draw();

    cans->Update();
    cans->Modified();
*/
    double chi2 = total->GetChisquare();
    int ndf = total->GetNDF();

    //MINIMA
    for(double i = 1; i<7.; i+=1.){ par[(int)i] = i*par[(int)i]; }
 
    std::vector<double> roots = {0,0,0,-15};
    if(par[5]!=0 && par[4]!=0 && par[3]!=0 && par[2]!=0 && par[1]!=0){
    roots =  ROOT::Math::Polynomial(par[5],par[4],par[3],par[2],par[1]).FindRealRoots();
    }

    //std::cout << roots[0] << " " << roots[1]<< " " << roots[2] << " " << roots[3] << std::endl; 
  /*  std::cout << "Fit: " << par[0] << " + " << par[1] << "x + " << par[2] << "x^2 + " << par[3] << "x^3 + " << par[4] << "x^4 + " << par[5] << "x^5" << std::endl; 
  */
    double minima = -15;

for(int m=0; m<4; m++){
    if( midpoint < roots[m] && roots[m] < noise_peak ){
        if ((par[2] +2*par[3]*roots[m] + 3*par[4]*roots[m]*roots[m] + 4*par[5]*roots[m]*roots[m]*roots[m])>0){ minima = roots[m]; }}}

//std::cout << "minima = " << minima << " midpoint= " << midpoint << " 3noise_peak= "<< 3*noise_peak << std::endl;

 //   std::cout << "minima: " << "x_1 = "<< x_1 << " x_2 = " << x_2 << " x_3 " << x_3 << " x_4 = " << x_4 << std::endl;
    if( n>2900 && n<4350){file << n << " " << (minima*1.2) << " " << chi2 << " " << ndf << " " << chi2/ndf << " " << noise_height << " " << noise_peak << endl;}
    else {file << n << " " << minima << " " << chi2 << " " << ndf << " " << chi2/ndf << " " << noise_height << " " << noise_peak << endl;}

}

file.close();
return 0;
}
