#ifndef SIMPLEPARAM_HH
#define SIMPLEPARAM_HH

#include "RooTupleHistoMaker/Drawable.hh"

class SimpleParam : public Drawable {
public:
  SimpleParam();
  SimpleParam(Ntuple* ntp, TString leaf, float min, float max, int nBin, TString Title, TString VarTitle,bool includeOutOfRange=true);
  virtual ~SimpleParam(){}

  virtual double GetValue(int couple=0); 

protected:
  virtual void InitOperation(TString);
  int _varInd;
  char _c;

};

#endif
