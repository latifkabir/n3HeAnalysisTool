// Filename: InstAsym.cc
// Description: n3He Summary Tree Maker Class
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Mar 16 00:48:40 2015 (-0400)
// Package-Requires: ROOT, TBranchBinary
// URL: latifkabir.github.io


//Note: The channel map is continuous in the created root file unlike the binary file.

#include<iostream>
#include<string.h>
#include<fstream>
#include<stdio.h>
#include<iomanip>
#include<cmath>
#include<TFile.h>
#include<TH1D.h>
#include"InstAsym.h"
#include"Constants.h"
using namespace std;


InstAsym::InstAsym(int runNumber)
{ 
    run=runNumber;   
    det_data=new detData2;
    dir_data=new dirData2;
    t=new TTreeRaw(run);

    fill_status=true; //Whether to continue fillling tree
    error_code=1;
    nevents=24991;

    ctbin_i=0;  // Clean data first time bin  
    ctbin_f=49; //Clean data last time bin
    dtbin_i=0; // Dirty data first time bin
    dtbin_f=1624; //Dirty data last time bin
    ntbin_c=(ctbin_f-ctbin_i); //Clean data number of time bins per event
    ntbin_d=(dtbin_f-dtbin_i); //Dirty data number of time bins per event
    spin_cutoff=400; //Sum of absolute value of RFSF signal in Volts.
    beam_cutoff=1.5; //Minimum average beam power in volt
    no_beam_cutoff=5000; //Maximum number of pulses with no beam allowed
    histo_range=0.5;

}

InstAsym::~InstAsym()
{
    delete det_data;
    delete dir_data;
    delete t;
    // delete T; //deleting TFile will automatically delete T, so deleting again will give rise to crash in loop.
}
//Load Detector(Clean) DAQ event
void InstAsym::GetDetEntry(TBranch *b,int i)
{
    b->SetAddress(&det_data->header[0]);
    b->GetEntry(i); 
}

//Load Dirty DAQ event
void InstAsym::GetDirEntry(TBranch *b,int i)
{
    b->SetAddress(&dir_data->header[0]);
    b->GetEntry(i); 
}

//Get All the branches
int InstAsym::GetAllBranches( int run,TBranch **b)
{
    //Get the branches
    b[0]=t->GetBranch("b21");
    b[1]=t->GetBranch("b22");
    b[2]=t->GetBranch("b23");
    b[3]=t->GetBranch("b24");
    b[4]=t->GetBranch("b30");

    int entries[5];
    entries[0]=b[0]->GetEntries();
    entries[1]=b[1]->GetEntries();
    entries[2]=b[2]->GetEntries();
    entries[3]=b[3]->GetEntries();
    entries[4]=b[4]->GetEntries();

    if((entries[0]==entries[1] && entries[2]==entries[3] && entries[3]==entries[4] && entries[0]==entries[4] ))
	return (entries[0]);
    else if((entries[0]==entries[1] && entries[2]==entries[3] && entries[3]==entries[0]) && (entries[0]==entries[4] -1))
    {
	error_code=0;
	return (entries[0]);
    }
    else if((entries[0]==entries[1] && entries[2]==entries[3] && entries[3]==entries[0]) && (entries[4]==entries[0] -1))
    {
	error_code=0;
	return (entries[4]);
    } 
    else
	return -1;
}

//Fill Clean DAQ events
void InstAsym::FillClean(TBranch **b,int entry,double sumc[][36],double &norm)
{
    double volt;
    norm=0;    
    for(int i=0;i<4;i++)
    {
	GetDetEntry(b[i],entry);
	for(int nch=0;nch<36;nch++)
	{
	    sumc[i][nch]=0;
	    //Loops through the sample for the loaded event
	    for(int k=ctbin_i;k<ctbin_f;k++)
	    {
		volt=((nch<18)? (det_data->det[k][nch]>>8)*FACTOR : (det_data->det[k][nch+6]>>8)*FACTOR); //This makes channels map in root file continuous.
		sumc[i][nch]=sumc[i][nch]+volt;
		if(abs(volt) > 5)
		{
		    fill_status=false;
		    error_code=-1;
		}
	    }
	    if((i==1 && nch==29) || (i==3 && nch==29) || (i==3 && nch==33)) //Fix polarity issues in some wires
	    {
		sumc[i][nch]=-1.0*sumc[i][nch];
	    }
	    norm+=sumc[i][nch];
	}
    }
}

//Fill dirty DAQ events
void InstAsym::FillDirty(TBranch *b,int entry,double *sumd,int &spin)
{
    double volt;
    double beam_avg; //Average Beam intensity per sample  
    GetDirEntry(b,entry);
    for(int nch=0;nch<2;nch++)
    {
	sumd[nch]=0;
	//Loops through the sample for the loaded event
	for(int k=dtbin_i;k<dtbin_f;k++)
	{
	    volt=(dir_data->sig[k][nch]>>8)*FACTOR;
	    sumd[nch]=sumd[nch]+abs(volt);
	    if(abs(volt) > 5)
	    {
		fill_status=false;
		error_code=-1;
	    }
	}
    }

    beam_avg=sumd[0]/ntbin_d; //Typical value is 2.15 Volt per sample
    if(sumd[1]>spin_cutoff)
	spin=1;
    else
	spin=0;
}

void InstAsym::FillAsym(int entry,double sumc[][36],double sumc_prev[][36],double norm,double norm_prev,int spin,int spin_prev,double asym[][36])
{
    double up;  // By up I actually mean RFSF ON state in this script.
    double down; // By down I actually mean RFSF OFF state in this script.

    for(int i=0;i<4;i++)
    {
    	for(int nch=0;nch<36;nch++)
    	{    	
    	    if(entry == 0)
    		asym[i][nch]=run;
	    else
	    {
		up=sumc[i][nch]/norm;
		down=sumc_prev[i][nch]/norm_prev;

		if(spin==1 && spin_prev==0)
		{
		    asym[i][nch]=(up - down)/20; //(up + down)  //This is actually (SF_on -SF_off) which corresponds to (Spin_down - Spin_up), So we need to correct the sign while filling histogram. 20 is the full range of ADc  
                    //check range here
		    if(abs(asym[i][nch]) > histo_range)
		    {
			error_code=-10;
			fill_status=false;
		    }
		}
		else if(spin==0 && spin_prev==1)
		{
		    asym[i][nch]=(down - up)/20; //(up + down) //20 is the full range of ADC
                    //check range here
		    if(abs(asym[i][nch]) > histo_range)
		    {
			error_code=-10;
			fill_status=false;
		    }
		}
		else
		{
		    asym[i][nch]=-1.0;
		    fill_status=false;
		    error_code=-8;
		}
	    }
    	}
    }
}


//Fill the tree
void InstAsym::FillInstData(TH1D ***myHist)
{
    double sumc[4][36]={0.0};
    double sumc_prev[4][36]={0.0};
    double sumd[2]={0.0};     //Dirty DAQ sum of samples per event
    int spin=0;          //Spin state of current event
    int spin_prev=0;     //Spin state of last event
    double asym[4][36]={0.0};
    double norm=0; //Sum of all detector signals to be used for normalization
    double norm_prev=0;

    TBranch *br[5]; // Branch does not work with new operator
    nentries=GetAllBranches(run,br);
    if(nentries==-1)
    {
    	cout<<"Bad Run with different number of entries in five DAQs"<<endl;
    	error_code=-3;
    	fill_status=false;
    	return;
    }
    if(nentries<nevents)
    {
	error_code=-4;
    	fill_status=false;
    	return;
    }

    for(int i = 0;i < nevents; i++)
    {
	memcpy(sumc_prev,sumc,sizeof(sumc));
	norm_prev=norm;
	spin_prev=spin;

	FillClean(br,i,sumc,norm);
	FillDirty(br[4],i,sumd,spin);
	FillAsym(i,sumc,sumc_prev,norm,norm_prev,spin,spin_prev,asym);
	
	if(!fill_status)
	    break;

       //Fill the histograms here
	if( i!=0 && i%2==1)
	{
	    for(int k=0;k<4;k++)
	    {
		for(int nch=0;nch<36;nch++)
		    myHist[k][nch]->Fill(-1.0*asym[k][nch]);
	    }
	}
    }
}

//Keep record of run goodness
void InstAsym::RunList(int runNumber)
{
    ofstream runlist("/home/kabir/GIT/n3HeAnalysisTool/LevelData/InstAsym_runList.txt",ofstream::app);

    if(runlist)
    {
	runlist <<setw(30);
	runlist<<runNumber<<"                                  "<<error_code<<"                             ";
	switch(error_code)
	{
	case 0:
	    runlist<<"GOOD_NO_BEAM"<<endl;
	    break;
	case -1:
	    runlist<<"HEADER_ISSUE"<<endl;
	    break;
	case -2:
	    runlist <<"PARTIAL_OR_NO_BEAM"<<endl;
	    break;
	case -3:
	    runlist <<"DIFFERENT_NUMBER_OF_ENTRIES"<<endl;
	    break;
	case -4:
	    runlist <<"SHORT_RUN"<<endl;
	    break;
	case -5:
	    runlist <<"SYNC_ISSUE"<<endl;
	    break;
	case -6:
	    runlist <<"NO_DATA_FILE"<<endl;
	    break;
	case -7:
	    runlist << "WRONG_CHOPPER_PHASE" <<endl;
	    break;
	case -8:
	    runlist << "WRONG_RFSF_PHASE" <<endl;
	    break;
	case -9:
	    runlist << "HI_VOLTAGE_OFF" <<endl;
	    break;
	case -10:
	    runlist << "TOO_LARGE_ASYMMETRY" <<endl;
	    break;
	default:
	    runlist<<"GOOD_NO_BEAM"<<endl;
	}

	runlist.close();
    }
}

//Make the tree
void InstAsym::CalAsym(TH1D ***myHist, double range)
{
    if(!t->runExist)
    {
	cout<<"The run does NOT exist"<<endl;
	error_code=-6;
	RunList(run);
	return;
    }
    cout << "Now checking if the run is a NO beam run ..." <<endl;
    TH1D *h=new TH1D("h","h",200,-1.5e9,1.5e9);
    t->Draw("d30[][0]>>h","d30[][0]>0","goff");
    
    if(h->GetEntries()!=0)
    {
	cout << "Run number "<<run<<" has Non zero beam. Skipping the run."<<endl;
	return;
    }
    delete h;

    cout <<"Reading raw files for run number: "<<run<<"... ..." <<endl;
    histo_range=range;
    FillInstData(myHist);  
    if(fill_status)
    {
	// T->Print();  
	// T->Write();
	// f->Write(); // Saves current directory (Note trees  and histograms are created in current directory).
	cout<<"\nThe run "<<run<<" is GOOD. Done filling asymmetries for the run."<<endl;
    }
    else
    {
	switch(error_code)
	{
	case -1:
	    cout << "The Run "<<run<<" has Trigger/Header issue" <<endl;
	    break;
	case -2:
	    cout << "The run "<<run<<" has no beam, partial beam or too many dropped pulses" <<endl;
	    break;
	case -3:
	    cout << "The run "<<run<<" has branches with different number of enntries" <<endl;
	    break;
	case -4:
	    cout << "The run "<<run<<" is a short run" <<endl;
	    break;
	case -5:
	    cout << "The run "<<run<<" has synchronization issue among all  five DAQs" <<endl;
	    break;
	case -6:
	    cout << "The run "<<run<<" has NO data files on disk." <<endl;
	    break;
	case -7:
	    cout << "The run "<<run<<" has wrong chopper phases." <<endl;
	    break;
	case -8:
	    cout << "The run "<<run<<" has wrong RFSF phases." <<endl;
	    break;
	case -9:
	    cout << "The run "<<run<<" has hi voltage turned off." <<endl;
	    break;
	case -10:
	    cout << "The run "<<run<<" has too large asymmetry." <<endl;
	    break;
	default:
	    cout<<"The run "<<run<<" is GOOD. Done filling Asymmetries."<<endl;
	}

    	cout<<"Skipping the run number "<<run<<" . --BAD RUN."<<endl;
    }
    RunList(run);
}


//Typical sumd[0] is ~3400 Volts at 840KW beam power.
//Typical sumd[0] for dropped pulses is ~340 volts at 840KW beam power. 
//At 850KW max d30[][0] signal 500-600 x 10^6
//For dropped pulses average d30[][0] < 5 x10^7
