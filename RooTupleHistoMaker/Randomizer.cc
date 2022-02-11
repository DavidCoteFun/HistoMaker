#include "RooTupleHistoMaker/Randomizer.hh"

#include "TMatrix.h"
#include "TVector.h"
#include "TRandom3.h"

Randomizer::Randomizer()
{
  gRandom->SetSeed(666);
}

void 
Randomizer::SetSeed( long seed ){
  gRandom->SetSeed(seed);
}  

TVectorF 
Randomizer::randomizePars(int nPar, TMatrixF cov, TVectorF val){ 
  //From RooFitResult V02-00-07
  //Calculate the elements of the upper-triangular matrix L that gives Lt*L = C
  //where Lt is the transpose of L (the "square-root method")

  TMatrix L(nPar,nPar);
  for(Int_t iPar= 0; iPar < nPar; iPar++) {
    // calculate the diagonal term first
    L(iPar,iPar)= cov(iPar,iPar);
    for(Int_t k= 0; k < iPar; k++) {
      Double_t tmp= L(k,iPar);
      L(iPar,iPar)-= tmp*tmp;
    }
    L(iPar,iPar)= sqrt(L(iPar,iPar));

    // then the off-diagonal terms
    for(Int_t jPar= iPar+1; jPar < nPar; jPar++) {
      L(iPar,jPar)= cov(iPar,jPar);
      for(Int_t k= 0; k < iPar; k++) {
	L(iPar,jPar)-= L(k,iPar)*L(k,jPar);
      }
      L(iPar,jPar)/= L(iPar,iPar);
    }
  }
  
  TMatrixF Lt= TMatrix(TMatrix::kTransposed,L);
  
  // create a vector of unit Gaussian variables
  TVector g(nPar);
  for(Int_t k= 0; k < nPar; k++) g(k)= gRandom->Gaus();
  // multiply this vector by Lt to introduce the appropriate correlations
  g*=Lt;

  // add the mean value offsets and store the results
  TVectorF randVal(nPar);
  for(int iPar=0;iPar<nPar;iPar++){
    randVal(iPar)=val(iPar)+g(iPar);
  }
  return randVal;
}
