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

    int run;
    int error_code;
    string error;
    int index=0;
    int n3he_first=14785; //First available n3He run number  
    int n3he_last=61000;  //Last available n3He run number 

    if(start_run==0)
    {
	cout << "Syntax: RunStatus(lower_range,upper_range)" <<endl;
	cout << "Available Run range "<<n3he_first<<" to "<<n3he_last<<endl;

	return;
    }

    if(start_run!=0 && last_run==0)
	last_run=start_run;

    if(start_run<n3he_first || start_run > n3he_last || last_run<n3he_first || last_run > n3he_last)
    {
	cout<<"Run Status for run number below "<<n3he_first<<" or above "<<n3he_last<<" is NOT available"<<endl;
	return;
    }
    
    cout<<"Run Number"<<"\t\t"<<"Error Code"<<"\t\t"<<"Run Status"<<endl;
    cout<<"==========="<<"\t\t"<<"==========="<<"\t\t"<<"==========="<<endl;

    while(!inFile.eof())
    {
	inFile>>run>>error_code>>error;

	if(run>=start_run && run<=last_run)
	{	    
	    cout << setw(5) <<run
		 <<"\t\t\t"<< setw(5) <<error_code
		 <<"\t\t\t"<< setw(5) <<error.c_str()<<endl;
	}
	index++;

	if(run>=last_run)
	    break;
    }

    inFile.close();
}
