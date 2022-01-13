//library commands
#include "cascadeProd.h"
#include "lindhard.h"
#include "weisskopf.h"
#include "isotope_info.h"
#include <iostream>
//ROOT stuff
#include "rootUtil.h"

//seed an MT random number with 1
std::mt19937 *mtrand = new std::mt19937(1);

//get the ionization deposited between 100 and 50 eV
// double *ionization;
double E0 = 100; //eV
double E1 = 50; //eV
vector<double> ionization = geIonizationInRange_k(E0,E1,0.159,mtrand); //k-value for Germanium (accepted)

//print the ionization
int main(){
    std::cout << "Ionization Energy: " << ionization[0] << " eV " << endl;
    std::cout << "Ionization Pairs: " << ionization[1] << " eV " << endl;
return 0;
}