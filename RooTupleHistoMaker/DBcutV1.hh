#ifndef DBCUTV1_HH
#define DBCUTV1_HH

#include "RooTupleHistoMaker/DBcutInterface.hh"
class Ntuple;
class Cut;

class DBcutV1 : public DBcutInterface {
public:
  DBcutV1();
  DBcutV1(Ntuple*);
  virtual ~DBcutV1(){}
  
  virtual MultiCut* GetFromDB(TString mode, TString Key); 

private:
  Ntuple* _ntp;
     
};

#endif
