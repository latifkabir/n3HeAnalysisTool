// Filename: ReadChain.cc
// Description: Sample Code SHowing How to Read a TChain
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 24 17:20:33 2015 (-0400)
// URL: latifkabir.github.io

#include<TChain.h>

void ReadChain()
{
    TChain chain("T");
    //Add files to the chain
    chain.Add("run-26230.root");
    chain.Add("run-26250.root");

    chain.Print();

        //Create the buffer to hold the entries
    	double asym[4][36];
    	double sumc[4][36];
    	double sumd[2];

    chain.SetBranchAddress("asym",&asym[0][0]);
    chain.SetBranchAddress("sumc",&sumc[0][0]);
    chain.SetBranchAddress("sumd",&sumd[0]);
    
    int nEntries=chain.GetEntries();
    cout << "Total Entries :"<<nEntries<<endl;

    for(int i=0;i<=5;i++) 
    {
    	chain.GetEntry(i);     //Note the entry number is global here

    	cout << "Asym:"<<asym[0][0] <<endl;
    	cout << "Sumc:"<<sumc[0][0] <<endl;
    	cout << "Sumd:"<<sumd[0]<<endl;
    }

    //Verify using Scan statement
    chain.Scan("sumc[0][0]:sumd[0]:asym[0][0]");
}
