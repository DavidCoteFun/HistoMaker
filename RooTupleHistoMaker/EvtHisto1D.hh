#ifndef EVTHISTO1D_HH
#define EVTHISTO1D_HH

#include "RooTupleHistoMaker/Histo1D.hh"

class EvtHisto1D : public Histo1D {
public:
  EvtHisto1D();
  virtual ~EvtHisto1D(){}
  virtual void FillHisto();
 
};

#endif
