#include "RooTupleHistoMaker/DBcutV1.hh"

#include "RooTupleHistoMaker/Ntuple.hh"
#include "RooTupleHistoMaker/MultiCutBasic.hh"

using std::cout;
using std::endl;

DBcutV1::DBcutV1() : DBcutInterface()
{ }

DBcutV1::DBcutV1(Ntuple* ntp) : DBcutInterface()
{ _ntp=ntp; }

MultiCut*
DBcutV1::GetFromDB(TString mode, TString Key)
{
  if(Key=="MultiCutBasic") return new MultiCutBasic(_ntp,mode,Key);

  //else... 
  cout <<"ERROR! Unknown CutName in DBcutV1.cc  -->"<<Key<< endl; exit(1);
  exit(1);
  return NULL;
}
