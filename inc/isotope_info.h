#include <string>

#ifndef ALL_INFO
#define ALL_INFO
/**
  get the recoil energy of the ground-state transition for isotope (default 70Ge). Return value is
energy in **eV**.
*/
double getRecoilEnergy(std::string isotope="70Ge");
/**
  get the mass for isotope (default 70Ge). Return value is in **GeV**.
*/
double getMass(std::string isotope="70Ge");
/**
  get the mass deficit for isotope (default 70Ge). Return value is in **MeV**.
*/
double getDelta(std::string isotope="70Ge");
/**
  get the neutron number for isotope (default 70Ge). Return value is an integer cast to double.
*/
double getN(std::string isotope="70Ge");
/**
  get the proton (atomic) number for isotope (default 70Ge). Return value is an integer cast to
double.
*/
double getZ(std::string isotope="70Ge");
/**
  get the neutron separation energy **MeV** for isotope (default 70Ge). Return value is in
**MeV**.
*/
double getSn(std::string isotope="70Ge");
/**
  Print all available information. 
*/
void listStuff();
#endif
