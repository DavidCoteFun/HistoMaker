#include "RooTupleHistoMaker/Ntuple.hh"
#include "RooTupleHistoMaker/Dataset.hh"
#include "HistoMakerUser/TreeSpecFromCompiledCode.hh"

//Root Stuff
#include <TROOT.h>
#include <TChain.h>
#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TFile.h"
#include "TLeaf.h"

using std::cout;
using std::endl;

Ntuple::Ntuple()
{ 
  _currentGlobalEvt=-1;
  _nGlobalLeaf=0;
  _nGlobalLeafInt=0;
  _nGlobalLeafDouble=0;
  _nRemainingGlobalLeaf=0;
  _nRemainingGlobalLeafInt=0;
  _nRemainingGlobalLeafDouble=0;
  _makeNewNtuple=false;
  _newntuple=0;
  _ConfigPatch="default"; //Changeable with SetConfigPatch for DATA, Syst_GamKill18, or any other systematic error key
  _seed=666; //Seed for experiment-by-experiment level random stuff, i.e. Branching Fractions and Efficiencies
  _q2Binning="Nominal12Bins";

  _TreeSpec= new TreeSpecFromCompiledCode();
}

Ntuple::~Ntuple()
{ if(_newntuple!=NULL) _newntuple->Delete(); }

int
Ntuple::AddNewBranchAndReturnIndex(TString branch)
{
  char c;
  return AddNewBranchAndReturnIndex(branch, &c);
}

int
Ntuple::AddNewBranchAndReturnIndex(TString branch, char *c)
{
  //cout<<"Branch: "<<branch<<endl;
  //cout<<"var[pos]="<<branch<<"; pos+=1;"<<endl; 
  TLeaf* l = _tree->GetLeaf(branch);
  TString tn = l->GetTypeName();

  if( tn=="Int_t" ) { *c='i'; return AddBranchIntAndReturnIndex(branch); }
  else if( tn=="Float_t" ) { *c='f'; return AddBranchAndReturnIndex(branch); }
  else if( tn=="Double_t" ) { *c='d'; return AddBranchDoubleAndReturnIndex(branch); }
  else cout<<"Leaves of type "<<tn<<" are not implemented yet... The program will give crazy results!"<<endl;
  return -1;
}


int
Ntuple::AddBranchAndReturnIndex(TString branch)
{
  //Check if the branch address is already set, if yes return the index of the corresponding _globalLeaf
  int ind=0;  
  bool alreadyFound=false;
  for(int glob=0; glob<_nGlobalLeaf && !alreadyFound; glob++)
    {
      if( branch==_globalLeaf[glob]) alreadyFound=true;
      else ind+=1;
    }
  //If the branch address wasn't already set, set it and update the Global Leaf bookkeeping
  if(!alreadyFound)
    {
      _tree->SetBranchAddress( branch,_leafVal[ind]);
      _globalLeaf[ind]=branch;
      _nGlobalLeaf+=1;
      
      if(_makeNewNtuple){
	TString Nb=xxxNb(branch);
	if(Nb=="nothing"){ return ind;}
	if(Nb=="-666") {
	  //Event variable
	  _newntuple->Branch(branch, &_leafVal[ind][0], branch+"/F");
	}
	else if(Nb.Contains("lnu")) {
	  //Y variable
	  _newntuple->Branch(branch, _RemainingleafVal[ind], branch+"["+Nb+"]/F");
	}
	else{
	  //Detector Variable
	  _newntuple->Branch(branch, _leafVal[ind], branch+"["+Nb+"]/F");	  
	}
      }
    
    }

  //In any case return the local index of the now global branch
  return ind;
}


int
Ntuple::AddBranchIntAndReturnIndex(TString branch)
{
  //Check if the branch address is already set, if yes return the index of the corresponding _globalLeaf
  int ind=0;  
  bool alreadyFound=false;
  for(int glob=0; glob<_nGlobalLeafInt && !alreadyFound; glob++)
    {
      if( branch==_globalLeafInt[glob]) alreadyFound=true;
      else ind+=1;
    } 
  //If the branch address wasn't already set, set it and update the Global Leaf bookkeeping
  if(!alreadyFound)
    {
      //Global bookkeeping
      _tree->SetBranchAddress( branch,_leafIntVal[ind]);
      _globalLeafInt[ind]=branch;
      _nGlobalLeafInt+=1;
      
      if(_makeNewNtuple){
	TString Nb=xxxNb(branch);
	if(Nb=="nothing"){ return ind;}
	if(Nb=="-666") {
	  //Event variable
	  _newntuple->Branch(branch, &_leafIntVal[ind][0], branch+"/I");
	}
	else if(Nb.Contains("lnu")) {
	  //Y variable
	  _newntuple->Branch(branch, _RemainingleafIntVal[ind], branch+"["+Nb+"]/I");
	}
	else{
	  //Detector Variable
	  _newntuple->Branch(branch, _leafIntVal[ind], branch+"["+Nb+"]/I");	  
	}
      }
      
    }

  //In any case return the local index of the now global "Int" branch  (only global bookkeeping for these guys)
  return ind;
}

int
Ntuple::AddBranchDoubleAndReturnIndex(TString branch)
{
  //Check if the branch address is already set, if yes return the index of the corresponding _globalLeaf
  int ind=0;  
  bool alreadyFound=false;
  for(int glob=0; glob<_nGlobalLeafDouble && !alreadyFound; glob++)
    {
      if( branch==_globalLeafDouble[glob]) alreadyFound=true;
      else ind+=1;
    }
  //If the branch address wasn't already set, set it and update the Global Leaf bookkeeping
  if(!alreadyFound)
    {
      //Global bookkeeping
      _tree->SetBranchAddress( branch,_leafDoubleVal[ind]);
      _globalLeafDouble[ind]=branch;
      //_globalBr[ind]=_tree->GetBranch(branch);
      _nGlobalLeafDouble+=1;
      if(_makeNewNtuple){
	TString Nb=xxxNb(branch);
	if(Nb=="nothing"){ return ind;}
	if(Nb=="-666") {
	  //Event variable
	  _newntuple->Branch(branch, &_leafDoubleVal[ind][0], branch+"/D");
	}
	else if(Nb.Contains("lnu")) {
	  //Y variable
	  _newntuple->Branch(branch, _RemainingleafDoubleVal[ind], branch+"["+Nb+"]/D");
	}
	else{
	  //Detector Variable
	  _newntuple->Branch(branch, _leafDoubleVal[ind], branch+"["+Nb+"]/D");	  
	}
      }
      
    }

  //In any case return the local index of the now global "Int" branch  (only global bookkeeping for these guys)
  return ind;
}


void
Ntuple::GetGlobalEvent(int e)
{
  // useless now (It's called only once per event anyway...)
  if(_currentGlobalEvt==e) return;  //This function has to be excuted only once per event 
  else _currentGlobalEvt=e; 
    
  _tree->GetEvent(e); 
  return; 

  //Note: the old way of loading events was to do
  // _tree->GetBranch(_globalLeafInt[gbl])->GetEntry(_currentGlobalEvt);
  //as is commented below, and to also do 
  //int e=_tree->LoadTree(ent); GetGlobalEvent(e);
  //everywhere in the loop methods like YHisto1D, etc.

  //The new way of getting events is to do in Ntuple::ResetNtpPointers()
  //_tree->SetBranchStatus("*",0); //disable all branches 
  //_tree->SetBranchStatus(_globalLeafInt[gbl],1);  //enable only the branches we need
  //This new method is MUCH faster, and we're now ALWAYS using it.

  //WARNING: Using int e=_tree->LoadTree(ent); GetGlobalEvent(e); with the new method
  //result in a bug! Looping over a chain of x files, each file woule be processed twice!
  //One time fast, and a second time very slow.

  //WARNING: Using the old method without int e=_tree->LoadTree(ent); GetGlobalEvent(e);
  //also result in a bug! 

  //So the old and new methods are incompatible, and the old method is abandonned forever.

  /*
  //First get all the "Int" branches entry needed in by the group of all operations
  for(int gbl=0; gbl<_nGlobalLeafInt; gbl++)
    {
      //_globalBr[gbl]->GetEntry(_currentGlobalEvt);
      _tree->GetBranch(_globalLeafInt[gbl])->GetEntry(_currentGlobalEvt);	    
    }

  //Now get all the "Float" branches entry needed in by the group of all operations
  for(int gbl=0; gbl<_nGlobalLeaf; gbl++)
    {
      _tree->GetBranch(_globalLeaf[gbl])->GetEntry(_currentGlobalEvt);	    
    }

  //Now get all the "Double" branches entry needed in by the group of all operations
  for(int gbl=0; gbl<_nGlobalLeafDouble; gbl++)
    {
      _tree->GetBranch(_globalLeafDouble[gbl])->GetEntry(_currentGlobalEvt);	    
    }
  */
 
}

void 
Ntuple::ResetNtpPointers()
{
  //see: http://root.cern.ch/root/html/TTree.html#TTree:GetEntry
  _tree->SetBranchStatus("*",0); //disable all branches 

  //re-set all the branches address...
  //First set all the "Int" branches address needed by the group of all operations
  for(int gbl=0; gbl<_nGlobalLeafInt; gbl++)
    {
      _tree->SetBranchAddress( _globalLeafInt[gbl], &_leafIntVal[gbl]);
      _tree->SetBranchStatus(_globalLeafInt[gbl],1);  //enable only the branches we need
    }

  //Now set all the "float" branches address needed by the group of all operations
  for(int gbl=0; gbl<_nGlobalLeaf; gbl++)
    {
      _tree->SetBranchAddress( _globalLeaf[gbl], &_leafVal[gbl]);
      _tree->SetBranchStatus(_globalLeaf[gbl],1); //enable only the branches we need
    }  

  //And finally the "double" branches adderess needed by the group of all operations
  for(int gbl=0; gbl<_nGlobalLeafDouble; gbl++)
    {
      _tree->SetBranchAddress( _globalLeafDouble[gbl], &_leafDoubleVal[gbl]);      
      _tree->SetBranchStatus(_globalLeafDouble[gbl],1); //enable only the branches we need
    }

  return;
}

TString
Ntuple::xxxNb(TString branch)
{
  TString xxxNb="-666";
  static int n=1;
  if(n==1) { cout<<"Warning: Don't use ntuple branch names containing _Nb unless" 
		 <<"these represent the Nb of elements in a block"<<endl; n++; }
  if(branch.Contains("_Nb") && branch.Contains("lnu")) return "nothing";
  if(branch.Contains("_Nb")) return xxxNb;

  if( branch.Contains("lnu") && branch.Contains("_") )
    {
      TString temp=branch;
      int end = temp.Length();
      int pos = temp.First("_");
      TString mode=temp.Remove(pos,end);
      // Remaining Couples (..._Nb)
      xxxNb=mode+"_Nb";
    }
  else if (branch.Contains("TruBlk_")) xxxNb="TruBlk_Nb"; 
  else if (branch.Contains("TruSig_")) xxxNb="TruSig_Nb";
  else if (branch.Contains("trk_")) xxxNb="trk_Nb"; 
  else if (branch.Contains("emc_")) xxxNb="emc_Nb";
  else if (branch.Contains("ifr_")) xxxNb="ifr_Nb";
  
  return xxxNb;
}

void
Ntuple::Test()
{
  cout<<"1 2 testing..."<<endl;
  TChain* myChain = new TChain("ntp1","myChain");
  myChain->Add("/nfs/slac/g/udem/u1/users/ProdV5/ntuples/B0B0bar-Run4-1.root");
  _tree=myChain;

  double bit[90];
  _tree->SetBranchAddress("Tru_eBadRecoed",bit);
 
  _tree->GetBranch("Tru_eBadRecoed")->GetEntry(4);


  return;
}

void 
Ntuple::SetConfigPatch(TString key){
  _ConfigPatch=key;
  return;
}

void 
Ntuple::SetSeed(int seed){
  _seed=seed;
  return;
}

float 
Ntuple::ScaleFactorToOnPeakLumi(TString type, TString mode){
  float lumiOn=_dataset->Lumi("OnPeak");
  float thisLumi=_dataset->Lumi(type);
  return ((lumiOn/thisLumi) * ScaleFactorToFullDatasetProcessing(type,mode));
}

float
Ntuple::ScaleFactorToFullDatasetProcessing(TString type, TString mode){
  int tInd=GetTypeInd(type);
  int mInd=GetModeInd(mode);
  float fracProcessed=float(_Processed[tInd][mInd])/float(_dataset->TotNbOfEvents(type));  
  return (1.0/fracProcessed);
}

int 
Ntuple::GetTypeInd(TString type){
  //Warning: not guaranteed to be equivalent to Dataset's or Cut's GetTypeInd (!!)

  for(int t=0;t<_nUsedType;t++){
    if(type==_usedType[t]) return t;
  }

  cout<<"Ntuple: Unkown type "<<type<<"... Exiting!"<<endl; exit(1);
  return -1;
}

int 
Ntuple::GetModeInd(TString mode){
  
  for(int m=0;m<_nUsedMode;m++){
    if(mode==_usedMode[m]) return m;
  }

  cout<<"Ntuple: Unkown mode "<<mode<<"... Exiting!"<<endl; exit(1);
  return -1;
}
