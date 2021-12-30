#include <string>

#ifndef ALL_INFO
#define ALL_INFO
/**
  get the recoil energy for isotope (default 70Ge)
*/
double getRecoilEnergy(std::string isotope="70Ge");
/**
  get the mass for isotope (default 70Ge)
*/
double getMass(std::string isotope="70Ge");
/**
  get the mass deficit for isotope (default 70Ge)
*/
double getDelta(std::string isotope="70Ge");
/**
  get the neutron number for isotope (default 70Ge)
*/
double getN(std::string isotope="70Ge");
/**
  get the proton (atomic) number for isotope (default 70Ge)
*/
double getZ(std::string isotope="70Ge");
/**
  get the neutron separation energy **MeV** for isotope (default 70Ge)
*/
double getSn(std::string isotope="70Ge");
/**
  Print all available information. 
*/
void listStuff();
#endif
