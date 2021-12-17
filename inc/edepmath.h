// #include "MersenneTwister.h"
#include <random>

#ifndef EDEP_MATH
#define EDEP_MATH

using namespace std;

int poissonKnuth(double lambda,mt19937 *rand);
int poissonAtkinson(double lambda,mt19937 *rand);
int poisson(double lambda,mt19937 *rand);
int poissonFano(double lambda,double F,mt19937 *rand);
#endif
