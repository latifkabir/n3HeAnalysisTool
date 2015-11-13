// Filename: TreeReader.cc
// Description: Read root file using TTreeReader class <Requires ROOT 6>
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Fri Apr 10 14:50:00 2015 (-0400)
// URL: latifkabir.github.io

#include "TFile.h"
#include "TH1F.h"
#include<TH2D.h>

struct myData
{
    double sumc[4][36];
};

void VWire_to_ADC(int &layer, int &wire)
{    
    const int nLayers=16;
    const int nWires=9;
    const int glayer=layer;
    const int gwire=wire;

    int Layer_to_DAQ_map[nLayers]={21, 23, 21, 23, 21, 23, 21, 23,
                         22, 24, 22, 24, 22, 24, 22, 24};

    int Layer_to_ADC_channel_map[nLayers][nWires] =
    {
      {0,1,2,3,4,5,6,7,8},
      {0,1,2,3,4,5,6,7,8},
      {9,10,11,12,13,14,15,16,17},
      {9,10,11,12,13,14,15,16,17},
      {24,25,26,27,28,29,30,31,32},
      {24,25,26,27,28,29,30,31,32},
      {33,34,35,36,37,38,39,40,41},
      {33,34,35,36,37,38,39,40,41},
      {0,1,2,3,4,5,6,7,8},
      {0,1,2,3,4,5,6,7,8},
      {9,10,11,12,13,14,15,16,17},
      {9,10,11,12,13,14,15,16,17},
      {24,25,26,27,28,29,30,31,32},
      {24,25,26,27,28,29,30,31,32},
      {33,34,35,36,37,38,39,40,41},
      {33,34,35,36,37,38,39,40,41}
    };

    if(layer>=nLayers || wire>=nWires)
    {
	cout<<"Invalid layer or wire request"<<endl;
	layer=-1;
	wire=-1;
	return;
    }
    layer=Layer_to_DAQ_map[glayer]-21; //In my analysis ADC is 0 to 3
    wire=Layer_to_ADC_channel_map[glayer][gwire];
    wire=(wire>17) ? wire-6 : wire; //In my analysis channels are continuous.  
}

void IntensityPlot() 
{
    // Create a histogram for the values we read.
    TH2D *gr = new TH2D("h","h",16,0,16,9,0,9);
 
    // Open the file containing the tree.
    TFile *myFile = TFile::Open("run-20245.root");
    TTree *T=(TTree*)myFile->Get("T");

    myData md;
    TBranch *b=T->GetBranch("sumc");
    b->SetAddress(&md.sumc[0][0]);

    int l=0; //The layer number
    int w=0; //The wire number
    b->GetEntry(0);
    int point=0;
    for(int i=0;i<16;i++)
    {
	for(int j=0;j<9;j++)
	{
	    l=i;
	    w=j;
	    VWire_to_ADC(l,w);		  
	    if(i==0 && (j==5 || j==6))
	    {
		l=0;
		w=6;
		VWire_to_ADC(l,w);	
		gr->Fill(i,j,0.5*md.sumc[l][w]);
	    }
	    else		
		gr->Fill(i,j,md.sumc[l][w]);
	    cout <<"Layer: "<<i<<"  Wire:" <<j<<" Intensity:"<<md.sumc[l][w] <<endl;
	    point++;
	}
    }
    
    // gr->Draw("colz");
    gr->GetXaxis()->SetTitle("Layer#");
    gr->GetYaxis()->SetTitle("Wire#");
    gr->GetZaxis()->SetTitle("Intensity");    
    gr->Draw("lego2");
}
