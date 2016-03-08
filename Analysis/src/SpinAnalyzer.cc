// Filename: n3He data SpinAnalyzer version:1
// Description: calculate Asymmetry from root files.
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 10 14:50:00 2015 (-0400)
// URL: latifkabir.github.io

#include<iostream>
#include<fstream>
#include<iomanip>
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
#include<TCut.h>
#include<TCanvas.h>
#include<TGraph.h>
#include"Constants.h"

int up_runs=0;
int down_runs=0;
int up_pairs=0;
int down_pairs=0;

struct myData
{
    double asym[4][36];
}; 
double n3HeSpinAnalyzer(int start_run,int stop_run,int skip_pulses=9,int cut_off=2000,int option=0) 
{
    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-0.5;  
    double x_up=0.5;   
    int skip_pls=skip_pulses; //Skip dropped pulse including 1 before and 8 after (total 10 pulses are skipped).
    int run_counter=0;
    int run_counter_up=0;
    int run_counter_down=0;
    int n_adc=4; //Number of ADC
    int n_ch=36; //Number of Channels
    int level=0;  //Level of analysis accomplishment
    char file_name[200];
    int tb_i=0;
    int tb_f=48;
    double sumd[2]; //Buffer for sumd branch
    int MaxDropped=43; //Maximum number of dropped pulses allowed to be considered as Good run.
    double spin_cutoff=400; //Sum of absolute value of RFSF signal in Volts.
    int spinFlag=0;    
    double asym_diff=0;
    double sig_sq=0;
    double chi_sq=0;

    // =============Declare 4x36 Histograms in the heap to be filled=================
    TH1D ***myHistUp;
    TH1D ***myHistDown;
    myHistUp=new TH1D**[n_adc];
    for(int i=0;i<n_adc;i++)
    	myHistUp[i]=new TH1D*[n_ch];

    for (int i = 0; i < n_adc; i++) 
    {
	for(int j=0;j<n_ch;j++)
	{
	    stringstream h_name;
	    h_name << "AsymHisto:" << i<<"-";
	    h_name << j;
	    myHistUp[i][j] = new TH1D(h_name.str().c_str(),(h_name.str() + ";asymmetry;count").c_str(),n_bin, x_low, x_up);
	}
    }

    myHistDown=new TH1D**[n_adc];
    for(int i=0;i<n_adc;i++)
    	myHistDown[i]=new TH1D*[n_ch];

    for (int i = 0; i < n_adc; i++) 
    {
	for(int j=0;j<n_ch;j++)
	{
	    stringstream h_name;
	    h_name << "AsymHisto2:" << i<<"-";
	    h_name << j;
	    myHistDown[i][j] = new TH1D(h_name.str().c_str(),(h_name.str() + ";asymmetry;count").c_str(),n_bin, x_low, x_up);
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
	TTree *T=(TTree*)myFile->Get("T");

	//=================Create the buffer to hold event entries===================

	myData md, clean;
	TBranch *b=T->GetBranch("asym");
	TBranch *d=T->GetBranch("sumd");
	TBranch *c=T->GetBranch("sumc");
	b->SetAddress(&md.asym[0][0]);
	d->SetAddress(&sumd[0]);
	c->SetAddress(&clean.asym[0][0]); // In this line asym to be interpreted as data.

	//================== Get the drop pulses information=======================
	TCut droppedCut = Form("sumd[0]<%i",cut_off);
	T->Draw(">>list_temp",droppedCut,"entrylist");
	TEntryList *list = (TEntryList*)gDirectory->Get("list_temp");
	// list->Print("all"); //Print all events that are dropped pulses for verification.
	int n_dpulses=list->GetN(); //Number of dropped pulses in the runNumber

	if(n_dpulses>MaxDropped)
	{
	    cout << " TOO MANY DROPPED. SKIPPED "<<endl;
	    myFile->Close();
	    continue;
	}
	int currentDroppedPulse,nextDroppedPulse;
	currentDroppedPulse=nextDroppedPulse=list->GetEntry(0); //First dropped pulse
	int firstEvent=0;

        //==========Check if first dropped pulse event if spin up or down ===============================
	d->GetEntry(currentDroppedPulse);
	if(sumd[1]>spin_cutoff)
	{
	    cout<<"The spin is Up."<<endl;
	    spinFlag=1;
	    run_counter_up++;
	}
	else
	{
	    cout<<"The spin is Down."<<endl;
	    spinFlag=-1;
	    run_counter_down++;
	}

	
	//=============Loop over all entries of the TTree or TChain to fill histogram or to do some analysis===============
	
	int nentries=b->GetEntries(); //Number of total events to be considered.
	
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
	    if(event==nextDroppedPulse-1 || (event > currentDroppedPulse && event < (currentDroppedPulse+skip_pls)))
		continue;

	    if(event<8)  //Skip first 8 pulses of the run in case there was a dropped pulse just before start of the run. Also first event is just run number flag
		continue;
	    
	    if(event%2==1)  //Fill only unique pairs (Any one of the two set of pairs)
	    {
		b->GetEntry(event);
		for(int i=0;i<n_adc;i++)
		{
		    for(int j=0;j<n_ch;j++)
		    {
			if(spinFlag==1)	
			    myHistUp[i][j]->Fill(-1*md.asym[i][j]); // x(-1) to fix the fact that SF on is spin down & SF off is spin up
			else if(spinFlag==-1) 
			    myHistDown[i][j]->Fill(-1*md.asym[i][j]); // x(-1) to fix the fact that SF on is spin down & SF off is spin up

			//Keep track of max and min asym for outliar
			if(abs(-1*md.asym[i][j]) > x_up)
			{
			    cout << "\nThe Run Number:"<<run << " has asymmmetry "<< -1*md.asym[i][j] << " which is greater than "<< x_up<<endl;
			    cout << "This happens for ADC: "<<i <<" Channel: "<<j<<" Event: "<<event<<endl;
			    cout << "You should investigate before proceeding" <<endl;
			    cout << "Aborting the analysis :) ... ... "<<endl;
			    myFile->Close();
			    asymmetry.close();
			    return -1;
			}
		    }
		}
	    }
	}

	myFile->Close();
	run_counter++;
	cout << " DONE!! "<<endl;
    }


    sprintf(file_name,ASYM_FILE,level);
    asymmetry.open(file_name,ios::app);

    if(!asymmetry)
    {
	cout<<"Unable to create asymmetry record file"<<endl;
	return -1;
    }


    for(int i=0;i<n_adc;i++)
    {
	for(int j=0;j<n_ch;j++)
	{
	    asym_diff= pow( ( myHistUp[i][j]->GetMean() - myHistDown[i][j]->GetMean() ) ,2);
	    sig_sq = pow(myHistUp[i][j]->GetMeanError(),2) + pow(myHistDown[i][j]->GetMeanError(),2); // sig_sq actually delta_A (sig/sqrt(N) )
	    if(sig_sq==0)
	    {
		cout << "Zero in the denominator !!! Exiting." <<endl;
		return -1;
	    }
	    chi_sq = chi_sq+asym_diff/sig_sq;
	}
    }
    

    up_runs=run_counter_up;
    down_runs=run_counter_down;
    up_pairs=myHistUp[0][0]->GetEntries();
    down_pairs= myHistDown[0][0]->GetEntries();
    //===========================Do closing task or free up the memory======================

    asymmetry<<setw(10);
    asymmetry<<setprecision(8);
    asymmetry<<skip_pulses<<"                         "<<chi_sq/144<<endl;

    asymmetry.close();


    //Deleting these will not allow draw the hist 
    for (int i = 0; i < n_adc; i++) 
    {
    	for(int j=0;j<n_ch;j++)
    	    delete myHistUp[i][j];
    	delete[] myHistUp[i];
    }
    delete[] myHistUp;

    for (int i = 0; i < n_adc; i++) 
    {
    	for(int j=0;j<n_ch;j++)
    	    delete myHistDown[i][j];
    	delete[] myHistDown[i];
    }
    delete[] myHistDown;


    
    return chi_sq/144;
}

void SpinAnalyzer(int start_run=0,int stop_run=0,int max_skip_pulses=9,int cut_off=2000,int option=0) 
{


    if(start_run<=0 || stop_run<=0 || start_run > stop_run)
    {
	cout<<"Please enter a valid run range"<<endl;
	return;
    }

    TGraph *gr=new TGraph();
    double chi_sq;
    int nPoint=0;
    for(int i=3;i<=max_skip_pulses;i++)
    {
	cout << "===================Now starting analysis for "<<i<<" post dropped pulses to be skipped===========" <<endl;
	chi_sq=n3HeSpinAnalyzer(start_run,stop_run,i,cut_off,option);
	if(chi_sq==-1)
	{
	    cout << "Error occured !!!" <<endl;
	    return;
	}

	gr->SetPoint(nPoint,i,chi_sq);
	nPoint++;
    }

    cout << "\n===============Printing for record============"<<endl;
    cout << "Start run: "<<start_run <<"  Stop run: "<<stop_run<<endl;
    cout << "Cut off for dropped or low beam pulse: "<<cut_off<<" Volts" <<endl;
    cout << "Beam Asymmetry cut option: "<<option<<endl;
    cout << "Max dropped allowed in a run: "<<43<<endl;
    cout << "First time bin: "<< 0<<" Last time bin:"<<48<<endl;
    cout << "Up spin runs: "<< up_runs<<" Down spins runs: "<<down_runs<<endl;
    cout << "Up pairs: "<< up_pairs<<" Down pairs: "<<down_pairs<<endl;
    cout << "==============================================" <<endl<<endl;

    gr->Draw("APL*");
}


// Option==0 : No beam asymmetry cut
// Option==1: 0.1% M1 asymmetry cut 
// Option==2: 0.1% beam asymmetry cut 
