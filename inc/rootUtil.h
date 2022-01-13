#include <stdint.h> 

#include "TTree.h"
#include "TFile.h"
#include "Rtypes.h"

#include "cascadeProd.h"

#ifndef ROOT_UTILITIES
#define ROOT_UTILITIES
/**
  Add data to the output TTree (`ROOT` data class). `t` is a pointer to a `TTree` object which is
a `ROOT` internal object for storing data; `nr` is the number of recoils in a particular cascade
realization; `cri` is the full recoil object the `cri` struct. The function returns a bool that
indicates success by a `true` value.
**/
bool addToNRTTree(TTree *t,int nr,vector<cri> &recoil_lists,cli cascade_spec);
#endif
