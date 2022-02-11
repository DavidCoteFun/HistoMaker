#ifndef DBDATASETINTERFACE_HH
#define DBDATASETINTERFACE_HH

#include <iostream>
#include "Dataset.hh"

class DBdatasetInterface  {
public:
  DBdatasetInterface(){}
  virtual ~DBdatasetInterface(){}

  virtual Dataset* GetFromDB( TString )=0; 

};

#endif
