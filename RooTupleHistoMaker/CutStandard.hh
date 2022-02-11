#ifndef CUTSTANDARD_HH
#define CUTSTANDARD_HH

#include "RooTupleHistoMaker/Cut.hh"

class CutStandard : public Cut {
public:
  CutStandard();
  CutStandard(Ntuple* ntp, TString mode, TString opt, TString var, float min=-666, float max=666, bool inc=true, bool invert=false);
  virtual ~CutStandard(){}

  virtual bool PassEvtCut();
  virtual bool PassCoupleCut(int couple);

protected:
  virtual void InitOperation(TString mode);

  bool _inc;
  TString _opt;
  TString _var;
  float _min;
  float _max;
  char _c;

  int _varInd;

};

#endif
