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

void uni_thresholds(){


ofstream oneval;
oneval.open("/a/home/houston/afadeeva/uBoone/OutMichel/uni5_thresh.txt");

int a, b, c, d, e, f, g;
c = d = e = f = g  = 0;

for(int i = 1; i<8257; i++){
  a = i;

 if(i<2401){ 
  b = 5;//-25
  oneval << a << " " << b << " " << c << " " << d << " " << e << " " <<f << " " << g << endl;}
 if(i>2400 && i<4801){
  b = 5;//15
  oneval << a << " " << b << " " << c << " " << d << " " << e << " " <<f << " " << g << endl;}
 if(i>4800){
  b = 5;
  oneval << a << " " << b << " " << c << " " << d << " " << e << " " <<f << " " << g << endl;}
  
}

oneval.close();

}
