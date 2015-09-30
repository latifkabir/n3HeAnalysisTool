// Filename: plot.cc
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Wed Jul 15 23:07:05 2015 (-0400)
// URL: latifkabir.github.io

void plot()
{

    TGraph *gr= new TGraph();

    gr->SetPoint(0,1.0/sqrt(319506),2.42758e-7);
    gr->SetPoint(1,1.0/sqrt(922637),1.33169e-7);
    gr->SetPoint(2,1.0/sqrt(1.52628e6),1.01792e-7);
    gr->SetPoint(3,1.0/sqrt(2.12805e06),8.92297e-8);
    gr->SetPoint(4,1.0/sqrt(3.33873e6),6.9761e-8);

    gStyle->SetOptFit(1);
    gr->Draw("AP");
    // gr->Draw();

    gr->Fit("pol1");

}

// p0                        = -1.04713e-08   +/-   4.05832e-09 
// p1                        =  0.000142004   +/-   3.8217e-06  
// Number of Good runs=275
// Number of pulse pairs= 3.33873e6

