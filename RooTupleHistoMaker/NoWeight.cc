#include "RooTupleHistoMaker/NoWeight.hh"

NoWeight::NoWeight() : Weight()
{}

NoWeight::NoWeight(Ntuple* ntp,TString mode) : Weight(ntp,mode)
{ 
  InitOperation("do nothing");
}


void 
NoWeight::InitOperation(TString leaf)
{
}

float
NoWeight::AdditionalWeight(int couple)
{
  return 1.0;
} 

float 
NoWeight::BranchingFractionWeight()
{
  return 1.0;
}

