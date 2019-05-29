/*==================cascadeTest.cpp==================================== 
   
      PROGRAMMER:  Anthony Villano 01/11/19 

      UPDATES: 


      PURPOSE: read in cascad file and interpret the weisskopf values, then
        re-print. 
                         
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
    std::cerr << "ERROR! cascadePrint: no file supplied" << std::endl;
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


  //go through the input files
  bool success=false;
  for(int nf=0;nf<filenames.size();nf++){
    //readCascadeDistributionFile(filenames[i]);
    int numc;
    cli *cascadeFile = readCascadeDistributionFile(numc,filenames[nf],success);
    if(success){
      for(int i=0;i<numc;i++){
        //string of isotope name
        char isoname[5]; 
        sprintf(isoname,"%dGe",cascadeFile[i].A);

        cout << cascadeFile[i].frac << "\t" << isoname << "\t" << cascadeFile[i].A << "\t"
          << "[";
        for(int j=0;j<cascadeFile[i].n;j++){
         cout << cascadeFile[i].Elev[j];
         if(j<cascadeFile[i].n-1)
           cout << "\t";
        }
        cout << "]"; 

         cout << "\t" << "[";
        for(int j=0;j<cascadeFile[i].n;j++){
         cout << cascadeFile[i].taus[j];
         if(j<cascadeFile[i].n-1)
           cout << "\t";
        }
        cout << "]" << endl; 
      }
      freecliarray(numc,cascadeFile);
    }
    else{
      cout << "ERROR! (readCascadeDistributionFile): one or more of the elements not read correctly " << endl;
    }


  }


  return 0;
}
