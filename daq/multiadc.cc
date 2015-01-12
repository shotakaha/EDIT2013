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
const int Nmod        = 2;
const int ModId[Nmod] = {15, 16};

// Number of channels you are using for each ADC
const int Nch       = 8;
const int ChId[Nch] = {0, 1, 2, 4, 5, 6, 7, 8};

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
void Usage(const char* prog)
{
    fprintf(stderr, "Usage:\t%s event# ofn\n", prog);
}

// __________________________________________________
int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Error:\tWrong number of arguments. (%d)\n\n", argc);
        Usage(argv[0]);
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
    int buf[Nmod][Nch] = {};
    int iwhile = 0;
    double now = 0;
    int sec = 0, usec = 0;

    // I'm not sure if I need this part. DAQ seems working fine.
    // for (int imod = 0; imod < Nmod; imod++) {
//         printf("Enable LAM for Mod #%d :Started\n", imod);
//         naf = NAF(ModId[imod], ChId[0], ELAM);
//         camac = CAMAC(naf, &data, &q, &x);
//         printf("Enable LAM for Mod #%d :Finished\n", imod);
//     }

    printf("Start event loop : %d events\n", nevent);
    for(int ievent = 0; ievent < nevent; ievent++) {
        printf("--------------------------------------------------\n");
        printf("Processed : %8d / %8d events.\n", ievent, nevent);

        // Clear previous data
        for (int imod = 0; imod < Nmod; imod++) {
            for (int ich = 0; ich < Nch; ich++) {
                // Clear data. ##########
                naf   = NAF(ModId[imod], ChId[ich], CDATA);
                camac = CAMAC(naf, &data, &q, &x);

                // Clear LAM ##########
                naf   = NAF(ModId[imod], ChId[ich], CLAM);
                camac = CAMAC(naf, &data, &q, &x);
            }
        }

        // Start 1 event loop
        // Test LAM. ##########
        for (int imod = 0; imod < Nmod; imod++) {
            // fprintf(stdout, "Mod #%d TestLAM\n", imod);
            q = 0;
            while(q == 0){
                naf   = NAF(ModId[imod], ChId[0], TLAM);
                camac = CAMAC(naf, &data, &q, &x);
                iwhile++;
            }
            
            // Read data. ##########
            for (int ich = 0; ich < Nch; ich++) {
                naf   = NAF(ModId[imod], ChId[ich], READ);
                camac = CAMAC(naf, &data, &q, &x);
                buf[imod][ich] = data;    // store data
            }
        } // End of 1event
        
        now = gettimeofday_sec_usec(&sec, &usec);
        //printf("PrintData: %10.20f [sec] since epoch\n", now);
        for (int imod = 0; imod < Nmod; imod++) {
            for (int ich = 0; ich < Nch; ich++) {
                if (imod == 0 && ich ==0) printf("PL0 : ");
                if (imod == 0 && ich ==4) printf("\nPL1 : ");
                if (imod == 1 && ich ==0) printf("PL2 : ");
                if (imod == 1 && ich ==4) printf("\nPL3 : ");
                fprintf(stdout, "%4d ", (buf[imod][ich] > 150 ? buf[imod][ich] : 0));
                fprintf(fout, "%4d ", buf[imod][ich]);    // write data
            }
            fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");

        fprintf(fout, "%10d %10d ", sec, usec);    // write timestamp
        fprintf(fout, "%s", opt.Data());    // write timestamp
        fprintf(fout, "\n");
        fflush(fout);
    }// End of ALL events
    
    printf("Finished event loop : %d events\n", nevent);
    fclose(fout);
    return 0;
}
