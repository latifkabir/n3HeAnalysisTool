//Demo Online Analysis using n3He Library.(By Online I mean  'from CINT, doing analysis on the fly, less thoughtful but preferred by in some conditions')
//Author: Latiful Kabir
//Date: 12/23/14

void TTreeDraw()
{
    // gSystem->Load("libTree");    //You need to load libTree first in order to Load libn3He. This is not necessary if you include TTree.h
    // gSystem->Load("libn3He.so");

    TTreeRaw *t=new TTreeRaw(100);
    t->Draw("d21[][0]:Iteration$");

    TCanvas *c1=new TCanvas();
    t->Draw("d21[][0]:49*Entry$+Iteration$","Entry$<5");

    //Another trick is to plot signal vs Pulse pulse(Event number) but still plotting each time bin, in the following way
    // t->Draw("d21[][0]:Entry$+Iteration$/49","Entry$<5"); //In this case your each time bin is one entry or pulse i.e. 16.66ms.
                                                         //And inside each event, each 0.1 point is (16.66/49) ms. 

}

//Note : Entry$ = The event(pulse) variable
//      Iteration$ = The time bin or sample variable
// In continuous data taking mode, one time bin consists one event. So in that case, Entry$ = time bin variable.
// In the above example 49 is the number of time bin per event(pulse).
