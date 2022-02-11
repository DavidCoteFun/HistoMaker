#ifndef UNSETDRAWABLE_HH
#define UNSETDRAWABLE_HH

#include "RooTupleHistoMaker/Drawable.hh"

class UnsetDrawable : public Drawable {
public:
  UnsetDrawable();
  UnsetDrawable(Ntuple* ntp, TString leaf, float min, float max, int nBin, TString Title, TString VarTitle);
  virtual ~UnsetDrawable(){}

  virtual double GetValue(int couple=-666);
  virtual double AdditionalWeight(int couple);

protected:
  virtual void InitOperation(TString);
 
};

#endif
