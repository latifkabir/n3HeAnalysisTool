// Filename: TTreeAsymmetry.h
// Description: n3He Summary Tree Maker Class
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Tue Apr 14 22:10:08 2015 (-0400)
// URL: latifkabir.github.io

#include <TTree.h>
#include <TBranch.h>
#include<TString.h>
#include<TEntryList.h>
#include"TTreeRaw.h"

#define FACTOR 1.192092896e-6 //factor for ADC count to Volt conversion.

//Create a struc buffer to keep your events 
struct detData
{
    int header[48];
    int det[49][48];  
};

struct dirData
{
    int header[2];
    int sig[1624][2];  
};

class TTreeAsymmetry
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

    detData *det_data;
    dirData *dir_data;
    TFile *f;
    TTreeRaw *t;
    TString fName;
    TEntryList *list;
    int nentries;
    int nevents;
    int n_dpulses;
    int dpulse;
    int first_dropped;
    int last_dropped;
    bool save_tree;
    int norm_option;
    
    void GetDetEntry(TBranch *b,int i);
    void GetDirEntry(TBranch *b,int i);
    int GetAllBranches( int run,TBranch **b);
    void FillClean(TBranch **b,int entry,double sumc[][36],double &norm);
    void FillDirty(TBranch *b,int entry,double *sumd,int &spin);
    void FillAsym(int entry,double sumc[][36],double sumc_prev[][36],double norm,double norm_prev,int spin,int spin_prev,double asym[][36]);
    void GetDroppedPls();
    void CheckSyncStatus(double sumc1,double sumc2,double sumc3,double sumc4,double sumc5,double sumc6,double sumc7,double sumc8);
    void FillTree(TTree* tr);
    void RunList(int runNumber);

public:
    int run;
    int error_code;
    bool fill_status;
    TTree *T;
    void Init(int run_number);
    void InitRootFile();
    TTree* MakeTree();
    TTreeAsymmetry(int runNumber, bool saveTree);
    TTreeAsymmetry(int runNumber , bool saveTree,int ctb_i, int ctb_f,int option);
    ~TTreeAsymmetry();

ClassDef(TTreeAsymmetry,0)
};

