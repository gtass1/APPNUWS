/*
 * PlotTestTomoFCut_1.C
 *
 *      Author: gianf
 */
//
// Run it inside rappnuwsanalyzer: $RAPPNSDIR/rappnuwsanalyzer.exe -I
// Otherwise, to run in ROOT, you need add the class declaration and to load the dictionaries
//

#include "TomoFCut.C"

void PlotTestTomoFCut_1(TFile *f0, Bool_t useCut=true, TFile *f1=0x0, TString addTitle0="with Source", TString addTitle1="without Source") {

//    gROOT->LoadMacro("$RAPPNSDIR/../macros/TomoFCut.C");

    TTree *rec0 = (TTree*) f0->Get("RecoData");
    TTree *rec1 = 0x0;
    if (f1!=0x0) { rec1 = (TTree*) f1->Get("RecoData"); }

    TClonesArray *scatt0 = new TClonesArray("rAPPNUWSBTTTTracksScatter");
    rec0->SetBranchAddress("BTTTTracksScatter",&scatt0);

    TClonesArray *scatt1 = new TClonesArray("rAPPNUWSBTTTTracksScatter");
    if (rec1!=0x0) { rec1->SetBranchAddress("BTTTTracksScatter",&scatt1); }

    TF1 *ftmcut = new TF1("ftmcut",TomoFCut,0,500,4);
    ftmcut->SetParameters(0.0061,0.000465,150,20);

    std::vector<double> pos0x;
    std::vector<double> pos0y;
    std::vector<double> pos0z;

    for (int iev=0; iev<rec0->GetEntries(); ++iev) {
        rec0->GetEntry(iev);
        for (int isp=0; isp<scatt0->GetEntries(); ++isp) {
            rAPPNUWSBTTTTracksScatter* tmpScPnt = (rAPPNUWSBTTTTracksScatter*) scatt0->At(isp);
            TVector3 *scpos = tmpScPnt->Getpos();
            Double_t scposArr[3];
            scpos->GetXYZ(scposArr);
            if (useCut) {
                if ( tmpScPnt->Getangle() < ftmcut->Eval(tmpScPnt->Getdist()) ) {
                    pos0x.push_back(scposArr[0]);
                    pos0y.push_back(scposArr[1]);
                    pos0z.push_back(scposArr[2]);
                }
            } else {
                pos0x.push_back(scposArr[0]);
                pos0y.push_back(scposArr[1]);
                pos0z.push_back(scposArr[2]);
            }
        }
    }

    std::vector<double> pos1x;
    std::vector<double> pos1y;
    std::vector<double> pos1z;
    if (rec1!=0x0) {
        for (int iev=0; iev<rec1->GetEntries(); ++iev) {
            rec1->GetEntry(iev);
            for (int isp=0; isp<scatt1->GetEntries(); ++isp) {
                rAPPNUWSBTTTTracksScatter* tmpScPnt = (rAPPNUWSBTTTTracksScatter*) scatt1->At(isp);
                TVector3 *scpos = tmpScPnt->Getpos();
                Double_t scposArr[3];
                scpos->GetXYZ(scposArr);
                if (useCut) {
                    if ( tmpScPnt->Getangle() < ftmcut->Eval(tmpScPnt->Getdist()) ) {
                        pos1x.push_back(scposArr[0]);
                        pos1y.push_back(scposArr[1]);
                        pos1z.push_back(scposArr[2]);
                    }
                } else {
                    pos0x.push_back(scposArr[0]);
                    pos0y.push_back(scposArr[1]);
                    pos0z.push_back(scposArr[2]);
                }
            }
        }
    }


    TGraph2D *g0 = new TGraph2D(pos0x.size(), pos0z.data(), pos0x.data(), pos0y.data());

    g0->SetTitle(Form("track segments deviation %s;z [mm];x [mm];y [mm]",addTitle0.Data()) );
    g0->SetMarkerSize(0.6);
    g0->SetMarkerStyle(8);
    g0->SetMarkerColor(38);
    g0->SetFillColor(38);

    TCanvas *c0 = new TCanvas();
    c0->cd();
    g0->Draw();

    if (rec1!=0x0) {
        TGraph2D *g1 = new TGraph2D(pos1x.size(), pos1z.data(), pos1x.data(), pos1y.data());
        g1->SetTitle(Form("track segments deviation %s;z [mm];x [mm];y [mm]",addTitle1.Data()) );
        g1->SetMarkerSize(0.6);
        g1->SetMarkerStyle(8);
        g1->SetMarkerColor(46);
        g1->SetFillColor(46);

        TCanvas *c1 = new TCanvas();
        c1->cd();
        g1->Draw();
    }

}
