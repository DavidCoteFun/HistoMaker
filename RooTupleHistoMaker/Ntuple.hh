#ifndef NTUPLE_HH
#define NTUPLE_HH
 
#include "TChain.h"
#include <iostream>
#include "RooTupleHistoMaker/TreeSpecInterface.hh"

const int maxNbCut=50;
const int maxNbQ2Bins=27;
const int maxNbQ2FitBin=15;
const int maxNbType=14;//In principle this can go up to 14
const int maxNbMode=2; //In principle this can go up to 10
const int maxNbVar=200;
const int maxNbGlobalVar=800;
const int maxNbCouple=250; //can go up to 1250
const int maxNbFiles=2000;
const int ProdNum=10;

//Stuff for Operation::RandomNumber
const int maxID=2;
const int maxNbCouRand=50;
const int maxNbCallRand=100;

class DBdrawableInterface;
class Dataset;

class Ntuple {
public:
  Ntuple();
  virtual ~Ntuple();

  TChain* _tree;

  void Test();
  void SetConfigPatch(TString key);
  void SetQ2Binning(TString method){ _q2Binning=method; }
  void SetSeed(int seed);
  void IWantToUseTheGetEventCommand(bool opinion);

  int _nGlobalLeaf, _nRemainingGlobalLeaf;
  int _nGlobalLeafInt, _nRemainingGlobalLeafInt;  
  int _nGlobalLeafDouble, _nRemainingGlobalLeafDouble;  
  
  //One and only one per used leaf in the job (Float)
  TString _globalLeaf[maxNbGlobalVar]; 
  float _leafVal[maxNbGlobalVar][maxNbCouple];
  TString _RemainingglobalLeaf[maxNbGlobalVar]; 
  float _RemainingleafVal[maxNbGlobalVar][maxNbCouple]; 

  //One and only one per used leaf in the job (Int)
  TString _globalLeafInt[maxNbGlobalVar];
  int _leafIntVal[maxNbGlobalVar][maxNbCouple]; 
  TString _RemainingglobalLeafInt[maxNbGlobalVar];
  int _RemainingleafIntVal[maxNbGlobalVar][maxNbCouple];  

  //One and only one per used leaf in the job (Double)
  //For the size, one would normally use maxNbGlobalVar instead of 5, but we know there is no double in our ntuples
  //and in fact these arrays of doubles takes a lot of live memory!
  TString _globalLeafDouble[5];           
  double _leafDoubleVal[5][maxNbCouple];  
  TString _RemainingglobalLeafDouble[5]; 
  double _RemainingleafDoubleVal[5][maxNbCouple]; 

  void GetGlobalEvent(int e);
  int CurrentEvt(){ return _currentGlobalEvt; }
  void ResetNtpPointers();
  int AddNewBranchAndReturnIndex(TString branch, char *c); 
  int AddNewBranchAndReturnIndex(TString branch); 

  //For Cut.hh and its family...
  //All this complicated stuff is to ensure that only the couples that survived the previous
  //cuts are used by the subsequent cuts. This fixed a problem with the Couple cuts in EvtHisto
  //and allows to make the OneCouple cut. See code in Cut.cc and MultiCut.cc for details...
  int _RemainingCoupleNbOfThisCut;
  int _RemainingCoupleNbOfNextCut;
  int _RemainingCoupleIndOfThisCut[maxNbCouple];
  int _RemainingCoupleIndOfNextCut[maxNbCouple];

  DBdrawableInterface* _DBdrawable;
  TreeSpecInterface* _TreeSpec;

  TString _ConfigPatch; //Changeable with SetConfigPatch for DATA, Syst_GamKill18, or any other systematic error key
  int _seed;
  TString _q2Binning;

  float ScaleFactorToOnPeakLumi(TString type, TString mode="pilnu");
  float ScaleFactorToFullDatasetProcessing(TString type, TString mode="pilnu");
  int GetTypeInd(TString type); //Warning: not guaranteed to be equivalent to Dataset's or Cut's GetTypeInd (!!)
  int GetModeInd(TString mode);
  Dataset* _dataset;
  int _Processed[maxNbType][maxNbMode];
  TString _usedType[maxNbType]; int _nUsedType;
  TString _usedMode[maxNbMode]; int _nUsedMode;  

protected:
  TString xxxNb(TString branch);
  bool _makeNewNtuple;
  TTree* _newntuple;

private:
  int AddBranchAndReturnIndex(TString branch);
  int AddBranchIntAndReturnIndex(TString branch);
  int AddBranchDoubleAndReturnIndex(TString branch);

  int  _currentGlobalEvt;
};

#endif
