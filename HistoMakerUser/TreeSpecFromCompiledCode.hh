#ifndef TREESPECFROMCOMPILEDCODE_HH
#define TREESPECFROMCOMPILEDCODE_HH

#include "RooTupleHistoMaker/TreeSpecInterface.hh"

class TreeSpecFromCompiledCode : public TreeSpecInterface {
public:
  TreeSpecFromCompiledCode();
  virtual ~TreeSpecFromCompiledCode(){}
  
  virtual TString GetLoopNbName( TString key ); 

private:
     
};

#endif
