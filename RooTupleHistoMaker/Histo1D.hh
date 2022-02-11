#ifndef HISTO1D_HH
#define HISTO1D_HH

#include "RooTupleHistoMaker/Histo.hh"

class Histo1D : public Histo {
public:
  Histo1D();
  virtual ~Histo1D(){}
  
protected:
  virtual void SetupDrawable(TString var[]);
  virtual void CreateHisto(TString cut, TString sig); 
  virtual void FillHisto()=0;
  virtual void SetAllVarWeights(int type);
};

#endif

