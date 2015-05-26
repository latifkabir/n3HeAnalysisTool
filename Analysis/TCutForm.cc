// Filename: ReadSpeValue.cc
// Description: This is a sample code to show how to read specific value of a branch and entry and how to use TCut. 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Apr  6 14:25:48 2015 (-0400)
// URL: latifkabir.github.io
//THIS SCRIPT IS A SAMPLE ONLY & IS NOT CONSISTANT FOR CURRENT N3HE ROOT FILE AS IT NO MORE CONTAINS DROPPED PULSE NUMBER
#include<TTree.h>
#include<TFile.h>

void TCutForm()
{

    TFile *f=new TFile("run-26230.root");
    TTree *T=(TTree*)f->Get("T");
    T->Print();
    T->Scan("sumc[0][0]:sumd[0]:sumd[1]:asym[0][0]");
    // T->Draw("sumc1[0]-sumc1[1]","(Entry$-39)%600!=0");
    int ch=0;

    //Get The First Dropped Pulse 
    T->GetEntry(24990);
    int dpulse=T->GetLeaf("asym")->GetValue(1); //First dropped pulse of the run
   
    TCut cut1=Form("Entry$!=0 && (Entry$-%i+5)%600 > 10 && Entry$%2==1 && asym[0][%i]!=-1 && Entry$!=24990",dpulse,ch);
    TCut cut2=Form("Entry$!=0 && (Entry$-%i+5)%600 > 10 && Entry$%2==0 && asym[0][%i]!=-1 && Entry$!=24990",dpulse,ch);
    T->Draw("asym[0][0]",cut1); //First of the two sets of asymmetry
    
    TCanvas *c2=new TCanvas();
    T->Draw("asym[0][0]",cut2); //The other of the two sets of asymmetry
}









