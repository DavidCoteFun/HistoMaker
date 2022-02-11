#ifndef CUTONECOUPLE_HH
#define CUTONECOUPLE_HH

#include "RooTupleHistoMaker/Cut.hh"

class CutOneCouple : public Cut {
public:
  CutOneCouple();
  CutOneCouple(Ntuple* ntp, TString mode);
  virtual ~CutOneCouple(){}

  virtual bool PassEvtCut();
  virtual bool PassCoupleCut(int couple);

protected:
  virtual void InitOperation(TString mode);

};

#endif
