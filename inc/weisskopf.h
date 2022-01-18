#include <string>

#ifndef WEISSKOPF_FUNCTIONS
#define WEISSKOPF_FUNCTIONS
/**
  return the Weisskopf estimated lifetime. `Egam` is the emitted gamma energy in **MeV**; `A` is
the mass number of the decaying nucleus; `transition` is a string that represents the
multipolarity of the transition, for example `E1` means electric dipole. NOTE: Multipoles beyond
`M3` are not currently supported and that case will return the slowest multipole that is
available. The return value is a double-precision number in **fs** represented the estimated
lifetime of the transition. 
*/
double we(double Egam,double A,std::string transition="E1");
#endif
