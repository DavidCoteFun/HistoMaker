#include "RooTupleHistoMaker/DBdatasetV6.hh"

#include "RooTupleHistoMaker/DatXtraReducedPilnu.hh"

using std::cout;
using std::endl;

DBdatasetV6::DBdatasetV6() : DBdatasetInterface()
{ }

DBdatasetV6::DBdatasetV6(Ntuple* ntp) : DBdatasetInterface()
{ _ntp=ntp; }



Dataset*
DBdatasetV6::GetFromDB(TString datasetName)
{
  if(datasetName=="DatXtraReducedPilnu") return new DatXtraReducedPilnu();
  
  //else...
  cout<<"ERROR! Unknown dataset in DBdatasetV6.cc!! --> "<<datasetName<<endl;
  exit(1);

  return NULL;
}
