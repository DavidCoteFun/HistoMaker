#ifndef PARAMMULTIPLICITY_HH
#define PARAMMULTIPLICITY_HH

#include "RooTupleHistoMaker/Drawable.hh"

class ParamMultiplicity : public Drawable {
public:
  ParamMultiplicity();
  ParamMultiplicity(Ntuple* ntp, float min, float max, int nBin, TString Title, TString VarTitle);
  virtual ~ParamMultiplicity(){}

  virtual double GetValue(int couple=0);

protected:
  virtual void InitOperation(TString);

};

#endif
