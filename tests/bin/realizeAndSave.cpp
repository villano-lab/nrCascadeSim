//library commands
#include "cascadeProd.h"
#include "lindhard.h"
#include "weisskopf.h"
#include "isotope_info.h"

//ROOT stuff
#include "rootUtil.h"

//get the file name
string filenames="inputfile.txt";

//read the contents of a file into a cli object
int numc;
bool success=false;

//get a root file and make
TFile *f = TFile::Open("output.root","recreate");
TTree *t = new TTree("cascade","cascade");

//calculate the cascades
  int main(){
  int num = 10000;
  for(int k=0;k<numc;k++){
    cli *cascadeFile = readCascadeDistributionFile(numc,filenames[k],success);
    int nrealize = num*cascadeFile[k].frac;
    cout << "Realizing " << nrealize << " events of cascade ID " << cascadeFile[k].cid << endl;
    cri *cascade_data;
    cascade_data = Cascade(nrealize,cascadeFile[k].cid,cascadeFile[k].Sn,cascadeFile[k].n,cascadeFile[k].Elev,cascadeFile[k].taus,cascadeFile[k].A,mtrand);
    cout << "Cascade realization " << k << " success: " << addToNRTTree(t,nrealize,cascade_data,cascadeFile[k]) << endl;

    freecriarray(nrealize,cascade_data);
  }
  freecliarray(numc,cascadeFile);

  //write the ROOT file
  t->Write("",TObject::kOverwrite);
  f->Close();

  return 0;
}