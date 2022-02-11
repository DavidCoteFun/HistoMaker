#include "RooTupleHistoMaker/CutOneCouple.hh"

CutOneCouple::CutOneCouple() : Cut()
{}

CutOneCouple::CutOneCouple(Ntuple* ntp, TString mode) : Cut(ntp, mode)
{
  _CutName="OneCouple";
  _CutNameWithValues="Only 1 Couple";
  InitOperation(mode);
}

void 
CutOneCouple::InitOperation(TString mode)
{
  //_index=AddNewBranchAndReturnIndex(mode+"_XXX");  
 
  return;
}

bool 
CutOneCouple::PassEvtCut()
{
  int un=1;
  if( _ntp->_RemainingCoupleNbOfThisCut!=un ) return false;
  return true;
}
 
bool 
CutOneCouple::PassCoupleCut(int couple)
{
  //Best is to put a list of negative condition with "if" here...
  //if( !PassSigCondition(couple) ) return false;
  //if( !_ntp->_leafVal[_index][couple]<= zzz ) return false;

  return true;
}
