#ifndef EVTHISTO2D_HH
#define EVTHISTO2D_HH

#include "RooTupleHistoMaker/Histo2D.hh"

class EvtHisto2D : public Histo2D {
public:
  EvtHisto2D();
  virtual ~EvtHisto2D(){}
  virtual void FillHisto();
   
};

#endif
