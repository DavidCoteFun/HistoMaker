#include "RooTupleHistoMaker/CutStandard.hh"

using std::cout;
using std::endl;

CutStandard::CutStandard() : Cut()
{}

CutStandard::CutStandard(Ntuple* ntp, TString mode, TString opt, TString var, float min, float max, bool inc, bool invert) : Cut(ntp,mode,invert)
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
CutStandard::InitOperation(TString mode)
{
  if (_opt=="evt") _varInd=AddNewBranchAndReturnIndex(_var,&_c); 
  else if (_opt=="cou") _varInd=AddNewBranchAndReturnIndex(mode+"_"+_var,&_c);  
  else if (_opt=="trk" || _opt=="emc" || _opt=="TruSig" || _opt=="TruBlk"){
    AddNewBranchAndReturnIndex(_opt+"_Nb");
    _varInd=AddNewBranchAndReturnIndex(_opt+"_"+_var,&_c);  
  }
  else cout << "WARNING! Unset Option in CutStandard, the program will ignore this cut" << endl;
  return;
}

bool 
CutStandard::PassEvtCut()
{
  if (_opt=="evt"){
    if(_c=='i'){
      int value=_ntp->_leafIntVal[_varInd][0];
      if(_inc){
	if( _min!=-666 && value<_min ) return false;
	if( _max!=666 && value>_max ) return false;
      }
      else{
	if( _min!=-666 && value<=_min ) return false;
	if( _max!=666 && value>=_max ) return false;
      }
    }
    else {
      float value=_ntp->_leafVal[_varInd][0];
      if(_inc){
	if( _min!=-666 && value<_min ) return false;
	if( _max!=666 && value>_max ) return false;
      }
      else{
	if( _min!=-666 && value<=_min ) return false;
	if( _max!=666 && value>=_max ) return false;
      }
    }
    return true;
  }
  return !_invert;
}
 
bool 
CutStandard::PassCoupleCut(int couple)
{
  if (_opt=="cou" || _opt=="trk" || _opt=="emc" || _opt=="TruSig" || _opt=="TruBlk"){
    if(_c=='i'){
      int value=_ntp->_leafIntVal[_varInd][couple];  
      if(_inc){
	if( _min!=-666 && value<_min ) return false;
	if( _max!=666 && value>_max ) return false;
      }
      else{
	if( _min!=-666 && value<=_min ) return false;
	if( _max!=666 && value>=_max ) return false;
      }
    }
    else {
      float value=_ntp->_leafVal[_varInd][couple]; 
      if(_inc){
	if( _min!=-666 && value<_min ) return false;
	if( _max!=666 && value>_max ) return false;
      }
      else{
	if( _min!=-666 && value<=_min ) return false;
	if( _max!=666 && value>=_max ) return false;
      }
    }
    return true;
  }
  return !_invert;
}
