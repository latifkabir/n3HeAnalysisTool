// Filename: HistoTree.cc
// Description: Make histogram, save histogram in root file as tree, read histogram from root file as tree. 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Oct 19 22:12:18 2015 (-0400)
// URL: latifkabir.github.io


#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

#include<TFile.h>
#include<TH1D.h>
#include<TTree.h>
#include<TString.h>
#include<TList.h>
#include<TObjString.h>
#include<TSystem.h>
#include"Constants.h"

void SaveHisto(int level,int first_run,int last_run,int runs_analyzed,TH1D ***myHist)
{
    TString fName;  //Name of the root file
    fName+=LEVEL_DATA;
    fName+=level;
    fName+=".root";
    TFile *f=new TFile(fName,"recreate");
    TTree *t=new TTree("T","Histo Tree");

    TH1D tempHist; //Temporary buffer

    
    t->Branch("myHist","TH1D",&tempHist);  //Note the order.

    for(int i=0;i<4;i++)
    {
    	for(int j=0;j<36;j++)
    	{
    	    tempHist=*myHist[i][j];

    	    t->Fill();
    	}
    }


//=============Add User Info part======================
    ostringstream oss;
    oss << "First Run: "<<first_run<<" Last Run: "<<last_run<<" Runs Analyzed: " << runs_analyzed;


    string vlab=oss.str();

    TList *labels = new TList();
    labels->SetName("Label");


    TObjString os;
    os = vlab.c_str();
    labels->Add((TObject*)&os);

    t->GetUserInfo()->Add(labels);
    
    t->Print();
    t->Write();

    f->Close();
   //Note the tree has just one entry in the root file here


   //===========Keep Record of Analysis in a Text File============
    ofstream record(ANALYSIS_RECORD,ofstream::app);
    //Generate current date and time of the run in desired format
    time_t now = time(0);    
    // convert now to string form
    char* dt = ctime(&now);

    if(record)
    {
	record<<dt<<"          "<<level<<"          "<<first_run<<"          "<<last_run<<"          "<<runs_analyzed<<endl;
	record.close();
    }
}


TH1D* RetrieveHisto(int level,int entry) 
{

    TString fName;  //Name of the root file
    fName+=LEVEL_DATA;
    fName+=level;
    fName+=".root";

    TFile *f = new TFile(fName);
    TTree *t = (TTree*)f->Get("T");
    TH1D *hpx = 0;
  
  
    t->SetBranchAddress("myHist",&hpx);
    t->GetEntry(entry);
 
    f->Close();
    return hpx;
}


void GetLevelInfo(int level)
{
    TString fName;  //Name of the root file
    fName+=LEVEL_DATA;
    fName+=level;
    fName+=".root";

    bool status=gSystem->AccessPathName(fName); //Note bizzare return type convention
    if(status) 
    {
	cout << "NO file found for requested level: "<<level<<endl;
	return;
    }


    TFile *f = new TFile(fName);
    TTree *t = (TTree*)f->Get("T"); 

    t->GetUserInfo()->FindObject("Label")->Print();
    f->Close();
}
