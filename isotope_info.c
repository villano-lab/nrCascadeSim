#include <string>
#include <iostream>
#include <vector>

using namespace std;

#include "isotope_info.h"

double getRecoilEnergy(string isotope)
{

  double mnuc = getMass(isotope);
  double sn = getSn(isotope);

  return sn*sn/(2*mnuc);
}
double getMass(string isotope)
{

  //get the mass of the nucleus in MeV
  double delnuc = getDelta(isotope);
  double deln = 8.071;
  double melec = 0.510998;
  double mnbar = 931.494045;
  double mnuc = (getN(isotope)+getZ(isotope))*mnbar + delnuc -getZ(isotope)*melec;

  return mnuc;
}
double getDelta(string isotope){

  //return the mass defect of isotope as per Nuclear Wallet Cards Jangdish K. Tuli (2005)
  if(isotope=="70Ge")
    return -70.563;
  else if(isotope=="71Ge")
    return -69.908;
  else if(isotope=="72Ge")
    return -72.586;
  else if(isotope=="73Ge")
    return -71.298;
  else if(isotope=="73mGe")
    return -71.298+0.0667; //metastable
  else if(isotope=="74Ge")
    return -73.422;
  else if(isotope=="74cGe")
    return -73.422; //cascade; 2-gamma transition w/ fast decay
  else if(isotope=="75Ge")
    return -71.856;
  else if(isotope=="75mGe")
    return -71.856 + 0.1397;
  else if(isotope=="76Ge")
    return -73.213;
  else if(isotope=="77Ge")
    return -71.214;
  else if(isotope=="77mGe")
    return -71.214 + 0.1597;
  else if(isotope=="40Ar")
    return -35.0398;
  else if(isotope=="41Ar")
    return -33.0675;
  else if(isotope=="28Si")
    return -21.493;
  else if(isotope=="29Si")
    return -21.895;
  else if(isotope=="30Si")
    return -24.433;
  else if(isotope=="31Si")
    return -22.949;
  else if(isotope=="32Si")
    return -24.081;
  else if(isotope=="33Si")
    return -20.49;
  else if(isotope=="20Ne")
    return -7.0419;
  else if(isotope=="21Ne")
    return -5.7317;
  else if(isotope=="22Ne")
    return -8.0247;
  else if(isotope=="23Ne")
    return -5.1540;
  else
    return -99999999.0; //pretty bound


}
double getN(string isotope){

  //return the number of neutrons 
  if(isotope=="70Ge")
    return 70-32;
  else if(isotope=="71Ge")
    return 71-32;
  else if(isotope=="72Ge")
    return 72-32;
  else if(isotope=="73Ge")
    return 73-32;
  else if(isotope=="73mGe")
    return 73-32;
  else if(isotope=="74Ge")
    return 74-32;
  else if(isotope=="74cGe")
    return 74-32; //cascade; 2-gamma transition w/ fast decay
  else if(isotope=="75Ge")
    return 75-32;
  else if(isotope=="75mGe")
    return 75-32;
  else if(isotope=="76Ge")
    return 76-32;
  else if(isotope=="77Ge")
    return 77-32;
  else if(isotope=="77mGe")
    return 77-32;
  else if(isotope=="40Ar")
    return 40-18;
  else if(isotope=="41Ar")
    return 41-18;
  else if(isotope=="28Si")
    return 28-14;
  else if(isotope=="29Si")
    return 29-14;
  else if(isotope=="30Si")
    return 30-14;
  else if(isotope=="31Si")
    return 31-14;
  else if(isotope=="32Si")
    return 32-14;
  else if(isotope=="33Si")
    return 33-14;
  else if(isotope=="20Ne")
    return 20-10;
  else if(isotope=="21Ne")
    return 21-10;
  else if(isotope=="22Ne")
    return 22-10;
  else if(isotope=="23Ne")
    return 23-10;
  else
    return -99999999.0; //pretty bound


}
double getZ(string isotope){

  //return the number of protons 
  if(isotope=="70Ge")
    return 32;
  else if(isotope=="71Ge")
    return 32;
  else if(isotope=="72Ge")
    return 32;
  else if(isotope=="73Ge")
    return 32;
  else if(isotope=="73mGe")
    return 32;
  else if(isotope=="74Ge")
    return 32;
  else if(isotope=="74cGe")
    return 32; //cascade; 2-gamma transition w/ fast decay
  else if(isotope=="75Ge")
    return 32;
  else if(isotope=="75mGe")
    return 32;
  else if(isotope=="76Ge")
    return 32;
  else if(isotope=="77Ge")
    return 32;
  else if(isotope=="77mGe")
    return 32;
  else if(isotope=="40Ar")
    return 18;
  else if(isotope=="41Ar")
    return 18;
  else if(isotope=="28Si")
    return 14;
  else if(isotope=="29Si")
    return 14;
  else if(isotope=="30Si")
    return 14;
  else if(isotope=="31Si")
    return 14;
  else if(isotope=="32Si")
    return 14;
  else if(isotope=="33Si")
    return 14;
  else if(isotope=="20Ne")
    return 10;
  else if(isotope=="21Ne")
    return 10;
  else if(isotope=="22Ne")
    return 10;
  else if(isotope=="23Ne")
    return 10;
  else
    return -99999999.0; //pretty bound


}
double getSn(string isotope){

  //use references:
  //M.A. Islam, PRC 43 1086 (1991)
  //S. Raman, PRC 46 972 (1992)
  //nndc.bnl.gov failing precise values from above

  //return the mass defect of isotope as per Nuclear Wallet Cards Jangdish K. Tuli (2005)
  if(isotope=="70Ge")
    return 11.5325;
  else if(isotope=="71Ge")
    return 7.41595;
  else if(isotope=="72Ge")
    return 10.7508;
  else if(isotope=="73Ge")
    return 6.78294;
  else if(isotope=="73mGe")
    return 6.78294 - 0.0667; //metastable
  else if(isotope=="74Ge")
    return 10.19631;
  else if(isotope=="74cGe")
    return 10.19631 - 0.5959; //cascade; 2-gamma transition w/ fast decay
  else if(isotope=="75Ge")
    return 6.50584;
  else if(isotope=="75mGe")
    return 6.50584 - 0.1397; //metastable
  else if(isotope=="76Ge")
    return 9.42724;
  else if(isotope=="77Ge")
    return 6.07129;
  else if(isotope=="77mGe")
    return 6.07129 - 0.1597;
  else if(isotope=="40Ar")
    return 9.8695;
  else if(isotope=="41Ar")
    return 6.0989;
  else if(isotope=="28Si")
    return 17.17972;
  else if(isotope=="29Si")
    return 8.47356;
  else if(isotope=="30Si")
    return 10.60924;
  else if(isotope=="31Si")
    return 6.58740;
  else if(isotope=="32Si")
    return 9.2000;
  else if(isotope=="33Si")
    return 4.5080;
  else if(isotope=="20Ne")
    return 16.86530;
  else if(isotope=="21Ne")
    return 6.76116;
  else if(isotope=="22Ne")
    return 10.36426;
  else if(isotope=="23Ne")
    return 5.20065;
  
  else
    return -99999999.0; //pretty bound


}
void listStuff()
{
  //list the important parameters for all the important isotopes
  vector<string> iso;
  iso.push_back("70Ge");
  iso.push_back("71Ge");
  iso.push_back("72Ge");
  iso.push_back("73Ge");
  iso.push_back("73mGe");
  iso.push_back("74Ge");
  iso.push_back("74cGe");
  iso.push_back("75Ge");
  iso.push_back("75mGe");
  iso.push_back("76Ge");
  iso.push_back("77Ge");
  iso.push_back("77mGe");
  iso.push_back("40Ar");
  iso.push_back("41Ar");
  iso.push_back("28Si");
  iso.push_back("29Si");
  iso.push_back("30Si");
  iso.push_back("31Si");
  iso.push_back("20Ne");
  iso.push_back("21Ne");
  iso.push_back("22Ne");
  iso.push_back("23Ne");

  for(int i=0;i<iso.size();i++){
    cout <<"Isotope: " << iso[i] << "; Sn: " << getSn(iso[i]) << " MeV; Recoil Energy: " << getRecoilEnergy(iso[i])*1e6 << " eV; Mass: " << getMass(iso[i]) << " MeV" << endl;
  }

  return;
}
