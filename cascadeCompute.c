#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <sys/dir.h>
#include <regex.h>
#include "weisskopf.h"
#include "lindhard.h"
#include "isotope_info.h"

#include "cascadeCompute.h"


cli *readCascadeDistributionFile(int &n, string file,bool &success)
{
  //first assume you will succeed
  success=true;

  //open the file
  ifstream in(file.c_str(),ios::in);

  //count the lines in the file
  string line;
  n=0;
  while(!getline(in,line).eof()){
    n++;
  }
  in.clear();
  in.seekg(0);

  //make an object
  cli *output = (cli*) malloc(n*sizeof(cli));

  //do some regex matching to parse the cascade info 
  regex_t regex;
  string matchfile="^([0-9]+\\.?[0-9]+?)\\s+([0-9]+\\.?[0-9]+?|[0-9]+[A-Z][a-z])\\s+([0-9]+)\\s+(\\[.*?\\])\\s+(\\[.*?\\])$";
  int reti = regcomp(&regex,matchfile.c_str(),REG_EXTENDED);

  int count=0;
  while(!getline(in,line).eof()){
    //cout << line << endl;
    regmatch_t matchptr[6];
    reti = regexec(&regex,line.c_str(),6,matchptr,0);
    if(!reti){
      
      //print out the match	    
      //cout << line.substr(matchptr[0].rm_so,matchptr[0].rm_eo-matchptr[0].rm_so) << endl;
      bool goodentry=true;

      //cout << "For this match: " << endl;
      int num;
      for(int i=1;i<6;i++){
        //cout << line.substr(matchptr[i].rm_so,matchptr[i].rm_eo-matchptr[i].rm_so) << endl;
        string component = line.substr(matchptr[i].rm_so,matchptr[i].rm_eo-matchptr[i].rm_so);
	bool isok=false;
        if(i==1)
          output[count].frac = interpretDbl(component,isok); 
	else if(i==2)
          output[count].Sn = interpretSn(component,isok); 
	else if(i==3)
          output[count].A = interpretDbl(component,isok); 
	else if(i==4)
          output[count].Elev = interpretElevVector(num,component,isok); 
	else if(i==5)
          output[count].taus = interpretTauVector(num,component,output[count].A,output[count].Elev,isok); 

	//cout << " iteration: " << i << " goodentry: " << goodentry << endl;
	//cout << " iteration: " << i << " isok: " << isok << endl;
	//cout << endl;
	goodentry=goodentry&&isok;
      }
      output[count].n=num;
      output[count].success=goodentry;
      //cout << endl;

      //cout << " this file (success): " << success << " (goodentry): " << goodentry << endl;
      success=success&&goodentry;

      count++;
    }
  }



  return output;
}
double interpretDbl(string in,bool &success)
{
  //use a regex to see if the thing is numeric
  regex_t regex;
  string numeric="^-?[0-9]+([.][0-9]+)?$";
  int reti = regcomp(&regex,numeric.c_str(),REG_EXTENDED);
  regmatch_t matchptr[2];
  reti = regexec(&regex,in.c_str(),2,matchptr,0);
  if(reti!=0){
    success=false;
    return -999999;
  }

  //if we get here we can be sure it's numeric
  istringstream instream(in);
  double dbl;
  instream>>dbl;
  success=true;
  return dbl;
}
double interpretSn(string in,bool &success)
{
  //sn value
  double Sn;
  Sn=getSn(in);
  if(Sn>0){
    success=true;
    return Sn;
  }

  //otherwise the only hope is that it's a number
  double dbl;
  dbl=interpretDbl(in,success);
  return dbl;

}
double interpretWeisskopf(string in,double Egam,double A,bool &success)
{
  //need to input egamma--do it in MeV
  //and mass number


  //check if it's numeric first
  double dbl;
  dbl=interpretDbl(in,success);
  if(success==true)
    return dbl;

  //if not numeric try to decide if it's an interperetable Weisskopf specifier
  regex_t regex;
  string weisskopf="^w\\(([E,M][0-9]+)(,([0-9]+([.][0-9]+)?))?\\)$";
  int reti = regcomp(&regex,weisskopf.c_str(),REG_EXTENDED);
  regmatch_t wdeconstruct[5];
  reti = regexec(&regex,in.c_str(),5,wdeconstruct,0);
  if(reti!=0){
    success=false;
    return -999999;
  }

  //otherwise, we have to parse the Weisskopf thing into a number
  double mult;
  if(wdeconstruct[3].rm_so>0){
    string multiplier;
    multiplier=in.substr(wdeconstruct[3].rm_so,wdeconstruct[3].rm_eo-wdeconstruct[3].rm_so);
    istringstream smult(multiplier);
    smult>>mult;
  }
  else
    mult=1.0;
    
  double time = mult*we(Egam,A,in.substr(wdeconstruct[1].rm_so,wdeconstruct[1].rm_eo-wdeconstruct[1].rm_so));
  success=true;
  return time;
}
double *interpretElevVector(int &n,string in,bool &success)
{

  //get vector elements
  vector<string> elements;
  elements = vsplit(in);
  n=elements.size();

  success=true;

  //allocate the output vector
  double *out = (double*) malloc(n*sizeof(double));
  for(int i=0;i<n;i++){ 
    bool isok=false;
    out[i] = interpretDbl(elements[i],isok); 
    success=success&&isok;
  }

  return out;
}
double *interpretTauVector(int n,string in,double A,double *Elev,bool &success) //need the Elev vector and Sn for Weisskopf
{

  //get vector elements
  vector<string> elements;
  elements = vsplit(in);
  n=elements.size();

  success=true;



  //allocate the output vector
  double *out = (double*) malloc(n*sizeof(double));
  for(int i=0;i<n;i++){ 
    bool isok=false;
    double Egam;
    if(i==(n-1))
      Egam=0.0; //might as well be zero but I think that would make the lifetime inf
    else
      Egam=Elev[i]-Elev[i+1];
    out[i] = interpretWeisskopf(elements[i],Egam/1000.0,A,isok); 
    success=success&&isok;
  }

  return out;


}
vector<string> vsplit(string in)
{
  //make an istream to split out whitespace
  istringstream streamin(in);

  //vector
  vector<string> output;
  string element;
  while(!streamin.eof()){
    streamin>>element;
    int posl=element.find("[");
    int posr=element.find("]");
    if(posl!=string::npos && posr==string::npos)
      element = element.substr(posl+1,element.size()-posl-1);
    else if(posr!=string::npos && posl==string::npos)
      element = element.substr(0,element.size()-1);
    else if(posl!=string::npos && posr!=string::npos)
      element = element.substr(posl+1,element.size()-posl-2);
    

    output.push_back(element);
  }

  return output;
}
