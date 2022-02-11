#ifndef WEIGHT_HH
#define WEIGHT_HH

#include "Operation.hh"

class Weight : public Operation {
public:
  Weight(); //do not use!
  Weight(Ntuple* ntp, TString mode);
  virtual ~Weight(){}

  virtual float GetWeight(int couple=0); 
  void SetLumiWeight(double lumiWeight);

protected:  
  virtual void InitOperation(TString)=0;
  virtual float AdditionalWeight(int couple)=0;
  virtual float BranchingFractionWeight()=0;
  
  float _lumiWeight;

  //Random scale factor of weights
  bool _needToInitializeRandScaleFactor;
  float _rsLep;  //lepton ID
  float _rsPich; //pi+/- ID
  float _rsPi0;  //Pi0 ID
  float _rsTrk;  //trk eff -->Would need to be consistent with trk killing in ParamEvtStuffWithTrkSel 
                 //           if the two syst are applied at the same time... (but they normally aren't)
  float _wElecContPilnu;
  float _wMuContPilnu;

  TString _mode; 

private:
  void InitializeRandomScaleFactors();
};

#endif
