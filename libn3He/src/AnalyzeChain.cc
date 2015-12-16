// Filename: n3HeChain.cc
// Description: Making TChain out of n3He root data files. Very Simple Analysis Version
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Sat May 23 16:12:53 2015 (-0400)
// URL: latifkabir.github.io

#include<fstream>
#include<iostream>
#include<iomanip>
using namespace std;

#include<TChain.h>
#include<TSystem.h>
#include<TROOT.h>
#include<TEventList.h>
#include<TStopwatch.h>
#include<TH1D.h>
#include<TDirectory.h>
#include"Constants.h"
#include"AnalyzeChain.h"


void AnalyzeChain(int start_run,int stop_run)
{

    if(stop_run==0)
	stop_run=start_run;

    TChain chain("T");
    int count_chain=0;
    TStopwatch time;
    // gROOT->SetBatch();
    time.Start();

    ofstream asymmetry("asymmetry.txt");

    TH1D *h=new TH1D("h","h",100,-0.5,0.5);

    if(!asymmetry)
    {
	cout<<"Unable to create asymmetry.txt file"<<endl;
	return;
    }

    for(int run=start_run;run<=stop_run;run++)
    {
	TString fName;
	fName=ROOT_FILE_PATH;
	fName+="run-";
	fName+=run;
	fName+=".root";
 
	//============ Open the file containing the tree==================
	bool status=gSystem->AccessPathName(fName); //Note bizzare return type convention
	if(status) 
	{
	    cout << "\n--------->The root file for run number :"<<run<<" does NOT exist<-------------" <<endl;
	    cout << "--------->Skipping the run number : "<<run <<"<-------------\n"<<endl;
	    continue;
	}
	//========Add n3He root summary data files to the chain=================
	chain.Add(fName);
	count_chain++;
    }
    cout << "Total Root files added in the Chain:"<<count_chain<<endl;


    //======Create a main list of events with odd or even events only and also skip events (first event of root file) having run number as flag and first 8 entries in case there was dropped pulse just before the start of the run===== 
    chain.Draw(">>list_temp1","Entry$%24991!=0 && (Entry$%24991)>7 && (Entry$%24991)%2==1","eventlist"); //24991 is the number of entries in one root file.
    TEventList *list1 = (TEventList*)gDirectory->Get("list_temp1");
    
    //We Can compare the list for verification
    // list1->Print("all");   
    // chain.Scan("asym[0]","Entry$%24991==0");

    //===========Create a list having all dropped pulses events======================
    chain.Draw(">>list_temp2","sumd[0]<2000","eventlist");
    TEventList *list2 = (TEventList*)gDirectory->Get("list_temp2");
    // list2->Print("all");  // Print for verification


    //=====Create another list having events around dropped pulses.====================
    //(Adding these events dynamicly to the same list of dropped pulses did NOT work)
    TEventList *list3=new TEventList();
    int sEvt; //Event corresponding to dropped pulse
    int k=0;
    int skip_max=9;  //Maximum number of pulses to be skipped after dropped pulses.

    while(k<list2->GetN())
    {
	sEvt=list2->GetEntry(k);
	// cout << "Event:"<<sEvt <<endl;
        //Take Care of End Effect
	if((sEvt%24991+skip_max)>24991)  //24991 is the number of entries in one root file.
	    skip_max=(24991-(sEvt%24991));

	for(int i=-1;i<skip_max;i++)
	{
	    if(sEvt==0 && i==-1)
		continue;
	    if((sEvt+i) >= chain.GetEntries())
		continue;

	    if(i!=0)
		list3->Enter(sEvt+i);
	}
	skip_max=9;
	k++;
    }

    // list3->Print("all"); //Print for verification

    //=================Subtract all dropped pulse events and events around it from main list===============
    list1->Subtract(list2);
    list1->Subtract(list3);

    //==================Impose the Cut for our chain==============================
    chain.SetEventList(list1);

    //=====================Draw The desired channel=======================
    cout<<"Now analyzing channels of each ADC ..."<<endl;
    for(int adc=0;adc<4;adc++)
    {
	for(int ch=0;ch<36;ch++)
	{
	    chain.Draw(Form("-asym[%i][%i]>>h",adc,ch),"","goff");  //goff disables drawing(showing) graphics(plots) 
                                                                    // x(-1) to fix the fact that SF on is spin down & SF off is spin up

	    asymmetry<<setw(10);
	    asymmetry<<setprecision(8);
	    asymmetry<<adc<<"    "<<"    "<<ch<<"    "<<h->GetMean()<<"    "<<h->GetMeanError()<<"    "<<h->GetRMS()<<endl;

	    // cout << "Channel:"<<ch<<"Mean:"<<h->GetMean()<<endl;
	    // cout << "Channel:"<<ch<<"RMS:"<<h->GetRMS()<<endl;
	    // cout << "Channel:"<<ch<<"Mean Error:"<<h->GetMeanError()<<endl;
	    // h->Draw();
	    cout << "."<<flush;

	}
	cout <<endl;

    }
    asymmetry.close();
    cout << "Number of Entries:"<< h->GetEntries() <<endl;

    //==============Must delete lists to re-run same script from CINT smoothly============================
    delete list1;
    delete list2;
    delete list3;
    time.Stop();
    time.Print();
}

//The rule of thumb is: For TTree use TEntryList
// But for chain (which require global indexing) use TEventList
//Note the Next() function for TEntryList only.
