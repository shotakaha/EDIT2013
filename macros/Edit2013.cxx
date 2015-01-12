//____________________________________________________________________
//
// DOCUMENT ME
// 

//____________________________________________________________________ 
//  
// $Id$ 
// Author: Shota TAKAHASHI <shotakaha@gmail.com>
// Update: 2013-03-01 19:52:33+0900
// Copyright: 2013 (C) Shota TAKAHASHI
//
#ifndef ROOT_Edit2013
#include "Edit2013.h"
#endif

//____________________________________________________________________
ClassImp(Edit2013);

//____________________________________________________________________
Edit2013::Edit2013()
{
    // Default constructor
}

// __________________________________________________
void Edit2013::GenerateDummyData(const char* ofn = "dummydata.txt")
{
    std::cout << "Edit2013::GenerateDummyData" << std::endl;
}

// __________________________________________________
void Edit2013::ConvertText2Root(const char* ifn, const char* ofn)
{
    std::cout << "Edit2013::ConvertText2Root" << std::endl;
    TString fTreeName = "adc";
    TString fTreeTitle = "EDIT2013";
    TString fBranchDescriptor = "ch[0]/I:ch[1]/I:ch[2]/I:ch[3]/I:ch[4]/I:ch[5]/I:ch[5]/I:ch[7]/I:ch[8]/I:ch[9]/I:ch[10]/I:ch[11]/I:ch[12]/I:ch[13]/I:ch[14]/I:ch[15]/I";

    TFile *f = new TFile(ofn, "recreate");
    TTree *t = new TTree(fTreeName.Data(), fTreeTitle.Data());
    t->ReadFile(ifn, fBranchDescriptor.Data());
    t->Write();
    f->Close()
}

//____________________________________________________________________ 
//  
// EOF
//
