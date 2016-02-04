#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TStyle.h>
#include"Wire2adc.h"
#include"GFactor.h"
#include"Constants.h"

void MakePlotPair(int level,const char *option,const char *file_name)
{
    char fName[200];
    //Generate the root file name for desired run
    if(level>=0)
	sprintf(fName,ASYM_FILE,level);
    if(level<-1) 
    {
	cout<<"Invalid level."<<endl;
	return;
    }

    string opt[2]={"raw","phy"};

    ifstream data;
    double adc[150],channel[150],mean[150],error[150],rms[150];
    int index=0;

    if(level>=0)
	data.open(fName);
    else if(level==-1)
	data.open(file_name);

    if(!data)
    {
	cout << "The data file does NOT exist" <<endl;
	return;
    }

    while(!data.eof())
    {	
	data >>adc[index]>>channel[index]>>mean[index]>>error[index]>>rms[index];
	index++;
    }
    cout<<"Total number of Entries "<<(index-1)<<endl;

    if((index-1)!=64)
    {
	cout << "Make sure you have asymmetries for all the channles or EOF is correct" <<endl;
	return;
    }

    // TCanvas *c = new TCanvas("c","TGraph example",0,0,600,400);
    TGraphErrors *gr = new TGraphErrors();

    int point=0;
    int l=0; //The layer number
    int w=0; //The wire number
    double raw_asym;  // The raw(measured) asymetry
    double raw_error;  // The error in raw(measured) asymetry
    double phy_asym;  // The Physics asymmetry
    double phy_error;  // The error in Physics asymmetry
    for (int i=0; i<16; i++) 
    {
	for(int j=0;j<4;j++)
	{
	    l=i;
	    w=j;
	    Wire2adc(l,w);  // l and w are converted to ADC channels here after, l:0 to 3 and w:0 to 3 now
	    if(w<4)
		w=w;
	    if(w>=9 && w<13)
		w=w-5;
	    if(w>=18 && w<22)
		w=w-10;
	    if(w>=27 && w<31)
		w=w-15;

	    raw_asym=mean[l*16+w];
	    raw_error=error[l*16+w];
	    // phy_asym=(1.0/GFactor(i,j))*raw_asym;
	    // phy_error=abs((1.0/GFactor(i,j)))*raw_error;

	    // if(point==4)
	    // {
	    // 	point++;
	    // 	continue;
	    // }
	    if(option==opt[0])
	    {
		gr->SetPoint(point,i*10+j,raw_asym); //i*20+j instead of i*9+j to make layes distingulshable
		gr->SetPointError(point, 0, raw_error);
	    }
	    else if(option==opt[1])
	    {
		// gr->SetPoint(point,i*20+j,phy_asym); //i*20+j instead of i*9+j to make layes distingulshable
		// gr->SetPointError(point, 0, phy_error);
	    }
	    else
	    {
		cout << "Invalid Plotting Option" <<endl;
		return;
	    }
	    point++;
	}
    }

    gStyle->SetPalette(1);
    gr->GetXaxis()->SetTitle("Layer x 10+Wire");
    if(option==opt[0])
	gr->GetYaxis()->SetTitle("Raw Asym");
    else
	gr->GetYaxis()->SetTitle("Physics Asym");

    // gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1);
    gr->Draw("AP");
    gr->Fit("pol0");
}
