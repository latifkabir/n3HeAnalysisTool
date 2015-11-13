 // Filename: MakePlot.h
 // Description: 
 // Author: Latiful Kabir < siplukabir@gmail.com >
 // Created: Fri May  8 17:08:34 2015 (-0400)
 // URL: latifkabir.github.io

void Wire_to_ADC(int &layer, int &wire);
double Gfactor(int layer, int wire);
void MakePlot(int level=0,const char *option="phy");

