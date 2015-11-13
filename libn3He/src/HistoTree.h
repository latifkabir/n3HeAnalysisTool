// Filename: HistoTree.h
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Tue Oct 20 19:46:43 2015 (-0400)
// URL: latifkabir.github.io

#include<TH1D.h>
void SaveHisto(int level,int first_run,int last_run,int runs_analyzed,TH1D ***myHist);
TH1D* RetrieveHisto(int level,int entry);
void GetLevelInfo(int level);
