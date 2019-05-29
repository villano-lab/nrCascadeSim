/*==================evalCascades.cpp==================================== 
   
      PROGRAMMER:  Anthony Villano 10/19/17 

      UPDATES: 


      PURPOSE: calculate things for specific cascades 
                         
======================================================================*/
#include <string>
#include <time.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
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
#include "lindhard.h"
#include "weisskopf.h"
#include "isotope_info.h"
#include "MersenneTwister.h"

//ROOT stuff
#include "rootUtil.h"
#include "TMath.h"
#include "TF1.h"

using namespace std;

//The name of this program. 
const char* program_name;

//Prints usage information for this program to STREAM (typically
//stdout or stderr), and exit the program with EXIT_CODE.  Does not
//return. 

TF1 *significanceNSF(double sig,double bknd,double livetime,double sigwidth, double dres0=200,bool realdays=false,double dutycycle=0.5);
void print_usage (FILE* stream, int exit_code)
{
  fprintf (stream, "Usage:  %s options [ inputfile(s) ]\n", program_name);
  fprintf (stream,
	   //"\n"
           "  -c, --clist         <filename>     file with special cascades in it \n"
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
  std::string selectcascadesfile="sc.txt";
  uint num=1;
  uint verbosity=0;
  uint quietness=0;
  bool quiet=false;
  bool dataquiet=false;
   
  const struct option longopts[] =
  {
    {"clist",     required_argument,  0, 'c'},
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
    iarg = getopt_long(argc, argv, "+c:n:o:q::sv::V", longopts, &index);

    switch (iarg)
    {

      case 'c':
        selectcascadesfile = optarg;
        break;

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
       	  std::cerr << "ERROR evalCascades: invalid quietness value" << std::endl;
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
    std::cerr << "ERROR! evalCascades: no file supplied" << std::endl;
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

  //setup, get a boolean and some random numbers
  bool success=false;
  srand(time(NULL));
  int cl=rand();
  //cout << "A clock tick: " << cl << endl;

  MTRand *mtrand = new MTRand(cl);

  //get a root file and make 
  TFile *f = new TFile("out.root","recreate");
  TTree *t = new TTree("eval","eval");

  //read the select cascades
  int cnum;
  std::vector<int> cnumvec;
  std::ifstream scf_stream(selectcascadesfile.c_str(),std::ios::in);
  string line;
  while(!getline(scf_stream,line).eof()){
    std::istringstream sline(line);
    sline >> cnum;
    cnumvec.push_back(cnum);
    //cout << cnum << endl;
  }

  //go through the input files
  for(int i=0;i<filenames.size();i++){
    int numc;
    cli *cascadeFile = readCascadeDistributionFile(numc,filenames[i],success);

    //turn off this printing
    if(success){
      //cout << "**************" << filenames[i] << "***************" << endl;
      //cout << endl;
      //for(int i=0;i<numc;i++){
      //  cout << "Cascade ID: " << i+1 << "/" << numc << endl;
      //  cout << "Fraction of this cascade: " << cascadeFile[i].frac << endl;
      //  cout << "Neutron separation: " << cascadeFile[i].Sn << endl;
      //  cout << "Mass number: " << cascadeFile[i].A << endl;
      //  cout << "Number of steps: " << cascadeFile[i].n << endl;
      //  cout << endl;
      //  cout << "Energy Levels (keV)\t|\ttau (fs)" << endl;
      //  cout << "------------------------------------------------" << endl;
      //  cout << setfill('0') << setw(5) << setprecision(5);
      //  cout << "      *****       " << "\t \t" << " ***** " << endl;
      //  for(int j=0;j<cascadeFile[i].n;j++){
      //    cout << "      "<< cascadeFile[i].Elev[j] << "       " << "\t \t" << " "<< cascadeFile[i].taus[j] << " " << endl;
      //  }
      //  cout << endl;
      //}

      //**************************do stuff with this cascade realization********************
      //calculate the first cascade
      for(int k=0;k<cnumvec.size();k++){

        double ec=0,y=0,ece=0,ye=0; //energy center, energy error, yield, yield error
        double yav = 0;
	double wsum=0;
	int n = cascadeFile[cnumvec[k]].n;
	int nsamp=100000;

	//define parameters (might be different between Ge/Si)
	double klind = 0.143; //k-value for Silicon (accepted)
	double fano=0.115;
	double egam = 3.8;
	double sig = 0; //number of signal events over 10 days
	double sig_live = 0; //number of signal events over (livedays) days of running
	double significance = 0; //significance of measurement after (livedays) days of running
	double daysto5sigma = 10000; //number of days for the measurement to get to 5 sigma

        //inputs
	double V = 90;
	double Vsigperc = 0.1;
	double sigp0 = 10; //10 eV phonon resolution ?
	double numfrac = 1.0; //factor to account for cross section and gamma-ray geom eff and gamma-ray escape 
	double livedays = 15;
	double bknd = 100; //estimate the background in number of counts per 10 live days over the energy region
	
	if(cascadeFile[cnumvec[k]].A < 60){ //ONLY good for Ge and Si
	  klind = 0.143; //k-value for Silicon (accepted)
	  fano=0.115;
	  egam = 3.8;

	  //use moded phonon resolution
	  sigp0 = 16;
	  //Si detector of CDMS size has 1.85M captures in 10 days but x100 reduction for geom 10% reduction for escape
	  //the last factor adjusts the (originally 10) livedays to the requested amount
	  numfrac = (1.85/4.0)*(1/100.0)*(0.9)*(livedays/10.0);
	}
	else{
	  klind = 0.159; //k-value for Germanium (accepted)
	  fano=0.129;
	  egam = 3.0;

	  //use moded phonon resolution
	  sigp0 = 32;
	  //Ge detector of CDMS size has 21M captures in 10 days but x100 reduction for geom x10 reduction for escape
	  //the last factor adjusts the (originally 10) livedays to the requested amount
	  numfrac = (21/4.0)*(1/100.0)*(1/10.0)*(livedays/10.0);
	}


	//first get nsamp samples of this cascade
        cri *cascade_data;
        cascade_data = Cascade(nsamp,cascadeFile[cnumvec[k]].cid,cascadeFile[cnumvec[k]].Sn,cascadeFile[cnumvec[k]].n,cascadeFile[cnumvec[k]].Elev,
			cascadeFile[cnumvec[k]].taus,cascadeFile[cnumvec[k]].A,mtrand);
        //cout << "Cascade realization " << cnumvec[k] << " success: " << addToNRTTree(t,nsamp,cascade_data) << endl; 

        //get a vector of yields
	double *yvec = (double*) malloc(n*sizeof(double));
	double *nehvec = (double*) malloc(n*sizeof(double));
	double *evec = (double*) malloc(n*sizeof(double));
	for(int l=0;l<n;l++){
          yvec[l] = 0;
	  nehvec[l] = 0;
	  evec[l] = 0;
	}
	int count;
        for(int m=0;m<n;m++){
	  count=0;
       	  for(int l=0;l<nsamp;l++){
	    if(cascade_data[l].time[0]>100){
	      count++;
              yvec[m]+=cascade_data[l].Ei[m]/cascade_data[l].delE[m];
              nehvec[m]+=cascade_data[l].I[m];
              evec[m]+=cascade_data[l].delE[m];
	    }
	  }
	  yvec[m]/=(double)count;
	  nehvec[m]/=(double)count;
	  evec[m]/=(double)count;
	  //cout << "yvec[" << m << "]: " << yvec[m] << endl;
	  //cout << "nehvec[" << m << "]: " << nehvec[m] << endl;
	  //cout << "evec[" << m << "]: " << evec[m] << endl;

	  ec+=nehvec[m]*evec[m];
	  wsum+=nehvec[m];
	  yav+=yvec[m];
	}
	yav/=(double)n;
	ec/=wsum;
	sig = 4000000*cascadeFile[cnumvec[k]].frac*numfrac*(10/livedays);
	sig_live = 4000000*cascadeFile[cnumvec[k]].frac*numfrac;
	//cout << "Count: " << 4000000*cascadeFile[cnumvec[k]].frac*numfrac << endl;
	TF1 *f = significanceNSF(sig,bknd,1.0,100,200,true,1.0);
	f->SetNpx(10000);
	significance = f->Eval(livedays);
	daysto5sigma = f->GetX(5.0);
	//cout << "Significance: " << f->Eval(livedays) << endl;
	//cout << "Days to 5 sigma: " << f->GetX(5.0) << endl;
	//cout << "Energy point: " << ec << endl;

	//compute the yield
	double par[1];
	double x[1];
	par[0]=klind;
	x[0]=ec;
        //cout << cascadeFile[cnumvec[k]].A << "\t" << klind << endl;
	if(cascadeFile[cnumvec[k]].A < 60) //ONLY good for Ge and Si
          y = lindhard_si_k(x,par);
	else
          y = lindhard_ge_k(x,par);
	

        //number of e/h pair =ionization[1];
        //ionization energy =ionization[0];
	//y=ionization[0]/ec;
	//cout << "Yield point: " << y << endl;

	//compute the yield errors
	double sigp = sqrt(pow(sigp0,2.0) + pow(V*sqrt(fano*(ec*y/egam)),2.0));
	//cout << "Fano error: " << V*sqrt(fano*(ec*y/egam)) << endl;
	//cout << "Pt error: " << sigp << endl;
	double sigV = V*Vsigperc;
	double Ep = ec*(1+(V/egam)*y);
	double prefac_Ep = pow((egam/(V*ec)),2.0);
	double prefac_V = pow(((Ep/ec)-1)*(egam/pow(V,2.0)),2.0);
	
	ye = sqrt(prefac_Ep*pow(sigp,2.0) + prefac_V*pow(sigV,2.0));
	//cout << "P error: " << sqrt(prefac_Ep*pow(sigp,2.0)) << endl;
	//cout << "V error: " << sqrt(prefac_V*pow(sigV,2.0)) << endl;
	//cout << "Yield error: " << ye << endl;

	//compute the energy error
        //first find out if there is a dominant recoil
	int max_index=0;
	double max = nehvec[0];
	double nonmaxsum = 0;
	bool isOneDominant = false;
	for(int m=1;m<n;m++){
	  if(nehvec[m]>max){
	    max = nehvec[m];
	    max_index = m;
	  }
	}
	for(int m=0;m<n;m++){
	  if(max_index == m)
            continue;
	 
	  nonmaxsum+=nehvec[m];
	}
	if(nonmaxsum<0.1*max)
	  isOneDominant = true;
	
        if(isOneDominant){
          //don't use yav in next calc but y+(yield of sum of ionization)
	  yav = y+(nonmaxsum*egam/ec);
	}

	//cout << "Average yield: " << yav << endl;
	//cout << "Is one dominant? " << isOneDominant << endl;
	double tempy = y;
	double tempe = ec;
	double tempdel = ec/10.0;
	double eps=0.001;
	while(sqrt(pow(tempy-yav,2.0))>eps*yav){
	  //cout << tempy << "  " << tempdel << endl;
	  bool isPos = true;
          if((tempy-yav)>0)
	    tempe-=tempdel;
	  else{
	    isPos = false;
	    tempe+=tempdel;
	  }

	  x[0]=tempe;
	  //cout << cascadeFile[cnumvec[k]].A << "\t" << klind << endl;
	  if(cascadeFile[cnumvec[k]].A < 60) //ONLY good for Ge and Si
            tempy = lindhard_si_k(x,par);
	  else
            tempy = lindhard_ge_k(x,par);

          if((tempy-yav)>0 && !isPos)
	    tempdel/=10.0;
	  else if((tempy-yav)<=0 && isPos)
	    tempdel/=10.0;

	   
	}
	//cout << tempy << endl;
	ece = sqrt(pow(tempe-ec,2.0));
	//cout << "Energy error: " << ece << endl;



        //cout << "Cascade realization " << cnumvec[k] << " success: " << addToNRTTree(t,nsamp,cascade_data) << endl; 
	  //if(cascadeFile[cnumvec[k]].A < 60) //ONLY good for Ge and Si
	  //  cout << "Silicon: " << cnumvec[k] << "\t" << ec << "\t" << y << "\t" << ece << "\t" << ye << "\t" << isOneDominant << "\t" << yav << endl;
	  //else
	  //  cout << "Germanium: " << cnumvec[k] << "\t" << ec << "\t" << y << "\t" << ece << "\t" << ye << "\t" << isOneDominant << "\t" << yav << endl;
	  //if(cascadeFile[cnumvec[k]].A < 60) //ONLY good for Ge and Si
	  //  cout << "Silicon: " << cnumvec[k] << "\t" << ec << "\t" << y << "\t" << ece << "\t" << ye/sqrt(sig_live) << endl;
	  //else
	  //  cout << "Germanium: " << cnumvec[k] << "\t" << ec << "\t" << y << "\t" << ece << "\t" << ye/sqrt(sig_live) << endl;
	  if(cascadeFile[cnumvec[k]].A < 60) //ONLY good for Ge and Si
	    cout << "Silicon: " << cnumvec[k] << "\t" << ec << "\t" << y << "\t" << ece << "\t" << ye/sqrt(sig_live) << "\t" << sig_live << "\t" << significance << "\t" << daysto5sigma <<  endl;
	  else
	    cout << "Germanium: " << cnumvec[k] << "\t" << ec << "\t" << y << "\t" << ece << "\t" << ye/sqrt(sig_live) << "\t" << sig_live << "\t" << significance << "\t" << daysto5sigma << endl;
	//int nrealize = num*cascadeFile[k].frac;
	//cout << "Realizing " << nrealize << " events of cascade ID " << cascadeFile[k].cid << endl;
        //cascade_data = Cascade(nrealize,cascadeFile[k].cid,cascadeFile[k].Sn,cascadeFile[k].n,cascadeFile[k].Elev,cascadeFile[k].taus,cascadeFile[k].A,mtrand);
        //cout << "Cascade realization " << k << " success: " << addToNRTTree(t,nrealize,cascade_data) << endl; 
  
        freecriarray(nsamp,cascade_data);

      //************************************************************************************

      }
      freecliarray(numc,cascadeFile);
    }
    else{
      cout << "ERROR! (readCascadeDistributionFile): one or more of the elements not read correctly " << endl;
    }


  }

  t->Write("",TObject::kOverwrite);
  f->Close();

  return 0;
}
TF1 *significanceNSF(double sig,double bknd,double livetime,double sigwidth, double dres0,bool realdays,double dutycycle)
{
  //all assuming 90V for now
  //all assuming 200 eV resolution on peaks

  //sig is signal in events/10days
  //bknd is background in same unit -- expect, say 100 events over the whole range 0 to 25 keV
  bknd*=(1200.0/25000.0); //so we can quote the background over the whole interval
  //livetime is the livetime fraction
  //sigwidth is the width (in eV) of the assumed gaussian signal

  //account for resolution and timing broadening
  double sigtotal = sqrt(pow(dres0,2.0));

  //double sigfac = 0.68; //assume 1sigma is exactly 50eV
  double sigfac = TMath::Erf((600/sigtotal)*(1/sqrt(2))); //assume 1200 eV analysis region
  //cout << "sigfac: " << sigfac << endl;

  double factor; 
  factor = ((sig/10.0)*livetime*sigfac)/sqrt((bknd/10.0)*livetime); //src and bknds are in units of 10 days
  TF1 *f;
 
  if(realdays)
    f = new TF1("significance",Form("%f*(x/sqrt(x))*(%f/sqrt(%f))",factor,dutycycle,dutycycle),0,60);
  else
    f = new TF1("significance",Form("%f*x/sqrt(x)",factor),0,60);

  return f;
}
