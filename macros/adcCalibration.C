// __________________________________________________
TH1D *histText(const char* hname, const char* ifn)
{
    TTree *t = new TTree("adc", "MPPC test");
    TString bd = "ch01/I:sec/I:usec/I:adc/I:ch/I:pwidth/D:pheight/D";
    t->ReadFile(ifn, bd.Data());

    Int_t    adc, ch;
    Double_t ph, pw;
    t->SetBranchAddress("adc", &adc);
    t->SetBranchAddress("ch" , &ch);
    t->SetBranchAddress("pheight", &ph);
    t->SetBranchAddress("pwidth" , &pw);
    t->GetEntry(0);
    
    TString htitle;
    htitle.Form("ADC%d CH%02d;ADC [count];Entries [#]", adc, ch);

    TH1D *h = new TH1D(hname, htitle.Data(), 2500, 0, 2500);
    t->Project(h->GetName(), "ch01");

    Double_t charge = pw * ph / 50.; //[pC]
    Double_t mean   = h->GetMean();
    Double_t ratio  = charge / mean;

    printf("ADC%d CH%02d | inputQ : %6.2f [pC] / %6.2f [count] = %.3f [pC/count]\n", adc, ch, charge, mean, ratio);

    //h->Draw();
    return h;
}

// __________________________________________________
Int_t adcCalibration(const Int_t &start)
{
    
    const Int_t Ndata = 3;
    TH1D *h[Ndata];

    TString ifn, hname;
    Int_t inum = 0;
    for (Int_t ihist = 0; ihist < Ndata; ihist++) {
        inum = ihist + start;
        ifn.Form("../data/20130307/adccalib%03d.txt", inum);
        hname.Form("h%d", ihist+start);
        h[ihist] = histText(hname.Data(), ifn.Data());
        //h[ihist]->SetLineColor(inum);
        //h[ihist]->SetFillColor(inum);
    }

    TCanvas *c1 = new TCanvas("cAdcCalibration", "MPPC test", 500, 500);
    c1->SetLogy();
    for (Int_t ihist = 0; ihist < Ndata; ihist++) {
        if (ihist == 0) h[ihist]->Draw();
        else            h[ihist]->Draw("same");
    }
}

