#ifndef MULTICUTBASIC_HH
#define MULTICUTBASIC_HH

#include "RooTupleHistoMaker/MultiCut.hh"

class MultiCutBasic : public MultiCut {
public:
  MultiCutBasic();
  MultiCutBasic(Ntuple* ntp, TString mode, TString CutName);
  virtual ~MultiCutBasic();

protected:
  //Set this way mostly to make the MultiCut layer an abstract class... ;-)
  virtual void AddAllCuts(Ntuple* ntp, TString mode, TString CutName);

};

#endif
