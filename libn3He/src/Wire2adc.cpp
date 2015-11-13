// Filename: Wire2adc.cpp
// Description: The wire to ADC conversion 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Nov  2 17:25:56 2015 (-0500)
// URL: latifkabir.github.io

#include<iostream>
#include"Wire2adc.h"
using namespace std;

const int nLayers=16;
const int nWires=9;

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


void Wire2adc(int &layer, int &wire)
{    
    const int glayer=layer;
    const int gwire=wire;

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

//In our wire map (ADC ch 0) wire-1 is beam bottom and wire-9 is beam top, and in the simulation (where in cos_theta theta is wrt +x axis), so positive values represent beam top side. So the GF association needs to be multiplied accordingly.
//For left we need to be sure which way it was rotated.
