#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

#include "rootUtil.h"

bool addToNRTTree(TTree *t,int nr,vector<cri> &recoil_lists,vector<cli> &cascade_spec)
{
  //set up some variables
  Long64_t n,cid; //number of steps and cascade ID
  double frac,Sn,A; //fraction represented by this cascade,n sep,mass number
  double Elev[1000],taus[1000]; //levels and taus of input
  double E[1000],delE[1000],I[1000],Ei[1000],time[1000],Eg[1000];

  //check and/or reset branch linkages
  int nbranch = t->GetListOfBranches()->GetEntries();
  if(t->FindBranch("n")==NULL)
    t->Branch("n",&n,"n/L");
  else
    t->SetBranchAddress("n",&n);

  if(t->FindBranch("cid")==NULL)
    t->Branch("cid",&cid,"cid/L");
  else
    t->SetBranchAddress("cid",&cid);

  if(t->FindBranch("Elev")==NULL)
    t->Branch("Elev",&Elev,"Elev[n]/D");
  else
    t->SetBranchAddress("Elev",&Elev);

  if(t->FindBranch("taus")==NULL)
    t->Branch("taus",&taus,"taus[n]/D");
  else
    t->SetBranchAddress("taus",&taus);

  if(t->FindBranch("E")==NULL)
    t->Branch("E",&E,"E[n]/D");
  else
    t->SetBranchAddress("E",&E);

  if(t->FindBranch("delE")==NULL)
    t->Branch("delE",&delE,"delE[n]/D");
  else
    t->SetBranchAddress("delE",&delE);

  if(t->FindBranch("I")==NULL)
    t->Branch("I",&I,"I[n]/D");
  else
    t->SetBranchAddress("I",&I);

  if(t->FindBranch("Ei")==NULL)
    t->Branch("Ei",&Ei,"Ei[n]/D");
  else
    t->SetBranchAddress("Ei",&Ei);

  if(t->FindBranch("time")==NULL)
    t->Branch("time",&time,"time[n]/D");
  else
    t->SetBranchAddress("time",&time);

  if(t->FindBranch("Eg")==NULL)
    t->Branch("Eg",&Eg,"Eg[n]/D");
  else
    t->SetBranchAddress("Eg",&Eg);

  //loop over nr cascade realizations 
  for(int i=0;i<nr;i++){
    //set the variables
    n = recoil_lists[i].n;
    cid = recoil_lists[i].cid;
    for(int j=0;j<n;j++){
      E[j] = recoil_lists[i].E[j];
      delE[j] = recoil_lists[i].delE[j];
      I[j] = recoil_lists[i].I[j];
      Ei[j] = recoil_lists[i].Ei[j];
      time[j] = recoil_lists[i].time[j];
      Eg[j] = recoil_lists[i].Eg[j];
      Elev[j] = cascade_spec.Elev[j];
      taus[j] = cascade_spec.taus[j];
    }
    t->Fill();
  }

  return t;
}
