#include <iostream>
#include <random>
#include <vector>

int baseline_gen_other() {

  double var = 0; //each 64 block has a variance
  double bas = 0; //each 64 block has a baseline
  double del = 0;
  std::vector<double> waveform; //this is contains the ADC values
  std::vector<double> vars;
  std::vector<double> vardiff;
  
  TH1D *variances = new TH1D("variances","differences in variances",100,-10,10);
  
  unsigned seed = 913275619; //changed it so as not to confuse with an adc value
  std::default_random_engine generator (seed);

  //this distribution represends the distribution of ADC values
  std::normal_distribution<double> distribution (2047.,2.03);//changed this to 2047 to better reflect half the full dynamic range (12bit ADC means 2^12=4096)
  //std::cout << "baseline is " << std::endl;
  
  for (int i = 0; i<640000; i++) { //this loop should generate a set of 6400 ADC values, which corresponds to 100 64-sample blocks
    
    //    std::cout << "ADC is " << distribution(generator) << std::endl;
    //Note: every time you call distribution(generator) it will draw a different number. So this is how you will print out the value you actually save in the waveform:
    double x = distribution(generator);
    std::cout << "ADC is " << x << std::endl;    
    waveform.push_back(x);//you are now saving ADC values, not baseline values
    
  }

  int n;
  //computing baseline first, then variance! (each block has a different baseline!)
  //baseline = [Sum_(n is from 1 to 64) adc_n ]/ 64
  //variance = [Sum_(n is from 1 to 64) (Delta_n)^2 ]/ 64
  for (int a = 0; a<10000; a++){//loop over 100 64-sample blocks
    //first calculate baseline for this block
    bas=0;//reset baseline value first
    for (int n = 0; n<64; n++){
      bas += waveform[a*64+n]; 
      //      if(  std::abs(waveform[n] - 2000) < 63) { del +=(std::abs(waveform[n] - 2000))*(std::abs(waveform[n] - 2000));}
      //      else if(  std::abs(waveform[n] - 2000) > 63) { del += 4095*4095;}      
    }
    bas = bas/64.;

    //then calculate variance
    del=0;//reset del value first
    for (n = 0; n<64; n++){
      if(  std::abs(waveform[a*64+n] - bas) <= 63) { del += ((std::abs(waveform[a*64+n] - bas))*(std::abs(waveform[a*64+n] - bas)));} //less than or equal; also changed to use the varying baseline per 64 block
      else if(  std::abs(waveform[a*64+n] - bas) > 63) { del += (4095*4095);}      
    }
    var = del/64.;
    std::cout << "starting adc sample " << a << "\tending adc sample " << n << "\t baseline is " << bas  << "\t variance is " << var << std::endl;
    vars.push_back(var);
  }
  
  for (int m = 0; m<10000; m++){ //this should loop over the 100 saved variances (one per 64-sample block)
    for (int j = 0; j<10000; j++){
      if (m!=j) {//you need this to avoid comparing a given 64-sample block to itself!)
	if (j>m) {//you need this to avoid double counting
	  double diff = vars[m] - vars[j];
	  variances->Fill(diff);
	}
      }
    }
  }
  
  TCanvas *cans = new TCanvas("cans","variances",500,500);
  cans->cd();
  variances->Fit("gaus","","",-3,3);
  variances->Draw();
  
  return 0;
}
