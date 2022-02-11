#include "RooTupleHistoMaker/CutVectorLength.hh"

CutVectorLength::CutVectorLength() : Cut()
{}

CutVectorLength::CutVectorLength(Ntuple* ntp, TString mode) : Cut(ntp, mode)
{
  _CutName="CutVectorLength"; 
  _CutNameWithValues="VectorLength > 0"; 
  InitOperation(mode);
}

void 
CutVectorLength::InitOperation(TString mode)
{
  TString varName = _ntp->_TreeSpec->GetLoopNbName(mode);
  _modeNbInd=AddNewBranchAndReturnIndex(varName);   
  
  return;
}

bool 
CutVectorLength::PassEvtCut()
{
  int mode_Nb = _ntp->_leafIntVal[_modeNbInd][0];
  if(mode_Nb==0) return false; //Intrinsic cut: at least one Y of this mode in the event
  return true;
}

bool 
CutVectorLength::PassCoupleCut(int couple)
{
  return true;
}

