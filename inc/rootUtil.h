#include <stdint.h> 

#include "TTree.h"
#include "TFile.h"
#include "Rtypes.h"

#include "cascadeProd.h"

#ifndef ROOT_UTILITIES
#define ROOT_UTILITIES
bool addToNRTTree(TTree *t,int nr,cri *recoil_lists,cli cascade_spec);
#endif
