// Filename: PlotFmText.cc
// Description: Plt data from text file
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Mar  7 10:26:30 2016 (-0500)
// URL: latifkabir.github.io

/*

Constructor :  TGraph(const char* filename, const char* format = "%lg %lg", Option_t* option = "")

 Graph constructor reading input from filename.
 filename is assumed to contain at least two columns of numbers.
 the string format is by default "%lg %lg".
 this is a standard c formatting for scanf. If columns of numbers should be
 skipped, a "%*lg" or "%*s" for each column can be added,
 e.g. "%lg %*lg %lg" would read x-values from the first and y-values from
 the third column.

If you ahve just two columns then the following should just work:


    TGraph *gr=new TGraph("pathe/to/file");
    gr->Draw("APL*");


*/

#include<TGraph.h>

void PlotFmText()
{
    TGraph *gr=new TGraph("/home/kabir/GIT/n3HeAnalysisTool/LevelData/Asymmetry-00.txt","%lg %lg","");
    gr->Draw("APL*");

}
