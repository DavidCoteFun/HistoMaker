#ifndef YHISTO1D_HH
#define YHISTO1D_HH

#include "RooTupleHistoMaker/Histo1D.hh"

class YHisto1D : public Histo1D {
public:
  YHisto1D();
  virtual ~YHisto1D(){}
  virtual void FillHisto();
 
private:
  void SetModeNbLeafs();
  int _mode_Nb[maxNbMode]; //Note: maxNbMode is a global variable defined in Ntuple.hh
  
};

#endif
