// Filename: n3He.cc
// Description: The help menu for n3He analysis tool
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Sun May  3 04:50:14 2015 (-0400)
// URL: latifkabir.github.io
#include <iostream>
#include<string>
using namespace std;

void Help()
{
    cout << "\t\t---To create a Tree (TTree object) of the raw data file: "<<endl;
    cout<<"\t\t\tTTreeRaw t(int run_number)" <<endl<<endl;
    cout << "\t\t---To create make summary root file for a single run:"<<endl;
    cout<<"\t\t\tTTreeAsymmetry t(int run_number)"<<endl; 
    cout<<" \t\t\tt.MakeTree() " <<endl<<endl;
    cout << "\t\t---To create the summary root files for a range of runs:" <<endl;
    cout << "\t\t\tTreeMaker(int start_run,int stop_run)" <<endl<<endl;
    cout << "\t\t---To Calculate Asymmetry out of created or existing root file: "<<endl;
    cout<<"\t\t\tAnalyzer(int start_run,int last_run)" <<endl<<endl;
    cout << "\t\t---To make asymmetry plot:" <<endl;
    cout << "\t\t\tMakePlot(const char* data_file,const char* option)" <<endl;
    cout << "\t\t\twhere 'option' can be either 'raw' or 'phy' "<<endl;
    cout<<"\t\t\tto make raw or physics asymmetry plot respectively" <<endl<<endl;
    cout<<"\t\t---To analyze root files using TChain:" <<endl;
    cout<<"\t\t\tAnalyzeChain(int first_run,int last_run)" <<endl<<endl;
    cout<<"\t\t---The n3HeChain (a TChain) Class:"<<endl;
    cout<<"\t\t  --To Create TChain object out of root files:" <<endl;
    cout<<"\t\t\tn3HeChain myChain(int start_run,int stop_run)" <<endl;
    cout<<"\t\t  --Following is the n3HeChain class structure:" <<endl;
    cout<<"\n";
    cout<<"\t\t\tpublic:"<<endl;
    cout<<"\t\t\tint start_run;"<<endl;
    cout<<"\t\t\tint stop_run;"<<endl;
    cout<<"\t\t\tTEventList *oddList;"<<endl;
    cout<<"\t\t\tTEventList *evenList;"<<endl;
    cout<<"\t\t\tTEventList *droppedPulses;"<<endl;
    cout<<"\t\t\tTEventList *adjacentPulses;"<<endl<<endl;
    cout<<"\t\t\tvoid Init(int start_run,int stop_run);"<<endl;
    cout<<"\t\t\tn3HeChain(int start_run,int stop_run);"<<endl;
    cout<<"\t\t\tTEventList* RemoveCut(const char* sel=\"odd\");"<<endl;
    cout<<"\t\t\tint GetLocalEntry(int globalEntry);"<<endl;
    cout<<"\t\t\tTTree* GetMemberTree(int globalEntry);"<<endl;
    cout<<"\t\t\tint GetRunNumber(int globalEntry);"<<endl;
    cout<<"\t\t\tTTreeRaw* GetTTreeRaw(int globalEntry);"<<endl;
    cout<<"\t\t\t~n3HeChain();"<<endl<<endl;
    cout<<"\t\t---For detailed instruction and sample analysis:" <<endl;
    cout << "\t\t\tCheck Analysis Manual on n3He Software Git page." <<endl;
}
