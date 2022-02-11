#ifndef DBWEIGHTV1_HH
#define DBWEIGHTV1_HH

#include "RooTupleHistoMaker/DBweightInterface.hh"
class Ntuple;
class Weight;

class DBweightV1 : public DBweightInterface {
public:
  DBweightV1();
  DBweightV1(Ntuple*);
  virtual ~DBweightV1(){}
  
  virtual Weight* GetFromDB(TString mode, TString cutName); 

private:
  Ntuple* _ntp;
     
};

#endif
