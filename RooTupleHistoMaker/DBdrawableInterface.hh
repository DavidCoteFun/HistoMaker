#ifndef DBDRAWABLEINTERFACE_HH
#define DBDRAWABLEINTERFACE_HH
 
#include <iostream>
#include "TString.h"
class Drawable;

class DBdrawableInterface  {
public:
  DBdrawableInterface(){}
  virtual ~DBdrawableInterface(){}

  virtual Drawable* GetFromDB( TString, TString )=0; 

};

#endif
