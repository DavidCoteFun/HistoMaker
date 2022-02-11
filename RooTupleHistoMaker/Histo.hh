#ifndef HISTO_HH
#define HISTO_HH
 
#include "TH2.h"
#include "TLeaf.h"

#include "Ntuple.hh"
#include "DBdatasetInterface.hh"
#include "DBcutInterface.hh"
#include "DBweightInterface.hh"
#include "Drawable.hh"

//Note: maxNbType, maxNbMode, maxNbVar and maxNbCouple are global variables defined in Ntuple.hh

class Histo : public Ntuple {
public:
  Histo();
  virtual ~Histo();

  void SetMaxEvtNb(int Nb){ _maxEvtNb=Nb; }
  void SetNbQ2Bins(int Nb);
  void MakeHisto(TString datasetName, TString var[], TString type[], TString mode[], TString sig="all", TString cut="no", TString weightName="NoWeight");
  void MakeNewNtuple(TString datasetName, TString varToFill[], TString type[], TString mode[], TString sig, TString cut, TString FileName);
  void MakeNewClonedNtuple(TString datasetName, TString varToFill[], TString type[], TString mode[], TString sig, TString cut, TString FileName);
  
  TH1* _histosQ2[maxNbType][maxNbMode][maxNbVar][maxNbQ2Bins]; //[t][m][v][q2]
  TH1* _histos[maxNbType][maxNbMode][maxNbVar]; //[t][m][v]
  //DBdrawableInterface* _DBdrawable; //<---moved to Ntuple.hh
  DBcutInterface* _DBcut;
  DBweightInterface* _DBweight;
  DBdatasetInterface* _DBdataset;
  
  TString _usedVar[maxNbVar+200][2];  int _nUsedVar;
  int _Filled[maxNbType][maxNbMode];
  
  MultiCut* _usedCut[maxNbMode]; //one object per mode
  Weight* _usedWeight[maxNbType][maxNbMode]; //one object per [type][mode]
  Drawable* _usedDrawable[maxNbMode][maxNbVar+200][2]; //one object per mode/branch to draw/dimension

protected: 
  void GetTree(int t);
  void ResetOperations(int t);
  void SetupCommon(TString var[], TString type[],  TString mode[], TString sig="all", TString cut="no", TString weightName="NoWeight");
  
  virtual void SetupDrawable(TString var[])=0;
  virtual void CreateHisto(TString cut="no", TString sig="all")=0; 
  virtual void FillHisto()=0;
  virtual void SetAllVarWeights(int type)=0;
  
  int _maxEvtNb;
  int _nQ2Bins;
 
private:
};

#endif

