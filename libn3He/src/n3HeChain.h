//The n3HeChain Class that reads raw n3He data files in TTree format
//Author: Latiful Kabir
//Date: 12/25/14


#ifndef N3HECHAIN_H
#define N3HECHAIN_H

#include <TChain.h>
#include <TString.h>
#include <TBranch.h>
#include"Constants.h"



class n3HeChain: public TChain
{

public:

    int start_run;
    int stop_run;

    void Init(int start_run,int stop_run);
    n3HeChain(int start_run,int stop_run);
    ~n3HeChain();
protected:
    
ClassDef(n3HeChain,0)
};

#endif
