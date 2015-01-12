//#include "time.h"

// __________________________________________________
TCanvas *histText(const char* ifn)
{
    const Int_t Nch = 16;

    TString bd, tmp;
    for (Int_t ich = 0; ich < Nch; ich++) {
        tmp.Form("ch%02d/I:", ich);
        bd += tmp;
    }
    bd += TString("sec/I:usec/I");
    std::cout << bd.Data() << std::endl;
    TTree *t = new TTree("adc", "16ch Tracker");
    t->ReadFile(ifn, bd.Data());
    
    TH1D *h[Nch];
    TString hname, htitle, varexp, select;
    for (Int_t ich = 0; ich < Nch; ich++) {
        //varexp.Form("ch%02d", ich);
        //select.Form("");
        varexp.Form("((ch%02d - 200)) * 0.25 / 1.6", ich);  // gain ~ 10ADCcount
        //varexp.Form("((ch%02d - 200) / 10) * 0.25 / 1.6", ich);  // gain ~ 10ADCcount
        //varexp.Form("((ch%02d - 200.) / 10.)", ich);  // gain ~ 10ADCcount; i.e. 1p.e. = 10ADCcount
        select.Form("ch%02d > 200", ich);
        
        hname.Form("hCh%02d", ich);
        htitle.Form("%s;ADC [count];Entries [#]", hname.Data());
        h[ich] = new TH1D(hname.Data(), htitle.Data(), 200, 0., 200.);
        t->Project(h[ich]->GetName(), varexp.Data(), select.Data());
    }

    Int_t wx = gClient->GetDisplayWidth();
    Int_t wy = gClient->GetDisplayHeight();
        
    TCanvas *c = new TCanvas("cTracker", "Tracker", wx, wy);
    c->Divide(4, 4);
    for (Int_t ich = 0; ich < Nch; ich++) {
        c->cd(ich+1)->SetLogy();
        h[ich]->Draw();
    }
    return c;
}

// __________________________________________________
void eventDisplay(const char* ifn)
{
    const Int_t Nch = 16;

    TString bd, tmp;
    for (Int_t ich = 0; ich < Nch; ich++) {
        tmp.Form("ch%02d/I:", ich);
        bd += tmp;
    }
    bd += TString("sec/I:usec/I");
    //std::cout << bd.Data() << std::endl;
    std::cout << "Read : " << ifn << std::endl;
    TTree *t = new TTree("adc", "16ch Tracker");
    t->ReadFile(ifn, bd.Data());

    
    Int_t adc[Nch] = {0};
    TString bname;
    for (Int_t ich = 0; ich < Nch; ich++) {
        bname.Form("ch%02d", ich);
        t->SetBranchAddress(bname.Data(), &adc[ich]);
    }
    Int_t sec = 0;
    t->SetBranchAddress("sec", &sec);

    Int_t nentries = t->GetEntries();
    Int_t thr = 230;

    TString hname, htitle;
    hname.Form("hEventDisplay");
    htitle.Form("%s;Horizontal;Vertical;ADC count", hname.Data());
    TH2D *h = new TH2D(hname.Data(), htitle.Data(), 4, 0, 4, 4, 0, 4);
    h->SetStats(0);
    h->SetMarkerSize(2);
    h->GetZaxis()->SetRangeUser(0, 1000);
    
    TString cname, ctitle;
    ctitle.Form("%s : %d entries", ifn, nentries);
    TCanvas *c = new TCanvas("cEventDisplay", ctitle.Data());

    const Int_t nx = 4;
    const Int_t ny = 4;
    Int_t ich = 0;

    struct tm *time;
    char date[256];
    
    for (Int_t ientry = 0; ientry < nentries; ientry++){
        t->GetEntry(ientry);

        time = localtime(&sec);
        //strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", time);
        htitle.Form("%s %4d / %4d events", date, ientry, nentries);
        h->SetTitle(htitle);

        for (Int_t ix = 0; ix < nx; ix++) {
            for (Int_t iy = 0; iy < ny; iy++) {
                ich = ix + 12 - 4 * iy;
                //if (adc[ich] > thr) h->Fill(ix, iy, adc[ich]-thr);
                h->Fill(ix, iy, adc[ich]);
            }
        }

        h->Draw("colztext");
        c->Update();
        gSystem->ProcessEvents();
        gSystem->Sleep(2000);
        

        // std::cout << "type any key to continue > ";
//         char ans = 0;
//         std::cin >> ans;
//         if (ans == 'q') break;

        h->Reset();
    }
    h->Delete();
    c->Delete();
    return;
}


// __________________________________________________
Int_t tracker(const Int_t &start = 0)
{
    TString ifn;
    for (Int_t irun = start; irun < 100; irun++) {
        ifn.Form("../data/20130308/cosmic%03d.txt", irun);
        //ifn.Form("../data/20130308/cosmictest%03d.txt", irun);
        eventDisplay(ifn.Data());
    }

}

