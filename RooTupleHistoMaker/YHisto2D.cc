#include "RooTupleHistoMaker/YHisto2D.hh"
using std::cout;
using std::endl;

YHisto2D::YHisto2D() : Histo2D()
{}
   
YHisto2D::YHisto2D(TString type) : Histo2D(type)
{}
   
void
YHisto2D::FillHisto()
{ 
  SetModeNbLeafs();
  cout <<endl<< "Now Looping on 2D Y events..." << endl;
  for(int t=0; t<_nUsedType; t++) {
    GetTree( t );  //note: GetTree also calls ResetOperations() and ResetNtpPointers() 
    SetAllVarWeights(t);
    int Nentries=_dataset->TotNbOfEvents(_usedType[t]);
    if(Nentries<0){ Nentries=(int)_tree->GetEntries(); }
    cout << "Number of events for "+_usedType[t]+" : " << Nentries << endl; 
    bool passed=true;
    for(int ent=0; ent<Nentries && passed; ent++) {      
      passed=false;
      if((ent+1)%5000==0) cout<<"Processing event number: "<<ent+1<<endl; 
      GetGlobalEvent(ent);
      for(int m=0; m<_nUsedMode && (_Filled[t][m]<_maxEvtNb || _maxEvtNb==0); m++) {
	passed=true;
	_Processed[t][m]+=1; 
	_usedCut[m]->SetWeight(_usedWeight[t][m]);
	if(_usedCut[m]->PassCut()) {
	  for(int c=0; c<_RemainingCoupleNbOfThisCut; c++) {
	    float w=_usedWeight[t][m]->GetWeight(_RemainingCoupleIndOfThisCut[c]);
	    if(w>0){
	      _Filled[t][m]+=1;
	      for(int v=0; v<_nUsedVar; v++) {    
		if (_histos[t][m][v]!=NULL) { 
		  dynamic_cast<TH2 *>(_histos[t][m][v])->Fill( _usedDrawable[m][v][0]->GimmeValue(_RemainingCoupleIndOfThisCut[c]), _usedDrawable[m][v][1]->GimmeValue(_RemainingCoupleIndOfThisCut[c]), w );
		}
	      }
	    }
	  }
	}
      }
    } 
  }
}

void
YHisto2D::SetModeNbLeafs()
{
  for(int m=0; m<_nUsedMode; m++) {
    TString varName = _TreeSpec->GetLoopNbName(_usedMode[m]);  
    _mode_Nb[m]=AddNewBranchAndReturnIndex(varName);
  }
  return;
}
