void piechart()
{
    Float_t vals[] = {34754.0,902.0,838.0,6897.0,2602.0,56.0,107.0,50.0};
    Int_t colors[] = {2,3,4,5,6,7,8,9};
    Int_t nvals = sizeof(vals)/sizeof(vals[0]);

   TCanvas *cpie = new TCanvas("cpie","TPie test",700,700);


   TPie *pie3 = new TPie("pie3",
      "n3He Run Status for Beam Cycle 1",nvals,vals,colors);

   pie3->SetY(.32);
   pie3->GetSlice(1)->SetFillStyle(3031);
   pie3->SetLabelsOffset(-.1);
   pie3->Draw("3d t nol");
   pie3->SetEntryLabel(0,"GOOD");
   pie3->SetEntryLabel(1,"Partial Last Event");
   pie3->SetEntryLabel(2,"Header Issue");
   pie3->SetEntryLabel(3,"Partial or No Beam");
   pie3->SetEntryLabel(4,"Short Runs");
   pie3->SetEntryLabel(5,"Sync Issue");
   pie3->SetEntryLabel(6,"No Data File");
   pie3->SetEntryLabel(7,"Diff No of entries");


   TLegend *pieleg = pie3->MakeLegend();
   pieleg->SetY1(.56); pieleg->SetY2(.86);

}
