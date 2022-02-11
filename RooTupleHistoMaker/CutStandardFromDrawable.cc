#include "RooTupleHistoMaker/CutStandardFromDrawable.hh"
#include "RooTupleHistoMaker/Drawable.hh"
#include "RooTupleHistoMaker/DBdrawableInterface.hh"

using std::cout;
using std::endl;

CutStandardFromDrawable::CutStandardFromDrawable() : Cut()
{}

CutStandardFromDrawable::CutStandardFromDrawable(Ntuple* ntp, TString mode, TString opt, TString var, float min, float max, bool inc, bool invert) 
  : Cut(ntp,mode,invert)
{
  _CutName=var;
  if(inc){
    if(min==-666) _CutNameWithValues=Form(var+" <= %.3f",max);
    else if(max==666) _CutNameWithValues=Form(var+" >= %.3f",min);
    else _CutNameWithValues=Form("%.3f <= "+var+" <= %.3f",min,max);
  }
  else{ 
    if(min==-666) _CutNameWithValues=Form(var+" < %.3f",max);
    else if(max==666) _CutNameWithValues=Form(var+" > %.3f",min);
    else _CutNameWithValues=Form("%.3f < "+var+" < %.3f",min,max);
  }
  _var=var;
  _min=min;
  _max=max;
  _opt=opt;
  _inc=inc;
  InitOperation(mode);
}

void 
CutStandardFromDrawable::InitOperation(TString mode)
{
  _param = _ntp->_DBdrawable->GetFromDB(mode,_var);

  if(_opt!="evt"&&_opt!="cou"&&_opt!="trk"&&_opt!="emc"&&_opt!="TruSig"&&_opt!="TruBlk"){ 
    cout << "WARNING! Unset Option in CutStandardFromDrawable!!!!" << endl;
  }
  if(_min<=_param->_min && _param->DoIncludeOutOfRange() ){
    cout<<"CutStandardFromDrawable_"<<_var<<endl;
    cout<<"The min of the cut is lower than the min of the Drawable and Drawable::_includeOutOfRange is true!!!"<<endl;
    cout<<"Aborting..."<<endl;
    exit(1);
  }
  if(_max>=_param->_max && _param->DoIncludeOutOfRange() ){
    cout<<"CutStandardFromDrawable_"<<_var<<endl;
    cout<<"The max of the cut is higher than the max of the Drawable and Drawable::_includeOutOfRange is true!!!"<<endl;
    cout<<"Aborting..."<<endl;
    exit(1);
  }
  
  return;
}

bool 
CutStandardFromDrawable::PassEvtCut()
{
  if (_opt=="evt"){
    float value=_param->GimmeValue();
    if(_inc){
      if( _min!=-666 && value<_min ) return false;
      if( _max!=666 && value>_max ) return false;
    }
    else{
      if( _min!=-666 && value<=_min ) return false;
      if( _max!=666 && value>=_max ) return false;
    }    
    return true;
  }
  return !_invert;
}
 
bool 
CutStandardFromDrawable::PassCoupleCut(int couple)
{
  if (_opt=="cou" || _opt=="trk" || _opt=="emc" || _opt=="TruSig" || _opt=="TruBlk"){
    float value=_param->GimmeValue(couple); 
    if(_inc){
      if( _min!=-666 && value<_min ) return false;
      if( _max!=666 && value>_max ) return false;
    }
    else{
      if( _min!=-666 && value<=_min ) return false;
      if( _max!=666 && value>=_max ) return false;
    }    
    return true;
  }
  return !_invert;
}
