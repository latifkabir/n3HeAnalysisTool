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

TGraphErrors *PlotData(int level=-1,const char *option="raw",const char *file_name="")
{
    char fName[200];
    static int color=1;
    //Generate the root file name for desired run
    if(level>=0)
	sprintf(fName,ASYM_FILE,level);
    if(level<-1) 
    {
	cout<<"Invalid level."<<endl;
	return NULL;
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
	return NULL;
    }

    while(!data.eof())
    {	
	data >>adc[index]>>channel[index]>>mean[index]>>error[index]>>rms[index];
	index++;
    }
    cout<<"Total number of Entries "<<(index-1)<<endl;

    if((index-1)!=144)
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
	for(int j=0;j<9;j++)
	{
	    l=i;
	    w=j;
	    Wire2adc(l,w);  // l and w are converted to ADC channels here after, l:0 to 3 and w:0 to 35 now
	    raw_asym=mean[l*36+w];
	    raw_error=error[l*36+w];
	    phy_asym=(1.0/GFactor(i,j))*raw_asym;
	    phy_error=abs((1.0/GFactor(i,j)))*raw_error;

	    // if(point==4)
	    // {
	    // 	point++;
	    // 	continue;
	    // }
	    if(option==opt[0])
	    {
		gr->SetPoint(point,i*20+j,raw_asym); //i*20+j instead of i*9+j to make layes distingulshable
		gr->SetPointError(point, 0, raw_error);
	    }
	    else if(option==opt[1])
	    {
		gr->SetPoint(point,i*20+j,phy_asym); //i*20+j instead of i*9+j to make layes distingulshable
		gr->SetPointError(point, 0, phy_error);
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
    gr->GetXaxis()->SetTitle("Layer x 20+Wire");
    if(option==opt[0])
	gr->GetYaxis()->SetTitle("Raw Asym");
    else
	gr->GetYaxis()->SetTitle("Physics Asym");

    cout << "Color is :"<<color <<endl;

    gr->SetLineColor(color);
    color++;
    // gStyle->SetOptStat(1111);
    // gStyle->SetOptFit(1);
    // gr->Draw("AP");
    // gr->Fit("pol0");
    return gr;
}

void MultiGraph()
{

   // TCanvas *c3 = new TCanvas("Multi Chopper","Multi Chopper",1600, 400);

   TMultiGraph *mg = new TMultiGraph("mg","mg");
   // TGraphErrors *gr1=PlotData(1);
   // TGraphErrors *gr2=PlotData(2);
   TGraphErrors *gr1=PlotData(-1,"raw","/home/kabir/GIT/n3HeAnalysisTool/Results/13.TimeBeamDependence/UDRuns/UpSpin/Asymmetry-0.txt");
   TGraphErrors *gr2=PlotData(-1,"raw","/home/kabir/GIT/n3HeAnalysisTool/Results/13.TimeBeamDependence/UDRuns/DownSpin/Asymmetry-0.txt");
  
   // TGraphErrors *gr3=MakePlot2(3);

   mg->Add( gr1);
   mg->Add( gr2);
   // mg->Add( gr3);

   // // gr3->Draw("AP");
   mg->Draw("AP");

}
