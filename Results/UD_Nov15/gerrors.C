void gerrors() {
   //Draw a graph with error bars
   // To see the output of this macro, click begin_html <a href="gif/gerrors.gif">here</a>. end_html
   //Author: Rene Brun
   
   TCanvas *c1 = new TCanvas("c1","A Simple Graph with error bars",200,10,700,500);

   c1->SetFillColor(42);
   c1->SetGrid();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);

   const Int_t n = 16;
   Float_t x[n]  = {1, 2, 3, 4, 5, 6,7,8,9,10,11,12,13,14,15,16};
   Float_t y[n]  = {2.30415,-0.578002,0.577002,0.0890149,-4.53486,-4.97253,-1.26509,-1.89478,-7.33939,-3.60286,3.21164,-9.26686,-9.31284,6.95505,-5.42294,-3.49033};
   Float_t ex[n] = {0};
   Float_t ey[n] = { 8.48225,8.43192,8.60896,8.42923,8.29127,7.42627,8.42753,7.67649,7.23486,7.85891,12.0761,8.68877,7.33988,7.05461,8.85639,8.52456};
   TGraphErrors *gr = new TGraphErrors(n,x,y,ex,ey);
   gr->SetTitle("Asymmetry Trendline by Run Ranges");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("AP");

   c1->Update();
}
