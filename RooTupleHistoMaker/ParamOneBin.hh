#ifndef PARAMONEBIN_HH
#define PARAMONEBIN_HH

#include "RooTupleHistoMaker/Drawable.hh"

class ParamOneBin : public Drawable {
public:
  ParamOneBin();
  ParamOneBin(Ntuple* ntp, float min, float max, int nBin, TString Title, TString VarTitle);
  virtual ~ParamOneBin(){}

  virtual double GetValue(int couple=0);

protected:
  virtual void InitOperation(TString);
 
};

#endif
