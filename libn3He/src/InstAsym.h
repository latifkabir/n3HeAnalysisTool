// Filename: InstAsym.h
// Description: n3He Summary Tree Maker Class
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Tue Apr 14 22:10:08 2015 (-0400)
// URL: latifkabir.github.io

#include <TTree.h>
#include <TBranch.h>
#include<TString.h>
#include<TEntryList.h>
#include<TH1D.h>
#include"TTreeRaw.h"

#define FACTOR 1.192092896e-6 //factor for ADC count to Volt conversion.

//Create a struc buffer to keep your events 
struct detData2
{
    int header[48];
    int det[49][48];  
};

struct dirData2
{
    int header[2];
    int sig[1624][2];  
};

class InstAsym
{
protected:
    int ctbin_i;  // Clean data first time bin  
    int ctbin_f; //Clean data last time bin
    int dtbin_i; // Dirty data first time bin
    int dtbin_f; //Dirty data last time bin
    int ntbin_c; //Clean data number of time bins per event
    int ntbin_d; //Dirty data number of time bins per event
    double spin_cutoff; //Sum of absolute value of RFSF signal in Volts.
    double beam_cutoff;
    int no_beam_cutoff;
    double histo_range;
    detData2 *det_data;
    dirData2 *dir_data;

    TTreeRaw *t;

    TEntryList *list;
    int nentries;
    bool fill_status;
    int error_code;
    int nevents;

    void GetDetEntry(TBranch *b,int i);
    void GetDirEntry(TBranch *b,int i);
    int GetAllBranches( int run,TBranch **b);
    void FillClean(TBranch **b,int entry,double sumc[][36],double &norm);
    void FillDirty(TBranch *b,int entry,double *sumd,int &spin);
    void FillAsym(int entry,double sumc[][36],double sumc_prev[][36],double norm,double norm_prev,int spin,int spin_prev,double asym[][36]);
    /* void GetDroppedPls(); */
    /* void CheckSyncStatus(double sumc1,double sumc2,double sumc3,double sumc4,double sumc5,double sumc6,double sumc7,double sumc8); */
    void FillInstData(TH1D ***myHist);
    void RunList(int runNumber);

public:
    int run;

    void CalAsym(TH1D ***myHist, double range);
    InstAsym(int runNumber);
    ~InstAsym();

ClassDef(InstAsym,0)
};

