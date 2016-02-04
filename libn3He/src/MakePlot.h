 // Filename: MakePlot.h
 // Description: 
 // Author: Latiful Kabir < siplukabir@gmail.com >
 // Created: Fri May  8 17:08:34 2015 (-0400)
 // URL: latifkabir.github.io

double adc2volt=4.657e-9; //approx global factor for ADC count to Volt conversion(without shifting last 8 bits).
void Wire_to_ADC(int &layer, int &wire);
double Gfactor(int layer, int wire);
void MakePlot(int level=-1,const char *option="raw",const char *file_name="");
