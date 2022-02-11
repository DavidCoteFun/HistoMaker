#include "RooTupleHistoMaker/Drawable.hh"
#include "RooTupleHistoMaker/Weight.hh"

Drawable::Drawable() : Operation()
{}

Drawable::Drawable(Ntuple* ntp,float min,float max,int nBin,TString Title,TString VarTitle,bool includeOutOfRange) : Operation(ntp)
{
  _min=min;
  _max=max;
  _nBin=nBin;
  _Title=Title;
  _VarTitle=VarTitle;  
  _includeOutOfRange=includeOutOfRange;

  if(includeOutOfRange) { _GimmeValueFunction=&Drawable::GimmeValueAndConsiderRange; }
  else { _GimmeValueFunction=&Drawable::GimmeValueDontConsiderRange; }

}

double 
Drawable::GetWeight(int couple)
{
  if(_weight!=NULL) return _weight->GetWeight(couple);
  else return 1.0;
}


double
Drawable::GimmeValue(int couple)
{
  return (this->*_GimmeValueFunction)(couple);
}


double 
Drawable::GimmeValueAndConsiderRange(int couple)
{
  double val = GetValue(couple);
  
  if(val<_min) val=_min;              
  else if(val>= _max) val=_max-0.00001; 

  return val;
}
