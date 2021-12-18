#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <sys/dir.h>
#include "lindhard.h"

#include "leSpectrumProd.h"

void freesriarray(int n,sri *recoil_data)
{
  for(int i=0;i<n;i++){
    freesri((recoil_data+i));
  }
  free(recoil_data);
  return;
}
void freesri(sri *recoil_data)
{
  free(recoil_data->E);
  free(recoil_data->delE);
  free(recoil_data->I);
  free(recoil_data->Ei);
  free(recoil_data->time);
  return;
}
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *geCascade(int n, int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand)
{
  //input:
  //the neutron separation Sn in MeV
  //number of levels (or total gammas)
  //energy of levels in keV
  //lifetimes of levels in fs
  //mass number (assuming Ge)
  //a previously seeded random number

  //make an output struct
  cri *outinfo;
  outinfo = (cri*) malloc(n*sizeof(cri));

  //string of isotope name
  char isoname[5]; 
  sprintf(isoname,"%dGe",(int)A);

  for(int ev=0;ev<n;ev++){
    outinfo[ev].n = nlev;
    outinfo[ev].cid = cid;
    outinfo[ev].E = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].delE = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].I = (int*) malloc(outinfo[ev].n*sizeof(int));
    outinfo[ev].Ei = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].time = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].Eg = (double*) malloc(outinfo[ev].n*sizeof(double));

    //keep track of some running variables like energy (eV), deposited energy (eV), Ionization (npairs), time (fs)
    double E,delE,I,Ei,time,Eg;
    delE=0;
    I=0;
    Ei=0;
    time=0;
    Eg=0;


    //the initialization requires only E to have an initial value
    E = pow((Sn-(Elev[0]/1000.0)),2.0)/(2*(getMass(isoname)+(Elev[0]/1000.0)));
    E*=1e6; //put in eV

    //now start going through cascade.
    for(int i=0;i<nlev;i++){
      //some necessary values for this step
      double Eorig =E;
      double mass = (getMass(isoname)+(Elev[i]/1000.0))/1000.0; //in GeV at this step
      double egam;
      if(i==nlev-1)
        egam = 0; // in MeV
      else
        egam = ((Elev[i]/1000.0)-(Elev[i+1]/1000.0)); // in MeV

      //compute deposited energy and decay time
      double *stopinfo;
      stopinfo = geStop(E,mass,taus[i],mtrand);
      double vdecay = stopinfo[0]; 
      double t = stopinfo[1];
      free(stopinfo);
      double Eleft = ((mass*1e9)/2.0)*(pow(vdecay,2.0));
      double Erest = geDecay(vdecay,mass,egam,mtrand);

      //update things
      double *ionization;
      ionization = geIonizationInRange_k(E,Eleft,0.159,mtrand); //k-value for Germanium (accepted)
      I=ionization[1];
      Ei=ionization[0];
      free(ionization);
      delE=E-Eleft;
      time+=t;
      E=Erest;
      if(i==0)
        Eg=Sn-(Elev[i]/1000.0);
      else
        Eg=(Elev[i-1]/1000.0)-(Elev[i]/1000.0);


      //store
      outinfo[ev].E[i]=Eorig;
      outinfo[ev].delE[i]=delE;
      outinfo[ev].I[i]=(int)I;
      outinfo[ev].Ei[i]=Ei;
      outinfo[ev].time[i]=time;
      outinfo[ev].Eg[i]=Eg;
    }
  }

  return outinfo;
}
//return the Energy after the mid-stop kick. 
double geDecay(double v, double M, double Egam, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume v in units of c, M in GeV, and Egam in MeV.

  //generate a random direction wrt the incoming direction in the CM
  //I think one angle will do, the azimuthal angle shouldn't matter
  double costhet = (2*dist(mtrand))-1;
  double sinthet = sqrt(1-pow(costhet,2.0));

  //calculate the recoiling energy in the CM frame
  double Ecm = pow(Egam*1e6,2.0)/(2*(M*1e9));

  //cout << "Ecm: " << Ecm << endl;

  //boost to the lab frame using the velocity v in the direction -z
  double vx,vy;
  double vxpr,vypr;
  double vxnew,vynew;
  vx = sqrt(2*M*1e9*Ecm)/(M*1e9);
  vy = 0.0;
  vxpr = costhet*vx -sinthet*vy;
  vypr = sinthet*vx + costhet*vy;
  vxnew = vxpr + v;
  vynew = vypr;

  //cout << "vx: " << vx << endl;
  //cout << "vy: " << vy << endl;
  //cout << "vxpr: " << vxpr << endl;
  //cout << "vypr: " << vypr << endl;
  //cout << "vxnew: " << vxnew << endl;
  //cout << "vynew: " << vynew << endl;

  //now calculate the energy
  double El;
  El = ((M*1e9)/2.0)*(pow(vxnew,2.0)+pow(vynew,2.0));

  //return the energy in eV 
  return El;
}
//return the velocity at a random stopping time
double *geStop(double E, double M, double tau, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume energy in eV, mass in GeV, tau in fs.
 
  //return both the energy and stopping time
  double *ret;
  ret = (double*) malloc(2*sizeof(double));

  //use the Mersenne Twister for a uniform rand number (tau in fs)
  //this is done by inversion method
  double t = -log(1-dist(mtrand))*tau;

  //return the instantaneous velocity
  ret[0] = vgeS2(E,M,t);
  ret[1] = t;
  return ret;
}
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rgeS2(double E, double M, double t)
{
  //assume energy in eV, mass in GeV, t in fs.
  //return velocity in units of c
  //NS2 = s2 * (E/eps) * (rho/R)
  double pi=3.14159265358979323846;
  
  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=32.0;
  double z2=32.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  //cout << "Rmax: " << Rmax*hbarc << endl;
  //cout << "tmax: " << tmax*hbar << endl;
  //cout << "Pi: " << pi << endl;

  double R = (2*(M*1e9)*E - pow((sqrt(2*(M*1e9)*E) - NS2*t/hbar),2.0))/(2*(M*1e9)*NS2); 

  if(t>tmax*hbar)
    return Rmax*hbarc;
  else
    return R*hbarc;
}
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vgeS2(double E, double M, double t)
{
  double pi=3.14159265358979323846;

  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  double R = rgeS2(E,M,t);
  R/=hbarc;

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=32.0;
  double z2=32.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  if(t>tmax*hbar)
    return 0.0;
  else
    return sqrt(2*M*1e9*(E-NS2*R))/(M*1e9);
}
double vgeS2func(double *x,double *par)
{
  //parameters are starting energy in eV and mass in GeV
  double E = par[0];
  double M = par[1];

  //the independent variable x is time in fs

  return vgeS2(E,M,x[0]);

}
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *siCascade(int n, int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand)
{
  //input:
  //the neutron separation Sn in MeV
  //number of levels (or total gammas)
  //energy of levels in keV
  //lifetimes of levels in fs
  //mass number (assuming Ge)
  //a previously seeded random number

  //make an output struct
  cri *outinfo;
  outinfo = (cri*) malloc(n*sizeof(cri));

  //string of isotope name
  char isoname[5]; 
  sprintf(isoname,"%dSi",(int)A);

  for(int ev=0;ev<n;ev++){
    outinfo[ev].n = nlev;
    outinfo[ev].cid = cid;
    outinfo[ev].E = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].delE = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].I = (int*) malloc(outinfo[ev].n*sizeof(int));
    outinfo[ev].Ei = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].time = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].Eg = (double*) malloc(outinfo[ev].n*sizeof(double));

    //keep track of some running variables like energy (eV), deposited energy (eV), Ionization (npairs), time (fs)
    double E,delE,I,Ei,time,Eg;
    delE=0;
    I=0;
    Ei=0;
    time=0;
    Eg=0;


    //the initialization requires only E to have an initial value
    E = pow((Sn-(Elev[0]/1000.0)),2.0)/(2*(getMass(isoname)+(Elev[0]/1000.0)));
    E*=1e6; //put in eV

    //now start going through cascade.
    for(int i=0;i<nlev;i++){
      //some necessary values for this step
      double Eorig =E;
      double mass = (getMass(isoname)+(Elev[i]/1000.0))/1000.0; //in GeV at this step
      double egam;
      if(i==nlev-1)
        egam = 0; // in MeV
      else
        egam = ((Elev[i]/1000.0)-(Elev[i+1]/1000.0)); // in MeV

      //compute deposited energy and decay time
      double *stopinfo;
      stopinfo = siStop(E,mass,taus[i],mtrand);
      double vdecay = stopinfo[0]; 
      double t = stopinfo[1];
      free(stopinfo);
      double Eleft = ((mass*1e9)/2.0)*(pow(vdecay,2.0));
      double Erest = siDecay(vdecay,mass,egam,mtrand);

      //update things
      double *ionization;
      ionization = siIonizationInRange_k(E,Eleft,0.143,mtrand); //k-value approximated for Silicon (see Fallows thesis pg 89)
      I=ionization[1];
      Ei=ionization[0];
      free(ionization);
      delE=E-Eleft;
      time+=t;
      E=Erest;
      if(i==0)
        Eg=Sn-(Elev[i]/1000.0);
      else
        Eg=(Elev[i-1]/1000.0)-(Elev[i]/1000.0);


      //store
      outinfo[ev].E[i]=Eorig;
      outinfo[ev].delE[i]=delE;
      outinfo[ev].I[i]=(int)I;
      outinfo[ev].Ei[i]=Ei;
      outinfo[ev].time[i]=time;
      outinfo[ev].Eg[i]=Eg;
    }
  }

  return outinfo;
}
//return the Energy after the mid-stop kick. 
double siDecay(double v, double M, double Egam, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume v in units of c, M in GeV, and Egam in MeV.

  //generate a random direction wrt the incoming direction in the CM
  //I think one angle will do, the azimuthal angle shouldn't matter
  double costhet = (2*dist(mtrand))-1;
  double sinthet = sqrt(1-pow(costhet,2.0));

  //calculate the recoiling energy in the CM frame
  double Ecm = pow(Egam*1e6,2.0)/(2*(M*1e9));

  //cout << "Ecm: " << Ecm << endl;

  //boost to the lab frame using the velocity v in the direction -z
  double vx,vy;
  double vxpr,vypr;
  double vxnew,vynew;
  vx = sqrt(2*M*1e9*Ecm)/(M*1e9);
  vy = 0.0;
  vxpr = costhet*vx -sinthet*vy;
  vypr = sinthet*vx + costhet*vy;
  vxnew = vxpr + v;
  vynew = vypr;

  //cout << "vx: " << vx << endl;
  //cout << "vy: " << vy << endl;
  //cout << "vxpr: " << vxpr << endl;
  //cout << "vypr: " << vypr << endl;
  //cout << "vxnew: " << vxnew << endl;
  //cout << "vynew: " << vynew << endl;

  //now calculate the energy
  double El;
  El = ((M*1e9)/2.0)*(pow(vxnew,2.0)+pow(vynew,2.0));

  //return the energy in eV 
  return El;
}
//return the velocity at a random stopping time
double *siStop(double E, double M, double tau, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume energy in eV, mass in GeV, tau in fs.
 
  //return both the energy and stopping time
  double *ret;
  ret = (double*) malloc(2*sizeof(double));

  //use the Mersenne Twister for a uniform rand number (tau in fs)
  //this is done by inversion method
  double t = -log(1-dist(mtrand))*tau;

  //return the instantaneous velocity
  ret[0] = vsiS2(E,M,t);
  ret[1] = t;
  return ret;
}
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rsiS2(double E, double M, double t)
{
  //assume energy in eV, mass in GeV, t in fs.
  //return velocity in units of c
  //NS2 = s2 * (E/eps) * (rho/R)
  double pi=3.14159265358979323846;
  
  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=14.0;
  double z2=14.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  //cout << "Rmax: " << Rmax*hbarc << endl;
  //cout << "tmax: " << tmax*hbar << endl;
  //cout << "Pi: " << pi << endl;

  double R = (2*(M*1e9)*E - pow((sqrt(2*(M*1e9)*E) - NS2*t/hbar),2.0))/(2*(M*1e9)*NS2); 

  if(t>tmax*hbar)
    return Rmax*hbarc;
  else
    return R*hbarc;
}
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vsiS2(double E, double M, double t)
{
  double pi=3.14159265358979323846;

  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  double R = rsiS2(E,M,t);
  R/=hbarc;

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=14.0;
  double z2=14.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  if(t>tmax*hbar)
    return 0.0;
  else
    return sqrt(2*M*1e9*(E-NS2*R))/(M*1e9);
}
double vsiS2func(double *x,double *par)
{
  //parameters are starting energy in eV and mass in GeV
  double E = par[0];
  double M = par[1];

  //the independent variable x is time in fs

  return vsiS2(E,M,x[0]);

}
//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *arCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand)
{
  //input:
  //the neutron separation Sn in MeV
  //number of levels (or total gammas)
  //energy of levels in keV
  //lifetimes of levels in fs
  //mass number (assuming Ge)
  //a previously seeded random number

  //make an output struct
  cri *outinfo;
  outinfo = (cri*) malloc(n*sizeof(cri));

  //string of isotope name
  char isoname[5]; 
  sprintf(isoname,"%dAr",(int)A);

  for(int ev=0;ev<n;ev++){
    outinfo[ev].n = nlev;
    outinfo[ev].cid = cid;
    outinfo[ev].E = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].delE = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].I = (int*) malloc(outinfo[ev].n*sizeof(int));
    outinfo[ev].Ei = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].time = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].Eg = (double*) malloc(outinfo[ev].n*sizeof(double));

    //keep track of some running variables like energy (eV), deposited energy (eV), Ionization (npairs), time (fs)
    double E,delE,I,Ei,time,Eg;
    delE=0;
    I=0;
    Ei=0;
    time=0;
    Eg=0;


    //the initialization requires only E to have an initial value
    E = pow((Sn-(Elev[0]/1000.0)),2.0)/(2*(getMass(isoname)+(Elev[0]/1000.0)));
    E*=1e6; //put in eV

    //now start going through cascade.
    for(int i=0;i<nlev;i++){
      //some necessary values for this step
      double Eorig =E;
      double mass = (getMass(isoname)+(Elev[i]/1000.0))/1000.0; //in GeV at this step
      double egam;
      if(i==nlev-1)
        egam = 0; // in MeV
      else
        egam = ((Elev[i]/1000.0)-(Elev[i+1]/1000.0)); // in MeV

      //compute deposited energy and decay time
      double *stopinfo;
      stopinfo = arStop(E,mass,taus[i],mtrand);
      double vdecay = stopinfo[0]; 
      double t = stopinfo[1];
      free(stopinfo);
      double Eleft = ((mass*1e9)/2.0)*(pow(vdecay,2.0));
      double Erest = arDecay(vdecay,mass,egam,mtrand);

      //update things
      double *ionization;
      //see also https://arxiv.org/pdf/0712.2470.pdf for noble liquid k values
      ionization = arIonizationInRange_k(E,Eleft,0.144,mtrand); //k-value approximated for Argon 0.133Z^(2/3)A^(-1/2) 
      I=ionization[1];
      Ei=ionization[0];
      free(ionization);
      delE=E-Eleft;
      time+=t;
      E=Erest;
      if(i==0)
        Eg=Sn-(Elev[i]/1000.0);
      else
        Eg=(Elev[i-1]/1000.0)-(Elev[i]/1000.0);


      //store
      outinfo[ev].E[i]=Eorig;
      outinfo[ev].delE[i]=delE;
      outinfo[ev].I[i]=(int)I;
      outinfo[ev].Ei[i]=Ei;
      outinfo[ev].time[i]=time;
      outinfo[ev].Eg[i]=Eg;
    }
  }

  return outinfo;
}
//return the Energy after the mid-stop kick. 
double arDecay(double v, double M, double Egam, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume v in units of c, M in GeV, and Egam in MeV.

  //generate a random direction wrt the incoming direction in the CM
  //I think one angle will do, the azimuthal angle shouldn't matter
  double costhet = (2*dist(mtrand))-1;
  double sinthet = sqrt(1-pow(costhet,2.0));

  //calculate the recoiling energy in the CM frame
  double Ecm = pow(Egam*1e6,2.0)/(2*(M*1e9));

  //cout << "Ecm: " << Ecm << endl;

  //boost to the lab frame using the velocity v in the direction -z
  double vx,vy;
  double vxpr,vypr;
  double vxnew,vynew;
  vx = sqrt(2*M*1e9*Ecm)/(M*1e9);
  vy = 0.0;
  vxpr = costhet*vx -sinthet*vy;
  vypr = sinthet*vx + costhet*vy;
  vxnew = vxpr + v;
  vynew = vypr;

  //cout << "vx: " << vx << endl;
  //cout << "vy: " << vy << endl;
  //cout << "vxpr: " << vxpr << endl;
  //cout << "vypr: " << vypr << endl;
  //cout << "vxnew: " << vxnew << endl;
  //cout << "vynew: " << vynew << endl;

  //now calculate the energy
  double El;
  El = ((M*1e9)/2.0)*(pow(vxnew,2.0)+pow(vynew,2.0));

  //return the energy in eV 
  return El;
}
//return the velocity at a random stopping time
double *arStop(double E, double M, double tau, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume energy in eV, mass in GeV, tau in fs.
 
  //return both the energy and stopping time
  double *ret;
  ret = (double*) malloc(2*sizeof(double));

  //use the Mersenne Twister for a uniform rand number (tau in fs)
  //this is done by inversion method
  double t = -log(1-dist(mtrand))*tau;

  //return the instantaneous velocity
  ret[0] = varS2(E,M,t);
  ret[1] = t;
  return ret;
}
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rarS2(double E, double M, double t)
{
  //assume energy in eV, mass in GeV, t in fs.
  //return velocity in units of c
  //NS2 = s2 * (E/eps) * (rho/R)
  double pi=3.14159265358979323846;
  
  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  //this is a gas, need a pressure
  double rho1atm = 1.784; //in g/L
  rho1atm *= 1/1000.0; //in g/cm^3
  double P=1; //atm
  double rho = rho1atm*P; //P1V1 = P2V2 at the same pressure
                          //V1 (P1/P2)= V2
			  //rho2 = (P2/P1) rho1
  double Na=6.02214e23;
  double matomic = 39.348;
  //double Ntilde = ((rho*Na)/matomic)*hbarc*1e-39; //in MeV**3
  double Ntilde = ((rho*Na)/matomic)*pow(hbarc,3.0)*1e-39; //in MeV**3
  //double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=18.0;
  double z2=18.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  //cout << "Rmax: " << Rmax*hbarc << endl;
  //cout << "tmax: " << tmax*hbar << endl;
  //cout << "Pi: " << pi << endl;

  double R = (2*(M*1e9)*E - pow((sqrt(2*(M*1e9)*E) - NS2*t/hbar),2.0))/(2*(M*1e9)*NS2); 

  if(t>tmax*hbar)
    return Rmax*hbarc;
  else
    return R*hbarc;
}
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double varS2(double E, double M, double t)
{
  double pi=3.14159265358979323846;

  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  double R = rarS2(E,M,t);
  R/=hbarc;

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  //this is a gas, need a pressure
  double rho1atm = 1.784; //in g/L
  rho1atm *= 1/1000.0; //in g/cm^3
  double P=1; //atm
  double rho = rho1atm*P; //P1V1 = P2V2 at the same pressure
                          //V1 (P1/P2)= V2
			  //rho2 = (P2/P1) rho1
  double Na=6.02214e23;
  double matomic = 39.348;
  double Ntilde = ((rho*Na)/matomic)*pow(hbarc,3.0)*1e-39; //in MeV**3
  //cout << "number density: " << Ntilde << endl;
  //double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=18.0;
  double z2=18.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  if(t>tmax*hbar)
    return 0.0;
  else
    return sqrt(2*M*1e9*(E-NS2*R))/(M*1e9);
}
double varS2func(double *x,double *par)
{
  //parameters are starting energy in eV and mass in GeV
  double E = par[0];
  double M = par[1];

  //the independent variable x is time in fs

  return varS2(E,M,x[0]);

}

//do a generalized multi-step cascade (for now just print a table and do one event)
//eventually: can do n events, put in a yield model function, generalize to other elements 
cri *neCascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand)
{
  //input:
  //the neutron separation Sn in MeV
  //number of levels (or total gammas)
  //energy of levels in keV
  //lifetimes of levels in fs
  //mass number (assuming Ge)
  //a previously seeded random number

  //make an output struct
  cri *outinfo;
  outinfo = (cri*) malloc(n*sizeof(cri));

  //string of isotope name
  char isoname[5]; 
  sprintf(isoname,"%dNe",(int)A);

  for(int ev=0;ev<n;ev++){
    outinfo[ev].n = nlev;
    outinfo[ev].cid = cid;
    outinfo[ev].E = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].delE = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].I = (int*) malloc(outinfo[ev].n*sizeof(int));
    outinfo[ev].Ei = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].time = (double*) malloc(outinfo[ev].n*sizeof(double));
    outinfo[ev].Eg = (double*) malloc(outinfo[ev].n*sizeof(double));

    //keep track of some running variables like energy (eV), deposited energy (eV), Ionization (npairs), time (fs)
    double E,delE,I,Ei,time,Eg;
    delE=0;
    I=0;
    Ei=0;
    time=0;
    Eg=0;


    //the initialization requires only E to have an initial value
    E = pow((Sn-(Elev[0]/1000.0)),2.0)/(2*(getMass(isoname)+(Elev[0]/1000.0)));
    E*=1e6; //put in eV

    //now start going through cascade.
    for(int i=0;i<nlev;i++){
      //some necessary values for this step
      double Eorig =E;
      double mass = (getMass(isoname)+(Elev[i]/1000.0))/1000.0; //in GeV at this step
      double egam;
      if(i==nlev-1)
        egam = 0; // in MeV
      else
        egam = ((Elev[i]/1000.0)-(Elev[i+1]/1000.0)); // in MeV

      //compute deposited energy and decay time
      double *stopinfo;
      stopinfo = arStop(E,mass,taus[i],mtrand);
      double vdecay = stopinfo[0]; 
      double t = stopinfo[1];
      free(stopinfo);
      double Eleft = ((mass*1e9)/2.0)*(pow(vdecay,2.0));
      double Erest = arDecay(vdecay,mass,egam,mtrand);

      //update things
      double *ionization;
      //use argon k values?
      ionization = neIonizationInRange_k(E,Eleft,0.144,mtrand); //k-value approximated for Argon 0.133Z^(2/3)A^(-1/2) 
      I=ionization[1];
      Ei=ionization[0];
      free(ionization);
      delE=E-Eleft;
      time+=t;
      E=Erest;
      if(i==0)
        Eg=Sn-(Elev[i]/1000.0);
      else
        Eg=(Elev[i-1]/1000.0)-(Elev[i]/1000.0);


      //store
      outinfo[ev].E[i]=Eorig;
      outinfo[ev].delE[i]=delE;
      outinfo[ev].I[i]=(int)I;
      outinfo[ev].Ei[i]=Ei;
      outinfo[ev].time[i]=time;
      outinfo[ev].Eg[i]=Eg;
    }
  }

  return outinfo;
}
//return the Energy after the mid-stop kick. 
double neDecay(double v, double M, double Egam, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume v in units of c, M in GeV, and Egam in MeV.

  //generate a random direction wrt the incoming direction in the CM
  //I think one angle will do, the azimuthal angle shouldn't matter
  double costhet = (2*dist(mtrand))-1;
  double sinthet = sqrt(1-pow(costhet,2.0));

  //calculate the recoiling energy in the CM frame
  double Ecm = pow(Egam*1e6,2.0)/(2*(M*1e9));

  //cout << "Ecm: " << Ecm << endl;

  //boost to the lab frame using the velocity v in the direction -z
  double vx,vy;
  double vxpr,vypr;
  double vxnew,vynew;
  vx = sqrt(2*M*1e9*Ecm)/(M*1e9);
  vy = 0.0;
  vxpr = costhet*vx -sinthet*vy;
  vypr = sinthet*vx + costhet*vy;
  vxnew = vxpr + v;
  vynew = vypr;

  //cout << "vx: " << vx << endl;
  //cout << "vy: " << vy << endl;
  //cout << "vxpr: " << vxpr << endl;
  //cout << "vypr: " << vypr << endl;
  //cout << "vxnew: " << vxnew << endl;
  //cout << "vynew: " << vynew << endl;

  //now calculate the energy
  double El;
  El = ((M*1e9)/2.0)*(pow(vxnew,2.0)+pow(vynew,2.0));

  //return the energy in eV 
  return El;
}
//return the velocity at a random stopping time
double *neStop(double E, double M, double tau, mt19937 *mtrand)
{
  //random distribution
  uniform_real_distribution<float> dist(0.,1.);

  //assume energy in eV, mass in GeV, tau in fs.
 
  //return both the energy and stopping time
  double *ret;
  ret = (double*) malloc(2*sizeof(double));

  //use the Mersenne Twister for a uniform rand number (tau in fs)
  //this is done by inversion method
  double t = -log(1-dist(mtrand))*tau;

  //return the instantaneous velocity
  ret[0] = vneS2(E,M,t);
  ret[1] = t;
  return ret;
}
//constant stopping power stopping. return distance after time t for initial energy E and mass M
double rneS2(double E, double M, double t)
{
  //assume energy in eV, mass in GeV, t in fs.
  //return velocity in units of c
  //NS2 = s2 * (E/eps) * (rho/R)
  double pi=3.14159265358979323846;
  
  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  //this is a gas, need a pressure
  double rho1atm = 0.9002; //in g/L
  rho1atm *= 1/1000.0; //in g/cm^3
  double P=1; //atm
  double rho = rho1atm*P; //P1V1 = P2V2 at the same pressure
                          //V1 (P1/P2)= V2
			  //rho2 = (P2/P1) rho1
  double Na=6.02214e23;
  double matomic = 20.1797;
  //double Ntilde = ((rho*Na)/matomic)*hbarc*1e-39; //in MeV**3
  double Ntilde = ((rho*Na)/matomic)*pow(hbarc,3.0)*1e-39; //in MeV**3
  //double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=10.0;
  double z2=10.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  //cout << "Rmax: " << Rmax*hbarc << endl;
  //cout << "tmax: " << tmax*hbar << endl;
  //cout << "Pi: " << pi << endl;

  double R = (2*(M*1e9)*E - pow((sqrt(2*(M*1e9)*E) - NS2*t/hbar),2.0))/(2*(M*1e9)*NS2); 

  if(t>tmax*hbar)
    return Rmax*hbarc;
  else
    return R*hbarc;
}
//constant stopping power stopping. return velocity after time t for initial energy E and mass M
double vneS2(double E, double M, double t)
{
  double pi=3.14159265358979323846;

  //constants
  //need hbar as eV*fs so I can put a time into 1/eV like t/hbar
  double hbar = 0.6582119; //converted with google
  double hbarc = 197.327; //eV*nm

  double R = rneS2(E,M,t);
  R/=hbarc;

  //stopping power (constant)
  //double s2 = 0.32; //from Lindhard Paper Notes on Atomic Collisions II
  //if you're going to use a constant 0.1 is probably a better estimate in this region
  double s2 = 0.1; //from Lindhard Paper Notes on Atomic Collisions II

  //number density in natural units
  //this is a gas, need a pressure
  double rho1atm = 0.9002; //in g/L
  rho1atm *= 1/1000.0; //in g/cm^3
  double P=1; //atm
  double rho = rho1atm*P; //P1V1 = P2V2 at the same pressure
                          //V1 (P1/P2)= V2
			  //rho2 = (P2/P1) rho1
  double Na=6.02214e23;
  double matomic = 20.1797; 
  double Ntilde = ((rho*Na)/matomic)*pow(hbarc,3.0)*1e-39; //in MeV**3
  //cout << "number density: " << Ntilde << endl;
  //double Ntilde = 3.3739e-10; //in MeV**3
  Ntilde*=1e18;


  //calculate z-factor
  double z1=10.0;
  double z2=10.0;
  double zfac = (z1*z2)/(sqrt(pow(z1,(2/3.0))+pow(z2,(2/3.0))));

  //cout << "zfac: " << zfac << endl;

  //NS2 (see notes N-misc-17-001 pg 6) in eV**2
  //I think I dropped a 4pi in that calculation though, restored here
  double NS2 = 4*pi*s2*Ntilde*0.8853*zfac*((M*1e9/511e3)/(2*M*1e9)); //in eV**2

  //calculate maximum range and time
  double Rmax = (1/(4.0*pi))*(1/Ntilde)*(1/(s2*0.8853*zfac))*E*((2*M*1e9)/(M*1e9/511e3));
  double tmax = Rmax * sqrt(2*M*1e9/E);

  if(t>tmax*hbar)
    return 0.0;
  else
    return sqrt(2*M*1e9*(E-NS2*R))/(M*1e9);
}
double vneS2func(double *x,double *par)
{
  //parameters are starting energy in eV and mass in GeV
  double E = par[0];
  double M = par[1];

  //the independent variable x is time in fs

  return vneS2(E,M,x[0]);

}

cri *Cascade(int n,int cid, double Sn, int nlev, double *Elev, double *taus, double A, mt19937 *mtrand)
{
  //FIXME warning not general, only chooses Ge or Si
  if(A>44)
    return geCascade(n,cid,Sn,nlev,Elev,taus,A,mtrand);
  else if(A>33 && A<=44)
    return arCascade(n,cid,Sn,nlev,Elev,taus,A,mtrand);
  else if(A>23 && A<=33)
    return siCascade(n,cid,Sn,nlev,Elev,taus,A,mtrand);
  else if(A<=23)
    return neCascade(n,cid,Sn,nlev,Elev,taus,A,mtrand);
}

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

      //set the cid
      output[count].cid = count;

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
  string numeric="^-?[0-9]+([.][0-9]+)?|inf$";
  int reti = regcomp(&regex,numeric.c_str(),REG_EXTENDED);
  regmatch_t matchptr[2];
  reti = regexec(&regex,in.c_str(),2,matchptr,0);
  if(reti!=0){
    success=false;
    return -999999;
  }

  double dbl;
  //if it's "inf" then set it to 1000 seconds in fs that's 1.0e18
  if(in=="inf"){
    dbl=1.0e18;
    success=true;
    return dbl;
  }

  //if we get here we can be sure it's numeric
  istringstream instream(in);
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
