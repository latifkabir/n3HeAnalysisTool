// Filename: beamPowerDist.cc
// Description: Make histogram for beam power distribution
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Wed Jul  1 14:43:16 2015 (-0400)
// URL: latifkabir.github.io

#include<iostream>
using namespace std;

#include<TH1D.h>
#include<TString.h>
#include<TTree.h>
#include<TFile.h>
#include<TSystem.h>
#include"Constants.h"

void BeamPowerDist(int start_run,int stop_run)
{

    TH1D *power_hist=new TH1D("power_hist","power_hist",100,600,1500);
    double ave_power=0;
    
    for(int run=start_run;run<=stop_run;run++)
    {
	//Generate the root file name for desired run
	TString fName;
	fName=ROOT_FILE_PATH;
	fName+="run-";
	fName+=run;
	fName+=".root";
	
	//============ Open the file containing the tree==================
	bool status=gSystem->AccessPathName(fName); //Note bizzare return type convention
	if(!status) 
	    cout << "Now Reading run number: "<<run<<flush;
	else
	{
	    cout << "The root file for run number :"<<run<<" does NOT exist.  SKIPPED" <<endl;
	    continue;
	}
	TFile *myFile = TFile::Open(fName);
	TTree *t=(TTree*)myFile->Get("T");
	// T->Print();
	TH1D *h=new TH1D("h","h",100,0,6500);
	t->Draw("sumd[0]>>h","","goff");
	// h->Draw();
	ave_power=(850.0/3500.0)*h->GetMean(1); // Assume that 3500 Volt  per pulse (i.e. 3500/1624 =2.15 volt per tbin) corresponds to 850 KWatt power
	cout << " Mean Power:"<<ave_power<<" KWatt or "<<h->GetMean(1)<<" Volts" <<endl;
	power_hist->Fill(ave_power);
	myFile->Close();
    }
    power_hist->Draw();
}



