#ifndef OPERATION_HH
#define OPERATION_HH

#include "Ntuple.hh"

class Weight;

class Operation {
public:
  Operation();
  Operation(Ntuple* ntp);
  virtual ~Operation(){}

  virtual void ChangeNtpPointer(Ntuple* ntp); //Do this when you "change type", or change ntuple in general
  virtual void SetWeight(Weight* w) { _weight=w; }
  int AddNewBranchAndReturnIndex(TString branch);
  int AddNewBranchAndReturnIndex(TString branch, char *c);
  
  float AddErrToVal(float val, float StatErr, float SystErr);

protected:
  virtual void InitOperation(TString)=0;
  void CheckForNbBranches(TString leaf);

  Ntuple* _ntp;  //Warning! This is only the address of the object to modify
  Weight* _weight;  
    
  //The utility of RandomNumber is to avoid using RandGauss::setTheSeed more than necessary, as this is quite time consumming!
  //It's recommended for methods executed several times per event. RandGauss::setTheSeed  is done in StartRandLoop.
  float RandomGauss(float mean, float sigma, int ID, int couple=0);
  float RandomFlat(int modulo, int ID);
  void CloseRandLoop(int ID, int couple=0);

  //RandomNumber stuff: we don't put it private, but **don't change it outsite this class!! **
  int _currentEvt; 
  int _PilnuWeightID;
  int _ParamEvtSWTSID;
  bool _needGen[maxNbCouRand][maxID];

private:
  //RandomNumber stuff
  void SetSeed(int ID, int couple=0);
  void ResetRandNb();
  //maxID, maxNbCouRand, maxNbCallRand are global variables defined in Ntuple.hh
  //maxID stands for the number of ID integer defined below and used in the RandomNumber methods. 
  float _randNb[maxNbCallRand][maxNbCouRand][maxID]; 
  int _posForRnd[maxNbCouRand][maxID]; 
  bool _setSeed[maxNbCouRand][maxID];
  float _randNbFlat[maxNbCallRand];
  bool _needGenFlat;
  int _posFlat;

  int _trkPhiForSeed;
  int _couThLForSeed;
};

#endif
