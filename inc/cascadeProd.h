#include <string>
#include <vector>
#include <random>

using namespace std;

#ifndef CASCADE_PROD 
#define CASCADE_PROD 
//define a structure for the input cascade information
//use the acronym Cascade Level Info
/**
  A structure to hold input information on each cascade. the "Cascade Level Info" struct 
*/
struct cli { 
    /**
      Has the structure been set successfully and written correctly?  
    */
    bool success;
    /**
      number of levels in this cascade  
    */
    int n;
    /**
      cascade id   
    */
    int cid;
    /**
      probability of observing this particular cascade  
    */
    double frac;
    /**
      neutron separation energy  
    */
    double Sn;
    /**
      mass number of capturing isotope  
    */
    int A;
    /**
      array of energy levels in **keV**   
    */
    double *Elev;
    /**
      half-lives of energy level in **fs**  
    */
    double *taus;
};

//define a structure for the returned cascade information
//use the acronym Cascade Recoil Info
/**
  A structure to hold output information on each cascade. the "Cascade Recoil Info" struct 
*/
struct cri { 
    /**
      number of levels in the cascade that gave this recoil 
    */
    int n;
    /**
      cascade id for the cascade that gave this recoil 
    */
    int cid;
    /**
      recoil energy at beginning of recoil step **eV** 
    */
    double *E;
    /**
      energy deposited in step **eV** 
    */
    double *delE;
    /**
      ionization created in each step in n e/h pairs 
    */
    int *I;
    /**
      electron-equivalent ionization energy **eV** 
    */
    double *Ei;
    /**
      time since capture **fs**  
    */
    double *time;
    /**
      gamma energy emitted in each step **MeV** 
    */
    double *Eg;
};

typedef struct cli cli;
typedef struct cri cri;

//data functions

/**
  function to free memory in cli struct 
*/
void freecli(cli *cascade_levels);
/**
  function to free memory in array of cli structs 
*/
void freecliarray(int n,cli *cascade_levels);
/**
  function to free memory in cri struct 
*/
void freecri(cri *cascade_data);
/**
  function to free memory in array of cri structs 
*/
void freecriarray(int n,cri *cascade_data);

//******germanium*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a germanium isotope 
*/
cri *geCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
/**
  germanium func. to return the Energy after the mid-stop decay 
*/
double geDecay(double v, double M, double Egam, mt19937 *rand);
/**
  germanium func. to return the velocity at a random stopping time
*/
double *geStop(double E, double M, double tau, mt19937 *rand);
/**
  germanium func. return distance after time t for initial energy E and mass M
*/
double rgeS2(double E, double M, double t);
/**
  germanium func. constant stopping power stopping. return velocity after time t for initial energy E and mass M
*/
double vgeS2(double E, double M, double t);
/**
  germanium func. constant stopping power stopping. return velocity as function of time (x) for initial energy E and mass M (par)
*/
double vgeS2func(double *x,double *par);

//******silicon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a silicon isotope 
*/
cri *siCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
/**
  silicon func. to return the Energy after the mid-stop decay 
*/
double siDecay(double v, double M, double Egam, mt19937 *rand);
/**
  silicon func. to return the velocity at a random stopping time
*/
double *siStop(double E, double M, double tau, mt19937 *rand);
/**
  silicon func. return distance after time t for initial energy E and mass M
*/
double rsiS2(double E, double M, double t);
/**
  silicon func. constant stopping power stopping. return velocity after time t for initial energy E and mass M
*/
double vsiS2(double E, double M, double t);
/**
  silicon func. constant stopping power stopping. return velocity as function of time (x) for initial energy E and mass M (par)
*/
double vsiS2func(double *x,double *par);

//******argon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a argon isotope 
*/
cri *arCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
/**
  argon func. to return the Energy after the mid-stop decay 
*/
double arDecay(double v, double M, double Egam, mt19937 *rand);
/**
  argon func. to return the velocity at a random stopping time
*/
double *arStop(double E, double M, double tau, mt19937 *rand);
/**
  argon func. return distance after time t for initial energy E and mass M
*/
double rarS2(double E, double M, double t);
/**
  argon func. constant stopping power stopping. return velocity after time t for initial energy E and mass M
*/
double varS2(double E, double M, double t);
/**
  argon func. constant stopping power stopping. return velocity as function of time (x) for initial energy E and mass M (par)
*/
double varS2func(double *x,double *par);

//******neon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a neon isotope 
*/
cri *neCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);
/**
  neon func. to return the Energy after the mid-stop decay 
*/
double neDecay(double v, double M, double Egam, mt19937 *rand);
/**
  neon func. to return the velocity at a random stopping time
*/
double *neStop(double E, double M, double tau, mt19937 *rand);
/**
  neon func. return distance after time t for initial energy E and mass M
*/
double rneS2(double E, double M, double t);
/**
  neon func. constant stopping power stopping. return velocity after time t for initial energy E and mass M
*/
double vneS2(double E, double M, double t);
/**
  neon func. constant stopping power stopping. return velocity as function of time (x) for initial energy E and mass M (par)
*/
double vneS2func(double *x,double *par);

//*********generic************** WARNING right now this only chooses either Ge or Si, not fully general
/**
  Generic function for realizing cascades, WARNING currently only works with Si, and Ge 
*/
cri *Cascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand);

//input reading functions

/**
  function to read in the cascade file with n lines 
*/
cli *readCascadeDistributionFile(int &n,string file,bool &success);
/**
  function to interpret doubles from input using regex 
*/
double interpretDbl(string in,bool &success);
/**
  function to read Sn from input correctly using regex 
*/
double interpretSn(string in,bool &success);
/**
  function to convert Weisskopf abbreviations using regex 
*/
double interpretWeisskopf(string in,double Egam,double A,bool &success);
/**
  function to read E levels from input correctly 
*/
double *interpretElevVector(int &n,string in,bool &success);
/**
  function to read lifetimes from input correctly 
*/
double *interpretTauVector(int n,string in,double A,double *Elev,bool &success);
/**
  function for splitting strings 
*/
vector<string> vsplit(string in);
#endif
