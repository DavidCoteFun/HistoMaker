#ifndef YHISTO2D_HH
#define YHISTO2D_HH

#include "RooTupleHistoMaker/Histo2D.hh"

class YHisto2D : public Histo2D {
public:
  YHisto2D();
  YHisto2D(TString type);
  virtual ~YHisto2D(){}
  virtual void FillHisto();
 
private:
  void SetModeNbLeafs();
  int _mode_Nb[maxNbMode]; //Note: maxNbMode is a global variable defineds in Ntuple.hh

};

#endif
