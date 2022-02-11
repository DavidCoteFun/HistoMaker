#ifndef HISTO2D_HH
#define HISTO2D_HH

#include "RooTupleHistoMaker/Histo.hh"

class Histo2D : public Histo {
public:
  Histo2D();
  Histo2D(TString type);
  virtual ~Histo2D(){}
  
protected:
  virtual void SetupDrawable(TString var[]);
  virtual void CreateHisto(TString cut, TString sig); 
  virtual void FillHisto()=0;
  virtual void SetAllVarWeights(int type);
  
  TString _type;
};

#endif
