#include "RooTupleHistoMaker/MultiCut.hh"

MultiCut::MultiCut() : Cut()
{}

MultiCut::MultiCut(Ntuple* ntp, TString mode) : Cut(ntp, mode)
{
  _nbCuts=0;
  _modeNbInd=AddNewBranchAndReturnIndex(mode+"_Nb");   
}

MultiCut::~MultiCut()
{
  for(int i=0; i<_nbCuts; i++) {
    delete _AllCuts[i];
  }
}


bool 
MultiCut::PassCut()
{
  //Init...
  int mode_Nb = _ntp->_leafIntVal[_modeNbInd][0];
  _ntp->_RemainingCoupleNbOfThisCut=mode_Nb;
  for(int i=0;i<mode_Nb;i++){ _ntp->_RemainingCoupleIndOfThisCut[i]=i; }

  //Cut!
  for(int i=0; i<_nbCuts; i++) {
    if( ! _AllCuts[i]->PassCut() ) return false;

    for(int i=0;i<_ntp->_RemainingCoupleNbOfNextCut;i++) {
      _ntp->_RemainingCoupleIndOfThisCut[i]=_ntp->_RemainingCoupleIndOfNextCut[i]; 
    }
    _ntp->_RemainingCoupleNbOfThisCut=_ntp->_RemainingCoupleNbOfNextCut;
  }
  return true;
}

void 
MultiCut::Add(Cut* c)
{
  _AllCuts[_nbCuts]=c;
  _nbCuts+=1;
  return;
}

void
MultiCut::GetAllCounters(TString type,float counterEvt[],float counterCouple[],TString cutNames[],TString cutNamesWithValues[],TString scaleFactorKey)
{
  float scaleFactor=1.0;
  if(scaleFactorKey=="ScaleToOnPeakLumi"){ scaleFactor=_ntp->ScaleFactorToOnPeakLumi(type,_mode); }
  else if(scaleFactorKey=="ScaleToFullDataset"){ scaleFactor=_ntp->ScaleFactorToFullDatasetProcessing(type,_mode); }

  for(int i=0; i<_nbCuts; i++) {
    counterEvt[i] = _AllCuts[i]->GetEvtCounter(type)*scaleFactor;
    counterCouple[i] = _AllCuts[i]->GetCoupleCounter(type)*scaleFactor;
    cutNames[i] = _AllCuts[i]->GetName();
    cutNamesWithValues[i] = _AllCuts[i]->GetNameWithValues();
  }
    
  return;
}

void 
MultiCut::SetWeight(Weight* w)
{
  _weight=w;
  for(int i=0; i<_nbCuts; i++) {
    _AllCuts[i]->SetWeight(w);
  }
  return;
}

void 
MultiCut::setSignalToAccept(TString key)
{
  for(int i=0; i<_nbCuts; i++) {
    _AllCuts[i]->setSignalToAccept(key);
  }
  return;
}

int 
MultiCut::ChangeTypeAndReturnInd(TString type)
{
  int tmp=0;
  for(int i=0; i<_nbCuts; i++) {
    tmp = _AllCuts[i]->ChangeTypeAndReturnInd(type);
  }
  //We possibly want to call The Cut:::ChangeTypeAndReturnInd(TString type) method 
  //if the MultiCut object also cares about counters (but I don't think it does)
  return tmp;
}

void 
MultiCut::ChangeNtpPointer(Ntuple* ntp)
{
  _ntp=ntp;
  for(int i=0; i<_nbCuts; i++) {
    _AllCuts[i]->ChangeNtpPointer(ntp);
  }
  return;
}



