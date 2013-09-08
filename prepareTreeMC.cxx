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
    Float_t pair_probeMultiplicity, tag_pt, pt;
    Int_t Glb, TM, PF, pair_BestZ, event;
    tIn->SetBranchAddress("pair_probeMultiplicity", &pair_probeMultiplicity);
    tIn->SetBranchAddress("pair_BestZ", &pair_BestZ);
    tIn->SetBranchAddress("Glb", &Glb);
    tIn->SetBranchAddress("TM", &TM);
    tIn->SetBranchAddress("PF", &PF);
    tIn->SetBranchAddress("event", &event);
    tIn->SetBranchAddress("tag_pt", &tag_pt);
    tIn->SetBranchAddress("pt", &pt);

	

    TFile *fOut = new TFile("/tmp/hbrun/tnpZ_intermediate.root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = tIn->CloneTree(0);
  

    int step = tIn->GetEntries()/1000;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
    int oldEvent=0;
    int iteBegin = 0;
    printf("nb Entries=%i \n",tIn->GetEntries());
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
        //if (event<762930) continue;
        //if (event>763130) continue;
	//if (event !=33082194) continue;
        int nbPFmuonInEvent = 0;
        if (event==oldEvent) {
       //    printf("real i=%i, event=%i, multi=%f, tag_pt=%f, pt=%f, PF=%i \n",i,event, pair_probeMultiplicity, tag_pt, pt, PF);
           bool passLoose = ((Glb||TM)&&PF);
            if (passLoose) tOut->Fill();//printf("on sauve !  i=%i, event=%i, multi=%f, tag_pt=%f, pt=%f, PF=%i \n",i,event, pair_probeMultiplicity, tag_pt, pt, PF);
         //   if (passLoose) printf("on sauve !  i=%i, event=%i, multi=%f, tag_pt=%f, pt=%f, PF=%i \n",i,event, pair_probeMultiplicity, tag_pt, pt, PF);

        }
        if (event!=oldEvent){
            iteBegin = i;
            oldEvent = event;
            while (event == oldEvent){
           //   printf("i=%i, event=%i, multi=%f, tag_pt=%f, pt=%f, PF=%i \n",i,event, pair_probeMultiplicity, tag_pt, pt, PF);
                if (PF) nbPFmuonInEvent++;
                i++;
                tIn->GetEntry(i);
                if (i>tIn->GetEntries()) break;
            }
        //    printf("nbPF=%i, ite=%i \n",nbPFmuonInEvent, iteBegin);
            if (nbPFmuonInEvent<=2) i=iteBegin-1;
            else i=i-1;
            
        }
        
  /*      //	printf("i=%i, event=%i, multi=%f \n",i,event, pair_probeMultiplicity);
        int nbPFmuon = 0;
        int theRightPair = i; // the only pair with the 2 muons passing 
        if (pair_probeMultiplicity>1) { 
	oldEvent=event;
//		printf("multi=%f\n",pair_probeMultiplicity);
            int multi = (int) pair_probeMultiplicity;
            for (int j=i ; j<(i+multi) ; j++){
                tIn->GetEntry(j);
                if (PF) {
                    nbPFmuon++;
                    theRightPair=j;
                }

            }  
//	    printf("nbPFmuons=%i\n",nbPFmuon);
            i=i+multi-1;//go in the next pair
            if (nbPFmuon>1) continue; //should have only 1 probe passing PF per pair
            tIn->GetEntry(theRightPair); //with this we save the only pair with the probe passing the PF
        }
        
        bool passLoose = ((Glb||TM)&&PF);
        if (!(passLoose)) continue; //probe passing the loose ID (the tag is already passing it)
//	printf("on va sauver ! ");
        tOut->Fill();*/
    }

    tOut->AutoSave();
    fOut->Close();
}
