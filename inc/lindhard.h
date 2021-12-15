// #include "MersenneTwister.h"

#ifndef LINDHARD_FUNCTIONS
#define LINDHARD_FUNCTIONS
double lindhard(double *x, double *par);
double lindhard_ge_k(double *x, double *par);
double *geIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
double lindhard_si_k(double *x, double *par);
double *siIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
double lindhard_ar_k(double *x, double *par);
double *arIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
double lindhard_ne_k(double *x, double *par);
double *neIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
#endif
