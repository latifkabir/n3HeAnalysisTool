// Filename: ReadRootFile.cc
// Description: Sample n3He Analysis Method-2
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 10 14:50:00 2015 (-0400)
// URL: latifkabir.github.io

#include "TFile.h"
#include "TH1F.h"

void ReadRootFile() 
{
    TStopwatch time;
    time.Start();
    // Create a histogram for the values we read.
    TH1F *myHist = new TH1F("h1", "ntuple", 100, -0.005, 0.005);
 
    // Open the file containing the tree.
    TFile *myFile = TFile::Open("run-26230.root");
    TTree *T=(TTree*)myFile->Get("T");

    //Create the buffer to hold entries
    double sumc[4][36];
    double sumd[2];
    double asym[4][36];

    // myData md;
    TBranch *b1=T->GetBranch("sumc");
    TBranch *b2=T->GetBranch("sumd");
    TBranch *b3=T->GetBranch("asym");
    b1->SetAddress(&sumc[0][0]);
    b2->SetAddress(&sumd[0]);
    b3->SetAddress(&asym[0][0]);

    //Get The run number embeded in asymmetry for first event
    b3->GetEntry(0);
    int runNumber=asym[0][0];
    int dpulse=317;  //First dropped pulse of the run, found using TTreeRaw

    cout << "The run number under unvestigation:"<<runNumber <<endl;

    // Loop over all entries of the TTree or TChain.
    for(int i=0;i<b3->GetEntries();i++) 
    {
	b3->GetEntry(i);
	if((i-dpulse+5)%600>10 && i!=0  && i%2==0 && i!=24990) // This is our cut for this run, assuming all dropped pulses are periodic.
	    myHist->Fill(asym[0][0]);
    }
    myHist->Draw();
    cout << "Mean:"<<myHist->GetMean() <<endl;
    cout << "RMS:"<<myHist->GetRMS() <<endl;
    cout << "Entries:"<<myHist->GetEntries() <<endl;

    time.Stop();
    time.Print();
}
