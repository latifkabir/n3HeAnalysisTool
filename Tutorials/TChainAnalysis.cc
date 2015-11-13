// Filename: TChainAnalysis.cc
// Description: Sample online analysis using (TChain object) n3HeChain class object. Mainly features other than TTree.
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Sun May 31 23:19:15 2015 (-0400)
// URL: latifkabir.github.io

void TChainAnalysis()
{
    //Create Chain of desired run range
    n3HeChain ch(26230,26250);
    //Print to see content
    ch.Print();

    //Load member Tree corresponding to any specific global entry
    int localEntry=ch.LoadTree(30000); // The return value is local entry number in that member tree.

    //Once desired member tree is loaded , retrive that tree
    TTree *t=ch.GetTree();
    //Print to see the content
    t->Print(); 

    // Retrive the run number corresponding to this member tree
    t->GetEntry(0);
    int runNumber=(int)t->GetLeaf("asym")->GetValue(0); //In the root file the first asymmetry value is a flag for run number.
    //We can also get the run number easily bases of global entry number calculation

    // Create TTreeRaw object for the retrived run number 
    TTreeRaw tr(runNumber);
    //Print to see the content
    tr.Print();
}
