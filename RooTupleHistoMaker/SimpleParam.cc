#include "RooTupleHistoMaker/SimpleParam.hh"

SimpleParam::SimpleParam() : Drawable()
{}

SimpleParam::SimpleParam(Ntuple* ntp, TString leaf, float min, float max, int nBin, TString Title, TString VarTitle,bool includeOutOfRange) : 
  Drawable(ntp,min,max,nBin,Title,VarTitle,includeOutOfRange)
{ 
  InitOperation(leaf);
}


void 
SimpleParam::InitOperation(TString leaf)
{
  CheckForNbBranches(leaf);
  _varInd=AddNewBranchAndReturnIndex(leaf,&_c);  
  return;
}

double 
SimpleParam::GetValue(int couple)
{
  if(_c=='i') return _ntp->_leafIntVal[_varInd][couple];
  else return _ntp->_leafVal[_varInd][couple];
}



