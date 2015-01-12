// __________________________________________________
TH1D *histText(const char* hname, const char* ifn)
{
    TTree *t = new TTree("adc", "MPPC test");
    TString bd = "ch01/I:sec/I:usec/I:mppcid/I:bias/D:gate/D";
    t->ReadFile(ifn, bd.Data());

    Int_t    id;
    Double_t bv;
    t->SetBranchAddress("mppcid", &id);
    t->SetBranchAddress("bias"  , &bv);
    t->GetEntry(0);

    TString htitle;
    htitle.Form("MPPC%4d %.2f V;ADC [count];Entries [#]", id, bv);

    TH1D *h = new TH1D(hname, htitle.Data(), 500, 0, 500);
    t->Project(h->GetName(), "ch01");
    return h;
}

// __________________________________________________
TTree *histText2(const char* hname, const char* ifn)
{
    TTree *t = new TTree("adc", "MPPC test");
    TString bd = "ch01/I:ch02/I:sec/I:usec/I:mppcid1/I:mppcid2/I:bias/D:ledv/D";
    t->ReadFile(ifn, bd.Data());

    Int_t    id1, id2;
    Double_t bv;
    t->SetBranchAddress("mppcid1", &id1);
    t->SetBranchAddress("mppcid2", &id2);
    t->SetBranchAddress("bias"   , &bv);
    t->GetEntry(0);

    TString name, htitle;
    name.Form("%s_1", hname);
    htitle.Form("MPPC%4d %.2f V;ADC [count];Entries [#]", id1, bv);

    TH1D *h1 = new TH1D(name.Data(), htitle.Data(), 500, 0, 500);
    t->Project(h1->GetName(), "ch01");

    name.Form("%s_2", hname);
    htitle.Form("MPPC%4d %.2f V;ADC [count];Entries [#]", id2, bv);
    TH1D *h2 = new TH1D(name.Data(), htitle.Data(), 500, 0, 500);
    t->Project(h2->GetName(), "ch02");

    TCanvas *c = new TCanvas("c", "c");
    c->Divide(2, 1);
    c->cd(1);
    h1->Draw();
    c->cd(2);
    h2->Draw();
    
    return t;
}


// __________________________________________________
Int_t mppcTest()
{
    const Int_t Ndata = 9;
    TH1D *h[Ndata];

    TString ifn, hname;
    for (Int_t ihist = 0; ihist < Ndata; ihist++) {
        ifn.Form("../data/20130304/mppctest%03d.txt", ihist+1);
        hname.Form("h%d", ihist+1);
        h[ihist] = histText(ifn.Data(), hname.Data());
        h[ihist]->SetLineColor(ihist+1);
        h[ihist]->SetFillColor(ihist+1);
    }

    TCanvas *c1 = new TCanvas("cMppcTest", "MPPC test", 500, 500);
    c1->SetLogy();
    for (Int_t ihist = 0; ihist < Ndata; ihist++) {
        if (ihist == 0) h[ihist]->Draw();
        else            h[ihist]->Draw("same");
    }

    TCanvas *c2 = new TCanvas("cMppcTestDiv", "MPPC test", 500, 500);
    if (Ndata == 1);
    else if (Ndata ==  2) c2->Divide(2, 1);
    else if (Ndata <=  4) c2->Divide(2, 2);
    else if (Ndata <=  6) c2->Divide(3, 2);
    else if (Ndata <=  8) c2->Divide(4, 2);
    else if (Ndata ==  9) c2->Divide(3, 3);
    else if (Ndata <= 10) c2->Divide(5, 2);
    else if (Ndata <= 12) c2->Divide(4, 3);
    
    for (Int_t ihist = 0; ihist < Ndata; ihist++) {
        c2->cd(ihist+1)->SetLogy();
        h[ihist]->Draw();
    }
}

