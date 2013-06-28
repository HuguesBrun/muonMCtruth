#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TRandom.h" 
#include <cstdlib>
#include <cmath>


void cleanPairs(){
    using namespace std;
    TTree *tIn  = (TTree *) gFile->Get("tpTree/fitter_tree");
    Float_t pt, abseta, tag_pt, tag_eta;
 
    tIn->SetBranchAddress("pt", &pt);
    tIn->SetBranchAddress("abseta", &abseta);
    tIn->SetBranchAddress("tag_pt", &tag_pt);
    tIn->SetBranchAddress("tag_eta", &tag_eta);

    TFile *fOut = new TFile("/tmp/hbrun/tnpZ_theTreeCleaned.root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = tIn->CloneTree(0);

    
    int step = tIn->GetEntries()/100;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
	float ptNow, tagPtNow;
    float ptNext, tagPtNext;
    TRandom *rando = new TRandom();
   for (int i = 0, n = tIn->GetEntries(); i < n; ++i) {
        tIn->GetEntry(i);
        if ((i+1) % step == 0) {
            double totalTime = timer.RealTime()/60.; timer.Continue();
            double fraction = double(i+1)/double(n+1), remaining = totalTime*(1-fraction)/fraction;
            printf("Done %9d/%9d   %5.1f%%   (elapsed %5.1f min, remaining %5.1f min)\n", i, n, i*evDenom, totalTime, remaining);
            fflush(stdout);
        }

        ptNow = pt;
        tagPtNow = tag_pt;
        
        tIn->GetEntry(i+1);
        ptNext = pt;
        tagPtNext = tag_pt;
        
        if ((ptNext==tagPtNow)&&(tagPtNext==ptNow)){ //find a pair double counted
            float theRand = rando->Uniform(0,2);
            if (theRand>=1) tIn->GetEntry(i); else tIn->GetEntry(i+1); // takes randomly the first or the second occurence of the double counted pair (important only when in the 10 to 20 GeV region)
            i++;
        }
        else tIn->GetEntry(i);
        tOut->Fill();

        
       // printf("event=%i, pt=%f, abseta=%f, tag_pt=%f, tag_eta=%f \n", event, pt, abseta, tag_pt, tag_eta);
        
    }
    
    tOut->AutoSave(); 
    fOut->Close();
}
