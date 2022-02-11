#ifndef RANDOMIZER_HH
#define RANDOMIZER_HH
 
#include "TMatrixF.h"
#include "TVectorF.h"

class Randomizer {
public:
  Randomizer();
  ~Randomizer(){} //trivial destructor

  void SetSeed( long seed );
  TVectorF randomizePars(int nPar, TMatrixF cov, TVectorF val);
};

#endif
