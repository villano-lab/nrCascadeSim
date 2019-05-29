#include "MersenneTwister.h"

#ifndef EDEP_MATH
#define EDEP_MATH
int poissonKnuth(double lambda,MTRand *rand);
int poissonAtkinson(double lambda,MTRand *rand);
int poisson(double lambda,MTRand *rand);
int poissonFano(double lambda,double F,MTRand *rand);
#endif
