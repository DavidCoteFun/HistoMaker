#include "RooTupleHistoMaker/MultiCutBasic.hh"
#include "RooTupleHistoMaker/CutVectorLength.hh"
#include "RooTupleHistoMaker/CutStandard.hh"

MultiCutBasic::MultiCutBasic() : MultiCut()
{}

MultiCutBasic::MultiCutBasic(Ntuple* ntp, TString mode, TString CutName) : MultiCut(ntp, mode)
{
  _CutName="MultiCutBasic"; 
  AddAllCuts(ntp,mode,CutName);
}

MultiCutBasic::~MultiCutBasic(){}

void 
MultiCutBasic::AddAllCuts(Ntuple* ntp, TString mode, TString CutName)
{
  Add( new CutVectorLength(ntp,mode) ); 
  Add( new CutStandard(ntp,mode,"cou","q2AveFit",2,20) );

  return;
}


