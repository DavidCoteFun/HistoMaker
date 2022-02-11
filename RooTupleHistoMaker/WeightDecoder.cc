#include "RooTupleHistoMaker/WeightDecoder.hh"

#include "CLHEP/Random/Randomize.h"


WeightDecoder::WeightDecoder()
{ _w=1.0; _err=1000; }

WeightDecoder::WeightDecoder(float encodedWeight)
{
  //note: for w = 1.2346 +/- 0.5615, this returns encodedWeight=1235.562  
  
  int iTmp = (int)encodedWeight;
  _w = (float)iTmp/1000.0;

  float fTmp= encodedWeight*1000;  //
  iTmp = (int)fTmp;     //important to do this in two steps!
  iTmp = iTmp%1000;
  _err = (float)iTmp/1000.0;
}


float 
WeightDecoder::GaussianShuffle(float mu, float sigma)
{
  float w = RandGauss::shoot(mu,sigma);
  return w;
}

float 
WeightDecoder::FlatShuffle(float mu, float sigma)
{
  float dice=rand()%1000; //dice= 0-999 with flat probability 
  float sign=rand()%2; //sign= 0-1 with flat probability
  float zeErr=dice*sigma/1000.0;
  if(sign==0) zeErr*=-1;
  
  return mu+zeErr;
}
