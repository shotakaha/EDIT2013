




//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////




class Edit2013 :
{
private:
    TChain *fChain;
    Bool_t IsChainExists;
    TString fTreeName;
public:
    Edit2013();
    ~Edit2013();
    void Generator();
    void ConvertText2Root(const char* ifn, const char* ofn);
    void ReadData(const char* ifn);
    void Display();
};

// __________________________________________________
Edit2013::Edit2013()
{
    std::cout << "Edit2013 Constructor" << std::endl;
    fTreeName = "adc";
    IsChainExists = kFALSE;
}

// __________________________________________________
Edit2013::~Edit2013()
{
    std::cout << "Edit2013 Destructor" << std::endl;

}

// __________________________________________________
void Edit2013::ReadData(const char*ifn)
{
    if (!IsChainExists) {
        fChain = new TChain(fTreeName);
        IsChainExists = kTRUE;
    }
    fChain->Add(ifn);
    std::cout << fChain->GetEntries() << std::endl;
}

// __________________________________________________
Int_t main(Int_t argc, char** argv)
{
    Edit2013 *edit = new Edit2013();
    for (Int_t iarg = 0; iarg < argc; iarg++) {
        edit->ReadData(argv[iarg]);
    }
}



//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


    
