// Filename: n3HeChain.cc
// Description: Making TChain out of n3He root data files. Very Simple Analysis Version
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Sat May 23 16:12:53 2015 (-0400)
// URL: latifkabir.github.io
#include<TChain.h>
#include<TEventList.h>

void n3HeChain()
{
    TChain chain("T");
    //========Add n3He root summary data files to the chain=================
    chain.Add("run-26230.root");
    chain.Add("run-26250.root");

    //======Create a main list of events with odd or even events only and also skip events (first event of root file) having run number as flag===== 
    chain.Draw(">>list_temp1","Entry$%24991!=0 && Entry$%2==1","eventlist");
    TEventList *list1 = (TEventList*)gDirectory->Get("list_temp1");
    
    //We Can compare the list for verification
    // list1->Print("all");   
    // chain.Scan("asym[0]","Entry$%24991==0");

    //===========Create a list having all dropped pulses events======================
    chain.Draw(">>list_temp2","sumd[0]<2000","eventlist");
    TEventList *list2 = (TEventList*)gDirectory->Get("list_temp2");
    // list2->Print("all");  // Print for verification


    //=====Create another list having events around dropped pulses.====================
    //(Adding these events dynamicly to the same list of dropped pulses did NOT work)
    TEventList *list3=new TEventList();
    int sEvt;
    int k=0;
    
    while(k<list2->GetN())
    {
	sEvt=list2->GetEntry(k);
	// cout << "Event:"<<sEvt <<endl;
	for(int i=-1;i<8;i++)
	{
	    if(i!=0)
		list3->Enter(sEvt+i);
	}
	k++;
    }

    // list3->Print("all"); //Print for verification

    //=================Subtract all dropped pulse events and events around it from main list===============
    list1->Subtract(list2);
    list1->Subtract(list3);

    //==================Impose the Cut for our chain==============================
    chain.SetEventList(list1);

    //=====================Draw The desired channel=======================
    chain.Draw("asym[0][0]");
    
    //==============Must delete lists to re-run same script from CINT smoothly============================
    delete list1;
    delete list2;
    delete list3;
}

//The rule of thumb is: For TTree use TEntryList
// But for chain (which require global indexing) use TEventList

