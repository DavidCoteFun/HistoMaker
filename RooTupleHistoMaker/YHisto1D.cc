#include "RooTupleHistoMaker/YHisto1D.hh"
using std::cout;
using std::endl;

YHisto1D::YHisto1D() : Histo1D()
{}

void
YHisto1D::FillHisto() 
{
  SetModeNbLeafs();
  cout <<endl<< "Now Looping on 1D Y events..." << endl;

  //To debug...
  //int lowID = AddNewBranchAndReturnIndex("lowerID");
  //int upID = AddNewBranchAndReturnIndex("upperID");
  //int tl = AddNewBranchAndReturnIndex("pilnu_thLAveFit");
  //cout<<setprecision(10);

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
	    float w=_usedWeight[t][m]->GetWeight(_RemainingCoupleIndOfThisCut[c]); //Once per Event/couple for all variables
	    if(w>0){
	      _Filled[t][m]+=1; 

	      //To debug...
	      //int cou=_RemainingCoupleIndOfThisCut[c];
	      //cout<<_leafVal[upID][0]<<" "<<_leafVal[lowID][0]<<" "<<cos(_leafVal[tl][cou])<<" "<<w<<" "<<cou<<endl;
	      for(int v=0; v<_nUsedVar; v++) {    
		if (_histos[t][m][v]!=NULL) { 
		  _histos[t][m][v]->Fill( _usedDrawable[m][v][0]->GimmeValue(_RemainingCoupleIndOfThisCut[c]), w );
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
YHisto1D::SetModeNbLeafs()
{
  for(int m=0; m<_nUsedMode; m++) {
    TString varName = _TreeSpec->GetLoopNbName(_usedMode[m]);  
    _mode_Nb[m]=AddNewBranchAndReturnIndex(varName);
  }
  return;
}
