#ifndef CUTSTANDARDFROMDRAWABLE_HH
#define CUTSTANDARDFROMDRAWABLE_HH

#include "RooTupleHistoMaker/Cut.hh"
class Drawable;

class CutStandardFromDrawable : public Cut {
public:
  CutStandardFromDrawable();
  CutStandardFromDrawable(Ntuple* ntp, TString mode, TString opt, TString var, float min=-666, float max=666, bool inc=true, bool invert=false);
  virtual ~CutStandardFromDrawable(){}

  virtual bool PassEvtCut();
  virtual bool PassCoupleCut(int couple);

protected:
  virtual void InitOperation(TString mode);

  bool _inc;
  TString _opt;
  TString _var;
  float _min;
  float _max;

  Drawable* _param;
};

#endif
