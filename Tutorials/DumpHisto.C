//This script show how to dump histo from TTree draw statement and retrive various parameters 
//Author: Latiful Kabir
//Date: 12/23/14
#include<TH2F.h>

void DumpHisto()
{
    // gSystem->Load("libTree");    //You need to load libTree first in order to Load libn3He. This is not necessary if you include TTree.h
    // gSystem->Load("libn3He.so");
    // gROOT->SetBatch();
    TTreeRaw *t=new TTreeRaw(100);
    TH2I *h=new TH2I("h","h",50,0,50,100,0,30e6);
    t->Draw("d21[][0]:Iteration$>>h","Entry$==0");
    h->Draw();
    cout<<"The mean X is:"<<h->GetMean(1)<<endl;
    cout<<"The mean Y is:"<<h->GetMean(2)<<endl;
    cout<<"The RMS X is:"<<h->GetRMS(1)<<endl;
    cout<<"The RMS Y is:"<<h->GetRMS(2)<<endl;
}
