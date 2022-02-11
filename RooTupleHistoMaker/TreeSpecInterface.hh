#ifndef TREESPECINTERFACE_HH
#define TREESPECINTERFACE_HH

#include "TString.h"

class TreeSpecInterface {
public:
  TreeSpecInterface(){}
  virtual ~TreeSpecInterface(){}

  virtual TString GetLoopNbName( TString key )=0; 
};

#endif
