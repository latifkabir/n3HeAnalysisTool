//Loading libn3He automatically eact time you start root.
//Author: Latiful kabir 
{
    cout << "\n\t\t=======================================================" <<endl;
    cout << "\t\t|  Welcome to n3He Analysis Tool Integrated in ROOT   |"<<endl;
    cout << "\t\t|   Type: 'Help()' for a list of available options    |" <<endl;
    cout << "\t\t|   Report issues to: latifulkabir@uky.edu            |" <<endl;
    cout << "\t\t=======================================================" <<endl;

    cout << "\t\t  Attempting to load libn3He.so ... ";
    gSystem->Load("libTree");
    gSystem->Load("libHist");
    gSystem->Load("libn3He.so");
    cout << "attempt completed\n\n";

    gStyle->SetMarkerStyle(7);    // Filled square
    gStyle->SetMarkerSize(.4);     // larger than half a pixel ...
    gStyle->SetMarkerColor(kBlack);

}


//Place this script inside "macros" directory under your ROOT installation directory.  
//In that case each time you start root , it will load libn3he.so automatically.
