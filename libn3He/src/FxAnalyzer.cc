// Filename: FxAnalyzer.cc
// Description: n3He Asymmetry analyzer with flexibility
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Thu Jun 11 12:13:54 2015 (-0400)
// URL: latifkabir.github.io
#include<iostream>
#include<fstream>
#include<iomanip>
#include<sstream>
#include<cmath>
using namespace std;
#include<TTree.h>
#include<TH1D.h>
#include<TString.h>
#include<TEntryList.h>
#include<TBranch.h>
#include<TSystem.h>
#include<TDirectory.h>
#include<TCut.h>
#include"Constants.h"
#include"HistoTree.h"
#include"TTreeAsymmetry.h"

struct myFxData
{
    double asym[4][36];
}; 
void n3HeFxAnalyzer(int start_run,int stop_run,int ctb_i,int ctb_f,int skip_pulses,int cut_off,double hist_range) 
{
    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-1*hist_range;  
    double x_up=hist_range;   
    int skip_pls=skip_pulses; //Skip dropped pulse including 1 before and 8 after (total 10 pulses are skipped).
    int run_counter=0;
    int n_adc=4; //Number of ADC
    int n_ch=36; //Number of Channels
    int level=0;  //Level of analysis accomplishment
    char file_name[200];

   //=========================Load list for GOOD runs=========
    ifstream inFile("/mnt/idata05/summary/runListUD.txt");
    if(!inFile)
    {
	cout<<"Could NOT locate data file"<<endl;
	return;
    }

    int run;
    int error_code;
    string error;
    int index=0;
    int n3he_first=17500; //First available n3He run number  
    int n3he_last=62000;  //Last available n3He run number 

    if(start_run==0)
    {
	cout << "Syntax: RunStatus(lower_range,upper_range)" <<endl;
	cout << "Available Run range "<<n3he_first<<" to "<<n3he_last<<endl;

	return;
    }

    if(start_run!=0 && stop_run==0)
	stop_run=start_run;

    if(start_run<n3he_first || start_run > n3he_last || stop_run<n3he_first || stop_run > n3he_last)
    {
	cout<<"Run Status for run number below "<<n3he_first<<" or above "<<n3he_last<<" is NOT available"<<endl;
	return;
    }


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


    while(!inFile.eof())
    {
	inFile>>run>>error_code>>error;

	if(run>=start_run && run<=stop_run && error_code>-1)
	{	    
	    TTreeAsymmetry tr(run,false,ctb_i,ctb_f);
	    TTree *T=tr.MakeTree();
	    if(tr.error_code<0)
		continue;
	    //=================Create the buffer to hold event entries===================	    
	    myFxData md;
	    TBranch *b=T->GetBranch("asym");
	    b->SetAddress(&md.asym[0][0]);

	    //================== Get the drop pulses information=======================
	    TCut droppedCut = Form("sumd[0]<%i",cut_off); 
	    T->Draw(">>list_temp",droppedCut,"entrylist");
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
		if(event==nextDroppedPulse-1 || (event > currentDroppedPulse && event < (currentDroppedPulse+skip_pls)))
		    continue;

		if(event<8)  //Skip first 8 pulses of the run in case there was a dropped pulse just before start of the run.
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
				cout << "Aborting the analysis :( ... ... "<<endl;
				asymmetry.close();
				return;
			    }
			}
		    }
		}
	    }

	    run_counter++;
	    cout << " DONE!! "<<endl;

	}
	if(run>=stop_run)
	    break;
    }
    inFile.close();

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


void FxAnalyzer(int start_run=0,int stop_run=0,int ctb_i=0,int ctb_f=48,int skip_pls=9,int cut_off=2000, double hist_range=0.5) 
{

    if(start_run<=0 || stop_run<=0 || start_run > stop_run)
    {
	cout<<"Please enter a valid run range"<<endl;
	return;
    }

    cout << "==============Printing for record================" <<endl;
    cout << "Start run: "<<start_run<<" Stop Run: "<<stop_run<<endl;
    cout << "First time bin: "<<ctb_i<<" Last time bin: "<<ctb_f<<endl;
    cout << "Pulses to be skipped after dropped : "<<skip_pls <<endl;
    cout << "Cut off for dropped or low beam pulse: "<<cut_off<<" volts" <<endl;
    cout << "Range for histogram: +/-"<<hist_range<<endl;
    cout << "=================================================" <<endl<<endl;

    if(ctb_i<0 || ctb_f<0 || ctb_f>49 || skip_pls<0 || skip_pls > 200 || cut_off<0 || cut_off >5000 || hist_range<0) 
    {
	cout << "One or more parameters have abnormal values. Check the parameters" <<endl;
	return;
    }

    n3HeFxAnalyzer(start_run,stop_run,ctb_i,ctb_f,skip_pls,cut_off,hist_range);
}
