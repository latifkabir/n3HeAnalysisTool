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
    int status1=gSystem->Load("libn3He.so");
    if(status1>-1)
	cout << "attempt completed !!\n";
    else
	cout << "attempt failed.\n";

    cout << "\t\t  Attempting to load libAnalysis.so ...";
    int status2=gSystem->Load("libAnalysis.so");
    if(status2>-1)
	cout << "attempt completed !!\n\n";
    else
	cout << "attempt failed.\n\n";
   
    gStyle->SetMarkerStyle(7);    // Filled square
    gStyle->SetMarkerSize(.4);     // larger than half a pixel ...
    gStyle->SetMarkerColor(kBlack);

}


//Place this script inside "macros" directory under your ROOT installation directory.  
//In that case each time you start root , it will load libn3he.so automatically.
