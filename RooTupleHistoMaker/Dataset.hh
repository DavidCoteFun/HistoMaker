#ifndef DATASET_HH
#define DATASET_HH

#include "Ntuple.hh"  //That dependency is there for the global variables

#include "TTree.h"
#include <iostream>


class Dataset {
public:
  Dataset();
  virtual ~Dataset(){}

  TString DatasetName() {return _DatasetName; }

  int NbRootFiles(TString type);

  float nBBbarOnPeak(){ return _nBBbarOnPeak; }
  float Lumi(TString type); //Answer in fb-1
  long int TotNbOfEvents(TString type); 

  int GetTypeInd(TString type);
  TString _RootFilesName[maxNbType][maxNbFiles];  int _nbRootFiles[maxNbType];

protected:

  virtual void AddRootFilesNameAndSetLumi()=0;  //This method must do what its name says... ;-)
                                                //Also, don't forget to set your _DatasetName, possibly in the constructor

  void AddRootFile(TString type, TString RootFileName);
  void SetLumi(TString type, float lum);
  void SetTotNbEvts(TString type, long int nb);

  TString _DatasetName;  //To be set in the inheriting classes
  float _nBBbarOnPeak;

private:

  TString _types[maxNbType];  int _nbTypes;

  long int _TotNbEvts[maxNbType];
  float _Lumi[maxNbType]; //in fb-1
  float _Lumi2[maxNbType]; //in fb-1
};

#endif





