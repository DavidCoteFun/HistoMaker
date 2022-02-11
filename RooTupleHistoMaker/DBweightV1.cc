#include "RooTupleHistoMaker/DBweightV1.hh"

#include "RooTupleHistoMaker/NoWeight.hh"

using std::cout;
using std::endl;

DBweightV1::DBweightV1() : DBweightInterface()
{ }

DBweightV1::DBweightV1(Ntuple* ntp) : DBweightInterface()
{ _ntp=ntp; }



Weight*
DBweightV1::GetFromDB(TString mode, TString weightName)
{
  if(weightName=="NoWeight") return new NoWeight(_ntp,mode);

  //else...  
  cout << "ERROR!  Unknown Weight in DBweightV1.cc" << endl;
  exit(1);
  return NULL;
}
