#ifndef MULTICUT_HH
#define MULTICUT_HH

#include "Cut.hh"

class MultiCut : public Cut {
public:
  MultiCut();
  MultiCut(Ntuple* ntp, TString mode);
  virtual ~MultiCut();

  void GetAllCounters(TString type,float counterEvt[],float counterCouple[],TString cutNames[],TString cutNamesWithValues[],
		      TString scaleFactorKey="None");
  int GetNbCuts() { return _nbCuts; }

  virtual void SetWeight(Weight* w);
  virtual void setSignalToAccept(TString key);
  virtual int ChangeTypeAndReturnInd(TString type);
  virtual void ChangeNtpPointer(Ntuple* ntp); //Do this when you "change type", or change ntuple in general

  virtual bool PassCut();
  virtual bool PassEvtCut(){return true;}              // EvtCut and CoupleCut have no meaning for a multicut...
  virtual bool PassCoupleCut(int couple){return true;} //

protected:
  virtual void InitOperation(TString mode){return;}
  virtual void AddAllCuts(Ntuple* ntp, TString mode, TString CutName)=0;
  void Add(Cut* c);

  int _nbCuts;
  int _modeNbInd;
  Cut* _AllCuts[maxNbCut]; //Note: maxNbCut is a global variable defined in Ntuple.hh
};

#endif
