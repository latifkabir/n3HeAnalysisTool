// Filename: fix.cc
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Thu Jun 11 12:13:54 2015 (-0400)
// URL: latifkabir.github.io
#include<iostream>
#include<fstream>
#include<iomanip>


using namespace std;


void n3HeRunStatus(int start_run,int last_run)
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

    int code0=0,code1=0,code_1=0,code_2=0,code_3=0,code_4=0,code_5=0,code_6=0;
    
    for(int i=0;i<index-1;i++)
    {
	if(error_code[i]==0)
	    code0++;
	if(error_code[i]==1)
	    code1++;
	if(error_code[i]==-1)
	    code_1++;
	if(error_code[i]==-2)
	    code_2++;
	if(error_code[i]==-3)
	    code_3++;
	if(error_code[i]==-4)
	    code_4++;
	if(error_code[i]==-5)
	    code_5++;
	if(error_code[i]==-6)
	    code_6++;
    }

    cout << "Total Runs on Tape:"<<index-1<<endl;
    cout << "GOOD Runs: "<<code1<<endl;
    cout << "Partial Last Event Runs: "<<code0<<endl;
    cout << "Header Issue Runs: "<<code_1<<endl;
    cout << "Partial or No beam Runs: "<<code_2<<endl;
    cout << "Diff number of entries Runs: "<<code_3<<endl;
    cout << "Short Runs: "<<code_4<<endl;
    cout << "Sync Issue Runs: "<<code_5<<endl;
    cout << "No Data file on basestar Runs: "<<code_6<<endl;

}
