#include <random>

#ifndef EDEP_MATH
#define EDEP_MATH

using namespace std;

/**
  Generate a Poisson-distributed integer random number with the Knuth method. `lambda` is the
parameter of the Poisson distribution; `rand` is a random generator object--specifically the
`std::mt19937`. The return value is a single integer realization of a Poisson random variable. 
*/
int poissonKnuth(double lambda,mt19937 *rand);
/**
  Generate a Poisson-distributed integer random number with the Atkinson method. DANGEROUS: NEVER
USE for `lambda`<30. `lambda` is the parameter of the Poisson distribution; `rand` is a random
generator object--specifically the `std::mt19937`. The return value is a single integer
realization of a Poisson random variable. 
*/
int poissonAtkinson(double lambda,mt19937 *rand);
/**
  Generate a Poisson-distributed integer random number that chooses between the Knuth and Atkinson
method based on performance. Uses the Atkinson function when `lambda`<30 and the Knuth method
otherwise. `lambda` is the parameter of the Poisson distribution; `rand` is a random generator
object--specifically the `std::mt19937`. The return value is a single integer realization of a
Poisson random variable. 
*/
int poisson(double lambda,mt19937 *rand);
/**
  Generate a sudo-Poisson-distributed integer random number that sets the width based on the
so-called "Fano Factor" which is an empirical factor developed in the 50's for widening a sort-of
Poisson distrubtion. A Fano Factor of 1 corresponds to the standard Poisson distrubtion.  `lambda`
is the parameter of the Poisson distribution; `F` is the dimensionless Fano Factor typically a
number less than 1 (0.13 for germanium, 0.115 for silicon are _nominal_ values) for Electron
Recoils; `rand` is a random generator object--specifically the `std::mt19937`. The return value is
a single integer realization of a Poisson random variable. 
*/
int poissonFano(double lambda,double F,mt19937 *rand);
#endif
