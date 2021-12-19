#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>
#include "edepmath.h"

using namespace std;

#include "lindhard.h"

double lindhard(double *x, double *par){

  //plausible values for parameters
  //root [5] f->SetParameter(0,3.0);
  //root [6] f->SetParameter(1,0.15);
  //root [7] f->SetParameter(2,0.7);
  //root [8] f->SetParameter(3,0.6);
  //root [9] f->SetParameter(4,32); //Z (germanium)
  //root [10] f->SetParameter(5,73); //A (average germanium)


  double Z = par[4];
  double A = par[5];
  double xvar = x[0]/1000.0; //change units from eV to keV

  double kscale = 0.133*pow(Z,(2/3.0))*pow(A,(-1/2.0));
  double escale = 11.5*pow(Z,(-7/3.0));
  double g = par[0]*pow((escale*xvar),par[1]) + par[2]*pow((escale*xvar),par[3]) + escale*xvar;
  return kscale*g/(1+kscale*g);

}
double lindhard_ge_k(double *x, double *par){

  //plausible values for parameters
  //root [5] f->SetParameter(0,0.159);


  double Z = 32;
  double A = 73;
  double xvar = x[0]/1000.0; //change units from eV to keV

  double kscale = par[0];
  double escale = 11.5*pow(Z,(-7/3.0));
  double g = 3.0*pow((escale*xvar),0.15) + 0.7*pow((escale*xvar),0.6) + escale*xvar;
  return kscale*g/(1+kscale*g);

}
double *geIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)
{
  //calculate the ionization energy expected of ge atom stopping between E0 and E1 (E1<E0)
  //E1 and E0 are to be supplied in eV
  //also calculate the number of e/h pairs (without poisson fluctuations)
  double *out;
  out = (double*) malloc(2*sizeof(double));

  //set up vectors for interface with lindhard function
  double x0[1];
  double x1[1];
  double par[1];

  //set up the lindhard value
  //par[0]=0.159; //k-value for Germanium (accepted)
  par[0]=k; //k-value for Germanium (accepted)

  //set the energies
  x0[0]=E0;
  x1[0]=E1;

  //do the output computations
  out[0] = E0*lindhard_ge_k(x0,par)-E1*lindhard_ge_k(x1,par);
  //out[1] = poisson(out[0]/3.0,rand); //eps_gam is about 3 eV for Ge and use simple Poisson stats
  out[1] = poissonFano(out[0]/3.0,0.129,rand); //eps_gam is about 3 eV for Ge and use Poisson stats w/ fano of 0.129

  return out;
}
double lindhard_si_k(double *x, double *par){

  //plausible values for parameters
  //root [5] f->SetParameter(0,0.159);


  double Z = 14;
  double A = 28;
  double xvar = x[0]/1000.0; //change units from eV to keV

  double kscale = par[0];
  double escale = 11.5*pow(Z,(-7/3.0));
  double g = 3.0*pow((escale*xvar),0.15) + 0.7*pow((escale*xvar),0.6) + escale*xvar;
  return kscale*g/(1+kscale*g);

}
double *siIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)
{
  //calculate the ionization energy expected of ge atom stopping between E0 and E1 (E1<E0)
  //E1 and E0 are to be supplied in eV
  //also calculate the number of e/h pairs (without poisson fluctuations)
  double *out;
  out = (double*) malloc(2*sizeof(double));

  //set up vectors for interface with lindhard function
  double x0[1];
  double x1[1];
  double par[1];

  //set up the lindhard value
  //par[0]=0.159; //k-value for Germanium (accepted)
  par[0]=k; //k-value for Germanium (accepted)

  //set the energies
  x0[0]=E0;
  x1[0]=E1;

  //do the output computations
  out[0] = E0*lindhard_si_k(x0,par)-E1*lindhard_si_k(x1,par);
  //out[1] = poisson(out[0]/3.8,rand); //eps_gam is about 3.8 eV for Si and use simple Poisson stats
  out[1] = poissonFano(out[0]/3.8,0.115,rand); //eps_gam is about 3.8 eV for Si and use Poisson stats w/ fano 0.115

  return out;
}
double lindhard_ar_k(double *x, double *par){

  //plausible values for parameters
  //root [5] f->SetParameter(0,0.159);


  double Z = 18;
  double A = 40;
  double xvar = x[0]/1000.0; //change units from eV to keV

  double kscale = par[0];
  double escale = 11.5*pow(Z,(-7/3.0));
  double g = 3.0*pow((escale*xvar),0.15) + 0.7*pow((escale*xvar),0.6) + escale*xvar;
  return kscale*g/(1+kscale*g);

}
double *arIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)
{
  //calculate the ionization energy expected of ge atom stopping between E0 and E1 (E1<E0)
  //E1 and E0 are to be supplied in eV
  //also calculate the number of e/h pairs (without poisson fluctuations)
  double *out;
  out = (double*) malloc(2*sizeof(double));

  //set up vectors for interface with lindhard function
  double x0[1];
  double x1[1];
  double par[1];

  //set up the lindhard value
  //par[0]=0.159; //k-value for Germanium (accepted)
  par[0]=k; //k-value for Germanium (accepted)

  //set the energies
  x0[0]=E0;
  x1[0]=E1;

  //do the output computations
  out[0] = E0*lindhard_ar_k(x0,par)-E1*lindhard_ar_k(x1,par);
  //out[1] = poisson(out[0]/3.8,rand); //eps_gam is about 3.8 eV for Si and use simple Poisson stats
  //http://www.sciencedirect.com/science/article/pii/0168900284901396 <-- fano factor of Ar
  //https://dept.astro.lsa.umich.edu/~cowley/ionen.htm <-- ionization energy in Ar
  out[1] = poissonFano(out[0]/15.7596,0.2,rand); //eps_gam is about 15.7596 eV for Ar and use Poisson stats w/ fano 0.2

  return out;
}
double lindhard_ne_k(double *x, double *par){

  //plausible values for parameters
  //root [5] f->SetParameter(0,0.159);


  double Z = 10;
  double A = 20;
  double xvar = x[0]/1000.0; //change units from eV to keV

  double kscale = par[0];
  double escale = 11.5*pow(Z,(-7/3.0));
  double g = 3.0*pow((escale*xvar),0.15) + 0.7*pow((escale*xvar),0.6) + escale*xvar;
  return kscale*g/(1+kscale*g);

}
double *neIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)
{
  //calculate the ionization energy expected of ge atom stopping between E0 and E1 (E1<E0)
  //E1 and E0 are to be supplied in eV
  //also calculate the number of e/h pairs (without poisson fluctuations)
  double *out;
  out = (double*) malloc(2*sizeof(double));

  //set up vectors for interface with lindhard function
  double x0[1];
  double x1[1];
  double par[1];

  //set up the lindhard value
  //par[0]=0.159; //k-value for Germanium (accepted)
  par[0]=k; //k-value for Germanium (accepted)

  //set the energies
  x0[0]=E0;
  x1[0]=E1;

  //do the output computations
  out[0] = E0*lindhard_ar_k(x0,par)-E1*lindhard_ar_k(x1,par);
  //out[1] = poisson(out[0]/3.8,rand); //eps_gam is about 3.8 eV for Si and use simple Poisson stats
  //GUESS FANO of 0.2, same as Argon
  //http://www.lenntech.com/periodic-chart-elements/ionization-energy.htm <--- ionization energy in Ne
  out[1] = poissonFano(out[0]/21.5645,0.2,rand); //eps_gam is about 21.5645 eV for Ar and use Poisson stats w/ fano 0.2

  return out;
}
