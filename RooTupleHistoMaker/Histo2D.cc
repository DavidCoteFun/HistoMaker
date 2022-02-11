#include "RooTupleHistoMaker/Histo2D.hh"
#include "RooTupleHistoMaker/DBdrawableInterface.hh"

#include "TProfile2D.h"

Histo2D::Histo2D() : Histo()
{
  _type="TH2F";
}

Histo2D::Histo2D(TString type) : Histo()
{
  _type=type;
}

void 
Histo2D::SetupDrawable(TString var[])
{
  for(int m=0; m<_nUsedMode; m++) {
    for(int v=0; v<_nUsedVar; v++) {
      /*
      TObjArray *strL = k.Tokenize(":");
      _usedVar[v][0] = ((TObjString*)strL->At(0))->GetString();
      _usedVar[v][1] = ((TObjString*)strL->At(1))->GetString();
      delete strL;
      */
      int ind = var[v].First(':');
      int end = var[v].Length();
      TString temp = var[v];
      _usedVar[v][0] = temp.Remove(ind,end);
      temp = var[v];
      _usedVar[v][1] = temp.Remove(0,ind+1);

      _usedDrawable[m][v][0] = _DBdrawable->GetFromDB( _usedMode[m],_usedVar[v][0] );
      _usedDrawable[m][v][1] = _DBdrawable->GetFromDB( _usedMode[m],_usedVar[v][1] );
    }
  }
  return;
}

void 
Histo2D::CreateHisto(TString cut, TString sig)
{
  for(int t=0; t<_nUsedType; t++) {
    for(int m=0; m<_nUsedMode; m++) {
      for(int v=0; v<_nUsedVar; v++) {
	TString XTitle = _usedDrawable[m][v][0]->_Title;
	TString YTitle = _usedDrawable[m][v][1]->_Title;
	if(XTitle=="unset") _histos[t][m][v] = NULL;
	else if(YTitle=="unset") _histos[t][m][v] = NULL;
	else
	  {
	    TString title = _usedType[t]+"_"+_usedMode[m]+"_"+_usedDrawable[m][v][0]->_VarTitle+"_VS_"+_usedDrawable[m][v][1]->_VarTitle;
	    if(cut!="no") title+="_"+cut;
	    title+="_"+sig;
	    if(_type=="TProfile2D"){ _histos[t][m][v]=new TProfile2D(title,title,_usedDrawable[m][v][0]->_nBin,_usedDrawable[m][v][0]->_min,_usedDrawable[m][v][0]->_max,_usedDrawable[m][v][1]->_nBin,_usedDrawable[m][v][1]->_min,_usedDrawable[m][v][1]->_max); }
	    else{ _histos[t][m][v]=new TH2F(title,title,_usedDrawable[m][v][0]->_nBin,_usedDrawable[m][v][0]->_min,_usedDrawable[m][v][0]->_max,_usedDrawable[m][v][1]->_nBin,_usedDrawable[m][v][1]->_min,_usedDrawable[m][v][1]->_max); 
	    _histos[t][m][v]->Sumw2();		    
	    }
	    _histos[t][m][v]->SetXTitle(XTitle); 
	    _histos[t][m][v]->SetYTitle(YTitle); 
	  }
      }
    }
  }
}


void
Histo2D::SetAllVarWeights(int type)
{
  for(int m=0; m<_nUsedMode; m++) {	
    for(int v=0; v<_nUsedVar; v++) {
      _usedDrawable[m][v][0]->SetWeight(_usedWeight[type][m]);
      _usedDrawable[m][v][1]->SetWeight(_usedWeight[type][m]);
    }
  }
  return;
}

