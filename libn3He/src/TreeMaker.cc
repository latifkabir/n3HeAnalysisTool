// Filename: OpenTree.cc
// Description: Reading Tree from .root file
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Apr  6 14:25:48 2015 (-0400)
// URL: latifkabir.github.io
#include<iostream>
#include"TTreeAsymmetry.h"

using namespace std;

void TreeMaker(int start_run=0,int stop_run=0)
{
    if(start_run<=0 || stop_run<=0)
    {
	cout << "Please enter a valid range for the run number" <<endl;
	return;
    }
    for(int run=start_run;run<=stop_run;run++)
    {
    	cout << "====================================================="<<endl;
    	cout << "Now Making root file for run:"<<run <<"... ..."<<endl;
    	cout << "=====================================================\n"<<endl;
    	TTreeAsymmetry *t=new TTreeAsymmetry(run,true);
    	t->MakeTree();
    	cout << "Done with run number "<<run<<endl;
	delete t;
    }
}
