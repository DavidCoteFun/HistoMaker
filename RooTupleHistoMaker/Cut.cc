#include "RooTupleHistoMaker/Cut.hh"
#include "RooTupleHistoMaker/Weight.hh"

using std::cout;
using std::endl;

Cut::Cut() : Operation()
{}

Cut::Cut(Ntuple* ntp, TString mode, bool invert) : Operation(ntp)
{
  _invert=invert;  
  _mode=mode;
  _nTypeUsed=0;
  _lastGoodEvt=-1;
  for(int t=0; t<maxNbType; t++){
    _counterCou[t]=0.0;
    _counterEvt[t]=0.0;
  }
}

bool 
Cut::PassCut()
{
  if( _invert==PassEvtCut() ) return false;

  //The logic is: at least one couple has to pass the couple cut in the event
  float oldCount = _counterCou[_currentType];
  float nb=0.0;
  for(int c=0; c<_ntp->_RemainingCoupleNbOfThisCut; c++) { 
    if( _invert!=PassCoupleCut(_ntp->_RemainingCoupleIndOfThisCut[c]) ) {

      _counterCou[_currentType] += _weight->GetWeight(_ntp->_RemainingCoupleIndOfThisCut[c]);
      _ntp->_RemainingCoupleIndOfNextCut[int(nb)]=_ntp->_RemainingCoupleIndOfThisCut[c];
      nb+=1.0;
    }
  }
  _ntp->_RemainingCoupleNbOfNextCut=int(nb);
  bool OK=false;
  if(nb>0) {
    OK=true;
    //For the event weight, we add the average weight in case several couples passed the cut
    _counterEvt[_currentType] += (_counterCou[_currentType]-oldCount)/nb; //average weight (more accurate?)
    //_counterEvt[_currentType] += _weight->GetWeight(); //similar to what's done in EvtHisto (approximate but simpler)
  }
  return OK;
  
  cout<<"Nothing should happend here... Bug?!?"<<endl;
  return false;
}

//float
//Cut::MeanEvtWeightWithCut(int mode_Nb, int evt)
//{
//  float totCount = 0.0;
//  float nb=0;
//  for(int c=0; c<mode_Nb; c++) { 
//    if(PassCoupleCut(c)) {
//      totCount += _weight->GetWeight(evt,c);
//      nb+=1;
//    }
//  }
//  
//  return totCount/nb;
//}


void 
Cut::setSignalToAccept(TString key)
{
  _sigKey=key;
  if(_sigKey=="all" || _sigKey=="sigCou" || _sigKey=="bkgCou" || _sigKey=="sigEvt" 
     || _sigKey=="bkgEvt" || _sigKey=="TrueXu" || _sigKey=="FakeXu" || _sigKey=="TrueLep" || _sigKey=="FakeLep") 
    {
      AddNewBranchAndReturnIndex(_mode+"_Nb"); 
      if(_sigKey=="sigEvt" || _sigKey=="bkgEvt") {
	_typeB1Ind =  AddNewBranchAndReturnIndex("Tru_typeB1");
	_typeB2Ind =  AddNewBranchAndReturnIndex("Tru_typeB2");
      }
      else if(_sigKey!="all") _TruSigInd = AddNewBranchAndReturnIndex(_mode+"_SigMC");

      //Also set the pointer to the appropriate function
      if(_sigKey=="all"){ _sigOKCou=&Cut::AlwaysTrue; _sigOKEvt=&Cut::AlwaysTrue; }
      else if(_sigKey=="bkgCou"){ _sigOKCou=&Cut::OnlyBkgCouples; _sigOKEvt=&Cut::AlwaysTrue; }
      else if(_sigKey=="sigCou"){ _sigOKCou=&Cut::OnlySigCouples; _sigOKEvt=&Cut::AlwaysTrue; }
      else if(_sigKey=="bkgEvt"){ _sigOKCou=&Cut::AlwaysTrue; _sigOKEvt=&Cut::OnlyBkgEvts; }
      else if(_sigKey=="sigEvt"){ _sigOKCou=&Cut::AlwaysTrue; _sigOKEvt=&Cut::OnlySigEvts; }
      else if(_sigKey=="TrueXu"){ _sigOKCou=&Cut::OnlyTrueXu; _sigOKEvt=&Cut::AlwaysTrue; }
      else if(_sigKey=="FakeXu"){ _sigOKCou=&Cut::OnlyFakeXu; _sigOKEvt=&Cut::AlwaysTrue; }
      else if(_sigKey=="TrueLep"){ _sigOKCou=&Cut::OnlyTrueLep; _sigOKEvt=&Cut::AlwaysTrue; }
      else if(_sigKey=="FakeLep"){ _sigOKCou=&Cut::OnlyFakeLep; _sigOKEvt=&Cut::AlwaysTrue; }
    }
  else cout<<"Unknown signal key: "<<_sigKey<<" in Cut::setSignalToAccept!!"<<endl;
  
  return;
}


bool 
Cut::PassSigCoupleCondition(int couple)
{
  //pointer to the appropriate function... (and answer!)
  bool ok=(this->*_sigOKCou)(couple);
  return ok;

}

bool 
Cut::PassSigEventCondition(int Key)
{
  //pointer to the appropriate function... (and answer!)
  bool ok=(this->*_sigOKEvt)(Key);
  return ok;

}

bool 
Cut::OnlySigCouples(int couple)
{
  float TruSig = _ntp->_leafVal[_TruSigInd][couple];
  if(TruSig>0) return true;
  else return false;
}

bool 
Cut::OnlySigEvts(int key)
{
  //For pi0lnu, sig events would have key=2,22,-2 or -22
  //The keys are pi/pi0/eta/etap/rhoC/rho0/omega = 1/2/3/4/5/6/7
  int typeB1 = abs(_ntp->_leafIntVal[_typeB1Ind][0]);
  if(typeB1==key) return true;

  int keyMuons = 10*key+key;
  if(typeB1==keyMuons) return true;
  
  int typeB2 = abs(_ntp->_leafIntVal[_typeB2Ind][0]);
  if(typeB2==key) return true;
  if(typeB2==keyMuons) return true;

  //else...
  return false;
}


bool 
Cut::OnlyTrueXu(int couple)
{
  //From BetaMiniUser/XSLReader::IsSignal
  //  -xxxxxxxx1x good Xu

  if(OnlySigCouples(couple)) return true;

  double TruSig = fabs(_ntp->_leafVal[_TruSigInd][couple]); //positive value!
  unsigned long int iSig=(unsigned long int)TruSig; 
  int ans = iSig%100 - iSig%10;
  ans/=10;

  if(ans==1) return true;
  else return false;
}

bool 
Cut::OnlyTrueLep(int couple)
{
  //From BetaMiniUser/XSLReader::IsSignal
  //  -xxxxxxxxx1 good electron, combanitorial background
  //  -xxxxxxxxx2 bad electron, combanitorial background
  //  -xxxxxxxxx3 good muon, combanitorial background
  //  -xxxxxxxxx4 bad muon, combanitorial background
  //  -xxxxxxxxx6 good electron,regular background
  //  -xxxxxxxxx7 bad electron, regular background
  //  -xxxxxxxxx8 good muon, regular background
  //  -xxxxxxxxx9 bad muon, regular background


  if(OnlySigCouples(couple)) return true;

  double TruSig = fabs(_ntp->_leafVal[_TruSigInd][couple]); //positive value!
  unsigned long int iSig=(unsigned long int)TruSig; 
  int ans = iSig%10;

  if(ans==1 || ans==3 || ans==6 || ans==8) return true;
  else return false;
}

int
Cut::ChangeTypeAndReturnInd(TString type)
{
  bool found=false;
  for(int t=0; t<_nTypeUsed && !found; t++) {
    if(type==_types[t]) { _currentType=t; found=true; }
  }
  
  if(!found) {
    _currentType=_nTypeUsed;
    _types[_currentType]=type;
    _nTypeUsed+=1;
  }

  return _currentType;
}

int
Cut::GetTypeInd(TString type)
{
  //Use this function for already existing types
  bool found=false;
  int ind=0;
  for(int t=0; t<_nTypeUsed && !found; t++) {
    if(type==_types[t]) { ind=t; found=true; }
  }
  if(!found) { cout<<"Cut: Request for unused type "<<type <<"... returning -1!"<<endl; return -1; }
  return ind;
}

float 
Cut::GetEvtCounter(TString type)
{
  int t = GetTypeInd(type);
  return GetEvtCounter(t);
}

float 
Cut::GetEvtCounter(int t)
{
  return _counterEvt[t];
}

float 
Cut::GetCoupleCounter(TString type)
{
  int t = GetTypeInd(type);
  return GetCoupleCounter(t);
}

float 
Cut::GetCoupleCounter(int t)
{
  return _counterCou[t];
}

