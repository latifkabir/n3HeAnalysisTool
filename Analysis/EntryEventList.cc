// Filename: EntryEventList.cc
// Description: Making TChain out of n3He root data files.
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Sat May 23 16:12:53 2015 (-0400)
// URL: latifkabir.github.io

//TEntryList or TEventList are linked list with all typical features of linked list(i.e. Add(),Enter(), Delete(), Subtract(),GetNext() ... )
// For TTree TEntryList and TEventList are same. But For Chain they are NOT same.
//The rule of thumb is: For TTree use TEntryList
// But for chain (which require global indexing) use TEventList
// Note that: as a class TEntryList is more dynamic/rich than TEventList(i.e. TEventList lacks GetNext(), ... )

#include<TChain.h>
#include<TEventList.h>
#include<TEntryList.h>

void EntryEventList()
{
    TChain chain("T");
    // Add root files to the chain
    chain.Add("run-26230.root");
    chain.Add("run-26250.root");

    // Create a Event list having only even or odd events and skipping first event of file
    chain.Draw(">>list_temp1","Entry$%24991!=0 && Entry$%2==1","eventlist");
    TEventList *list1 = (TEventList*)gDirectory->Get("list_temp1");
    // list1->Print("all");  // Print for verification

    // Create Another List with dropped pulses events.
    chain.Draw(">>list_temp2","sumd[0]<2000","eventlist");
    TEventList *list2 = (TEventList*)gDirectory->Get("list_temp2");

    //Create another list with events around dropped pulses
    TEventList *list3=new TEventList();
    int sEvt; //Skip event
    int k=0;
    cout << "GetN():"<<list2->GetN()<<endl; // This will give umber of entries in list2

    while(k<list2->GetN())
    {
	sEvt=list2->GetEntry(k);  // This is how you get any specific Entry  from the list
	// cout << "Event:"<<sEvt <<endl; // Check it from print statement.

	for(int i=-1;i<8;i++)
	{
	    if(i!=0)
		list3->Enter(sEvt+i); // This is how you enter an element to the list
	}
	k++;
    }

    list3->Print("all"); // Its always good idea to print to check

    // Subtract dropped pulse and its surrounding events from main list (list1)
    list1->Subtract(list2);
    list1->Subtract(list3);
   
    //Impose main list for the Tree
    // chain.SetEventList(list1);
    //Now draw desired channel with the imposed cut
    // chain.Draw("asym[0][0]");
    

    //The Following is using TEntryList, so they are actually separate for each root files and NO global index is used. So good for individual TTree only, but NOT usable for Chain.
    chain.Draw(">>list_temp","sumd[0]<2000","entrylist");
    TEntryList *list = (TEntryList*)gDirectory->Get("list_temp");
    list->Print("all"); //This will print list for each root file separately

    TEntryList *mylist1=list->GetEntryList("T","run-26230.root"); // This is how you separate list corresponding to a specific root file.
    mylist1->Print("all");
    TEntryList *mylist2=list->GetEntryList("T","run-26250.root"); // This is how you separate list corresponding to a specific root file.
    mylist2->Print("all");
    mylist1->Add(mylist2);  //Now will give back list above.
    mylist1->Print("all");

    //Must delete lists to re-run same script from CINT
    delete list1;
    delete list2;
    delete list3;
}


