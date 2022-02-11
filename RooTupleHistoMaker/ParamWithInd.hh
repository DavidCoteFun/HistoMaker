#ifndef PARAMWITHIND_HH
#define PARAMWITHIND_HH

#include "RooTupleHistoMaker/Drawable.hh"

class ParamWithInd : public Drawable {
public:
  ParamWithInd();
  ParamWithInd(Ntuple* ntp,TString leaf,TString indleaf,float min,float max,int nBin,TString Title,TString VarTitle,bool includeOutOfRange=true);
  virtual ~ParamWithInd(){}

  virtual double GetValue(int couple=0);

protected:
  virtual void InitOperation(TString leaf);
  int _var;
  int _varInd;
  TString _indleaf;
  char _c;
};

#endif
