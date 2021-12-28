#include <string>
#include <vector>
#include <random>

using namespace std;

#ifndef CASCADE_PROD 
#define CASCADE_PROD 
//define a structure for the input cascade information
//use the acronym Cascade Level Info
/**
  A structure to hold input information on each cascade 
*/
struct cli { 
    bool success;
    int n; //number of levels 
    int cid; //cascade id-- start w/ just integer related to input list, then go to true identifier
    double frac; //fraction of cascades
    double Sn; //neutron separation energy
    int A; //capturing isotope mass number
    double *Elev; //keV (level energies)
    double *taus; //fs (time since capture)
};

//define a structure for the returned cascade information
//use the acronym Cascade Recoil Info
struct cri { 
    int n; //number of levels 
    int cid; //cascade id-- start w/ just integer related to input list, then go to true identifier
    double *E; //eV (recoil energy at step)
    double *delE; //eV (energy deposit)
    int *I; //particle number (ionization)
    double *Ei; //eV (ionization energy)
    double *time; //fs (time since capture)
    double *Eg; //MeV (gamma energy)
};

typedef struct cli cli;
typedef struct cri cri;

//data functions

void freecli(cli *cascade_levels);
void freecliarray(int n,cli *cascade_levels);
void freecri(cri *cascade_data);
void freecriarray(int n,cri *cascade_data);

//******germanium*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a germanium isotope 
*/
cri *geCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
//return the Energy after the mid-stop kick. 
double geDecay(double v, double M, double Egam, mt19937 *rand);
//return the velocity at a random stopping time
double *geStop(double E, double M, double tau, mt19937 *rand);
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rgeS2(double E, double M, double t);
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vgeS2(double E, double M, double t);
double vgeS2func(double *x,double *par);

//******silicon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *siCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
//return the Energy after the mid-stop kick. 
double siDecay(double v, double M, double Egam, mt19937 *rand);
//return the velocity at a random stopping time
double *siStop(double E, double M, double tau, mt19937 *rand);
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rsiS2(double E, double M, double t);
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vsiS2(double E, double M, double t);
double vsiS2func(double *x,double *par);

//******argon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *arCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
//return the Energy after the mid-stop kick. 
double arDecay(double v, double M, double Egam, mt19937 *rand);
//return the velocity at a random stopping time
double *arStop(double E, double M, double tau, mt19937 *rand);
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rarS2(double E, double M, double t);
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double varS2(double E, double M, double t);
double varS2func(double *x,double *par);

//******neon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *neCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
//return the Energy after the mid-stop kick. 
double neDecay(double v, double M, double Egam, mt19937 *rand);
//return the velocity at a random stopping time
double *neStop(double E, double M, double tau, mt19937 *rand);
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rneS2(double E, double M, double t);
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vneS2(double E, double M, double t);
double vneS2func(double *x,double *par);

//*********generic************** WARNING right now this only chooses either Ge or Si, not fully general
cri *Cascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

//input reading functions

cli *readCascadeDistributionFile(int &n,string file,bool &success);
double interpretDbl(string in,bool &success);
double interpretSn(string in,bool &success);
double interpretWeisskopf(string in,double Egam,double A,bool &success);
double *interpretElevVector(int &n,string in,bool &success);
double *interpretTauVector(int n,string in,double A,double *Elev,bool &success); //need the Elev vector and Sn for Weisskopf
vector<string> vsplit(string in);
#endif
