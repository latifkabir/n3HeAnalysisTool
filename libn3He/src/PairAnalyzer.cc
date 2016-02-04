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
#include<TCut.h>
#include<TCanvas.h>
#include"Constants.h"
#include"HistoTree.h"

struct myData
{
    double asym[4][36];
}; 
void n3HePainAnalyzer(int start_run,int stop_run,int skip_pulses,int cut_off,int option) 
{
    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-1.5;  
    double x_up=1.5;   
    int skip_pls=skip_pulses; //Skip dropped pulse including 1 before and 8 after (total 10 pulses are skipped).
    int run_counter=0;
    int n_adc=4; //Number of ADC
    int n_ch=36; //Number of Channels
    int n_ch_p=16; //Number of Channel pairs per DAQ (reduced) in pair analysis.
    int level=0;  //Level of analysis accomplishment
    char file_name[200];
    int tb_i=0;
    int tb_f=48;
    double m1AsymCutOff=0.001; //Impose cut based on beamasymmetry cutoff=0.1%.
    double beamAsymCutOff=0.001; //Impose cut based on beamasymmetry cutoff=0.1%.
    double sumd[2]; //Buffer for sumd branch
    double currentSumd;
    double previousSumd;
    double currentSumc;
    double previousSumc;
    double m1Asymmetry;
    double beamAsymmetry;
    int MaxDropped=100; //Maximum number of dropped pulses allowed to be considered as Good run.
    double upperConj=0.0;
    double lowerConj=0.0;

    TH1D *m1Asym =new TH1D("M1 Asymmetry","M1 Asymmetry",3000,-1.2,1.2);
    TH1D *selm1Asym =new TH1D("Sel M1 Asymmetry","Sel M1 Asymmetry",1000,-1.2*m1AsymCutOff,1.2*m1AsymCutOff);
    TH1D *beamAsym =new TH1D("Beam Asymmetry","Beam Asymmetry",3000,-1.2,1.2);
    TH1D *selBeamAsym =new TH1D("Sel Beam Asymmetry","Sel Beam Asymmetry",1000,-1*beamAsymCutOff,beamAsymCutOff);

    // =============Declare 4x36 Histograms in the heap to be filled=================
    TH1D ***myHist;
    myHist=new TH1D**[n_adc];
    for(int i=0;i<n_adc;i++)
    	myHist[i]=new TH1D*[n_ch_p];

    for (int i = 0; i < n_adc; i++) 
    {
	for(int j=0;j<n_ch_p;j++)
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

	//=============Loop over all entries of the TTree or TChain to fill histogram or to do some analysis===============
	
	int nentries=b->GetEntries(); //Number of total events to be considered.
	
	for(int event=firstEvent;event< nentries;event++) 
	{
	    if(option==1)
	    {
		//======================= Calculate M1 monitor Asymmetry===================
		previousSumd=sumd[0];
		d->GetEntry(event);
		currentSumd=sumd[0];
		m1Asymmetry=(currentSumd-previousSumd)/(currentSumd+previousSumd); // Note event=0 Asymmetry is just grarbage.
		if(event%2==1)  
		    m1Asym->Fill(m1Asymmetry);
	    }
	    if(option==2)
	    {
		//=====================Calculate beam asymmetry from sum over detectors signal================
		previousSumc=currentSumc;
		c->GetEntry(event);
		currentSumc=0;
		for(int i=0;i<n_adc;i++)
		{
		    for(int j=0;j<n_ch;j++)     
			currentSumc+=clean.asym[i][j]; // asym in this line to be interpreted as just data.		
		}	
		beamAsymmetry=(currentSumc-previousSumc)/(currentSumc+previousSumc); // Note event=0 Asymmetry is just grarbage.
		if(event%2==1)  
		    beamAsym->Fill(beamAsymmetry);
	    }
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

	    if(option==1)
	    {
		if(abs(m1Asymmetry)>m1AsymCutOff)  //Impose cut based on M1 asymmetry cutoff=0.1%.
		    continue;
	    }
	    if(option==2)
	    {
		if(abs(beamAsymmetry)>beamAsymCutOff)  //Impose cut based on beam asymmetry cutoff=0.1%.
		    continue;
	    }
	    
	    if(event%2==1)  //Fill only unique pairs (Any one of the two set of pairs)
	    {
		b->GetEntry(event);
		for(int i=0;i<n_adc;i++)
		{
		    for(int j=0;j<n_ch_p;j++)
		    {
			int k;
			if(j>=0 && j<4)
			{
			    k=j;
			    upperConj=-1*md.asym[i][8-k];// x(-1) to fix the fact that SF on is spin down & SF off is spin up
			    lowerConj=-1*md.asym[i][k];
			}
			if(j>=4 && j<8)
			{
			    k=j+5;
			    upperConj=-1*md.asym[i][26-k];
			    lowerConj=-1*md.asym[i][k];
			}
			if(j>=8 && j<12)
			{
			    k=j+10;
			    upperConj=-1*md.asym[i][44-k];
			    lowerConj=-1*md.asym[i][k];
			}
			if(j>=12 && j<16)
			{
			    k=j+15; 
			    upperConj=-1*md.asym[i][62-k];
			    lowerConj=-1*md.asym[i][k];
			}
			 
			myHist[i][j]->Fill(upperConj-lowerConj); 

			//Keep track of max and min asym for outliar
			if(abs(upperConj-lowerConj) > x_up)
			{
			    cout << "\nThe Run Number:"<<run << " has asymmmetry difference "<< (upperConj-lowerConj) << " which is greater than "<< x_up<<endl;
			    cout << "This happens for ADC: "<<i <<" Channel Pair: "<<j<<" Event: "<<event<<endl;
			    cout << "You should investigate before proceeding" <<endl;
			    cout << "Aborting the analysis :) ... ... "<<endl;
			    myFile->Close();
			    asymmetry.close();
			    return;
			}
		    }
		}
		if(option==1)
		    selm1Asym->Fill(m1Asymmetry);
		if(option==2)
		    selBeamAsym->Fill(beamAsymmetry);
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
	{
	    // Read last level number and generate level number for current analysis.
	    fstream levelRecord("/home/kabir/GIT/n3HeAnalysisTool/LevelData/levelRecord.txt");
	    if(levelRecord)
	    {
		levelRecord>>level;
		level++;
		levelRecord.seekg(0,ios::beg);
		levelRecord<<level;	
		levelRecord.close();
	    }
	    // SaveHisto(level,start_run,stop_run,run_counter,myHist);
	    ofstream runList("/home/kabir/GIT/n3HeAnalysisTool/LevelData/AnalyzedRunList.txt",ofstream::app);
	    if(runList)
	    {
		runList<<level<<"\t\t"<<start_run<<"\t\t"<<stop_run<<"\t\t"<<run_counter<<"\t\t"<<skip_pulses<<"\t\t"<<cut_off<<"\t\t"<<option<<"\t\t"<<MaxDropped<<"\t\t"<<tb_i<<"\t\t"<<tb_f<<endl;
		runList.close();
	    }
	}
    }

    //========Draw the histo and extract desired param==================
    TCanvas *c1= new TCanvas();
    myHist[0][15]->Draw();
    if(option==1)
    {
	TCanvas *c2= new TCanvas();
	m1Asym->Draw();
	// selm1Asym->SetLineColor(kRed);
	TCanvas *c3= new TCanvas();
	// selm1Asym->Draw("sames");        //Unlike "same", "sames" will force showing stat for both histo
	selm1Asym->Draw();        //Unlike "same", "sames" will force showing stat for both histo
    }
    if(option==2)	
    {
	// beamAsym->SetLineColor(kGreen);
	TCanvas *c4= new TCanvas();
	// beamAsym->Draw("sames");
	beamAsym->Draw();
	// selBeamAsym->SetLineColor(kYellow);
	TCanvas *c5= new TCanvas();
	//selBeamAsym->Draw("sames");
	selBeamAsym->Draw();
    }
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
	for(int j=0;j<n_ch_p;j++)
	{
	    asymmetry<<setw(10);
	    asymmetry<<setprecision(8);
	    asymmetry<<i<<"    "<<"    "<<j<<"    "<<myHist[i][j]->GetMean()<<"    "<<myHist[i][j]->GetMeanError()<<"    "<<myHist[i][j]->GetRMS()<<endl;
	}
    }
    
    cout << "Saved as Level: "<<level<<endl;
    cout << "Start Run: "<<start_run<<endl;
    cout << "Stop Run: "<<stop_run <<endl;
    cout << "Number of Runs Analyzed: "<<run_counter<<endl;
    cout << "Number of skipped or Bad runs: "<<(stop_run-start_run+1-run_counter)<<endl;
    cout << "Number of total entries considered: "<<myHist[0][0]->GetEntries()<<endl;


    //===========================Do closing task or free up the memory======================
    asymmetry.close();

    //Deleting these will not allow draw the hist 
    // delete m1Asym;
    // delete selm1Asym;
    // // just delete[] myHist; would not work. The thumb of rule is that you delete as many time as many you introduced new.
    // for (int i = 0; i < n_adc; i++) 
    // {
    // 	for(int j=0;j<n_ch;j++)
    // 	    delete myHist[i][j];
    // 	delete[] myHist[i];
    // }
    // delete[] myHist;
}

void PairAnalyzer(int start_run,int stop_run,int skip_pulses,int cut_off,int option) 
{

    cout << "\n===============Printing for record============"<<endl;
    cout << "Start run: "<<start_run <<"  Stop run: "<<stop_run<<endl;
    cout << "Pulses to be skipped after dropped: "<<skip_pulses <<endl;
    cout << "Cut off for dropped or low beam pulse: "<<cut_off<<" Volts" <<endl;
    cout << "Beam Asymmetry cut option: "<<option<<endl;
    cout << "Max dropped allowed in a run: "<<100<<endl;
    cout << "First time bin: "<< 0<<" Last time bin:"<<48<<endl;
    cout << "==============================================" <<endl<<endl;

    if(start_run<=0 || stop_run<=0 || start_run > stop_run)
    {
	cout<<"Please enter a valid run range"<<endl;
	return;
    }
    n3HePainAnalyzer(start_run,stop_run,skip_pulses,cut_off,option);

    cout << "\n===============Printing Again for record============"<<endl;
    cout << "Start run: "<<start_run <<"  Stop run: "<<stop_run<<endl;
    cout << "Pulses to be skipped after dropped: "<<skip_pulses <<endl;
    cout << "Cut off for dropped or low beam pulse: "<<cut_off<<" Volts" <<endl;
    cout << "Beam Asymmetry cut option: "<<option<<endl;
    cout << "Max dropped allowed in a run: "<<100<<endl;
    cout << "First time bin: "<< 0<<" Last time bin:"<<48<<endl;
    cout << "==============================================" <<endl<<endl;

}


// Option==0 : No beam asymmetry cut
// Option==1: 0.1% M1 asymmetry cut 
// Option==2: 0.1% beam asymmetry cut 

