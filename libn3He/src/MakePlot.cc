#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

#include<TCanvas.h>
#include<TGraphErrors.h>
#include<TAxis.h>
#include<TStyle.h>

void Wire_to_ADC(int &layer, int &wire)
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


double Gfactor(int layer, int wire)
{
    double GF[16][9]=
	{
	    {-0.80195, -0.62273,  -0.19846, -0.093971, 0.0011782,  0.082577,    0.2092,   0.62382,   0.80181},
	    {-0.78958,  -0.63079,  -0.24369,  -0.11284, 0.00044971,   0.10175,   0.25239,   0.63164,    0.7897},
	    {-0.76131,  -0.61601,  -0.26829,  -0.12514, 2.5428e-05,   0.11434,   0.27475,   0.61651,   0.76193},
	    {-0.73478,  -0.58979,  -0.27442,  -0.13064, -0.00033343,   0.12002,   0.27917,   0.59073,   0.73649},
	    {-0.73054,    -0.578,    -0.272,  -0.12965, -0.00050112,   0.11901,   0.27567,   0.57942,   0.73256},
	    {-0.72996,   -0.5763,  -0.27132,  -0.12947, -0.00056428,   0.11844,   0.27416,   0.57762,   0.73197},
	    {-0.72967,  -0.57504,  -0.27086,  -0.12933, -0.00067109,   0.11792,   0.27287,   0.57622,   0.73155},
	    {-0.73009,  -0.57473,  -0.27017,  -0.12861, -0.00077796,   0.11688,   0.27116,   0.57559,   0.73203},
	    {-0.72926,  -0.57303,  -0.27023,  -0.12883, -0.00093787,   0.11669,   0.27036,   0.57367,   0.73107},
	    {-0.72935,  -0.57263,   -0.2701,  -0.12842, -0.0010753,   0.11592,   0.26918,   0.57283,   0.73114},
	    {-0.72948,  -0.57204,  -0.27001,  -0.12811, -0.001211,   0.11522,   0.26801,   0.57172,   0.73121},
	    {-0.72895,   -0.5705,  -0.27041,  -0.12832, -0.0013972,   0.11501,   0.26741,    0.5697,   0.73037},
	    {-0.72946,  -0.57102,   -0.2705,   -0.1278, -0.0015394,   0.11415,   0.26626,   0.56949,   0.73087},
	    {-0.73027,  -0.57131,  -0.27086,  -0.12776, -0.0016938,   0.11369,   0.26553,   0.56916,   0.73148},
	    {-0.73347,  -0.57203,  -0.26924,  -0.12645, -0.0017852,   0.11201,   0.26284,   0.56916,   0.73437},
	    {-0.73498,   -0.5693,  -0.26252,  -0.12249, -0.001701,   0.10787,   0.25501,    0.5656,   0.73573}
	};

    return GF[layer][wire];

}

void MakePlot(const char *fName="asymmetry.txt",const char *option="phy")
{
    string opt[2]={"raw","phy"};

    ifstream data;
    double adc[150],channel[150],mean[150],error[150],rms[150];
    int index=0;

    data.open(fName);
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
	    Wire_to_ADC(l,w);  // l and w are converted to ADC channels here after
	    raw_asym=mean[l*9+w];
	    raw_error=error[l*9+w];
	    phy_asym=(1.0/Gfactor(i,j))*raw_asym;
	    phy_error=abs((1.0/Gfactor(i,j)))*raw_error;

	    if(option==opt[0])
	    {
		gr->SetPoint(point,i*9+j,raw_asym);
		gr->SetPointError(point, 0, raw_error);
	    }
	    else if(option==opt[1])
	    {
		gr->SetPoint(point,i*9+j,phy_asym);
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
    gr->GetXaxis()->SetTitle("Layer x 9+Wire");
    if(option==opt[0])
	gr->GetYaxis()->SetTitle("Raw Asym");
    else
	gr->GetYaxis()->SetTitle("Physics Asym");

    gr->Draw("AP");
}
