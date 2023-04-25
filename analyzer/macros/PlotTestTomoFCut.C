/*
 * PlotTestTomoFCut.C
 *
 *  Created on: Mar 16, 2023
 *      Author: gianf
 */

void PlotTestTomoFCut(TFile *f0, TFile *f1=0x0, TString addTitle0="with Source", TString addTitle1="without Source") {

//    gROOT->LoadMacro("$RAPPNSDIR/../macros/TomoFCut.C");

    TTree *rec0 = (TTree*) f0->Get("RecoData");
    TTree *rec1 = 0x0;
    if (f1!=0x0) { rec1 = (TTree*) f1->Get("RecoData"); }

    TH2F *h0 = new TH2F("h0","",50,0,500,40,0,0.4);
    TH2F *h1 = new TH2F("h1","",50,0,500,40,0,0.4);

    rec0->Draw("BTTTTracksScatter.angle:BTTTTracksScatter.dist>>h0","","");
    if (rec1!=0x0) { rec1->Draw("BTTTTracksScatter.angle:BTTTTracksScatter.dist>>h1","",""); }

    h0->SetTitle(Form("track segments deviation with %s;dist [mm]; angle [rad]",addTitle0.Data()) );
    h0->SetMarkerSize(0.6);
    h0->SetMarkerStyle(8);
    h0->SetMarkerColor(38);
    h0->SetFillColor(38);

    if (rec1!=0x0) {
        h1->SetTitle(Form("track segments deviation %s;dist [mm]; angle [rad]",addTitle1.Data()) );
        h1->SetMarkerSize(0.6);
        h1->SetMarkerStyle(8);
        h1->SetMarkerColor(46);
        h1->SetFillColor(46);
    }

    TF1 *ftmcut = new TF1("ftmcut",TomoFCut,0,500,4);
//    ftmcut->SetParameters(0.0045,0.00035,150,20);
//    ftmcut->SetParameters(0.0069,0.000465,150,20);
    ftmcut->SetParameters(0.0061,0.000465,150,20);

    h0->Draw();
    if (rec1!=0x0) {
        h1->Draw("same");
    }
    ftmcut->Draw("same");

}
