#ifndef DRAWABLE_HH
#define DRAWABLE_HH

#include "Operation.hh"

class Weight;

class Drawable : public Operation {
public:
  Drawable(); //do not use!
  Drawable(Ntuple* ntp, float min, float max, int nBin, TString Title, TString VarTitle,bool includeOutOfRange=true);
  virtual ~Drawable(){}

  double GimmeValue(int couple=0);
  virtual double GetWeight(int couple=-666); 
  bool DoIncludeOutOfRange(){ return _includeOutOfRange; }

  //Mandatory parameters of the histogram
  float _min;
  float _max;
  int _nBin;
  TString _Title;  
  TString _VarTitle;
  bool _includeOutOfRange;

protected:  
  virtual double GetValue(int couple)=0; 
  virtual void InitOperation(TString)=0;

private:
  double GimmeValueAndConsiderRange(int couple);
  double GimmeValueDontConsiderRange(int couple){ return GetValue(couple); }
  double (Drawable::*_GimmeValueFunction)(int);

};

#endif
