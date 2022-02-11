#include "RooTupleHistoMaker/EvtHisto1D.hh"
using std::cout;
using std::endl;

EvtHisto1D::EvtHisto1D() : Histo1D()
{}

void
EvtHisto1D::FillHisto()
{  
  cout <<endl<< "Now Looping on 1D Evt events..." << endl;
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
	  float w=_usedWeight[t][m]->GetWeight();
	  if(w>0){
	    _Filled[t][m]+=1;
	    for(int v=0; v<_nUsedVar; v++) { 
	      if (_histos[t][m][v]!=NULL) { 
		_histos[t][m][v]->Fill( _usedDrawable[m][v][0]->GimmeValue(), w );
	      }
	    }
	  }
	}
      }
    }
  }
}
