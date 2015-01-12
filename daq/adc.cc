#include <iostream>
#include <sys/time.h>

#include "/opt/hep/kinoko/drv/camdrv/camlib.h"
#include "TString.h"

// CAMAC functions
const int READ  = 0;
const int CREAD = 2;
const int TLAM  = 8;
const int CDATA = 9;
const int CLAM  = 10;
const int DLAM  = 24;
const int ELAM  = 26;

// Number of ADCs, and its station number
const int ModId = 2;

// Number of channels you are using for each ADC
const int ChId = 11;

//#define VERBOSE

// __________________________________________________
double gettimeofday_sec_usec(int *sec, int *usec)
{
    struct timeval t;
    gettimeofday(&t, NULL);
    *sec  = t.tv_sec;
    *usec = t.tv_usec;
    return (double)t.tv_sec + (double)t.tv_usec * 1e-6;
}

// __________________________________________________
void Usage()
{
    fprintf(stderr, "Usage:\t./adc event# ofn\n");
}

// __________________________________________________
int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Error:\tWrong number of arguments. (%d)\n\n", argc);
        Usage();
        fprintf(stderr, "\n");
        exit(-1);
    }
    int nevent = atoi(argv[1]);
    TString ofn = argv[2];

    // Check existence of output filename
    FILE *fin;
    fin = fopen(ofn.Data(), "r");
    if (fin) {
        fprintf(stderr, "Error:\tFile '%s' already exist.\n", ofn.Data());
        exit(-1);
    }
    
    TString opt = "";
    if (argc >= 3) {
        for (int iarg = 3; iarg < argc; iarg++) {
            opt += TString(argv[iarg]) + " ";
        }
    }
        
    // Initialize CAMAC, returns 0 if it is OK
    printf("CAMAC initialization : Started\n");
    if (COPEN())       fprintf(stderr, "Error:\tCAMAC open error\n");
    if (CGENZ())       fprintf(stderr, "Error:\tCAMAC initialization error\n");
    if (CELAM(0xffff)) fprintf(stderr, "Error:\tCAMAC LAM enable error\n");
    printf("CAMAC initialization : Finished\n");
    
    FILE *fout;
    fout = fopen(ofn.Data(), "w");
    
    int naf = 0, camac = 0, q = 0, x = 0, data = 0;
    int buf = 0;
    int iwhile = 0;
    double now = 0;
    int sec = 0, usec = 0;

    printf("Start event loop : %d events\n", nevent);
    for(int ievent = 0; ievent < nevent; ievent++) {
        printf("--------------------------------------------------\n");
        printf("Processed : %8d / %8d events.\n", ievent, nevent);
        //printf("Clearing Started\n");

        // Clear data. ##########
        //printf("(Mod, Ch)=(%2d, %2d) : ClearData Started ", ModId,ChId);
        //printf(" : naf: %4d, camac: %4d, q:%4d, x:%4d, data:%4d", naf, camac, q, x, data);
        //printf("\n");
        naf   = NAF(ModId, ChId, CDATA);
        camac = CAMAC(naf, &data, &q, &x);
        //printf("(Mod, Ch)=(%d, %2d) : ClearData Finished", ModId, ChId);
        //printf(" : naf: %4d, camac: %4d, q:%4d, x:%4d, data:%4d", naf, camac, q, x, data);
        //printf("\n");

        // Clear LAM ##########
        //printf("(Mod, Ch)=(%d, %2d) : ClearLAM  Started ", ModId, ChId);
        //printf(" : naf: %4d, camac: %4d, q:%4d, x:%4d, data:%4d", naf, camac, q, x, data);
        //printf("\n");
        naf   = NAF(ModId, ChId, CLAM);
        camac = CAMAC(naf, &data, &q, &x);
        //printf("(Mod, Ch)=(%d, %2d) : ClearLAM  Finished", ModId, ChId);
        //printf(" : naf: %4d, camac: %4d, q:%4d, x:%4d, data:%4d", naf, camac, q, x, data);
        //printf("\n");
        
        // Test LAM. ##########
        q = 0;
        //fprintf(stdout, "Mod #%d TestLAM\n", ModId);
        while(q == 0){
            //fprintf(stderr, ".");
            //if (iwhile == 80) {
            //fprintf(stderr, "\r");
            //iwhile = 0;
            //}
            naf   = NAF(ModId, ChId, TLAM);
            camac = CAMAC(naf, &data, &q, &x);
            iwhile++;
        }
        fprintf(stderr, "\n");
        
        // Read data. ##########
        //printf("(Mod, Ch)=(%d, %2d) : ReadData Started ", ModId, ChId);
        //printf(" : naf: %4d, camac: %4d, q:%4d, x:%4d, data:%4d", naf, camac, q, x, data);
        //printf("\n");
        naf   = NAF(ModId, ChId, READ);
        camac = CAMAC(naf, &data, &q, &x);
        buf   = data;
        //printf("(Mod, Ch)=(%d, %2d) : ReadData Finished", ModId, ChId);
        //printf(" : naf: %4d, camac: %4d, q:%4d, x:%4d, data:%4d", naf, camac, q, x, data);
        //printf("\n");

        now = gettimeofday_sec_usec(&sec, &usec);
        //printf("PrintData: %10.20f [sec] since epoch\n", now);
        fprintf(stdout, "Mod%02d CH%02d", ModId, ChId);
        fprintf(stdout, "%4d ", buf);
        fprintf(stdout, "\n");
        fprintf(stdout, "\n");

        fprintf(fout, "%4d ", buf);               // write data
        fprintf(fout, "%10d %10d ", sec, usec);   // write timestamp
        fprintf(fout, "%s", opt.Data());          // write additional info
        fprintf(fout, "\n");
        fflush(fout);
    }// End of ALL events
    printf("Finished event loop : %d events\n", nevent);
    fclose(fout);
    printf("Closing '%s'.\n", ofn.Data());
    return 0;
}

