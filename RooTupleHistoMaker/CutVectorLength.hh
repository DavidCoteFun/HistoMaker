#ifndef CUTVECTORLENGTH_HH
#define CUTVECTORLENGTH_HH

#include "RooTupleHistoMaker/Cut.hh"

class CutVectorLength : public Cut {
public:
  CutVectorLength();
  CutVectorLength(Ntuple* ntp, TString mode);
  virtual ~CutVectorLength(){}

  virtual bool PassEvtCut();
  virtual bool PassCoupleCut(int couple);

protected:
  virtual void InitOperation(TString mode);
  int _modeNbInd;
};

#endif
