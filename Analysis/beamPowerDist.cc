// Filename: beamPowerDist.cc
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Wed Jul  1 14:43:16 2015 (-0400)
// URL: latifkabir.github.io

#define ROOT_FILE_PATH "/mnt/idata05/summary/"

void beamPowerDist()
{

    TH1D *power_hist=new TH1D("power_hist","power_hist",100,600,1500);
    int start_run=17400,stop_run=38214;
    double ave_power=0;
    
    for(int run=start_run;run<=stop_run;run++)
    {
	// TH1D *h=new TH1D("h","h",100,3300,3600);
	//Generate the root file name for desired run
	TString fName;
	fName=ROOT_FILE_PATH;
	fName+="run-";
	fName+=run;
	fName+=".root";
	
	//============ Open the file containing the tree==================
	bool status=gSystem->AccessPathName(fName); //Note bizzare return type convention
	if(!status) 
	{
	    cout << "=========================================================" <<endl;
	    cout << "Now Reading run number: "<<run<<endl;
	    cout << "=========================================================\n" <<endl;
	}
	else
	{
	    cout << "\n--------->The root file for run number :"<<run<<" does NOT exist<-------------" <<endl;
	    cout << "--------->Skipping the run number : "<<run <<"<-------------\n"<<endl;
	    continue;
	}
	TFile *myFile = TFile::Open(fName);
	TTree *t=(TTree*)myFile->Get("T");
	// T->Print();
	TH1D *h=new TH1D("h","h",100,0,6500);
	t->Draw("sumd[0]>>h");
	h->Draw();
	ave_power=(850.0/3500.0)*h->GetMean(1); // Assume that 3500 Volt  per pulse (i.e. 3500/1624 =2.15 volt per tbin) corresponds to 850 KWatt power
	cout << "Mean Power:"<<ave_power<<" KWatt" <<endl;
	power_hist->Fill(ave_power);
	myFile->Close();
    }
    power_hist->Draw();

}



