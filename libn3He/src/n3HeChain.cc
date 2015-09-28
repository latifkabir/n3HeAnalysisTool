//The n3HeChain Class that reads raw n3He root files in TChain format
//Author: Latiful Kabir
//Date: 12/25/14

#include "n3HeChain.h"
#include <TChain.h>
#include <TSystem.h>
#include <TDirectory.h>
#include <TLeaf.h>
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

n3HeChain::n3HeChain(int start_run):TChain("T")
{
    Init(start_run,start_run);    
}


n3HeChain::~n3HeChain()
{
   // delete oddList;
   // delete evenList;
   // delete droppedPulses;
   // delete cut;
}

void n3HeChain::Init(int start_run,int stop_run)
{
    oddList = new TEventList("oddList");   
    evenList = new TEventList("evenList");   
    droppedPulses = new TEventList("droppedPulses");   
    cut = new TEventList("cut");

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
	//========Add n3He root summary data files to the theis=================
	this->Add(fName);
	count_chain++;
    }
    cout << "Total Root files added in the Chain:"<<count_chain<<endl;

    //======Create the list of events with odd events only and also skip events (first event of root file) having run number as flag===== 
    this->Draw(">>list_temp1","Entry$%24991!=0 && Entry$%2==1","eventlist");
    oddList = (TEventList*)gDirectory->Get("list_temp1");

    //======Create the list of events with even events only and also skip events (first event of root file) having run number as flag===== 
    this->Draw(">>list_temp2","Entry$%24991!=0 && Entry$%2==0","eventlist");
    evenList = (TEventList*)gDirectory->Get("list_temp2");

    //===========Create the list having all dropped pulses events======================
    this->Draw(">>list_temp3","sumd[0]<2000","eventlist");
    droppedPulses = (TEventList*)gDirectory->Get("list_temp3");

}

TEventList* n3HeChain::GenerateCut()
{
    //=====Create the list having events around dropped pulses.====================
    int sEvt;
    int k=0;
    while(k<droppedPulses->GetN())
    {
    	sEvt=droppedPulses->GetEntry(k);
    	// cout << "Event:"<<sEvt <<endl;
    	for(int i=-1;i<9;i++)
    	{
	    if(sEvt==0 && i==-1)
		continue;
	    if((sEvt+i) >= this->GetEntries())
		continue;
	    
	    cut->Enter(sEvt+i);
    	}
    	k++;
    }
    return cut;
}

TEventList* n3HeChain::SubtractCut(const char* sel)
{
    string opt[2]={"even","odd"}; //Choose any of of the two set of asymmetries.

    //=================Subtract all dropped pulse events and events around it from main list===============
    if(sel==opt[0])
    {
	evenList->Subtract(cut);
	return evenList;
    }
    else
    {
	oddList->Subtract(cut);
	return oddList;
    }
} 

void n3HeChain::ImposeCut(const char* sel)
{
    string opt[2]={"even","odd"}; //Choose any of of the two set of asymmetries.
    TEventList* n3HeList;
    GenerateCut();

    if(sel==opt[0])
	n3HeList=SubtractCut("even");
    else
	n3HeList=SubtractCut("odd");

    this->SetEventList(n3HeList);
}

int n3HeChain::GetLocalEntry(int globalEntry)
{
    if(globalEntry>=this->GetEntries() || globalEntry<0)
	return -1;
    //Load member Tree corresponding to any specific global entry
    int localEntry=this->LoadTree(globalEntry); // The return value is local entry number in that member tree.
    return localEntry;
}

TTree* n3HeChain::GetMemberTree(int globalEntry)
{
    if(globalEntry>=this->GetEntries() || globalEntry<0)
	cout << "Global entry number exceeds total number of entries or is invalid.Returning last pointer value" <<endl;

    //Load member Tree corresponding to any specific global entry
    int localEntry=this->LoadTree(globalEntry); // The return value is local entry number in that member tree.

    //Once desired member tree is loaded , retrive that tree
    TTree *t=this->GetTree();
    
    return t;
}

int n3HeChain::GetRunNumber(int globalEntry)
{
    if(globalEntry>=this->GetEntries() || globalEntry<0)
	return -1;
    TTree *t=GetMemberTree(globalEntry);
// Retrive the run number corresponding to this member tree
    t->GetEntry(0);
    int runNumber=(int)t->GetLeaf("asym")->GetValue(0); //In the root file the first asymmetry value is a flag for run number.
    return runNumber;
}

TTreeRaw* n3HeChain::GetTTreeRaw(int globalEntry)
{
    if(globalEntry>=this->GetEntries() || globalEntry<0)
	cout << "Global entry number exceeds total number of entries or is invalid. Returning last pointer value" <<endl;

    int runNumber=GetRunNumber(globalEntry);
    // Create TTreeRaw object for the retrived run number 
    TTreeRaw *tr=new TTreeRaw(runNumber);
    return tr;
} 
