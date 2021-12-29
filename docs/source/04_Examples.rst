====================================
4. Examples: Putting it all together
====================================


---------------------------
Reading a Level Input File
---------------------------

This example simply reads a level input file called `inputfile.txt` of the correct format. It then
prints out the contents of that file in a format showing the properties of each cascade to
standard output. 


.. code-block:: C 

  //library commands
  #include "cascadeProd.h"
  #include "lindhard.h"
  #include "weisskopf.h"
  #include "isotope_info.h"
  
  //ROOT stuff
  #include "rootUtil.h"

  //get the file name
  string filename="inputfile.txt";

  //read the contents of a file into a cli object
  int numc;
  bool success=false;
  cli *cascadeFile = readCascadeDistributionFile(numc,filenames[i],success);

  //print the info that was read in
  for(int i=0;i<numc;i++){
      cout << "Cascade ID: " << i+1 << "/" << numc << endl;
      cout << "Fraction of this cascade: " << cascadeFile[i].frac << endl;
      cout << "Neutron separation: " << cascadeFile[i].Sn << endl;
      cout << "Mass number: " << cascadeFile[i].A << endl;
      cout << "Number of steps: " << cascadeFile[i].n << endl;
      cout << endl;
      cout << "Energy Levels (keV)\t|\ttau (fs)" << endl;
      cout << "------------------------------------------------" << endl;
      cout << setfill('0') << setw(5) << setprecision(5);
      cout << "      *****       " << "\t \t" << " ***** " << endl;
      for(int j=0;j<cascadeFile[i].n;j++){
        cout << "      "<< cascadeFile[i].Elev[j] << "       " << "\t \t" << " "<< cascadeFile[i].taus[j] << " " << endl;
   }

   return 0;
          

----------------------------
Printing Isotope Information
----------------------------

This example prints all of the information inside of the isotope database to standard output.

.. code-block:: C 

  //library commands
  #include "cascadeProd.h"
  #include "lindhard.h"
  #include "weisskopf.h"
  #include "isotope_info.h"

  //ROOT stuff
  #include "rootUtil.h"

  //print out all isotope information in database
  int main(){

      listStuff();

      return 0;

  }

----------------------------------
Fetching an Ionization Yield Model
----------------------------------

This example computes the expected ionization based on the standard Lindhard model
[Lindhard1963]_. The computation is done for a slowing germanium ion between 100 eV and 50 eV. The
result is printed to standard output.

.. code-block:: C 

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
  double *ionization = geIonizationInRange_k(E0,E1,0.159,mtrand); //k-value for Germanium (accepted)

  //print the ionization
  int main(){
      std::cout << "Ionization Energy: " << ionization[0] << " eV " << endl;
      std::cout << "Ionization Pairs: " << ionization[1] << " eV " << endl;
  return 0;
  }

--------------------------------------------------
Generating a Single Cascade Realization and Saving
--------------------------------------------------

This example reads in a cascade input file called `inputfile.txt`, realizes approximately 10,000
total cascades in the distribution governed by the input file, and then ports the outputs to a ROOT file
named `output.root`. Each cascade that is realized is printed to standard output showing how many
events were realized for each particular type of cascade. 

.. code-block:: C 

  //library commands
  #include "cascadeProd.h"
  #include "lindhard.h"
  #include "weisskopf.h"
  #include "isotope_info.h"
  #include "MersenneTwister.h"
  
  //ROOT stuff
  #include "rootUtil.h"

  //get the file name
  string filename="inputfile.txt";

  //read the contents of a file into a cli object
  int numc;
  bool success=false;
  cli *cascadeFile = readCascadeDistributionFile(numc,filenames[i],success);

  //get a root file and make 
  TFile *f = TFile::Open("output.root","recreate");
  TTree *t = new TTree("cascade","cascade");

  //calculate the cascades
  int num = 10000; 
  for(int k=0;k<numc;k++){
    int nrealize = num*cascadeFile[k].frac;
    cout << "Realizing " << nrealize << " events of cascade ID " << cascadeFile[k].cid << endl;
    cri *cascade_data;
    cascade_data = Cascade(nrealize,cascadeFile[k].cid,cascadeFile[k].Sn,cascadeFile[k].n,cascadeFile[k].Elev,cascadeFile[k].taus,cascadeFile[k].A,mtrand);
    cout << "Cascade realization " << k << " success: " << addToNRTTree(t,nrealize,cascade_data,cascadeFile[k]) << endl; 
            
    freecriarray(nrealize,cascade_data);
  }
  freecliarray(numc,cascadeFile);

  //write the ROOT file
  t->Write("",TObject::kOverwrite);
  f->Close();

  return 0;
    
