#ifndef WEIGHTDECODER_HH
#define WEIGHTDECODER_HH


class WeightDecoder  {
public:
  WeightDecoder();
  WeightDecoder(float encodedWeight);
  virtual ~WeightDecoder(){}

  float weight(){ return _w; }
  float error(){return _err; }
  float GaussianShuffle(float mu, float sigma);
  float FlatShuffle(float mu, float sigma);
  
  float _w;
  float _err;

};

#endif
