#include "RooTupleHistoMaker/Operation.hh"

#include "CLHEP/Random/Randomize.h"

using std::cout;
using std::endl;

Operation::Operation()
{}

Operation::Operation(Ntuple *ntp)
{
  _ntp=ntp;
  _weight=NULL;  //This is set to null, but it is in fact mandatory to set the weight address for the Cuts to work

  //RandomGauss stuff:
  _PilnuWeightID=0; //Set once forever, used by RandomNumber
  _ParamEvtSWTSID=1; //Set once forever, used by RandomNumber
  _currentEvt=-1;
  AddNewBranchAndReturnIndex("trk_Nb");
  _trkPhiForSeed=AddNewBranchAndReturnIndex("trk_phiLab");
  AddNewBranchAndReturnIndex("pilnu_Nb");
  _couThLForSeed=AddNewBranchAndReturnIndex("pilnu_thLAveFit");
  for(int j=0;j<maxNbCouRand;j++){
    for(int k=0;k<maxID;k++){     
      _posForRnd[j][k]=0;
      _needGen[j][k]=true;
      _setSeed[j][k]=true;
      for(int i=0;i<maxNbCallRand;i++){ _randNb[i][j][k]=666.666; }
    }
  }

  //RandomGauss stuff:
  for(int i=0;i<maxNbCallRand;i++){ _randNbFlat[i]=666.666; }
  _posFlat=0;
  _needGenFlat=true;
}

int
Operation::AddNewBranchAndReturnIndex(TString branch)
{
  return _ntp->AddNewBranchAndReturnIndex(branch);
}

int
Operation::AddNewBranchAndReturnIndex(TString branch, char *c)
{
  return _ntp->AddNewBranchAndReturnIndex(branch,c);
}

void 
Operation::ChangeNtpPointer(Ntuple* ntp)
{
  //Do this when you "change type", or change ntuple in general
  _ntp=ntp; //change the ntp address
  return;
}

void
Operation::CheckForNbBranches(TString leaf)
{
  if( leaf.Contains("lnu") && leaf.Contains("_") )
    {
      TString temp=leaf;
      int end = temp.Length();
      int pos = temp.First("_");
      TString mode=temp.Remove(pos,end);
      AddNewBranchAndReturnIndex(mode+"_Nb");
    }
  else if (leaf.Contains("TruBlk_")) AddNewBranchAndReturnIndex("TruBlk_Nb"); 
  else if (leaf.Contains("TruSig_")) AddNewBranchAndReturnIndex("TruSig_Nb");
  else if (leaf.Contains("trk_")) AddNewBranchAndReturnIndex("trk_Nb"); 
  else if (leaf.Contains("emc_")) AddNewBranchAndReturnIndex("emc_Nb");
  else if (leaf.Contains("ifr_")) AddNewBranchAndReturnIndex("ifr_Nb");
  
  return;
}

float 
Operation::AddErrToVal(float val, float StatErr, float SystErr){
  //Stat and Syst errors added in quadrature, but keeping the possible sign difference.
  float totErr=sqrt(pow(StatErr,2) + pow(SystErr,2));
  float f1=StatErr/(fabs(StatErr) + fabs(SystErr));
  float f2=SystErr/(fabs(StatErr) + fabs(SystErr));   

  float newVal=val+f1*totErr+f2*totErr;
  return newVal;
}

//The utility of RandomNumber is to avoid using RandGauss::setTheSeed more than necessary, as this is quite time consumming!
//It's recommended for methods executed several times per event. RandGauss::setTheSeed  is done in StartRandLoop.
float 
Operation::RandomGauss(float mean, float sigma, int ID, int couple){
  if(_currentEvt!=_ntp->CurrentEvt()){ 
    ResetRandNb();
    _currentEvt=_ntp->CurrentEvt();
  }

  if(_setSeed[couple][ID]){ SetSeed(ID,couple); }

  int pos=_posForRnd[couple][ID]; //get current pos
  if(_needGen[couple][ID]){ _randNb[pos][couple][ID]=RandGauss::shoot(mean,sigma); }

  _posForRnd[couple][ID]+=1; //update pos for the next call
  if(_posForRnd[couple][ID]>=maxNbCallRand){ 
    cout<<"pos>maxNbCallRand for ID="<<ID<<" and couple "<<couple<<endl;
    _posForRnd[couple][ID]=0;
  }

  return _randNb[pos][couple][ID];
}

float 
Operation::RandomFlat(int modulo, int ID){
  if(_currentEvt!=_ntp->CurrentEvt()){ 
    ResetRandNb();
    _currentEvt=_ntp->CurrentEvt();
  }

  int pos=_posFlat;
  if(_setSeed[0][ID]){ SetSeed(ID); }
  if(_needGenFlat){ _randNbFlat[pos]=rand()%modulo; }

  _posFlat+=1; //update pos for the next call

  return _randNbFlat[pos];
}


void 
Operation::SetSeed(int ID, int couple){  //Time consumming!!
  //SetSeed() manages both RandomGauss and RandomFlat

  long seed = long(_ntp->_leafVal[_trkPhiForSeed][2]*1000000.0); //event based seed
  if(ID==_PilnuWeightID){ 
    if(_ntp->_ConfigPatch!="defaultFast"){ 
      int seedCou=int(_ntp->_leafVal[_couThLForSeed][couple]*1000000.0);
      RandGauss::setTheSeed( seed+seedCou ); 
    }
  }
  else if(ID==_ParamEvtSWTSID){ 
    srand( seed+54321 );    
    RandGauss::setTheSeed( seed ); 
  }
  else{ cout<<"Unknown ID: "<<ID<<"!! Exiting..."<<endl; exit(1); }

  _setSeed[couple][ID]=false;
  return;
}

void 
Operation::CloseRandLoop(int ID, int couple){
  //CompleteRandLoop() manages both RandomGauss and RandomFlat
  _needGen[couple][ID]=false;
  _posForRnd[couple][ID]=0;
  if(ID==_ParamEvtSWTSID){
    _posFlat=0;
    _needGenFlat=false;
  }
  return;
}

void
Operation::ResetRandNb(){
  //ResetRandNb() manages both RandomGauss and RandomFlat
  int c=0;
  //while(c<maxNbCouRand && !(_needGen[c][_PilnuWeightID]&&_needGen[c+1][_PilnuWeightID])){
  while(c<maxNbCouRand){
    _needGen[c][_PilnuWeightID]=true;
    _setSeed[c][_PilnuWeightID]=true;
    _posForRnd[c][_PilnuWeightID]=0;
    c+=1;
  }

  _needGen[0][_ParamEvtSWTSID]=true;
  _setSeed[0][_ParamEvtSWTSID]=true;
  _posForRnd[0][_ParamEvtSWTSID]=0;

  _posFlat=0;
  _needGenFlat=true;

  return;
}


