#ifndef DBDATASETV6_HH
#define DBDATASETV6_HH

#include "RooTupleHistoMaker/DBdatasetInterface.hh"
class Ntuple;

class DBdatasetV6 : public DBdatasetInterface {
public:
  DBdatasetV6();
  DBdatasetV6(Ntuple*);
  virtual ~DBdatasetV6(){}
  
  virtual Dataset* GetFromDB(TString datasetName); 

private:
  Ntuple* _ntp;
     
};

#endif
