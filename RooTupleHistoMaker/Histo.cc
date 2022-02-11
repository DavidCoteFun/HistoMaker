#include "RooTupleHistoMaker/Histo.hh"
#include "RooTupleHistoMaker/DBweightV1.hh"
#include "RooTupleHistoMaker/DBcutV1.hh"
#include "RooTupleHistoMaker/DBdatasetV6.hh"
#include "RooTupleHistoMaker/DBdrawableV8.hh"
#include "RooTupleHistoMaker/NoWeight.hh"

#include <time.h>

#include "TChain.h"
#include "TFile.h"
#include "TNtuple.h"

using std::cout;
using std::endl;

Histo::Histo() : Ntuple()
{ 
  GetTree( -1 );
  _DBweight = new DBweightV1(this); 
  _DBcut = new DBcutV1(this); 
  _DBdrawable = new DBdrawableV8(this); 
  _DBdataset = new DBdatasetV6(this); 
  
  SetMaxEvtNb(0);
  SetNbQ2Bins(1);
}

Histo::~Histo(){ delete _DBcut; delete _DBdrawable; delete _DBweight; delete _DBdataset;}

void
Histo::MakeHisto(TString datasetName, TString var[], TString type[], TString mode[], TString sig, TString cut, TString weightName)
{
  _dataset = _DBdataset->GetFromDB( datasetName ); 
  SetupCommon(var,type,mode,sig,cut,weightName);
  CreateHisto(cut,sig);
  FillHisto();
  _tree->Delete();
  return;
}


void 
Histo::GetTree(int t)
{ 
  TChain* myChain = new TChain("ntp1","myChain");
  if(t<0) {
    srand(time(NULL)); //Set seed to number of seconds since Jan 1st 1970
    int nb = rand()%100; //random number to avoid crashes when too many jobs try to access simultaneously the same ntuple
    nb+=1; //to avoid nb=0 ;-)
    myChain->Add("/atlas/discs/pool01/cote/BaBar/SP-4764-1.root"); //Just to load leaves
    _tree=myChain;
  }
  else {
    _tree->Delete();
    TString type = _usedType[t];
    cout<<"Lumi for "<<type<<" is "<<_dataset->Lumi(type)<<" fb-1"<<endl;
    int nFiles = _dataset->NbRootFiles(type);
    int typeInd = _dataset->GetTypeInd(type);
    for(int f=0; f<nFiles; f++) {
      myChain->Add(_dataset->_RootFilesName[typeInd][f]);
      cout<<"Histo::GetTree Added "<<_dataset->_RootFilesName[typeInd][f]<<endl;
    }
    _tree=myChain;
    ResetNtpPointers();
    ResetOperations(t);
  }
   
  return;
}

void
Histo::ResetOperations(int t)
{
  for(int m=0; m<_nUsedMode; m++)
    {
      _usedCut[m]->ChangeNtpPointer(this);
      _usedCut[m]->ChangeTypeAndReturnInd(_usedType[t]);//int tmp?
      
      _usedWeight[t][m]->ChangeNtpPointer(this);
      
      for(int v=0; v<_nUsedVar; v++)
	{
	  _usedDrawable[m][v][0]->ChangeNtpPointer(this);
	  _usedDrawable[m][v][1]->ChangeNtpPointer(this);
	}
    }
  return;
}

void 
Histo::SetupCommon(TString var[], TString type[], TString mode[], TString sig, TString cut, TString weightName)
{   
  int i=0; 
  while(type[i]!="") i++;
  _nUsedType=i; 
  for(int t=0; t<_nUsedType; t++) {
    _usedType[t]=type[t];
  }

  i=0;
  while(mode[i]!="") i++;
  _nUsedMode=i;
  for(int m=0; m<_nUsedMode; m++) {
    _usedMode[m]=mode[m];
  }

  i=0;
  while(var[i]!="") i++;
  _nUsedVar=i; 

  //Setting up Cut objects
  for(int m=0; m<_nUsedMode; m++) 
    {
      _usedCut[m] = _DBcut->GetFromDB( _usedMode[m], cut );
      _usedCut[m]->setSignalToAccept(sig);
    }

  
  //Setting up Weight objects 
  for(int t=0; t<_nUsedType; t++) { 
    for(int m=0; m<_nUsedMode; m++) { 
      _Processed[t][m]=0;
      _Filled[t][m]=0;
      _usedWeight[t][m] = _DBweight->GetFromDB( _usedMode[m], weightName );
    }
  }
  
  //Now setting up Drawable objects...
  SetupDrawable(var);
   
  return;
}


void
Histo::MakeNewNtuple(TString datasetName, TString varToFill[], TString type[], TString mode[], TString sig, TString cut, TString FileName)
{  
  //_dataset=dat;
  _dataset = _DBdataset->GetFromDB( datasetName ); 
  _makeNewNtuple=true; 
  TString weightName="NoWeight";//important that "NoWeight" is used because zero-weight events are rejected by the cuts and will be lost!

  // Setup type, mode and variable
  int i=0; 
  while(type[i]!="") i++;
  _nUsedType=i; 
  for(int t=0; t<_nUsedType; t++) {
    _usedType[t]=type[t];
  }
  i=0;
  while(mode[i]!="") i++;
  _nUsedMode=i;
  for(int m=0; m<_nUsedMode; m++) {
    _usedMode[m]=mode[m];
  }
  i=0;
  while(varToFill[i]!="") i++;
  _nUsedVar=i; 
  
  // Start Loop
  bool trouve; 
  TFile *newfile;
  cout <<endl<< "Now Looping on 1D Evt events..." << endl;
  for(int t=0; t<_nUsedType; t++) {
    int Nentries=_dataset->TotNbOfEvents(_usedType[t]);
    cout << "Number of events for "+_usedType[t]+" : " << Nentries << endl; 
    int LoopNb;
    int npassed=0;
    int npassedCouple[_nUsedMode];
    for(int m=0; m<_nUsedMode; m++) npassedCouple[m]=0;
    int FileInd=1;
    bool passed=false;
    TString name="";
    for(int ent=0; ent<LoopNb; ent++) { 
      // Create a NewTree
      if (npassed%100000==0 && !passed){
	name=Form(FileName+"-%i.root",FileInd); 
	FileInd++;
	passed=true;
	newfile = new TFile(name,"recreate");
	if (npassed==0){
	  _newntuple = new TTree("ntp1","ntp1");
	  // Create New Branch on the Tree (need to be called before GetTree)
	  for(int m=0; m<_nUsedMode; m++){
	    //Block Variable Value
	    int index = AddNewBranchAndReturnIndex(_usedMode[m]+"_Nb");
	    _newntuple->Branch(_usedMode[m]+"_Nb", &_RemainingleafIntVal[index][0], _usedMode[m]+"_Nb/I");
	    _RemainingglobalLeafInt[index]=_usedMode[m]+"_Nb";
	    _nRemainingGlobalLeafInt+=1;
	  }
	  // Create branch NB for all the other modes (Ntuple.cc dont add these branches)
	  /*	  
	  TString modeAll[11]={"pilnu","pi0lnu","eta2lnu","eta3lnu","etaplnuE2PP","etaplnuE3PP","etaplnuRG","rhoClnu","rho0lnu","omegalnu","Gammalnu"};
	  for(int j=0; j<11; j++){
	    bool modeExist=false; 
	    bool modeWanted=false; 
	    for(int m=0; m<_nUsedMode && !modeExist; m++){
	      if (modeAll[j]==_usedMode[m]) modeExist=true;
	      else for(int k=0; k<_nUsedVar && !modeWanted; k++){
		if (varToFill[k]==_usedMode[m]+"_Nb") modeWanted=true;
	      } 
	    }
	    if (!modeExist && modeWanted){
	      int index = AddNewBranchAndReturnIndex(modeAll[j]+"_Nb");
	      _newntuple->Branch(modeAll[j]+"_Nb", &_leafIntVal[index][0], modeAll[j]+"_Nb/I"); 
	    }
	  }
	  */
	   
	  if(t==0) {
	    //Setting up Cut objects
	    for(int m=0; m<_nUsedMode; m++) 
	      {
		_usedCut[m] = _DBcut->GetFromDB( _usedMode[m], cut );
		_usedCut[m]->setSignalToAccept(sig);
	      }	
	    //Setting up Weight objects 
	    for(int tt=0; tt<_nUsedType; tt++) { 
	      for(int m=0; m<_nUsedMode; m++) { 
		_Processed[tt][m]=0;
		_Filled[tt][m]=0;
		_usedWeight[tt][m] = _DBweight->GetFromDB( _usedMode[m], weightName );
	      }
	    }
	    //Now setting up Drawable objects...
	    SetupDrawable(varToFill);

	  }
	}
	// Create a clone tree for the other files
	else _newntuple = _newntuple->CloneTree(0);
      }
      if((ent+1)%5000==0) cout<<"Processing event number: "<<ent+1<<endl;

      if(ent==0){
	GetTree( t );  //note: GetTree also calls ResetOperations() and ResetNtpPointers()
	Nentries=(int)_tree->GetEntries();
	cout << "WARNING! Getting the number of events with _tree->GetEntries()..."<< endl;
	cout << "New number : " << Nentries << " entries" << endl; 
	if(_maxEvtNb==0 || _maxEvtNb>Nentries) LoopNb = Nentries;
	else LoopNb = _maxEvtNb;
	cout << "Loop over : " << LoopNb << " entries" << endl; 
      }
      GetGlobalEvent(ent); 

      trouve = false;
      for(int m=0; m<_nUsedMode; m++) {
	_usedCut[m]->SetWeight(_usedWeight[t][m]);
	if(_usedCut[m]->PassCut()){ 
	  trouve = true;	
	  //Block Variable Value
	  int index = AddNewBranchAndReturnIndex(_usedMode[m]+"_Nb");
	  _RemainingleafIntVal[index][0]=_RemainingCoupleNbOfThisCut;
	  
	  //Couple variables (float) (most couple variables are float)
	  for(int v=0; v<_nGlobalLeaf; v++) { 
	    if(_globalLeaf[v].Contains("lnu") && _globalLeaf[v].Contains("_") && !_globalLeaf[v].Contains("_Nb")){
	      int ind=AddNewBranchAndReturnIndex(_globalLeaf[v]); 
	      for(int couple=0; couple<_RemainingCoupleNbOfThisCut; couple++) {
		_RemainingleafVal[ind][couple]=_leafVal[ind][_RemainingCoupleIndOfThisCut[couple]];
	      }
	    }
	  }
	  //Couple variables (int)
	  for(int v=0; v<_nGlobalLeafInt; v++) { 
	    if(_globalLeafInt[v].Contains("lnu") && _globalLeafInt[v].Contains("_") && !_globalLeafInt[v].Contains("_Nb")){
	      int ind=AddNewBranchAndReturnIndex(_globalLeafInt[v]); 
	      for(int couple=0; couple<_RemainingCoupleNbOfThisCut; couple++) {
		_RemainingleafIntVal[ind][couple]=_leafIntVal[ind][_RemainingCoupleIndOfThisCut[couple]];
	      }
	    } 
	  }
	  //Couple variables (Double)
	  for(int v=0; v<_nGlobalLeafDouble; v++) { 
	    if(_globalLeafDouble[v].Contains("lnu") && _globalLeafDouble[v].Contains("_") && !_globalLeafDouble[v].Contains("_Nb")){
	      int ind=AddNewBranchAndReturnIndex(_globalLeafDouble[v]); 
	      for(int couple=0; couple<_RemainingCoupleNbOfThisCut; couple++) {
		_RemainingleafDoubleVal[ind][couple]=_leafDoubleVal[ind][_RemainingCoupleIndOfThisCut[couple]];
	      }
	    }
	  }
	  npassedCouple[m]+=_RemainingCoupleNbOfThisCut;
	}
      }//for(int m=0; m<_nUsedMode; m++)

      if (trouve){ 
	_newntuple->Fill();
	npassed++; 
	//npassedCouple+=_RemainingCoupleNbOfThisCut;
	passed=false;
	if (npassed%100000==0){
	  //_newntuple->Print();
	  _newntuple->AutoSave();
	  cout<<"****Writing ntuple "<<name<<" at event "<<ent<<" / "<<LoopNb<<endl; 
	  cout << npassed << " events passed cuts for the type "+_usedType[t] <<  endl;
	  for(int m=0; m<_nUsedMode; m++)
	    cout << npassedCouple[m] << " couples passed cuts for the mode "+_usedMode[m] <<  endl;
	}
      }
    }
    if (npassed%100000!=0){
      _newntuple->Print();
      _newntuple->AutoSave();
      cout<<"****Writing ntuple "<<name<<" at last event "<<LoopNb<<endl; 
      cout << npassed << " events passed cuts for the type "+_usedType[t] <<  endl; 
      for(int m=0; m<_nUsedMode; m++)
	cout << npassedCouple[m] << " couples passed cuts for the mode "+_usedMode[m] <<  endl;
    } 
  }  
  _tree->Delete();
  newfile->Close();
  newfile->Delete();

  return;
}


void
Histo::MakeNewClonedNtuple(TString datasetName, TString varToFill[], TString type[], TString mode[], TString sig, TString cut, TString FileName)
{  
  //_dataset=dat;
  _dataset = _DBdataset->GetFromDB( datasetName ); 

  // Note: weightName is useless
  //The variables to be stored have to be put in varToFill[], and SetupCommon will add them up via the DBdrawable and relevant Drawable contructors
  TString weightName="NoWeight";//important that "NoWeight" is used because zero-weight events are rejected by the cuts and will be lost!
  SetupCommon(varToFill,type,mode,sig,cut,weightName);  


  bool trouve; 
  TFile *newfile;
  TTree *newtree;
  cout <<endl<< "Now Looping on 1D Evt events..." << endl;
  for(int t=0; t<_nUsedType; t++) {
    GetTree( t );  //note: GetTree also calls ResetOperations() and ResetNtpPointers()

    //int Nentries=(int)_tree->GetEntries();
    int Nentries=_dataset->TotNbOfEvents(_usedType[t]);
    cout << "Number of events for "+_usedType[t]+" : " << Nentries << endl; 
    int LoopNb;
    int npassed=0;
    int FileInd=1;
    bool passed=false;
    if(_maxEvtNb==0 || _maxEvtNb>Nentries) LoopNb = Nentries;
    else LoopNb = _maxEvtNb;
    cout << "Loop over : " << LoopNb << " entries" << endl; 
    for(int ent=0; ent<LoopNb; ent++) { 
      if (npassed%100000==0 && !passed){
	TString name=Form(FileName+"-%i.root",FileInd);
	newfile = new TFile(name,"recreate");
	newtree = _tree->CloneTree(0);
	FileInd++;
	passed=true;
      }
      if((ent+1)%5000==0) cout<<"Processing event number: "<<ent+1<<endl;

      //if one wants ALL the branches in the reduced ntuples, one better use GetEvent, which is faster.
      //To NOT fill all the branches in the reduced ntuples, one MUST use GetGlobalEvent.
      _tree->GetEvent(ent); 
      //GetGlobalEvent(ent); 

      trouve = false; 
      for(int m=0; m<_nUsedMode && trouve==false; m++) { 
	_usedCut[m]->SetWeight(_usedWeight[t][m]);
	if(_usedCut[m]->PassCut()) trouve = true;
      }
      if (trouve){ 
	newtree->Fill();
	npassed++;
	passed=false;
	if (npassed%100000==0){
	  newtree->Print();
	  newtree->AutoSave();
	  cout << LoopNb << "  total events" << endl;
	  cout << npassed << "  events passed cuts" <<  endl;
	}
      }
    }
    if (npassed%100000!=0){
      newtree->Print();
      newtree->AutoSave();
      cout << LoopNb << "  total events" << endl;
      cout << npassed << "  events passed cuts" <<  endl;
    } 
  }  
  _tree->Delete();
  newtree->Delete();
  newfile->Close();
  newfile->Delete();
}

void 
Histo::SetNbQ2Bins(int Nb){
  if(Nb>maxNbQ2Bins || Nb<1){ cout<<"Illegal NbQ2Bins "<<Nb<<endl; assert(false); }
  _nQ2Bins=Nb; 
  return;
}
