/*==================regexPlayground.cpp================================ 
   
      PROGRAMMER:  Anthony Villano 12/26/21 

      UPDATES: 


      PURPOSE: Test regex matching for this codebase. 
                         
======================================================================*/
#include <string>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
//#include <random>
#include <map>
#include <vector>
#include <algorithm>

//needed only  for C++ version (commented below)
//#include <iterator>
//#include <regex>

//command line options
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

//regex
#include <regex.h>

using namespace std;

//The name of this program. 
const char* program_name;

//Prints usage information for this program to STREAM (typically
//cout or stderr), and exit the program with EXIT_CODE.  Does not
//return. 

void print_usage (FILE* stream, int exit_code)
{
  fprintf (stream, "Usage:  %s options \n", program_name);
  fprintf (stream,
	   //"\n"
           "  -h, --help                         print usage \n"
           "  -v, --verbose       <level>        Print verbose messages at level <level>\n"
           "  -V, --version                      print version and exit\n");

        printf("Report bugs to: villaa-at-gmail-dot-com \n");
        printf("regexPlayground (nrCascadeSim) home page: <https://github.com/villano-lab/nrCascadeSim> \n");
        printf("General help nrCascadeSim Software: <https://nrcascadesim.readthedocs.io/en/latest/> \n");
  exit (exit_code);
}

#define no_argument 0
#define required_argument 1 
#define optional_argument 2

int main(int argc, char** argv) {

  //***********Begin get input*********************//
 
  //set parameters for input system
  uint verbosity=0;
  uint quietness=0;
  bool silence=0;
   
  const struct option longopts[] =
  {
    {"silent",    no_argument,        0, 's'},
    {"help",    no_argument,        0, 'h'},
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
    iarg = getopt_long(argc, argv, "+shv::V", longopts, &index);

    switch (iarg)
    {

      case 's':
        silence = true;
        break;

      case 'v':
        if(optarg)
          verbosity = atoi(optarg);
        else
          verbosity = 1;
          break;

      case 'V':
        printf("regexPlayground (nrCascadeSim) %s\n", __GIT_VERSION);
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
    }
  }

  //define a buffer for keeping results of regex matching 
  char       buffer[100];                                                     

  //regex object to use
  regex_t regex;

  //*****regex match and input file line*******
  //string iline = "0.038974	29Si	29	[4934.39	1273.37	0]	[0.84	291.0	100000000000000.0]";
  string iline = "9.038974e-5	29Si	29	[4934.39	1273.37	0]	[0.84	291.0	100000000000000.0]";
  //string iline = "0.625846        29Si    29      [4934.39        0]      [0.84   100000000000000.0]";
  //string matchfile="^(0+\\.[0-9]?e?[+-]?[0-9]+)[ \\t\\r\\n\\v\\f]+([0-9]+\\.?[0-9]*|[0-9][0-9]?[0-9]?[A-Z][a-z]?)[ \\t\\r\\n\\v\\f]+([0-9][0-9]?[0-9]?)[ \\t\\r\\n\\v\\f]+(\\[.*\\])[ \\t\\r\\n\\v\\f]+(\\[.*\\])$";
  //string matchfile="^(0+\\.[0-9]?e?[+-]?[0-9]+)[ \\t]+([0-9]+\\.?[0-9]*|[0-9][0-9]?[0-9]?[A-Z][a-z]?)[ \\t]+([0-9][0-9]?[0-9]?)[ \\t]+(\\[.*\\])[ \\t]+(\\[.*\\])$";
  string matchfile="^(-?[0-9]+([.][0-9]+)?(e[+-]?[0-9]+)?)[[:space:]]+([0-9]+\\.?[0-9]*|[0-9][0-9]?[0-9]?[A-Z][a-z]?)[[:space:]]+([0-9][0-9]?[0-9]?)[[:space:]]+(\\[.*\\])[[:space:]]+(\\[.*\\])$";
  //string matchfile="^(0+\\.[0-9]?e?[+-]?[0-9]+)\\s+([0-9]+\\.?[0-9]*|[0-9][0-9]?[0-9]?[A-Z][a-z]?)\\s+([0-9][0-9]?[0-9]?)\\s+(\\[.*\\])\\s+(\\[.*\\])$";

 
  //compile the regex 
  int reti = regcomp(&regex,matchfile.c_str(),REG_EXTENDED);

  regerror(reti, &regex, buffer, 100);

  if(reti==0)
    printf("regex compile success!\n");
  else                                       
    printf("regcomp() failed with '%s'\n", buffer); 


  //match the input line
  regmatch_t input_matchptr[8];
  reti = regexec(&regex,iline.c_str(),8,input_matchptr,0);
  
  regerror(reti, &regex, buffer, 100);  
                    
  if(reti==0)
    printf("regex compile success!\n");
  else
    printf("regexec() failed with '%s'\n", buffer); 

  //******************************************
  


  //*******Try the C++ way****************************
  // https://en.cppreference.com/w/cpp/regex

 
 
  //  std::regex word_regex("^(0+\\.[0-9]?e?[+-]?[0-9]+)\\s+([0-9]+\\.?[0-9]*|[0-9][0-9]?[0-9]?[A-Z][a-z]?)\\s+([0-9][0-9]?[0-9]?)\\s+(\\[.*\\])\\s+(\\[.*\\])$");
  //  auto iline_begin = 
  //      std::sregex_iterator(iline.begin(), iline.end(), word_regex);
  //  auto iline_end = std::sregex_iterator();
 
  //  std::cout << "Found "
  //            << std::distance(iline_begin, iline_end)
  //            << " matches\n";
 
  //  for (std::sregex_iterator i = iline_begin; i != iline_end; ++i) {
  //      std::smatch match = *i;
  //      std::string match_str = match.str();
  //      std::cout << "  " << match_str << '\n';
  //  }

  //  std::smatch matches;
  //  if(std::regex_search(iline, matches, word_regex)) {
  //  std::cout << "Match found\n";

  //    for (size_t i = 0; i < matches.size(); ++i) {
  //        std::cout << i << ": '" << matches[i].str() << "'\n";
  //     }
  //  } else {
  //      std::cout << "Match not found\n";
  //  }
 
  //**************************************************
  return 0;
}
