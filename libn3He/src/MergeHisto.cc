// Filename: MergeHisto.cc
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Sun Oct 25 22:30:49 2015 (-0400)
// URL: latifkabir.github.io
#include<iostream>
#include<fstream>
#include<iomanip>
#include<sstream>
using namespace std;

#include<TH1D.h>
#include<TString.h>
#include<TFile.h>
#include<TTree.h>
#include<TSystem.h>
#include"Constants.h"

void MergeHisto(int first_level, int last_level=0) 
{

    if(last_level==0)
	last_level=first_level;

    int n_bin=100;
    double x_low=-0.5;  
    double x_up=0.5;   
    int n_adc=4; //Number of ADC
    int n_ch=36; //Number of Channels
    int adc;
    int channel;
    char file_name[200];
    sprintf(file_name,ASYM_FILE,500);
    ofstream asymmetry(file_name);
    
    if(!asymmetry)
    {
	cout << "Unable to create asymmetry file" <<endl;
	return;
    }
  
   // Create a histogram array as buffer.
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

    for(int level=first_level;level<=last_level;level++)
    {
    	TString fName;  //Name of the root file
    	fName+=LEVEL_DATA;
    	fName+=level;
    	fName+=".root";

	bool status=gSystem->AccessPathName(fName); //Note bizzare return type convention
	if(status) 
	{
	    cout << "NO file found for requested level: "<<level<<" Exiting ... ..."<<endl;
	    return;
	}

    	TFile *f = new TFile(fName);
    	TTree *t = (TTree*)f->Get("T");
    	TH1D *hpx = 0;
  
  
    	t->SetBranchAddress("myHist",&hpx);
    
    	for(int entry=0;entry<144;entry++)
    	{
    	    adc=(int)(entry/36);
    	    channel=entry%36;

    	    t->GetEntry(entry);
	    
    	    myHist[adc][channel]->Add(hpx);
    	}
 
    	f->Close();
    	delete f;
    }

    for(int i=0;i<n_adc;i++)
    {
    	for(int j=0;j<n_ch;j++)
    	{
    	    asymmetry<<setw(10);
    	    asymmetry<<setprecision(8);
    	    asymmetry<<i<<"    "<<j<<"    "<<myHist[i][j]->GetMean()<<"    "<<myHist[i][j]->GetMeanError()<<"    "<<myHist[i][j]->GetRMS()<<endl;
    	}
    }
    cout << "Total Number of Entries :"<<myHist[0][0]->GetEntries() <<endl;

    asymmetry.close();
    delete[] myHist;
}
