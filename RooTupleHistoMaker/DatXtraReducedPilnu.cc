#include "RooTupleHistoMaker/DatXtraReducedPilnu.hh"

DatXtraReducedPilnu::DatXtraReducedPilnu() : Dataset()
{
  _part="R16";
  _DatasetName="DatXtraReducedPilnu";
  AddRootFilesNameAndSetLumi();
}

DatXtraReducedPilnu::DatXtraReducedPilnu(TString part) : Dataset()
{
  _part=part;
  _DatasetName="DatXtraReducedPilnu";
  AddRootFilesNameAndSetLumi();
}

void
DatXtraReducedPilnu::AddRootFilesNameAndSetLumi()
{
  TString type;
  TString NtpDir="/atlas/discs/pool01/cote/BaBar/"; 

  ///////////////
  // ccbar  
  //////////////
  type="ccbar";
  //SetTotNbEvts(type,-1);
  SetTotNbEvts(type,32573);
  SetLumi(type,324.247);
  AddRootFile(type,NtpDir+"SP-1005-Run4-1.root");    
  
  ///////////////
  // pilnu
  //////////////
  type="pilnu";
  SetTotNbEvts(type,8148);
  AddRootFile(type,NtpDir+"SP-4764-1.root");
   
  return;
}









