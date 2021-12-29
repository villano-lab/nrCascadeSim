//library commands
#include "cascadeProd.h"
#include "lindhard.h"
#include "weisskopf.h"
#include "isotope_info.h"
#include <iostream>
#include <iomanip>
//ROOT stuff
#include "rootUtil.h"

//get the file name
string filenames="inputfile.txt";

//read the contents of a file into a cli object
int numc;
bool success=false;

int main(){
  //print the info that was read in
  for(int i=0;i<numc;i++){
      cli *cascadeFile = readCascadeDistributionFile(numc,filenames[i],success);
      std::cout << "Cascade ID: " << i+1 << "/" << numc << endl;
      std::cout << "Fraction of this cascade: " << cascadeFile[i].frac << endl;
      std::cout << "Neutron separation: " << cascadeFile[i].Sn << endl;
      std::cout << "Mass number: " << cascadeFile[i].A << endl;
      std::cout << "Number of steps: " << cascadeFile[i].n << endl;
      std::cout << endl;
      std::cout << "Energy Levels (keV)\t|\ttau (fs)" << endl;
      std::cout << "------------------------------------------------" << endl;
      std::cout << std::setfill('0') << std::setw(5) << std::setprecision(5);
      std::cout << "      *****       " << "\t \t" << " ***** " << endl;
      for(int j=0;j<cascadeFile[i].n;j++){
        std::cout << "      "<< cascadeFile[i].Elev[j] << "       " << "\t \t" << " "<< cascadeFile[i].taus[j] << " " << endl;
      }
  }

  return 0;
}