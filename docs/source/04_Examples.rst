====================================
4. Examples: Putting it all together
====================================


---------------------------
Reading a Level Input File
---------------------------



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
          

----------------------------
Printing Isotope Information
----------------------------

----------------------------------
Fetching an Ionization Yield Model
----------------------------------

--------------------------------------------------
Generating a Single Cascade Realization and Saving
--------------------------------------------------
