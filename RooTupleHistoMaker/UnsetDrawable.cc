#include "RooTupleHistoMaker/UnsetDrawable.hh"

UnsetDrawable::UnsetDrawable() : Drawable()
{}

UnsetDrawable::UnsetDrawable(Ntuple* ntp, TString leaf, float min, float max, int nBin, TString Title, TString VarTitle) :
  Drawable(ntp,min,max,nBin,Title,VarTitle)
{
  InitOperation("do nothing");
}


void
UnsetDrawable::InitOperation(TString leaf)
{
  return;
}

double
UnsetDrawable::GetValue(int couple)
{
  return -667.0;
}

double
UnsetDrawable::AdditionalWeight(int couple)
{
  return 1.0;
}
