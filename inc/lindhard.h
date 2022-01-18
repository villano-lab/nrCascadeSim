#include <random>

#ifndef LINDHARD_FUNCTIONS
#define LINDHARD_FUNCTIONS

using namespace std;

/**
  generic lindhard function (**do not use**). The input `x` is really an array with a single value
`x[0]` which is the input recoil energy in **eV**. The array `par` needs exactly 6 parameters to
construct the standard Lindhard ionization yield model. These are `par[0]` the pre-factor to the
first energy term in the Lindhard model (unitless); `par[1]` the exponent of the first energy
term; `par[2]` the pre-factor of thee second energy term; `par[3]` the exponent of the second
energy term; `par[4]` the atomic number Z; `par[5]` the mass number A. The return value is the
ionization yield (the ratio between the deposited ionization energy and the deposited total
energy, < 1.0). 
*/
double lindhard(vector<double> &x, vector<double> &par);
/**
  germanium Lindhard function for energy `x[0]`. The input `x` is really an array with a single value
`x[0]` which is the input recoil energy in **eV**. The array `par` needs exactly 6 parameters to
construct the standard Lindhard ionization yield model. These are `par[0]` the pre-factor to the
first energy term in the Lindhard model (unitless); `par[1]` the exponent of the first energy
term; `par[2]` the pre-factor of thee second energy term; `par[3]` the exponent of the second
energy term; `par[4]` the atomic number Z; `par[5]` the mass number A. The return value is the
ionization yield (the ratio between the deposited ionization energy and the deposited total
energy, < 1.0). 
*/
double lindhard_ge_k(vector<double> &x, vector<double> &par);
/**
  germanium Lindhard ionization in an energy range. `E0` is the initial energy of the atom in
**eV**; `E1` is the final energy of the recoiling atom in **eV**; `rand` is a random number
generator object--specifically `std::mt19937`. 
*/
vector<double> geIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
/**
  silicon Lindhard function for energy `x[0]`. The input `x` is really an array with a single value
`x[0]` which is the input recoil energy in **eV**. The array `par` needs exactly 6 parameters to
construct the standard Lindhard ionization yield model. These are `par[0]` the pre-factor to the
first energy term in the Lindhard model (unitless); `par[1]` the exponent of the first energy
term; `par[2]` the pre-factor of thee second energy term; `par[3]` the exponent of the second
energy term; `par[4]` the atomic number Z; `par[5]` the mass number A. The return value is the
ionization yield (the ratio between the deposited ionization energy and the deposited total
energy, < 1.0). 
*/
double lindhard_si_k(vector<double> &x, vector<double> &par);
/**
  silicon Lindhard ionization in an energy range. `E0` is the initial energy of the atom in
**eV**; `E1` is the final energy of the recoiling atom in **eV**; `rand` is a random number
generator object--specifically `std::mt19937`. 
*/
vector<double> siIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
/**
  argon Lindhard function for energy `x[0]`. The input `x` is really an array with a single value
`x[0]` which is the input recoil energy in **eV**. The array `par` needs exactly 6 parameters to
construct the standard Lindhard ionization yield model. These are `par[0]` the pre-factor to the
first energy term in the Lindhard model (unitless); `par[1]` the exponent of the first energy
term; `par[2]` the pre-factor of thee second energy term; `par[3]` the exponent of the second
energy term; `par[4]` the atomic number Z; `par[5]` the mass number A. The return value is the
ionization yield (the ratio between the deposited ionization energy and the deposited total
energy, < 1.0). 
*/
double lindhard_ar_k(vector<double> &x, vector<double> &par);
/**
  argon Lindhard ionization in an energy range. `E0` is the initial energy of the atom in
**eV**; `E1` is the final energy of the recoiling atom in **eV**; `rand` is a random number
generator object--specifically `std::mt19937`. 
*/
vector<double> arIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
/**
  neon Lindhard function for energy `x[0]`. The input `x` is really an array with a single value
`x[0]` which is the input recoil energy in **eV**. The array `par` needs exactly 6 parameters to
construct the standard Lindhard ionization yield model. These are `par[0]` the pre-factor to the
first energy term in the Lindhard model (unitless); `par[1]` the exponent of the first energy
term; `par[2]` the pre-factor of thee second energy term; `par[3]` the exponent of the second
energy term; `par[4]` the atomic number Z; `par[5]` the mass number A. The return value is the
ionization yield (the ratio between the deposited ionization energy and the deposited total
energy, < 1.0). 
*/
double lindhard_ne_k(vector<double> &x, vector<double> &par);
/**
  neon Lindhard ionization in an energy range. `E0` is the initial energy of the atom in
**eV**; `E1` is the final energy of the recoiling atom in **eV**; `rand` is a random number
generator object--specifically `std::mt19937`. 
*/
vector<double> neIonizationInRange_k(double E0,double E1,double k,mt19937 *rand);
#endif
