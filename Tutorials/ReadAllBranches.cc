// Filename: ReadALlBranches.cc
// Description: Sample code to show how to read all branches of a tree. 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Tue Oct 13 10:24:27 2015 (-0400)
// URL: latifkabir.github.io

#include<TFile.h>
#include<TNtuple.h>
#include<TFile.h>
#include<TH1F.h>
#define BRANCH(var)					\
    float var; TBranch* b_##var=ntp->GetBranch(#var);	\
    if(b_##var) ntp->SetBranchAddress(#var,&var)

// Note: The # operator causes a replacement-text token to be converted to a string surrounded by quotes. 
// The ## operator is used to concatenate two tokens.

void ReadAllBranches()
{

    //Read the Ntuple input file(the beam profile)
    TFile* f = new TFile("/home/siplu/GIT/n3He_Soft/Github/Simulation/data/mcstas.root");
    TNtuple* ntp = (TNtuple*)f->Get("mcstas");
    int nEntries=ntp->GetEntries();
    ntp->Print();

    // set up ntuple for each baranch
    BRANCH(l);    //Branch for wavelength
    BRANCH(pol);  //Branch for polarization
    BRANCH(p5);   //Branch for statistical weight at position-5
    BRANCH(p6);   //Branch for statistical weight at position-6
    BRANCH(t6);   //Branch for time at which neutron reaches position-6
    BRANCH(x6);   //Branch for neutron x-coard at position-6
    BRANCH(y6);   //Branch for neutron y-coard at position-6
    BRANCH(vx6);  //Branch for neutron velocity V_x at position-6
    BRANCH(vy6);  //Branch for neutron velocity V_y at position-6
    BRANCH(vz6);  //Branch for neutron velocity V_z at position-6
    BRANCH(f1);   //Branch for chopper phases of chopper-1
    BRANCH(g2);   //Branch for chopper phases of chopper-2


    for(int event=0;event<nEntries;event++)
    {
	// b_l->GetEntry(event);  // This Reads only "l" branch of the tree for the specified event
	// b_f1->GetEntry(event); // This Reads only "f1" branch of the tree for the specified event
	// b_pol->GetEntry(event); // This Reads only "pol" branch of the tree for the specified event
	ntp->GetEntry(event);  //This reads all branches of the tree for the specified event.

	cout << " \tl\t"<<l<<":\tpol\t"<<pol<<":\tf1\t"<<f1<<":\tg2\t"<<g2<<endl;

	if(event>100)
	    break;
    }

}



//=============== :Reference for the root file:==========================================
// v -->Velocity(magnitude) in m/s
// l -->Wave length (in A0)
// p -->(=p6) statistical weight at position-6, sum over p6 on the Ntuple= number of neutrins in 1 Pulse at 2MW
// x-->(=x5) x-coordinate at position-5 of the guide (in meter)
// y-->(=y5) y-coordinate at position-5 of the guide (in meter)
// z--> z=0 Local coordinate
// v_x,V_y,V_z --> Velocity at position-5 (m/s)
// t5 --> The time at which neutron reaches position-5 (in sec)
// f1 -->Phase of chopper as neutron passes the chopper-1  clockwise.
// g2 -->Phase of chopper as neutron passes the chopper-2 counter clockwise (note direction !) .
// pol--> Neutron polarization (0 to 1)
// n1,n2,m1,m2,q1,q2 --> Polarization variables.
// d6 --> How far neutron travelled V_z6 x t6


// The convention for position:

// Moderator(#0)---->Guide Edge---------->Chopper-1(#1)----------->Chopper-2(#2)----------->Chopper-3(#3)---------->Chopper-4(#4)-------->End of Guide(#5) ----->Super Mirror Polarizer(#6) ----------->RFSF -------------->Target
