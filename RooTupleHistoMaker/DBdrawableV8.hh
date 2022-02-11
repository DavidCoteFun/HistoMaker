#ifndef DBDRAWABLEV8_HH
#define DBDRAWABLEV8_HH

#include "RooTupleHistoMaker/DBdrawableInterface.hh"
class Ntuple;


class DBdrawableV8 : public DBdrawableInterface {
public:
  DBdrawableV8();
  DBdrawableV8(Ntuple*);
  virtual ~DBdrawableV8(){}
  
  virtual Drawable* GetFromDB(TString mode, TString key); 

private:
  Ntuple* _ntp;
     
};

#endif
