#include "RooTupleHistoMaker/Dataset.hh"

using std::cout;
using std::endl;

Dataset::Dataset() 
{
  //Initialize the "types" index and bookkeeping
  _nbTypes=maxNbType;
  TString types[maxNbType]={"B0B0bar","BpBm","ccbar","uds","tautau","OnPeak","OffPeak","pilnu","pi0lnu","etalnu","etaplnu","rhoClnu","rho0lnu","omegalnu"};
  if (ProdNum==7){
    float lumi[maxNbType]={673.062,690.613,211.083,261.815,237.800,210.486878,21.587341,1146.465,1484.849,4393.939,2104.466,1219.214,1425.079,1404.122};
    for (int t=0; t<maxNbType; t++){ _Lumi[t]=lumi[t]; }
    _nBBbarOnPeak=231737988.8;
  }
  else if (ProdNum==8){
    float lumi[maxNbType]={973.455,962.700,282.955,335.192,453.112,210.486878,21.587341,1292.929,111,111,111,111,111,111};
    for (int t=0; t<maxNbType; t++){ _Lumi[t]=lumi[t]; }
    _nBBbarOnPeak=231737988.8;
  }
  else if (ProdNum==9){
    types[11]="mmg";
    types[12]="eeg";
    types[13]="eeee";
    float lumi[maxNbType]={957.613,1053.563,324.247,323.923,383.879,206.4051,21.487524,1292.929,111,111,111,111,111,111};
    for (int t=0; t<maxNbType; t++){ _Lumi[t]=lumi[t]; }
    _nBBbarOnPeak=227394922.0;
  }
  else if (ProdNum==10){
    types[9]="offCc";
    types[10]="onCc";
    types[11]="mmg";
    types[12]="eeg";
    types[13]="eeee"; //Les lumis sont pour R16c seulement
    float lumi[maxNbType]={956.979,1052.864,324.247,323.923,383.879,206.4051,21.487524,4130.009575,111,111,111,111,111,111};
    for (int t=0; t<maxNbType; t++){ _Lumi[t]=lumi[t]; }
    _nBBbarOnPeak=227394922.0;
  }
  else { cout << "ERROR! Unknown ProdNum in Dataset.cc" << endl; assert(0); }

  for(int t=0; t<maxNbType; t++) {
    _types[t]=types[t];
    _nbRootFiles[t]=0;
    //_Lumi[t]=0.0;
    for(int f=0; f<maxNbFiles; f++) {
      _RootFilesName[t][f]="";
    }
  }
  
}


void 
Dataset::AddRootFile(TString type, TString RootFileName)
{
  int t = GetTypeInd(type);

  _RootFilesName[t][_nbRootFiles[t]] = RootFileName;  
  _nbRootFiles[t]+=1;

  return;
}

float 
Dataset::Lumi(TString type)
{
  //Answer in fb-1
  int t=GetTypeInd(type);

  return _Lumi[t];  
}

void 
Dataset::SetLumi(TString type, float lum)
{
  int t = GetTypeInd(type);
  _Lumi[t]=lum;

  return;
}

long int 
Dataset::TotNbOfEvents(TString type)
{
  int t=GetTypeInd(type);
  
  return _TotNbEvts[t];  
}

void 
Dataset::SetTotNbEvts(TString type, long int nb)
{
  int t = GetTypeInd(type);
  _TotNbEvts[t]=nb;

  return;
}


int
Dataset::GetTypeInd(TString type)
{
  bool found=false;
  int ind=0;
  for(int t=0; t<_nbTypes && !found; t++) {
    if(type==_types[t]) { ind=t; found=true; }
  }
  if(!found) { cout<<"Dataset.cc --> Request for unknown type "<<type <<"... returning -1!"<<endl; return -1; }
  return ind;
}

int 
Dataset::NbRootFiles(TString type)
{
  int t = GetTypeInd(type);
  return _nbRootFiles[t];
}


