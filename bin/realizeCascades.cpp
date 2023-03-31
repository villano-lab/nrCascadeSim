/*==================realizeCascades.cpp==================================== 
   
      PROGRAMMER:  Anthony Villano 03/08/17 

      UPDATES: 


      PURPOSE: Generate ROOT trees for realizations of neutron capture 
               cascades specified in input files.
                         
======================================================================*/
#include <string>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <random>
#include <map>
#include <vector>
#include <algorithm>

//command line options
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

//library commands
#include "cascadeProd.h"
#include "lindhard.h"
#include "weisskopf.h"
#include "isotope_info.h"

//ROOT stuff
#include "rootUtil.h"

using namespace std;

//The name of this program. 
const char* program_name;

//Prints usage information for this program to STREAM (typically
//cout or stderr), and exit the program with EXIT_CODE.  Does not
//return. 

void print_usage (FILE* stream, int exit_code)
{
  fprintf (stream, "Usage:  %s options [ inputfile(s) ]\n", program_name);
  fprintf (stream,
	   //"\n"
           "  -d, --seed          <integer>      Seed for random numbers \n"
           "  -h, --help                         Print usage \n"
           "  -n, --numgen        <number>       Number of traces to generate \n"
           "  -o, --outfile       <filename>     Name the output file \n"
           "  -s, --silent                       Silent, no standard out \n"
           "  -v, --verbose       <level>        Print verbose messages at level <level>. \n"
           "                                     Currently must use `--verbose=<level>` or `-v<level>` - no spaces.\n"
           "  -V, --version                      Print version and exit\n"
           "  -l, --log           <filename>     Log additional output to the specified file.\n"
           "                                     If this option is not used, no logging will occur.\n"
           );//"  -S, --stopping      <number>       Stopping power (default: 0.1)\n");

        printf("Report bugs to: villaa-at-gmail-dot-com \n");
        printf("realizeCascades (nrCascadeSim) home page: <https://github.com/villano-lab/nrCascadeSim> \n");
        printf("General help nrCascadeSim Software: <https://nrcascadesim.readthedocs.io/en/latest/> \n");
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
  bool seedquiet=false;
   
  const struct option longopts[] =
  {
    {"log",       required_argument,  0, 'l'},
    {"numgen",    required_argument,  0, 'n'},
    {"outfile",   required_argument,  0, 'o'},
    {"silent",    no_argument,        0, 's'},
    {"help",    no_argument,        0, 'h'},
    {"seed",      required_argument,  0, 'd'},
    {"verbose",   optional_argument,  0, 'v'},
    {"version",   no_argument,        0, 'V'},
    {"stopping",  required_argument,  0, 'S'},
    {0,0,0,0},
  };

  unsigned cl = std::chrono::system_clock::now().time_since_epoch().count();
  int index;
  int iarg=0;
  string logfile;

  opterr=1; //turn off getopt error message
  double s2 = 0.1; //set default stopping power

  double s2 = 0.1;

  while(iarg != -1)
  {
    iarg = getopt_long(argc, argv, "+l:n:o:shd:S:v::V", longopts, &index);

    switch (iarg)
    {
      case 'l':
        logfile = optarg;
        break;

      case 'n':
        num = atoi(optarg);
        break;

      case 'o':
        outputfile = optarg;
        break;

      case 's':
        seedquiet = true;
        break;

      case 'd':
        cl = atoi(optarg);
        if (seedquiet == false){
          cout << "Seed provided: " << cl << endl;
        }
        break;

      case 'v':
        if(optarg)
          verbosity = atoi(optarg);
        else
          verbosity = 1;
          break;

      case 'V':
        printf("realizeCascades (nrCascadeSim) %s\n", __GIT_VERSION);
        printf("Copyright (c) 2020 Anthony Villano, Kitty Harris, Staci Brown \n");
        printf("License The Expat license  <https://spdx.org/licenses/MIT.html> \n");
        printf("This is free software: you are free to change and redistribute it. \n");
        printf("There is NO WARRANTY, to the extent permitted by law. \n");
        return 0;
        break;

      case 'h':
        print_usage(stdout,0);
        return 0;
        break;

      //case for long only options
      case 0:
        break;

      case '?':
        print_usage(stderr,1);
        break;

      case 'S':
        s2 = atof(optarg);
        break;
    }
  } 

  //get the rest of the filenames in a vector of strings
  if(optind==argc){
    std::cerr << "ERROR! realizeCascades: no file supplied" << std::endl;
    exit(1);
  }
  std::vector<std::string> filenames;
  for(int i = optind; i < argc; i++){
    if( access( argv[i], F_OK ) == 0 ) {
      // file exists
      if(verbosity>=1){
        //print the filenames on a line
        printf("%s\n",argv[i]);
      }
      filenames.push_back(argv[i]);
    } else {
      // file doesn't exist
    }
  }

  //***********End get input*********************//

  //setup, get a boolean and some random numbers
  bool success=false;
  srand(time(NULL));
  if (seedquiet==false){
    cout << "Seed used: " << cl << endl;
  }

  std::mt19937 mtrand(cl);
  std::uniform_real_distribution<double> dist(0.0,1.0);
  cout << "MT Result: " << dist(mtrand) << endl;

  //get a root file and make 
  TFile *f = TFile::Open(outputfile.c_str(),"recreate");
  TTree *t = new TTree("cascade","cascade");

  ofstream logging;
  logging.open(logfile);

  //go through the input files
  for(int i=0;i<filenames.size();i++){
    int numc;
    vector<cli> cascadeFile = readCascadeDistributionFile(numc,filenames[i],success);
    if(success){
      if (!logfile.empty()){
        logging << "**************" << filenames[i] << "***************" << endl;
        logging << endl;
        for(int i=0;i<numc;i++){
          logging << "Cascade ID: " << i+1 << "/" << numc << endl;
          logging << "Fraction of this cascade: " << cascadeFile[i].frac << endl;
          logging << "Neutron separation: " << cascadeFile[i].Sn << endl;
          logging << "Mass number: " << cascadeFile[i].A << endl;
          logging << "Number of steps: " << cascadeFile[i].n << endl;
          logging << endl;
          logging << "Energy Levels (keV)\t|\ttau (fs)" << endl;
          logging << "------------------------------------------------" << endl;
          logging << setfill('0') << setw(5) << setprecision(5);
          logging << "      *****       " << "\t \t" << " ***** " << endl;
          for(int j=0;j<cascadeFile[i].n;j++){
            logging << "      "<< cascadeFile[i].Elev[j] << "       " << "\t \t" << " "<< cascadeFile[i].taus[j] << " " << endl;
          }
          logging << endl;
        }
      }

      if(verbosity > 0){
        cout << "s2: " << s2 << endl;
      }

      if (verbosity >= 2){
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
      }
      //**************************do stuff with this cascade realization********************
      //calculate the first cascade
      for(int k=0;k<numc;k++){
	      int nrealize = num*cascadeFile[k].frac;
              vector<cri> cascade_data;
              cascade_data = Cascade(nrealize,cascadeFile[k].cid,cascadeFile[k].Sn,cascadeFile[k].n,cascadeFile[k].Elev,cascadeFile[k].taus,cascadeFile[k].A,&mtrand);
              bool didsave = addToNRTTree(t,nrealize,cascade_data,cascadeFile[k]); 

        if(!logfile.empty()){  
          logging << "Realizing " << nrealize << " events of cascade ID " << cascadeFile[k].cid << endl;
                logging << "Cascade realization " << k << " success: " << didsave << endl; 
          
              //************************************************************************************
        }

        if(verbosity >= 2){  
          cout << "Realizing " << nrealize << " events of cascade ID " << cascadeFile[k].cid << endl;
                cout << "Cascade realization " << k << " success: " << didsave << endl; 
          
              //************************************************************************************
        }
  
      }
    
    }
    else{
      cout << "ERROR! (readCascadeDistributionFile): one or more of the elements not read correctly " << endl;
    }


  }

  logging.close();
  
  t->Write("",TObject::kOverwrite);
  f->Close();

  return 0;
}
