#include "RooTupleHistoMaker/ParamMultiplicity.hh"

ParamMultiplicity::ParamMultiplicity() : Drawable()
{}

ParamMultiplicity::ParamMultiplicity(Ntuple* ntp, float min, float max, int nBin, TString Title, TString VarTitle) 
  : Drawable(ntp,min,max,nBin,Title,VarTitle)
{}


void
ParamMultiplicity::InitOperation(TString leaf)
{
  return;
}

double
ParamMultiplicity::GetValue(int couple)
{
  return _ntp->_RemainingCoupleNbOfThisCut;
}
