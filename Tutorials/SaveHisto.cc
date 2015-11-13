// Filename: SaveHisto.cc
// Description: Make histogram, save histogram in root file as tree, read histogram from root file as tree. 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Oct 19 22:12:18 2015 (-0400)
// URL: latifkabir.github.io

//Run as:
// root > .L SaveHisto.cc
// root > TH1F* h= MakeHisto()
// root > HistoTree(h)
// root > ReadHisto()



#include<TFile.h>
#include<TH1F.h>
#include<TTree.h>

TH1F* MakeHisto()
{
    TH1F *h1=new TH1F("hgaus","histo from a gaussian",100,-3,3);
    h1->FillRandom("gaus",10000);
    return h1;
}

void HistoTree(TH1F *myHist)
{
    TFile *f=new TFile("test_file.root","recreate");
    TTree *t=new TTree("T","Histo Tree");

    t->Branch("myHist","TH1F",myHist);  //Note the order unlike non-object type.
    t->Fill();
    t->Print();
    t->Write();

    f->Close();
   //Note the tree has just one entry in the root file here
}

void ReadHisto() 
{
   TFile *f = new TFile("test_file.root");
   TTree *T = (TTree*)f->Get("T");
   TH1F *hpx = 0;
  
  
   T->SetBranchAddress("myHist",&hpx);
   T->GetEntry(0);
 
   hpx->Draw();
   f->Close();
}
