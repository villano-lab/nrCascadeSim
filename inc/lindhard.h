#include <random>

#ifndef LINDHARD_FUNCTIONS
#define LINDHARD_FUNCTIONS

using namespace std;

/**
  generic lindhard function (**do not use**)
*/
double lindhard(double *x, double *par);
/**
  germanium Lindhard yield for energy x[0] 
*/
double lindhard_ge_k(double *x, double *par);
/**
  germanium Lindhard ionization in an energy range 
*/
double *geIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
/**
  silicon Lindhard yield for energy x[0] 
*/
double lindhard_si_k(double *x, double *par);
/**
  silicon Lindhard ionization in an energy range 
*/
double *siIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
/**
  argon Lindhard yield for energy x[0] 
*/
double lindhard_ar_k(double *x, double *par);
/**
  argon Lindhard ionization in an energy range 
*/
double *arIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
/**
  neon Lindhard yield for energy x[0] 
*/
double lindhard_ne_k(double *x, double *par);
/**
  neon Lindhard ionization in an energy range 
*/
double *neIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
#endif
