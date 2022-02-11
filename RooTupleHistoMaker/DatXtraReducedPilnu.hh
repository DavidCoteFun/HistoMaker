#ifndef DATXTRAREDUCEDPILNU_HH
#define DATXTRAREDUCEDPILNU_HH

#include "RooTupleHistoMaker/Dataset.hh"

class DatXtraReducedPilnu : public Dataset {
public:
  DatXtraReducedPilnu();
  DatXtraReducedPilnu(TString part);
  virtual ~DatXtraReducedPilnu(){}
 
  virtual void AddRootFilesNameAndSetLumi();

  TString _part;
};

#endif
