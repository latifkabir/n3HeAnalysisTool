//The n3HeChain Class that reads raw n3He data files in TTree format
//Author: Latiful Kabir
//Date: 12/25/14


#ifndef N3HECHAIN_H
#define N3HECHAIN_H

#include <TTree.h>
#include <TChain.h>
#include <TString.h>
#include <TBranch.h>
#include <TEventList.h>
#include "TTreeRaw.h"
#include "Constants.h"



class n3HeChain: public TChain
{
public:
    int start_run;
    int stop_run;
    TEventList *oddList;
    TEventList *evenList;
    TEventList *droppedPulses;
    TEventList *cut;

    void Init(int start_run,int stop_run);
    n3HeChain(int start_run,int stop_run);
    n3HeChain(int chain_run);
    TEventList* GenerateCut();
    TEventList* SubtractCut(const char* sel="odd");
    void ImposeCut(const char* sel="odd");
    int GetLocalEntry(int globalEntry);
    TTree* GetMemberTree(int globalEntry);
    int GetRunNumber(int globalEntry);
    TTreeRaw* GetTTreeRaw(int globalEntry);
    ~n3HeChain();
protected:
    
ClassDef(n3HeChain,0)
};

#endif
