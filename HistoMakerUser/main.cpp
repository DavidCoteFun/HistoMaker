#include <iostream>
#include <TROOT.h>
#include "TFile.h"

using std::cout;
using std::endl;

#include "RooTupleHistoMaker/YHisto1D.hh" 

void SetHisto(TH1* h, int color);

int main(int argc, char* argv[])
{
  cout<<"Welcome to HistoMaker! :-)"<<endl;
  // Arguments  
  TString dataset = argv[1];
  TString method = argv[2];
  TString cut = argv[3];
  TString weight = argv[4];
  TString fileout = argv[5];
  int nbEvts = atoi(argv[6]);
  
  TString SystKey="default";   if(argc>7){ SystKey = argv[7]; }
  int seed = 666;              if(argc>8){ seed = atoi(argv[8]); }  
  cout<<dataset<<" "<<method<<" "<<cut<<" "<<weight<<" "<<fileout<<" "<<nbEvts<<" "<<SystKey<<" "<<seed<<endl;

  /////////////////////////////////
  TString type[13]={"ccbar","pilnu"};
  TString mode[11]={"pilnu"};
  TString var[30]={"deltaE","mES","q2AveFit"};
  Histo* h = new YHisto1D();  
  h->SetMaxEvtNb(nbEvts);    
  h->SetConfigPatch(SystKey);
  h->MakeHisto(dataset,var,type,mode,"all",cut,weight);
  
  float scale=h->ScaleFactorToOnPeakLumi("pilnu");
  SetHisto(h->_histos[0][0][0],1);
  SetHisto(h->_histos[0][0][1],1);
  SetHisto(h->_histos[0][0][2],1);
  SetHisto(h->_histos[1][0][0],2);
  SetHisto(h->_histos[1][0][1],2);
  SetHisto(h->_histos[1][0][2],2);
  
  TDirectory *dir = gDirectory; 
  TFile *f = new TFile(fileout+".root","recreate");
  dir->GetList()->Write();  
  //h->_histos[0][0][0]->Write();
  //h->_histos[0][0][1]->Write();
  //h->_histos[0][0][2]->Write();

  f->Close();
  f->Delete();
  delete h;

  return 1;
}

void SetHisto(TH1 *h, int color){
  h->Scale(1./h->GetEntries());
  h->SetLineColor(color);
  return;
}
