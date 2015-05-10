// Filename: n3He data Analyzer version:1
// Description: calculate Asymmetry from root files.
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 10 14:50:00 2015 (-0400)
// URL: latifkabir.github.io

#include<iostream>
#include<fstream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cmath>
using namespace std;

#include<TFile.h>
#include<TTree.h>
#include<TH1D.h>
#include<TString.h>
#include<TEntryList.h>
#include<TBranch.h>
#include<TSystem.h>
#include"Constants.h"

struct myData
{
    double asym[4][36];
}; 
void n3HeAnalyzer(int start_run,int stop_run) 
{
    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-0.05; 
    double x_up=0.05;
    int skip_pls=9; //Skip dropped pulse including 1 before and 5 after.
    int run_counter=0;
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
    double MinAsym=0;
    double MaxAsym=0;

    ofstream asymmetry("asymmetry.txt");

    if(!asymmetry)
    {
	cout<<"Unable to create asymmetry.txt file"<<endl;
	return;
    }
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
	{
	    cout << "=========================================================" <<endl;
	    cout << "Now filling run number: "<<run<<endl;
	    cout << "=========================================================\n" <<endl;
	}
	else
	{
	    cout << "\n--------->The root file for run number :"<<run<<" does NOT exist<-------------" <<endl;
	    cout << "--------->Skipping the run number : "<<run <<"<-------------\n"<<endl;
	    continue;
	}
	TFile *myFile = TFile::Open(fName);
	// if(!myFile)           //This approach still prints annoying error message, but works
	// if(!myFile->IsOpen())  // Thought this should also work, but crashes
	// {
	//     cout << "The root file for run:"<<run<<" does NOT exist" <<endl;
	//     continue;
	// }
	TTree *T=(TTree*)myFile->Get("T");

	//=================Create the buffer to hold event entries===================

	myData md;
	TBranch *b=T->GetBranch("asym");
	b->SetAddress(&md.asym[0][0]);

	//================== Get the drop pulses information=======================

	T->Draw(">>list_temp","sumd[0]<2000","entrylist");
	TEntryList *list = (TEntryList*)gDirectory->Get("list_temp");
	// list->Print("all"); //Print all events that are dropped pulses.
	int n_dpulses=list->GetN(); //Number of dropped pulses in the run
	int d_counter=0; //Counter for dropped pulses.

	//=============Loop over all entries of the TTree or TChain to fill histogram or to do some analysis===============

	int event=1; //Skip First event & start from second event as first one is just Run number flag.
	int nentries=(b->GetEntries()-1); //Number of total events to be considered. Skip last one.
	int max_iter=200; // Maximum iteration for adjacent dropped pulses.

	while(event < nentries) 
	{
	    //========================Skip pulses around dropped pulses========================

	    if(d_counter<n_dpulses)
	    {
		if((event+1)==list->GetEntry(d_counter))  //Check if next pulse is a dropped pulse
		{
		    for(int k=0;k<max_iter;k++)   //Most likely case is k=0
		    {
			if((event+1)==list->GetEntry(n_dpulses-1))
			{
			    if(list->GetEntry(n_dpulses-1)+skip_pls < nentries)
				event=list->GetEntry(n_dpulses-1)+skip_pls;
			    else
				event=-1;
			    break;
			}
			if((list->GetEntry(d_counter+k)+skip_pls) < list->GetEntry(d_counter+k+1))
			{
			    event=list->GetEntry(d_counter+k)+skip_pls;
			    d_counter+=(k+1);
			    break;
			}
			if(k>(max_iter-2))
			{
			    cout << "To Many Dropped Pulses." <<endl;
			    cout << "Skipping Rest of the run: "<<run <<endl;
			    event=-1;
			}
		    }
		}
	    }

	    if(event==-1)
		break;

	    if(event%2==1)  //Fill only unique pairs (Any one of the two set of pairs)
	    {
		b->GetEntry(event);
		for(int i=0;i<n_adc;i++)
		{
		    for(int j=0;j<n_ch;j++)
		    {
			myHist[i][j]->Fill(md.asym[i][j]);
			if(md.asym[i][j]>MaxAsym)
			    MaxAsym=md.asym[i][j];
			if(md.asym[i][j]<MinAsym)
			    MinAsym=md.asym[i][j];
		    }
		}
	    }
	    event++;
	}

	myFile->Close();
	run_counter++;
	cout << "Done with run number: \n"<<run<<endl;
    }

    //=================Check for Histogram range =========================

    if(abs(MaxAsym) > x_up || MinAsym < x_low)
    {
    	cout << "MaxAsym:"<<MaxAsym <<endl;
    	cout << "MinAsym:"<<MinAsym <<endl;
    	cout << "x_up:"<<x_up <<endl;
    	cout << "x_low:"<<x_low <<endl;
    	cout<<"Please adjust the range of the histogram to cover all events"<<endl;
    }
    //========Draw the histo and extract desired param==================
    myHist[0][1]->Draw();
    // cout << "Mean:"<<myHist[0][5]->GetMean() <<endl;
    // cout << "RMS:"<<myHist[0][5]->GetRMS() <<endl;
    // cout << "Error in Mean:"<<myHist[0][5]->GetMeanError() <<endl;
    // cout << "Entries:"<<myHist[0][5]->GetEntries() <<endl;
    // cout << "MaxAsym:"<<MaxAsym <<endl;
    // cout << "MinAsym:"<<MinAsym <<endl;

    for(int i=0;i<n_adc;i++)
    {
	for(int j=0;j<n_ch;j++)
	{
	    asymmetry<<setw(10);
	    asymmetry<<setprecision(8);
	    asymmetry<<i<<"    "<<"    "<<j<<"    "<<myHist[i][j]->GetMean()<<"    "<<myHist[i][j]->GetMeanError()<<"    "<<myHist[i][j]->GetRMS()<<endl;
	}
    }

    cout<<"Start Run: "<<start_run<<endl;
    cout << "Stop Run: "<<stop_run <<endl;
    cout << "Number of Runs Analyzed: "<<run_counter<<endl;
    cout << "Number of skipped or Bad runs: "<<(stop_run-start_run+1-run_counter)<<endl;
    cout << "Number of total entries considered: "<<myHist[0][0]->GetEntries()<<endl;

    asymmetry.close();
    delete[] myHist;
}

void Analyzer(int start_run=0,int stop_run=0) 
{

    if(start_run<=0 || stop_run<=0)
    {
	cout<<"Please enter a valid run range"<<endl;
	return;
    }
    n3HeAnalyzer(start_run,stop_run);
}
