#ifndef DBCUTINTERFACE_HH
#define DBCUTINTERFACE_HH

#include <iostream>
#include "MultiCut.hh"

class DBcutInterface  {
public:
  DBcutInterface(){}
  virtual ~DBcutInterface(){}

  virtual MultiCut* GetFromDB( TString, TString )=0; 

};

#endif
