// Filename: Sample analysis using the summary root file for a single run using TEntryList assuming dropped pulses are periodic(which is not the case always).
// Description: Reading Tree from .root file
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Apr  6 14:25:48 2015 (-0400)
// URL: latifkabir.github.io

#include<TTree.h>
#include<TFile.h>

void EntryList()
{

    //Open the root summary file
    TFile *f=new TFile("run-26230.root");
    //get the Tree
    TTree *T=(TTree*)f->Get("T");
    T->Print();
    //Scan the entries
    T->Scan("sumc[0][0]:sumd[0]:sumd[1]:asym[0][0]");
    int ch=0;

    //Get dropped pulses entries in a linked list
    T->Draw(">>list_temp","sumc[0][1]<1","entrylist");
    TEntryList *list = (TEntryList*)gDirectory->Get("list_temp");
    //Print the entries corresponds to dropped pulses
    list->Print("all");
    //Get The First Dropped Pulse
    int first_dropped=list->GetEntry(0);
 
    //Make the cut
    TCut cut1=Form("Entry$!=0 && (Entry$-%i+5)%600 > 10 && Entry$%2==0 && asym[0][%i]!=-1",first_dropped,ch);
    T->Draw("asym[0][0]",cut1); //First of the two sets of asymmetry
    f->Close();
    delete f;    
}









