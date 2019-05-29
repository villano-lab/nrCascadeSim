/*==================cascadeTest.cpp==================================== 
   
      PROGRAMMER:  Anthony Villano 02/16/17 

      UPDATES: 


      PURPOSE: test the neutron capture cascade library under development
	       in this directory.  This will generate various cascades
	       from input arguments or input file and display the full
	       cascade history for several specific realizations.
                         
======================================================================*/
#include <string>
#include <time.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>

//command line options
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

//library commands
#include "cascadeProd.h"
#include "edepmath.h"
#include "lindhard.h"
#include "weisskopf.h"
#include "isotope_info.h"
#include "MersenneTwister.h"

//root
#include "TFile.h"
#include "TH1D.h"

using namespace std;

//The name of this program. 
const char* program_name;

//Prints usage information for this program to STREAM (typically
//stdout or stderr), and exit the program with EXIT_CODE.  Does not
//return. 

void print_usage (FILE* stream, int exit_code)
{
  fprintf (stream, "Usage:  %s options [ inputfile(s) ]\n", program_name);
  fprintf (stream,
	   //"\n"
           "  -n, --numgen        <number>       number of traces to generate \n"
           "  -o, --outfile       <filename>     name the output file \n"
           "  -q, --quiet         <level>        quiet printing \n"
           "                                     optional argument integer > 0 \n"
           "                                     0,default: no non-event output\n"
           "                                     1,: no standard event output\n"
           "  -s, --silent                       silent, no standard out \n"
           "  -v, --verbose       <level>        Print verbose messages at level <level>\n"
           "  -V, --version                      print version and exit\n");

  exit (exit_code);
}

#define no_argument 0
#define required_argument 1 
#define optional_argument 2

int main(int argc, char** argv) {

  //***********Begin get input*********************//
 
  //set parameters for input system
  std::string outputfile="out.root";
  uint num=1;
  uint verbosity=0;
  uint quietness=0;
  bool quiet=false;
  bool dataquiet=false;
   
  const struct option longopts[] =
  {
    {"numgen",    required_argument,  0, 'n'},
    {"outfile",   required_argument,  0, 'o'},
    {"quiet",     optional_argument,  0, 'q'},
    {"silent",    no_argument,        0, 's'},
    {"verbose",   optional_argument,  0, 'v'},
    {"version",   no_argument,        0, 'V'},
    {0,0,0,0},
  };

  int index;
  int iarg=0;

  //turn off getopt error message
  opterr=1; 

  while(iarg != -1)
  {
    iarg = getopt_long(argc, argv, "+n:o:q::sv::V", longopts, &index);

    switch (iarg)
    {

      case 'n':
        num = atoi(optarg);
        break;

      case 'o':
        outputfile = optarg;
        break;

      case 'q':
        if(optarg)
          quietness = atoi(optarg);

        if(quietness == 0)
          quiet = true;
        else if(quietness == 1)
          dataquiet = true;
        else if(quietness == 2){
          dataquiet = true;
          quiet = true;
        }
        else{
       	  std::cerr << "ERROR cascadeTest: invalid quietness value" << std::endl;
          print_usage(stderr,1);
        }
        break;

      case 's':
        quiet = true;
        dataquiet = true;
        break;

      case 'v':
        if(optarg)
          verbosity = atoi(optarg);
        else
          verbosity = 1;
          break;

      case 'V':
        printf("Version: %s\n", __GIT_VERSION);
        return 0;
        break;

      //case for long only options
      case 0:
        break;

      case '?':
        print_usage(stderr,1);
        break;
    }
  } 

  //get the rest of the filenames in a vector of strings
  if(optind==argc){
    std::cerr << "ERROR! cascadeTest: no file supplied" << std::endl;
    exit(1);
  }
  std::vector<std::string> filenames;
  for(int i = optind; i < argc; i++){
    if(verbosity>=1){
      //print the filenames on a line
      printf("%s\n",argv[i]);
    }
    filenames.push_back(argv[i]);
  }

  //***********End get input*********************//


  //print some Weisskopf estimates:
  cout << "Egam (MeV)  |  transition    | lifetime (fs) " << endl;
  cout << "----------------------------------------------" << endl;
  for(int i=0;i<10;i++){
    cout << (double)i*0.2 << "\t" << "E1" << "\t" << we((double)i*0.2,73,"E1") << endl;
    cout << (double)i*0.2 << "\t" << "M1" << "\t" << we((double)i*0.2,73,"M1") << endl;
  }

  cout << endl;
  //print some lindhard values :
  double x[1];
  double park[1];
  cout << "Recoil E(eV)  |  yield  " << endl;
  cout << "------------------------------" << endl;
  for(int i=0;i<10;i++){
    x[0] = (double)i*10+1;
    park[0]= 0.159;
    cout << (double)i*10+1 << "\t" << lindhard_si_k(x,park) << endl;
  }

  cout << endl;

  srand(time(NULL));
  int cl=rand();
  cout << "A clock tick: " << cl << endl;

  //germanium decay times :
  MTRand *mtrand = new MTRand(cl);
  cout << "velocity (c)  |  mass (GeV) | Egam (MeV) | E (eV)  " << endl;
  cout << "-------------------------------------------------------" << endl;
  for(int i=0;i<10;i++){
    double Egam=3.0;
    double vel = geStop(300.0,getMass("70Ge")/1000.0+Egam*1e-3,100.0,mtrand)[0];
    cout << vel << "\t" << getMass("70Ge")/1000.0 << "\t" << Egam << "\t" << geDecay(vel,getMass("70Ge")/1000.0,Egam,mtrand) << endl;
  }

  cout << endl;
  //test out the new compound geCascade function
  int nlev=3;
  double Elev[3] = {392,66.7,0.0};
  double taus[3] = {9.0,100.0,1e9};
  cri *cascade_data;
  //cascade_data = geCascade(num,6.783,nlev,Elev,taus,73,mtrand);
  cascade_data = siCascade(num,1,6.783,nlev,Elev,taus,29,mtrand);
  for(int i=0;i<num;i++){
    cout << endl;
    cout << "E (eV)  |  delE (eV) | I (n pairs) | Ei (eV) | time (fs) | Egam (MeV)  " << endl;
    cout << "----------------------------------------------------------------------" << endl;
    //print
    for(int j=0;j<cascade_data[i].n;j++){
      cout << cascade_data[i].E[j] << "\t" << cascade_data[i].delE[j] 
	      << "\t" << cascade_data[i].I[j] << "\t" << cascade_data[i].Ei[j] 
	      << "\t" << cascade_data[i].time[j] << "\t" << cascade_data[i].Eg[j] << endl;
    }
    cout << endl;
  }
  freecriarray(num,cascade_data);


  //let's do some tests of my parsing code
  bool success=false;
  cout << interpretDbl("1.3",success) << endl;
  cout <<"Succeeded?: " << "\t" << success << endl;
  cout << interpretSn("1.3",success) << endl;
  cout <<"Succeeded?: " << "\t" << success << endl;
  cout << interpretSn("70Ge",success) << endl;
  cout <<"Succeeded?:" << "\t" << success << endl;
  cout << interpretWeisskopf("1",1.0,73,success) << endl;
  cout << "Succeeded?:" << "\t" << success << endl;
  cout << interpretWeisskopf("w(E1)",1.0,73,success) << endl;
  cout << "Succeeded?:" << "\t" << success << endl;
  cout << interpretWeisskopf("w(E1,1.0)",1.0,73,success) << endl;
  cout << "Succeeded?:" << "\t" << success << endl;
  cout << interpretWeisskopf("w(E1,10)",1.0,73,success) << endl;
  cout << "Succeeded?:" << "\t" << success << endl;
  cout << interpretWeisskopf("w(E1,100.0)",1.0,73,success) << endl;
  cout << "Succeeded?:" << "\t" << success << endl;
  cout << interpretWeisskopf("w(E1,-100.0)",1.0,73,success) << endl;
  cout << "Succeeded?:" << "\t" << success << endl;
  int n=0;
  double *out;
  out=interpretElevVector(n,"[4.2 1.3 0.1]",success);
  for(int i=0;i<n;i++) cout << out[i] << endl;
  cout << "Succeeded?:" << "\t" << success << endl;

  double *tau;
  tau=interpretTauVector(n,"[w(M1,10)	10	10]",73,out,success);
  for(int i=0;i<n;i++) cout << tau[i] << endl;
  cout << "Succeeded?:" << "\t" << success << endl;

  free(out);
  free(tau);

  vsplit("[5.3 2 1]");

  //go through the input files
  for(int i=0;i<filenames.size();i++){
    //readCascadeDistributionFile(filenames[i]);
    int numc;
    cli *cascadeFile = readCascadeDistributionFile(numc,filenames[i],success);

    if(success){
      cout << "**************" << filenames[i] << "***************" << endl;
      cout << endl;
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
        cout << endl;
      }
      freecliarray(numc,cascadeFile);
    }
    else{
      cout << "ERROR! (readCascadeDistributionFile): one or more of the elements not read correctly " << endl;
    }


  }

  //check the poissonFano function
  double lambda = 20.0;
  double F=0.1;
  cout << "Poisson value: " << poisson(lambda,mtrand) << endl;
  cout << "Poisson Fano value: " << poissonFano(lambda,F,mtrand) << endl;

  TH1D *p = new TH1D("poisson","poisson",100,0.0,4*lambda);
  TH1D *pF = new TH1D("poissonF","poissonF",100,0.0,4*lambda);

  for(int i=0;i<10000;i++){
    p->Fill(poisson(lambda,mtrand));
    pF->Fill(poissonFano(lambda,F,mtrand));
  }

  TFile *f = new TFile("poissontest.root","recreate");
  p->Write();
  pF->Write();
  f->Close();

  //****argon
  //print some lindhard values :
  //double x[1];
  //double park[1];
  cout << "Recoil E(eV)  |  yield  " << endl;
  cout << "------------------------------" << endl;
  for(int i=0;i<10;i++){
    x[0] = (double)i*10+1;
    park[0]= 0.144;
    cout << (double)i*10+1 << "\t" << lindhard_ar_k(x,park) << endl;
  }

  cout << endl;


  //argon decay times :
  double prevtime=0;
  cout << "energy (eV)  |  mass (GeV) | time (fs) | range (nm)  " << endl;
  cout << "-------------------------------------------------------" << endl;
  for(int i=0;i<1000;i++){
    double Egam=3.0;
    double vel = arStop(300.0,getMass("41Ar")/1000.0+Egam*1e-3,100.0,mtrand)[0];
    cout << 300 << "\t" << getMass("41Ar")/1000.0 << "\t" << 1000*i << "\t" << rarS2(300,getMass("41Ar")/1000.0,1000*i) << endl;
    prevtime=rarS2(300,getMass("41Ar")/1000.0,1000*i);
    //cout << 300 << "\t" << getMass("70Ge")/1000.0 << "\t" << 10*i << "\t" << rgeS2(300,getMass("70Ge")/1000.0,10*i) << endl;
  }

  cout << endl;
  //argon decay times :
  cout << "velocity (c)  |  mass (GeV) | Egam (MeV) | E (eV)  " << endl;
  cout << "-------------------------------------------------------" << endl;
  for(int i=0;i<10;i++){
    double Egam=3.0;
    double vel = arStop(300.0,getMass("41Ar")/1000.0+Egam*1e-3,100.0,mtrand)[0];
    cout << vel << "\t" << getMass("41Ar")/1000.0 << "\t" << Egam << "\t" << arDecay(vel,getMass("41Ar")/1000.0,Egam,mtrand) << endl;
  }

  return 0;
}
