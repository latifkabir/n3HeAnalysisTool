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
#include"HistoTree.h"

struct myData
{
    double asym[4][36];
}; 
void n3HeAnalyzer(int start_run,int stop_run) 
{
    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-0.5;  
    double x_up=0.5;   
    int skip_pls=9; //Skip dropped pulse including 1 before and 8 after (total 10 pulses are skipped).
    int run_counter=0;
    int n_adc=4; //Number of ADC
    int n_ch=36; //Number of Channels
    int level=0;  //Level of analysis accomplishment
    char file_name[200];

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
    int max_run;
    int min_run;
    int max_adc;
    int min_adc;
    int max_ch;
    int min_ch;
    int max_event;
    int min_event;
    ofstream asymmetry;


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
	    cout << "Now filling run number: "<<run<< "... ...\t\t"<<flush;
	}
	else
	{
	    cout << "The root file for run number :"<<run<<" does NOT exist.  SKIPPED " <<endl;
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
	// list->Print("all"); //Print all events that are dropped pulses for verification.
	int n_dpulses=list->GetN(); //Number of dropped pulses in the runNumber

	int currentDroppedPulse,nextDroppedPulse;
	currentDroppedPulse=nextDroppedPulse=list->GetEntry(0); //First dropped pulse
	int firstEvent=1;//Skip First event & start from second event as first one is just Run number flag.

	//=============Loop over all entries of the TTree or TChain to fill histogram or to do some analysis===============
	
	int nentries=b->GetEntries(); //Number of total events to be considered.
	//If the first pulse is a dopped pilse, it would need to be bypassed carefully to be consistant with rest of the algorithm.
	if(nextDroppedPulse==0)
	    firstEvent=0;

	
	for(int event=firstEvent;event< nentries;event++) 
	{
	    //========================Skip dropped pulses========================
	    if(event==nextDroppedPulse)
	    {
		currentDroppedPulse=nextDroppedPulse;
		nextDroppedPulse=list->Next();
		continue;   
	    }

	    //========================Skip pulses around dropped pulses========================
	    if(event==nextDroppedPulse-1 || (event > currentDroppedPulse && event < (currentDroppedPulse+9)))
		continue;

	    if(event%2==1)  //Fill only unique pairs (Any one of the two set of pairs)
	    {
		b->GetEntry(event);
		for(int i=0;i<n_adc;i++)
		{
		    for(int j=0;j<n_ch;j++)
		    {
			
			myHist[i][j]->Fill(-1*md.asym[i][j]); // x(-1) to fix the fact that SF on is spin down & SF off is spin up

			//Keep track of max and min asym for outliar
			if(abs(-1*md.asym[i][j]) > x_up)
			{
			    cout << "\nThe Run Number:"<<run << " has asymmmetry "<< -1*md.asym[i][j] << " which is greater than "<< x_up<<endl;
			    cout << "This happens for ADC: "<<i <<" Channel: "<<j<<" Event: "<<event<<endl;
			    cout << "You should investigate before proceeding" <<endl;
			    cout << "Aborting the analysis :) ... ... "<<endl;
			    myFile->Close();
			    asymmetry.close();
			    return;
			}
		    }
		}
	    }
	}

	myFile->Close();
	run_counter++;
	cout << " DONE!! "<<endl;
    }

    if(run_counter>0)
    {
	cout << "To save this analysis as Accomplishment, Enter non-zero level number:" <<endl;
	cin>>level;
	if(level!=0)
	    SaveHisto(level,start_run,stop_run,run_counter,myHist);
    }

    //========Draw the histo and extract desired param==================
    myHist[0][35]->Draw();
    // cout << "Mean:"<<myHist[0][5]->GetMean() <<endl;
    // cout << "RMS:"<<myHist[0][5]->GetRMS() <<endl;
    // cout << "Error in Mean:"<<myHist[0][5]->GetMeanError() <<endl;
    // cout << "Entries:"<<myHist[0][5]->GetEntries() <<endl;
    // cout << "MaxAsym:"<<MaxAsym <<endl;
    // cout << "MinAsym:"<<MinAsym <<endl;

    sprintf(file_name,ASYM_FILE,level);
    asymmetry.open(file_name);

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

    if(start_run<=0 || stop_run<=0 || start_run > stop_run)
    {
	cout<<"Please enter a valid run range"<<endl;
	return;
    }
    n3HeAnalyzer(start_run,stop_run);
}
