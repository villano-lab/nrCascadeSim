#include "MersenneTwister.h"
#include <string>
#include <vector>

using namespace std;

#ifndef CASCADE_COMPUTE 
#define CASCADE_COMPUTE 

//******germanium*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *geCascade(int n, double Sn, int nlev, double *Elev, double *taus, double A, MTRand *mtrand);
//return the Energy after the mid-stop kick. 
double geDecay(double v, double M, double Egam, MTRand *rand);
//return the velocity at a random stopping time
double *geStop(double E, double M, double tau, MTRand *rand);
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rgeS2(double E, double M, double t);
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vgeS2(double E, double M, double t);
double vgeS2func(double *x,double *par);

#endif
