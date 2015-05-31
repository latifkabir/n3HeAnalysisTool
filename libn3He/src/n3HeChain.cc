//The n3HeChain Class that reads raw n3He root files in TChain format
//Author: Latiful Kabir
//Date: 12/25/14

#include "n3HeChain.h"
#include <TChain.h>
#include <TSystem.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

ClassImp(n3HeChain)


n3HeChain::n3HeChain(int start_run,int stop_run):TChain("T")
{
    Init(start_run,stop_run);    
}

n3HeChain::~n3HeChain()
{


}

void n3HeChain::Init(int start_run,int stop_run)
{

    int count_chain=0;

    for(int run=start_run;run<=stop_run;run++)
    {
	TString fName;
	fName=ROOT_FILE_PATH;
	fName+="run-";
	fName+=run;
	fName+=".root";
 
	//============ Open the file containing the tree==================
	bool status=gSystem->AccessPathName(fName); //Note bizzare return type convention
	if(status) 
	{
	    cout << "\n--------->The root file for run number :"<<run<<" does NOT exist<-------------" <<endl;
	    cout << "--------->Skipping the run number : "<<run <<"<-------------\n"<<endl;
	    continue;
	}
	//========Add n3He root summary data files to the chain=================
	this->Add(fName);
	count_chain++;
    }
    cout << "Total Root files added in the Chain:"<<count_chain<<endl;
    
}
