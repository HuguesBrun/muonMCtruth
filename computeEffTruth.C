
TChain *chain = new TChain("tpTree/fitter_tree");



computeEffTruth(){

    

     chain->Add("/tmp/hbrun/tnpZ_theTreeCleaned.root");
    
    TString denominator = "(pt>20&&tag_pt>20&&abseta<2.4&&abs(tag_eta)<2.4&&mass>60&&mass<120&&mcTrue==1)";
    TString passMu17Mu8 = "((DoubleMu17Mu8_Mu17&&tag_DoubleMu17Mu8_Mu8leg)||(tag_DoubleMu17Mu8_Mu17&&DoubleMu17Mu8_Mu8leg))";
    TString passMu17TkMu8 = "((DoubleMu17TkMu8_Mu17&&tag_DoubleMu17TkMu8_TkMu8leg)||(tag_DoubleMu17TkMu8_Mu17&&DoubleMu17TkMu8_TkMu8leg))";
    
    TH1F *massDenominator = new TH1F("massDenominator","",40,60,120);
    chain->Draw("mass>>massDenominator",denominator);

    TH1F *massPassingMu17Mu8 = new TH1F("massPassingMu17Mu8","",40,60,120);
    chain->Draw("mass>>massPassingMu17Mu8",denominator+"&&"+passMu17Mu8);
                
    TH1F *massPassingMu17TkMu8 = new TH1F("massPassingMu17TkMu8","",40,60,120);
    chain->Draw("mass>>massPassingMu17TkMu8",denominator+"&&"+passMu17TkMu8);

    

    cout << "eff of Mu17Mu8 = " << 1.0*massPassingMu17Mu8->Integral()/massDenominator->Integral() << endl;
    cout << "eff of Mu17TkMu8 = " << 1.0*massPassingMu17TkMu8->Integral()/massDenominator->Integral() << endl;
    

    
}
