// Filename: fix.cc
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Thu Jun 11 12:13:54 2015 (-0400)
// URL: latifkabir.github.io
#include<iostream>
#include<fstream>
#include<iomanip>
#include"RunStatus.h"

using namespace std;


void RunStatus(int start_run,int last_run)
{
    ifstream inFile("/mnt/idata05/summary/runList.txt");
    if(!inFile)
    {
	cout<<"Could NOT locate data file"<<endl;
	return;
    }

    const int MaxRun=100000;
    int run[MaxRun];
    int error_code[MaxRun];
    string error[MaxRun];
    int index=0;
    int run_lower;
    int run_up;

    if((start_run!=0 && start_run<17400) || ( last_run!=0 && last_run<17400))
    {
	cout<<"Run Status for run number below 17400 is NOT available"<<endl;
	return;
    }

    if(start_run!=0 && last_run!=0 && last_run<start_run)
    {
	cout << "Syntax: RunStatus(lower_range,upper_range)" <<endl;
	return;
    }

    while(!inFile.eof())
    {
	inFile>>run[index]>>error_code[index]>>error[index];
	index++;
    }

    run_lower=start_run;
    if(last_run!=0)
        run_up=last_run;
    else if(last_run==0 && start_run!=0)
	run_up=start_run;
    else
	run_up=run[index-2];
    
    cout<<"Run Number"<<"\t\t"<<"Error Code"<<"\t\t"<<"Run Status"<<endl;
    cout<<"==========="<<"\t\t"<<"==========="<<"\t\t"<<"==========="<<endl;
    
    for(int i=0;i<index-1;i++)
    {
	if(run[i]>=run_lower && run[i]<=run_up)
	{	    
	    cout << setw(5) <<run[i]
		 <<"\t\t\t"<< setw(5) <<error_code[i]
		 <<"\t\t\t"<< setw(5) <<error[i].c_str()<<endl;
	}
    }
}
