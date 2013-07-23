
TChain *chain = new TChain("tpTree/fitter_tree");



computeEffTruth(){

    

     chain->Add("/tmp/hbrun/tnpZ_theTreeCleaned.root");
    
    TString denominator = "(pt>20&&tag_pt>20&&abseta<2.4&&abs(tag_eta)<2.4&&abseta>1.8&&abs(tag_eta)>1.8&&mass>80&&mass<100&&mcTrue==1)";
    TString passMu17Mu17 = "((Mu17||tag_Mu17))";
    
    TH1F *massDenominator = new TH1F("massDenominator","",40,60,120);
    chain->Draw("mass>>massDenominator",denominator);

    TH1F *massPassingMu17Mu17 = new TH1F("massPassingMu17Mu17","",40,60,120);
    chain->Draw("mass>>massPassingMu17Mu17",denominator+"&&"+passMu17Mu17);
 
    

    cout << "eff of Mu17Mu17 = " << 1.0*massPassingMu17Mu17->Integral()/massDenominator->Integral() << endl;
    

    
}
