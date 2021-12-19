#include <random>
#include <string>
#include <vector>

using namespace std;

#ifndef LESPECTRUM_PROD 
#define LESPECTRUM_PROD 

//define a structure for the returned recoil information
//use the acronym Spectral Recoil Info
struct sri { 
    bool isNR;
    double *E; //eV (recoils in time order)
    double *delE; //eV (energy deposit)
    int *I; //particle number (ionization)
    double *Ei; //eV (ionization energy)
    double *time; //fs (time since capture)
};

typedef struct sri sri;

//data functions

void freesri(sri *recoil_data);
void freesriarray(int n,sri *recoil_data);

//******germanium*****************
//throw a spectrum of electron or nuclear recoils in Ge, how specify the sampling?
sri *geRecoil(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

//******silicon*****************
//throw a spectrum of electron or nuclear recoils in Si, how specify the sampling?
sri *siRecoil(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

//******argon*****************
//throw a spectrum of electron or nuclear recoils in Ar, how specify the sampling?
sri *arRecoil(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

//******neon*****************
//throw a spectrum of electron or nuclear recoils in Ne, how specify the sampling?
sri *neRecoil(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

//*********generic************** WARNING right now this only chooses either Ge or Si, not fully general
sri *recoil(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

#endif
