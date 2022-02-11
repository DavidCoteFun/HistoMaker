#include "RooTupleHistoMaker/ParamWith2Ind.hh"

ParamWith2Ind::ParamWith2Ind() : Drawable()
{}

ParamWith2Ind::ParamWith2Ind(Ntuple* ntp, TString leaf, TString indleaf, TString indleaf2, float min, float max, int nBin, TString Title, TString VarTitle) : Drawable(ntp,min,max,nBin,Title,VarTitle)
{
  _indleaf=indleaf;
  _indleaf2=indleaf2;
  InitOperation(leaf);
}

void
ParamWith2Ind::InitOperation(TString leaf)
{
  
  CheckForNbBranches(leaf);
  CheckForNbBranches(_indleaf);
  CheckForNbBranches(_indleaf2);
  
  _var=AddNewBranchAndReturnIndex(leaf,&_c);
  _varInd=AddNewBranchAndReturnIndex(_indleaf);
  _varInd2=AddNewBranchAndReturnIndex(_indleaf2);
  return;
}

double
ParamWith2Ind::GetValue(int couple)
{
  int indice2 = _ntp->_leafIntVal[_varInd2][couple];
  int indice = _ntp->_leafIntVal[_varInd][indice2];
  if(_c=='i') return _ntp->_leafIntVal[_var][indice];
  else return _ntp->_leafVal[_var][indice];
}

