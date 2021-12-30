#include <stdint.h> 

#include "TTree.h"
#include "TFile.h"
#include "Rtypes.h"

#include "cascadeProd.h"

#ifndef ROOT_UTILITIES
#define ROOT_UTILITIES
/**
  Add data to the output TTree (`ROOT` data class)
**/
bool addToNRTTree(TTree *t,int nr,cri *recoil_lists,cli cascade_spec);
#endif
