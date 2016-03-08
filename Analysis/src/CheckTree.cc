// Filename: myChain.cc
// Description: Illustrating how to call libn3He funtionality in compiled analysis script. 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Tue Mar  8 15:04:52 2016 (-0500)
// URL: latifkabir.github.io

#include "TTreeRaw.h"
void CheckTree(int runNumber)
{
    TTreeRaw t(runNumber);
    t.Print();
}
