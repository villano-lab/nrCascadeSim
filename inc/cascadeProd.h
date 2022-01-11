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
      neutron separation energy in **MeV** 
    */
    double Sn;
    /**
      mass number of capturing isotope  
    */
    int A;
    /**
      array of energy levels in **keV**   
    */
    vector<double> Elev;
    /**
      half-lives of energy level in **fs**  
    */
    vector<double> taus;
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
    vector<double> E;
    /**
      energy deposited in step **eV** 
    */
    vector<double> delE;
    /**
      ionization created in each step in n e/h pairs 
    */
    vector<int> I;
    /**
      electron-equivalent ionization energy **eV** 
    */
    vector<double> Ei;
    /**
      time since capture **fs**  
    */
    vector<double> time;
    /**
      gamma energy emitted in each step **MeV** 
    */
    vector<double> Eg;
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
  A function to simulate n cascade realizations for a germanium isotope. `cid` is the cascade id;
`Sn` is the neutron separation energy in **MeV**; `nlev` is the number of levels of the cascade;
`Elev` is an array of level energys of size `nlev` in **keV**; `taus` is an array of level
lifetimes of size `nlev` in **fs**; `A` is the mass number; `mtrand` is the random number
generator object--in this case `std::mt19937`. The function returns a `cri` struct. 
*/
cri *geCascade(int n,int cid, double Sn, int nlev, vector<double> Elev, vector<double> &taus, double A, mt19937 *mtrand);
/**
  germanium func. to return the Energy after the mid-stop decay. `v` is the velocity in units of
the speed of light (c); `M` is the mass of the slowing object in **GeV**, `Egam` is the energy of
the emitted gamma ray in **MeV**; and `mtrand` is the random number generator object--in this case
`std::mt19937`. The function returns the energy in **eV** after the mid-step decay kick. 
*/
double geDecay(double v, double M, double Egam, mt19937 *rand);
/**
  germanium func. to return the velocity at a random stopping time. `E` is the initial atom energy
in **eV**; `M` is the mass of the slowing object in **GeV**; `tau` is the (random) decay time of the current
intermediate nuclear level; `mtrand` is the random generator object--in this case `std::mt19937`.
The function returns the velocity at the random decay time in units of the speed of light (c).
*/
double *geStop(double E, double M, double tau, mt19937 *rand);
/**
  germanium func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the distance traveled. Function returns distance in **nm** 
*/
double rgeS2(double E, double M, double t);
/**
  germanium func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the atoms velocity. Function returns velocity in units of the speed of light (c).
*/
double vgeS2(double E, double M, double t);
/**
  germanium func. constant stopping-power stopping. `x[0]` is the time (in **fs**) at
which to return the atoms velocity; `par[0]` is the atoms initial energy in **eV**; `par[1] is the
atoms mass in **GeV**. Function returns velocity in units of the speed of light (c).
*/
double vgeS2func(double *x,double *par);

//******silicon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a silicon isotope. `cid` is the cascade id;
`Sn` is the neutron separation energy in **MeV**; `nlev` is the number of levels of the cascade;
`Elev` is an array of level energys of size `nlev` in **keV**; `taus` is an array of level
lifetimes of size `nlev` in **fs**; `A` is the mass number; `mtrand` is the random number
generator object--in this case `std::mt19937`. The function returns a `cri` struct. 
*/
cri *siCascade(int n,int cid, double Sn, int nlev, vector<double> Elev, vector<double> &taus, double A, mt19937 *mtrand);
/**
  silicon func. to return the Energy after the mid-stop decay. `v` is the velocity in units of
the speed of light (c); `M` is the mass of the slowing object in **GeV**, `Egam` is the energy of
the emitted gamma ray in **MeV**; and `mtrand` is the random number generator object--in this case
`std::mt19937`. The function returns the energy in **eV** after the mid-step decay kick. 
*/
double siDecay(double v, double M, double Egam, mt19937 *rand);
/**
  silicon func. to return the velocity at a random stopping time. `E` is the initial atom energy
in **eV**; `M` is the mass of the slowing object in **GeV**; `tau` is the (random) decay time of the current
intermediate nuclear level; `mtrand` is the random generator object--in this case `std::mt19937`.
The function returns the velocity at the random decay time in units of the speed of light (c).
*/
double *siStop(double E, double M, double tau, mt19937 *rand);
/**
  silicon func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the distance traveled. Function returns distance in **nm** 
*/
double rsiS2(double E, double M, double t);
/**
  silicon func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the atoms velocity. Function returns velocity in units of the speed of light (c).
*/
double vsiS2(double E, double M, double t);
/**
  silicon func. constant stopping-power stopping. `x[0]` is the time (in **fs**) at
which to return the atoms velocity; `par[0]` is the atoms initial energy in **eV**; `par[1] is the
atoms mass in **GeV**. Function returns velocity in units of the speed of light (c).
*/
double vsiS2func(double *x,double *par);

//******argon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a argon isotope. `cid` is the cascade id;
`Sn` is the neutron separation energy in **MeV**; `nlev` is the number of levels of the cascade;
`Elev` is an array of level energys of size `nlev` in **keV**; `taus` is an array of level
lifetimes of size `nlev` in **fs**; `A` is the mass number; `mtrand` is the random number
generator object--in this case `std::mt19937`. The function returns a `cri` struct. 
*/
cri *arCascade(int n,int cid, double Sn, int nlev, vector<double> Elev, vector<double> &taus, double A, mt19937 *mtrand);
/**
  argon func. to return the Energy after the mid-stop decay. `v` is the velocity in units of
the speed of light (c); `M` is the mass of the slowing object in **GeV**, `Egam` is the energy of
the emitted gamma ray in **MeV**; and `mtrand` is the random number generator object--in this case
`std::mt19937`. The function returns the energy in **eV** after the mid-step decay kick. 
*/
double arDecay(double v, double M, double Egam, mt19937 *rand);
/**
  argon func. to return the velocity at a random stopping time. `E` is the initial atom energy
in **eV**; `M` is the mass of the slowing object in **GeV**; `tau` is the (random) decay time of the current
intermediate nuclear level; `mtrand` is the random generator object--in this case `std::mt19937`.
The function returns the velocity at the random decay time in units of the speed of light (c).
*/
double *arStop(double E, double M, double tau, mt19937 *rand);
/**
  argon func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the distance traveled. Function returns distance in **nm** 
*/
double rarS2(double E, double M, double t);
/**
  argon func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the atoms velocity. Function returns velocity in units of the speed of light (c).
*/
double varS2(double E, double M, double t);
/**
  argon func. constant stopping-power stopping. `x[0]` is the time (in **fs**) at
which to return the atoms velocity; `par[0]` is the atoms initial energy in **eV**; `par[1] is the
atoms mass in **GeV**. Function returns velocity in units of the speed of light (c).
*/
double varS2func(double *x,double *par);

//******neon*****************
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
/**
  A function to simulate n cascade realizations for a neon isotope. `cid` is the cascade id;
`Sn` is the neutron separation energy in **MeV**; `nlev` is the number of levels of the cascade;
`Elev` is an array of level energys of size `nlev` in **keV**; `taus` is an array of level
lifetimes of size `nlev` in **fs**; `A` is the mass number; `mtrand` is the random number
generator object--in this case `std::mt19937`. The function returns a `cri` struct. 
*/
cri *neCascade(int n,int cid, double Sn, int nlev, vector<double> Elev, vector<double> &taus, double A, mt19937 *mtrand);
/**
  neon func. to return the Energy after the mid-stop decay. `v` is the velocity in units of
the speed of light (c); `M` is the mass of the slowing object in **GeV**, `Egam` is the energy of
the emitted gamma ray in **MeV**; and `mtrand` is the random number generator object--in this case
`std::mt19937`. The function returns the energy in **eV** after the mid-step decay kick. 
*/
double neDecay(double v, double M, double Egam, mt19937 *rand);
/**
  neon func. to return the velocity at a random stopping time. `E` is the initial atom energy
in **eV**; `M` is the mass of the slowing object in **GeV**; `tau` is the (random) decay time of the current
intermediate nuclear level; `mtrand` is the random generator object--in this case `std::mt19937`.
The function returns the velocity at the random decay time in units of the speed of light (c).
*/
double *neStop(double E, double M, double tau, mt19937 *rand);
/**
  neon func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the distance traveled. Function returns distance in **nm** 
*/
double rneS2(double E, double M, double t);
/**
  neon func. constant stopping-power stopping. `E` is the initial energy of the stopping
atom in **eV**; `M` is the mass of the slowing object in **GeV**; `t` is the time (in **fs**) at
which to return the atoms velocity. Function returns velocity in units of the speed of light (c).
*/
double vneS2(double E, double M, double t);
/**
  neon func. constant stopping-power stopping. `x[0]` is the time (in **fs**) at
which to return the atoms velocity; `par[0]` is the atoms initial energy in **eV**; `par[1] is the
atoms mass in **GeV**. Function returns velocity in units of the speed of light (c).
*/
double vneS2func(double *x,double *par);

//*********generic************** WARNING right now this only chooses either Ge or Si, not fully general
/**
*/
/**
  Generic function for realizing n cascades, WARNING currently only works with Si, and Ge `cid` is
the cascade id; `Sn` is the neutron separation energy in **MeV**; `nlev` is the number of levels
of the cascade; `Elev` is an array of level energys of size `nlev` in **keV**; `taus` is an array
of level lifetimes of size `nlev` in **fs**; `A` is the mass number; `mtrand` is the random number
generator object--in this case `std::mt19937`. The function returns a `cri` struct. 
*/
cri *Cascade(int n,int cid, double Sn, int nlev, vector<double> Elev, vector<double> &taus, double A, mt19937 *mtrand);

//input reading functions

/**
  function to read in the cascade file with n lines. `n` is the number of cascade rows; `file` is
the full path to the file; and `success` is a flag that indicates successful reading, `true` for a
success. 
*/
cli *readCascadeDistributionFile(int &n,string file,bool &success);
/**
  function to interpret doubles from input using regex. `in` is the input string to interpret; and
`success` is `true` if it can be interpreted as a double-precision number (either in decimal or
scientific notation). A `cli` object is returned. 
*/
double interpretDbl(string in,bool &success);
/**
  function to read Sn from input correctly using regex. `in` is the input string to interpret;
`success` will be true if it can be read as a double-precision number (representing the neutron
separation energy) OR it is a recognized isotopic symbol like `74Ge`. A double representing the
string is returned. 
*/
double interpretSn(string in,bool &success);
/**
  function to convert Weisskopf abbreviations using regex. `in` is the input string to interpret;
`success` will be set to `true` if a Weisskopf multipolarity abbreviation is recognized, like
`w(E1)` for an electric dipole. `Egam` is the energy of the emitted gamma; `A` is the mass number.
A double giving the transition lifetime in **fs** is returned. 
*/
double interpretWeisskopf(string in,double Egam,double A,bool &success);
/**
  function to read E levels from input correctly. `n` is the number of elements in the vector to
be interpreted from the input; `in` is the input string to interpret; `success` will be set to
`true` if a vector of doubles in the correct format (surrounded by brackets [] and separated by
whitespace) is detected.
*/
double *interpretElevVector(int &n,string in,bool &success);
/**
  function to read lifetimes from input correctly. `n` is the number of elements in the vector to
be interpreted from the input; `in` is the input string to interpret; `success` will be set to
`true` if a vector of doubles or symbolic Weisskopof indicators in the correct format (surrounded
by brackets and separated by whitespace) is detected. 
*/
double *interpretTauVector(int n,string in,double A,vector<double> Elev,bool &success);
/**
  function for splitting strings. `in` is the input string to interpret. A vector of strings is
returned represented the whitespace-split version of the original.  
*/
vector<string> vsplit(string in);
#endif
