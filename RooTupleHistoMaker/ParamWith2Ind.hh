#ifndef PARAMWITH2IND_HH
#define PARAMWITH2IND_HH

#include "RooTupleHistoMaker/Drawable.hh"

class ParamWith2Ind : public Drawable {
public:
  ParamWith2Ind();
  ParamWith2Ind(Ntuple* ntp, TString leaf, TString indleaf, TString indleaf2, float min, float max, int nBin, TString Title, TString VarTitle);
  virtual ~ParamWith2Ind(){}

  virtual double GetValue(int couple=0);

protected:
  virtual void InitOperation(TString leaf);
  int _var;
  int _varInd;
  int _varInd2;
  TString _indleaf;
  TString _indleaf2;
  char _c;
};

#endif
