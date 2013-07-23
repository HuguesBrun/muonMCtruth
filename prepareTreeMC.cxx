#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TH1F.h"
#include <cstdlib>
#include <cmath>



void prepareTreeMC() {
	using namespace std;
    TTree *tIn  = (TTree *) gFile->Get("tpTree/fitter_tree");
    Float_t pair_probeMultiplicity;
    Int_t Glb, TM, PF, pair_BestZ;
    tIn->SetBranchAddress("pair_probeMultiplicity", &pair_probeMultiplicity);
    tIn->SetBranchAddress("pair_BestZ", &pair_BestZ);
    tIn->SetBranchAddress("Glb", &Glb);
    tIn->SetBranchAddress("TM", &TM);
    tIn->SetBranchAddress("PF", &PF);
	

    TFile *fOut = new TFile("/tmp/hbrun/tnpZ_intermediate.root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = tIn->CloneTree(0);
  

    int step = tIn->GetEntries()/1000;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
	
    //for (int i = 0, n = tIn->GetEntries(); i < n; ++i) {
	for (int i = 0, n = 500000; i < n; ++i) { //to save time use only 500000 events...
        tIn->GetEntry(i);
        //if (i > 20000) break;
        if ((i+1) % step == 0) { 
            double totalTime = timer.RealTime()/60.; timer.Continue();
            double fraction = double(i+1)/double(n+1), remaining = totalTime*(1-fraction)/fraction;
            printf("Done %9d/%9d   %5.1f%%   (elapsed %5.1f min, remaining %5.1f min)\n", i, n, i*evDenom, totalTime, remaining); 
            fflush(stdout);
        }
        int nbPFmuon = 0;
        int theRightPair = i; // the only pair with the 2 muons passing 
        if (pair_probeMultiplicity>1) {            
            int multi = (int) pair_probeMultiplicity;
            for (int j=i+1 ; j<(i+multi) ; j++){
                tIn->GetEntry(j);
                if (PF) {
                    nbPFmuon++;
                    theRightPair=j;
                }

            }
            if (nbPFmuon>1) continue; //should have only 1 probe passing PF per pair
            i=i+multi-1;//go in the next pair
            tIn->GetEntry(theRightPair); //with this we save the only pair with the probe passing the PF
        }
        
        bool passLoose = ((Glb||TM)&&PF);
        if (!(passLoose)) continue; //probe passing the loose ID (the tag is already passing it)
        tOut->Fill();
    }

    tOut->AutoSave();
    fOut->Close();
}
