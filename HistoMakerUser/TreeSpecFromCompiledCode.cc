#include "HistoMakerUser/TreeSpecFromCompiledCode.hh"

TreeSpecFromCompiledCode::TreeSpecFromCompiledCode() : TreeSpecInterface()
{}

TString TreeSpecFromCompiledCode::GetLoopNbName( TString key ){
  return "pilnu_Nb";
}
