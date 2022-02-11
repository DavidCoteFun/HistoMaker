#ifndef CUT_HH
#define CUT_HH

#include "Operation.hh"

class Cut : public Operation {
public:
  Cut();
  Cut(Ntuple* ntp, TString mode, bool invert=false);
  virtual ~Cut(){}

  virtual bool PassCut();
  virtual void setSignalToAccept(TString key);
  virtual int ChangeTypeAndReturnInd(TString type);
  int GetTypeInd(TString type);
  float GetEvtCounter(TString type);
  float GetEvtCounter(int t);
  float GetCoupleCounter(TString type);
  float GetCoupleCounter(int t);

  double EvtEfficiency(int type);
  double EvtEfficiency(TString type);

  TString GetName() { return _CutName; }
  TString GetNameWithValues() { return _CutNameWithValues; } 
  
  virtual bool PassEvtCut()=0;
  virtual bool PassCoupleCut(int couple)=0;
 
protected:
  bool PassSigCoupleCondition(int couple);
  bool PassSigEventCondition(int Key);
  bool (Cut::*_sigOKCou)(int);
  bool (Cut::*_sigOKEvt)(int);           

  virtual void InitOperation(TString)=0;

  TString _mode;
  TString _sigKey;

  int _TruSigInd;
  int _typeB1Ind;
  int _typeB2Ind;

  TString _CutName; 
  TString _CutNameWithValues;
 
  //counters
  int _nTypeUsed;
  int _currentType;
  TString _types[maxNbType]; //Note: maxNbType is a global variable defined in Ntuple.hh 
  float _counterCou[maxNbType]; //Note: maxNbType is a global variable defined in Ntuple.hh 
  float _counterEvt[maxNbType]; //Note: maxNbType is a global variable defined in Ntuple.hh 

  bool _invert;

private:
  int _lastGoodEvt;
  //float MeanEvtWeightWithCut(int mode_Nb, int evt);

  bool AlwaysTrue(int couple) {return true; }
  bool OnlySigEvts(int key);
  bool OnlyBkgEvts(int key) { return !OnlySigEvts(key); }
  bool OnlySigCouples(int couple);
  bool OnlyBkgCouples(int couple) { return !OnlySigCouples(couple); }
  bool OnlyTrueXu(int couple);
  bool OnlyFakeXu(int couple) { return !OnlyTrueXu(couple); }
  bool OnlyTrueLep(int couple);
  bool OnlyFakeLep(int couple) { return !OnlyTrueLep(couple); }
};

#endif


