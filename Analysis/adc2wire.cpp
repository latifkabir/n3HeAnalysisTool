// Filename: adc2wire.cpp
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Wed Apr 29 10:48:02 2015 (-0400)
// URL: latifkabir.github.io
#include<iostream>

using namespace std;

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
      {33,34,35,36,37,38,39,40,41},
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

int main()
{
    int layer=0;
    int wire=8;
    Wire_to_ADC(layer,wire);
    cout<<"Layer: "<<layer<<" Wire: "<<wire<<endl;
    return 0;
}
