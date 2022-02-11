#include "RooTupleHistoMaker/Weight.hh"
#include "CLHEP/Random/Randomize.h"

using std::cout;
using std::endl;

Weight::Weight() : Operation()
{}

Weight::Weight(Ntuple* ntp, TString mode) : Operation(ntp)
{
  _mode=mode;
  _lumiWeight=1.0; 
  _needToInitializeRandScaleFactor=true;
}

float 
Weight::GetWeight(int couple)
{
  if(_needToInitializeRandScaleFactor){ InitializeRandomScaleFactors(); } 
  //The BF are initialized once per experiment.
  //This command is put here instead of in the constructor to make sure the
  //user has previously properly set _ntp->_ConfigPatch.

  float w= _lumiWeight * AdditionalWeight(couple) * BranchingFractionWeight();
  return w;
  
}


void 
Weight::SetLumiWeight(double lumiWeight)
{
  _lumiWeight = lumiWeight;
  return;
}

void
Weight::InitializeRandomScaleFactors()
{
  //Note: these factors introduce 100% correlations between the errors of all tracks.
  //They should be used for things like systematic errors, but probably not stat errors.
  _rsLep=0.0;
  _rsPich=0.0;
  _rsPi0=0.0;
  _rsTrk=0.0;  //Would need to be consistent with trk killing in ParamEvtStuffWithTrkSel 
               //if the two syst are applied at the same time... (but they normally aren't)


  //Initialize Seed
  //WARNING: *no* randomized parameters should be set by default using _ntp->_seed!  Only with specific keys.
  //Otherwise, the default behavior will be affected in uninted ways by other systematic variations elsewhere in the code.
  RandGauss::setTheSeed( _ntp->_seed );

  //Get SystKey
  TString Key=_ntp->_ConfigPatch;
  
  _wElecContPilnu=1.65029;// +- 0.081
  _wMuContPilnu=1.01187;// +- 0.038
  if(Key=="Syst_HighEMuRatioCont"){ _wElecContPilnu=1.89; } //+3 sigmas
  else if(Key=="Syst_LowEMuRatioCont"){ _wElecContPilnu=1.41; }//-3 sigmas
  else if(Key=="Syst_EMuRatioContOfOne"||Key=="Syst_ContQ2CorrNoYieldCorr"){ 
    _wElecContPilnu=1.0; 
    _wMuContPilnu=1.0;
  }
  else if(Key=="Syst_ContYieldRand"){ 
    _wElecContPilnu=RandGauss::shoot(1.69664,0.0833); 
    _wMuContPilnu=RandGauss::shoot(1.01724,0.0380);
  }

  if(Key=="Syst_PIDlepRand"){ _rsLep=RandGauss::shoot(0,1); }
  else if(Key=="Syst_PIDlepPos"){ _rsLep=1.0; }
  else if(Key=="Syst_PIDlepNeg"){ _rsLep=-1.0; }
  else if(Key=="Syst_PIDpichRand"){ _rsPich=RandGauss::shoot(0,1); }
  else if(Key=="Syst_PIDpichPos"){ _rsPich=1.0; }
  else if(Key=="Syst_PIDpichNeg"){ _rsPich=-1.0; }
  else if(Key=="Syst_trkEffRand"){ _rsTrk=RandGauss::shoot(0,1); }
  else if(Key=="Syst_trkEffPos"){ _rsTrk=1.0; }
  else if(Key=="Syst_trkEffNeg"){ _rsTrk=-1.0; }
  else if(Key=="Syst_PIDpi0Rand"){ _rsPi0=RandGauss::shoot(0,1); }
  else if(Key=="Syst_RandDetector"){ 
    _rsPich=RandGauss::shoot(0,1); 
    _rsLep=RandGauss::shoot(0,1);
    _rsTrk=RandGauss::shoot(0,1);
    _rsPi0=RandGauss::shoot(0,1);
  }

  _needToInitializeRandScaleFactor=false;
  return;
}



















