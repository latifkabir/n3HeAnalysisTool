// Filename: TreeReader.cc
// Description: Read root file using TTreeReader class <Requires ROOT 6>
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 10 14:50:00 2015 (-0400)
// URL: latifkabir.github.io
// THIS SCRIPTS IS A PLACE HOLDER TO REMIND THIS APPROACH.IT WORKS ONLY IN ROOT 6
#include "TFile.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

struct myData
{
    double sumk[4][36];
};
 
void TreeReader() 
{
    // Create a histogram for the values we read.
    TH1F *myHist = new TH1F("h1", "ntuple", 100, -5, 5);
 
    // Open the file containing the tree.
    TFile *myFile = TFile::Open("run-2630.root");
 
    // Create a TTreeReader for the tree, for instance by passing the
    // TTree's name and the TDirectory / TFile it is in.
    TTreeReader myReader("T", myFile);
   
    // The branch "sumc" contains floats; access them as myValue.
    TTreeReaderValue<double> myValue(myReader, "sumc");
 
    // Loop over all entries of the TTree or TChain.
    while (myReader.Next()) 
    {
	// Just access the data as if myValue were iterators (note the '*'
	// in front of them):
	cout << *myValue <<endl;

	myHist->Fill(*myValue);
    }
 
    myHist->Draw();
}
