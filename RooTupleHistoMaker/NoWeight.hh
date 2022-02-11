#ifndef NOWEIGHT_HH
#define NOWEIGHT_HH

#include "RooTupleHistoMaker/Weight.hh"

class NoWeight : public Weight {
public:
  NoWeight();
  NoWeight(Ntuple* ntp,TString mode);
  virtual ~NoWeight(){}

  virtual float AdditionalWeight(int couple);
  virtual float BranchingFractionWeight();

protected:
  virtual void InitOperation(TString);
  int _varInd;

};

#endif
