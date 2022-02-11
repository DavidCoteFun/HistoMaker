#include "RooTupleHistoMaker/ParamWithInd.hh"

ParamWithInd::ParamWithInd() : Drawable()
{}

ParamWithInd::ParamWithInd(Ntuple* ntp,TString leaf,TString indleaf,float min,float max,int nBin,TString Title,TString VarTitle,bool includeOutOfRange)
  : Drawable(ntp,min,max,nBin,Title,VarTitle,includeOutOfRange)
{
  _indleaf=indleaf;
  InitOperation(leaf);
}

void
ParamWithInd::InitOperation(TString leaf)
{
  
  CheckForNbBranches(leaf);
  CheckForNbBranches(_indleaf);

  _var=AddNewBranchAndReturnIndex(leaf,&_c);
  _varInd=AddNewBranchAndReturnIndex(_indleaf);
  return;

}

double
ParamWithInd::GetValue(int couple)
{
  int indice = _ntp->_leafIntVal[_varInd][couple];
  if(_c=='i') return _ntp->_leafIntVal[_var][indice];
  else return _ntp->_leafVal[_var][indice];
}


