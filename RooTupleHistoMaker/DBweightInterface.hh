#ifndef DBWEIGHTINTERFACE_HH
#define DBWEIGHTINTERFACE_HH

#include <iostream>
#include "Weight.hh"

class DBweightInterface  {
public:
  DBweightInterface(){}
  virtual ~DBweightInterface(){}

  virtual Weight* GetFromDB( TString, TString )=0; 

};

#endif
