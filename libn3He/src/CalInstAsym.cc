// Filename: CalInstAsym.cc
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Thu Nov 19 17:29:02 2015 (-0500)
// URL: latifkabir.github.io


#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<TH1D.h>
#include"InstAsym.h"

using namespace std;




void CalInstAsym(int start_run=0,int stop_run=0)
{
    if(start_run<=0 || stop_run<=0)
    {
	cout << "Please enter a valid range for the run number" <<endl;
	return;
    }

    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-0.5;  
    double x_up=0.5;   
    int n_adc=4; //Number of ADC
    int n_ch=36; //Number of Channels

 // =============Declare 4x36 Histograms in the heap to be filled=================

    TH1D ***myHist;
    myHist=new TH1D**[n_adc];
    for(int i=0;i<n_adc;i++)
    	myHist[i]=new TH1D*[n_ch];

    for (int i = 0; i < n_adc; i++) 
    {
	for(int j=0;j<n_ch;j++)
	{
	    stringstream h_name;
	    h_name << "AsymHisto:" << i<<"-";
	    h_name << j;
	    myHist[i][j] = new TH1D(h_name.str().c_str(),(h_name.str() + ";asymmetry;count").c_str(),n_bin, x_low, x_up);
	}
    }

    for(int run=start_run;run<=stop_run;run++)
    {
    	cout << "Now Filling run number:"<<run <<"... ..."<<endl;
    	InstAsym *t=new InstAsym(run);
    	t->CalAsym(myHist,x_up);
    	cout << "Done !!! "<<endl;
	delete t;
    }

    ofstream asymmetry;
    asymmetry.open("/home/kabir/GIT/n3HeAnalysisTool/LevelData/InstAsym.txt");

    if(!asymmetry)
    {
	cout<<"Unable to create asymmetry record file"<<endl;
	return;
    }


    for(int i=0;i<n_adc;i++)
    {
	for(int j=0;j<n_ch;j++)
	{
	    asymmetry<<setw(10);
	    asymmetry<<setprecision(8);
	    asymmetry<<i<<"    "<<"    "<<j<<"    "<<myHist[i][j]->GetMean()<<"    "<<myHist[i][j]->GetMeanError()<<"    "<<myHist[i][j]->GetRMS()<<endl;
	}
    }

    asymmetry.close();
    delete[] myHist;
}
