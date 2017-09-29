#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "TFile.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "lardataobj/RecoBase/Cluster.h"
#include <iostream>
#include <fstream>

using namespace art;
using namespace std;

void
michel(std::string const& filename){
  std::vector<std::string> filenames;
  std::ifstream file(filename);
  std::string str;
  while (std::getline(file, str)){
   filenames.push_back(str); 	}

  file.close();

  InputTag michel{ "michel","electron"};

  // Don't do the following in compiled C++. This code relies on the
  // interactive ROOT system to (implicitly) own the histograms we
  // create on the heap. In a C++ program, this object would be leaked
  // (the memory unrecoverable by the program).

  TH1F* e_hist = 
     new TH1F("e_hist","Charge Sum",400,0,400);

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
  //auto const& charge_sum = *ev.getValidHandle<vector<recob::Cluster>>(michel);
  
  std::cout << "file is being opened" << std::endl;

  gallery::Handle<vector<recob::Cluster>> michel_handle;

  if(ev.getByLabel(michel,michel_handle)){
  //if (!charge_sum.empty()){
    std::cout << "michel_handle size is " << michel_handle->size() << std::endl;

    for(int i=0; i<michel_handle->size(); i++){
    //double energy = ((*michel_handle)[i].Integral())/183.;
    //e_hist->Fill(energy);
    e_hist->Fill(((*michel_handle)[i].Integral())/189);
    std::cout << (*michel_handle)[i].Integral() << std::endl;

     }}
  }
TFile efile("michel_E_spectrum_allev.root","RECREATE");
//TFile *efile = new TFile("michel_E_spectrum.root","RECEREATE");
e_hist->Draw();

e_hist->Write();

efile.Close();

}
