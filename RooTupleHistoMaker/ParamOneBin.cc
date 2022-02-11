#include "RooTupleHistoMaker/ParamOneBin.hh"

ParamOneBin::ParamOneBin() : Drawable()
{}

ParamOneBin::ParamOneBin(Ntuple* ntp, float min, float max, int nBin, TString Title, TString VarTitle) : Drawable(ntp,min,max,nBin,Title,VarTitle)
{
 
}


void
ParamOneBin::InitOperation(TString leaf)
{
  return;
}

double
ParamOneBin::GetValue(int couple)
{
  
  return 1;

}
