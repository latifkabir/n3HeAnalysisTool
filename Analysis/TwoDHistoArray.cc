// Filename: TwoDHistoArray
// Description: This Script shows how to make 2D array of histograms in the heap, generates title automatically. Also introduces TStopwatch !!
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 10 14:50:00 2015 (-0400)
// URL: latifkabir.github.io
//NOTE: THIS SCRIPT IS FOR DEMOSTRATION PURPOSE ONLY AND HAS NOT CUT, AND HISOGRAM RANGE IS NOT OPTIMIZED

#include "TFile.h"
#include "TH1F.h"
#include<fstream>
#include<iomanip>
struct myData
{
    double asym[4][36];
}; 
void TwoDHistoArray() 
{
    int run[2]={26230,26250};

    TStopwatch time;
    time.Start();
    // Create a histogram for the values we read.
    int n_bin=100;
    double x_low=-0.005; 
    double x_up=0.005;

    // =============Declare 4x36 Histograms in the heap to be filled=================
    TH1D ***myHist;
    myHist=new TH1D**[4];
    for(int i=0;i<4;i++)
    	myHist[i]=new TH1D*[36];

    for (int i = 0; i < 4; i++) 
    {
	for(int j=0;j<36;j++)
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
    for(int r=0;r<2;r++)
    {
	//Generate the root file name for desired run
	TString fName;
	fName="run-";
	fName+=run[r];
	fName+=".root";
 
	//============ Open the file containing the tree==================
	TFile *myFile = TFile::Open(fName);
	if(!myFile)
	{
	    cout << "The root file for run:"<<run[r]<<" does NOT exist" <<endl;
	    continue;
	}
	TTree *T=(TTree*)myFile->Get("T");

	//=================Create the buffer to hold event entries===================
	myData md;
	TBranch *b=T->GetBranch("asym");
	b->SetAddress(&md.asym[0][0]);


	//=============Loop over all entries of the TTree or TChain to fill histogram or to do some analysis===============
	int event=1; //Skip First event & start from second event as first one is just Run number flag.
	int nentries=(b->GetEntries()-1); //Number of total events to be considered. Skip last one as it contains droppled pulse info

	while(event < nentries) 
	{
	    if(event%2==1 && event!=-1)  //Fill only unique pairs (Any one of the two set of pairs)
	    {
		b->GetEntry(event);
		for(int i=0;i<4;i++)
		{
		    for(int j=0;j<36;j++)
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
    }
    //=================Check for Histogram range =========================
    // if(abs(MaxAsym) > x_up || MinAsym < x_low)
    {
    	cout << "MaxAsym:"<<MaxAsym <<endl;
    	cout << "MinAsym:"<<MinAsym <<endl;
    	cout << "x_up:"<<x_up <<endl;
    	cout << "x_low:"<<x_low <<endl;
    	cout<<"Please adjust the range of the histogram to cover all events"<<endl;
    	// return;
    }
    //========Draw the histo and extract desired param==================
    myHist[0][5]->Draw();
    cout << "Mean:"<<myHist[0][5]->GetMean() <<endl;
    cout << "RMS:"<<myHist[0][5]->GetRMS() <<endl;
    cout << "Error in Mean:"<<myHist[0][5]->GetMeanError() <<endl;
    cout << "Entries:"<<myHist[0][5]->GetEntries() <<endl;
    cout << "MaxAsym:"<<MaxAsym <<endl;
    cout << "MinAsym:"<<MinAsym <<endl;

    for(int i=0;i<4;i++)
    {
	for(int j=0;j<36;j++)
	{
	    asymmetry<<setw(10);
	    asymmetry<<setprecision(8);
	    asymmetry<<i<<"    "<<"    "<<j<<"    "<<myHist[i][j]->GetMean()<<"    "<<myHist[i][j]->GetMeanError()<<"    "<<myHist[i][j]->GetRMS()<<endl;
	}
    }
    asymmetry.close();

    time.Stop();
    time.Print();
    delete[] myHist;
}


// Note that the philosophy of declaring 2D Histogram array in the heap: "It just has t  be consistant with TH1F *h1=new TH1F()" i.e. From
// left to right the star decreases by one.
