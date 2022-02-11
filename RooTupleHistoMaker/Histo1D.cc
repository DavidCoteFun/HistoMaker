#include "RooTupleHistoMaker/Histo1D.hh"
#include "RooTupleHistoMaker/DBdrawableInterface.hh"

Histo1D::Histo1D() : Histo()
{}

void 
Histo1D::SetupDrawable(TString var[])
{
  for(int m=0; m<_nUsedMode; m++) {
    for(int v=0; v<_nUsedVar; v++) {
      _usedVar[v][0]=var[v]; _usedVar[v][1]= "unset";
      _usedDrawable[m][v][0] = _DBdrawable->GetFromDB( _usedMode[m],_usedVar[v][0] ); 
      _usedDrawable[m][v][1] = _DBdrawable->GetFromDB( _usedMode[m],_usedVar[v][1] );
    }
  }
  return;
}

void 
Histo1D::CreateHisto(TString cut, TString sig)
{
  
  for(int t=0; t<_nUsedType; t++) {
    for(int m=0; m<_nUsedMode; m++) {
      for(int v=0; v<_nUsedVar; v++) {
	TString XTitle = _usedDrawable[m][v][0]->_Title;
	if(XTitle=="unset") _histos[t][m][v] = NULL;
	else
	  {
	    TString title = _usedType[t]+"_"+_usedMode[m]+"_"+_usedDrawable[m][v][0]->_VarTitle;
	    if(cut!="no") title+="_"+cut;
	    title+="_"+sig;
	    _histos[t][m][v] = new TH1F(title,title,_usedDrawable[m][v][0]->_nBin,_usedDrawable[m][v][0]->_min,_usedDrawable[m][v][0]->_max); 
	    _histos[t][m][v]->SetXTitle(XTitle); 
	    _histos[t][m][v]->Sumw2();
	  }
      }
    }
  }
  return;
}

void
Histo1D::SetAllVarWeights(int type)
{
  for(int m=0; m<_nUsedMode; m++) {	
    for(int v=0; v<_nUsedVar; v++) {
      _usedDrawable[m][v][0]->SetWeight(_usedWeight[type][m]);
    }
  }
  return;
}
