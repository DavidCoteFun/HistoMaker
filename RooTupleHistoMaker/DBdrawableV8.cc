#include "RooTupleHistoMaker/DBdrawableV8.hh"
#include "RooTupleHistoMaker/Ntuple.hh"
#include "RooTupleHistoMaker/SimpleParam.hh"
#include "RooTupleHistoMaker/ParamWithInd.hh"
#include "RooTupleHistoMaker/ParamWith2Ind.hh"
#include "RooTupleHistoMaker/ParamMultiplicity.hh"
#include "RooTupleHistoMaker/ParamOneBin.hh"
#include "RooTupleHistoMaker/UnsetDrawable.hh"

using std::cout;
using std::endl;

DBdrawableV8::DBdrawableV8() : DBdrawableInterface()
{ }

DBdrawableV8::DBdrawableV8(Ntuple* ntp) : DBdrawableInterface()
{ _ntp=ntp; }


Drawable*
DBdrawableV8::GetFromDB(TString mode, TString key)
{   
  if(key=="unset") return new UnsetDrawable(_ntp,"unset",0,1,10,"unset","unset");    
  if(key=="deltaE") return new SimpleParam(_ntp,mode+"_"+key,-2.5,2.0,20,"#DeltaE (GeV)","DeltaE");
  if(key=="mES") return new SimpleParam(_ntp,mode+"_"+key,4.29,5.29,20,"m_{ES} (GeV/c^{2})","mES");  
  if(key=="q2AveFit") return new SimpleParam(_ntp,mode+"_"+key,0,26.4,20,"q^{2} (GeV^{2}/c^{4})","q2"); 
  
  cout << "***** ERROR... Unset variable on DBdrawable.cc  :  "+key+" *****" << endl;
  cout << "The Program will skip this variable" << endl; 
  return  new UnsetDrawable(_ntp,"unset",0,1,50,"unset","unset");    
}
