//
// Run it inside rappnuwsanalyzer: $RAPPNSDIR/rappnuwsanalyzer.exe -I
// Otherwise, to run in ROOT, you need add the class declaration and to load the dictionaries
//
//#include "generated/rAPPNUWSPreTomography.h"

#include <utility>

//#include "util/TomoUtils.h"
//#include "TomoFCut.C"

void TomographerResol(TString runList="0;1", TString foldBase=".", bool useCut=true/*, int fcBin=1*/) {

    //    gROOT->LoadMacro("$RAPPNSDIR/../macros/TomoFCut.C");

    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1111);

    TChain *trMC = new TChain("MCData");
    TChain *trRec = new TChain("RecoData");

    TObjArray *RunList=runList.Tokenize(";");
    int nRuns=RunList->GetEntries();

    TString frunN, fRCName, fMCName;

    for (Int_t irl=0; irl<nRuns; ++irl) {
      TString fRun = ((TObjString*)RunList->At(irl))->GetString();

      TObjArray *RunSeq=fRun.Tokenize("-");

      if (RunSeq->GetEntries()>1) {

          TString fRunStr = ((TObjString*)RunSeq->At(0))->GetString();
          TString fRunStp = ((TObjString*)RunSeq->At(1))->GetString();

          for (Int_t irsq=fRunStr.Atoi(); irsq<=fRunStp.Atoi(); ++irsq) {

              frunN=Form("%.05d.root",irsq);
              fRCName = foldBase+"/"+"RecoData"+frunN;
              fMCName = foldBase+"/"+"MCData"+frunN;
              trRec->Add(fRCName.Data());
              trMC->Add(fMCName.Data());
              cout<<"Analyzing run "<<irsq<<endl;
          }
      } else {
          frunN=Form("%.05d.root",fRun.Atoi());
          fRCName = foldBase+"/"+"RecoData"+frunN;
          fMCName = foldBase+"/"+"MCData"+frunN;
          trRec->Add(fRCName.Data());
          trMC->Add(fMCName.Data());
          cout<<"Analyzing run "<<fRun<<endl;
      }
    }

    TClonesArray *recTrkBT = new TClonesArray("rAPPNUWSRecoTracksBT");
    trRec->SetBranchAddress("RecoTracksBT",&recTrkBT);

    TClonesArray *mcTrk = new TClonesArray("rAPPNUWSGeantTrack");
    trMC->SetBranchAddress("MCTracks",&mcTrk);

//    std::unordered_map< unsigned long, std::vector< std::pair<double,double> > > voxHitMap;

    Int_t nEvents = trRec->GetEntries();
    cout<<"Number of events Stored --> "<<nEvents<<endl;

    TH1F *hresX = new TH1F("hresX","Tomoragher track extrapolation to center resolution on X (Reco-Truth);x [mm]",200,-100,100);
    TH1F *hresZ = new TH1F("hresZ","Tomoragher track extrapolation to center resolution on Z (Reco-Truth);z [mm]",200,-100,100);
    TH1F *hpullX = new TH1F("hpullX","Tomoragher track extrapolation to center pull on X",80,-10,10);
    TH1F *hpullZ = new TH1F("hpullZ","Tomoragher track extrapolation to center pull on Z",80,-10,10);
//    TH1F *hresX1 = new TH1F("hresX1","Tomoragher track extrapolation to center resolution on X (Reco-Truth);x [mm]",200,-100,100);
//    TH1F *hresZ1 = new TH1F("hresZ1","Tomoragher track extrapolation to center resolution on Z (Reco-Truth);z [mm]",200,-100,100);

    TH1F *hresTh = new TH1F("hresTh","Tomoragher track extrapolation to center resolution on #theta (Reco-Truth);#theta [rad]",200,-0.1,0.1);
    TH1F *hresPhi = new TH1F("hresPhi","Tomoragher track extrapolation to center resolution on #phi (Reco-Truth);#phi [rad]",200,-0.1,0.1);
    TH1F *hpullTh = new TH1F("hpullTh","Tomoragher track extrapolation to center pull on #theta",80,-10,10);
    TH1F *hpullPhi = new TH1F("hpullPhi","Tomoragher track extrapolation to center pull on #phi",80,-10,10);

    TH1F *hresMom = new TH1F("hresMom","Tomoragher track momentum resolution (Reco-Truth);p [GeV/c]",200,-0.5,0.5);
    TH1F *hpullMom = new TH1F("hpullMom","Tomoragher track momentum pull",80,-10,10);
    TH1F *hmomErr = new TH1F("hmomErr","Tomoragher track momentum error",2000,0,1000);
    TH1F *hmomDErr = new TH1F("hmomDErr","Tomoragher track momentum Delta error",200,-10,10);

    for (int ie=0; ie</*10*/nEvents; ++ie) {

      trRec->GetEntry(ie);  //load ie event
      trMC->GetEntry(ie);

      for (int ibttrk=0; ibttrk<recTrkBT->GetEntries(); ++ibttrk) {

          rAPPNUWSRecoTracksBT *iBTtrk = (rAPPNUWSRecoTracksBT *)recTrkBT->At(ibttrk);
          double tchi2n = iBTtrk->Getchi2()/((double)iBTtrk->Getdof());
          if (iBTtrk->GetIsFitted() && tchi2n>1e-6&&tchi2n<20) {

              Int_t mcTrkId = iBTtrk->GetTrkID();
              rAPPNUWSGeantTrack *iMCtrk=0x0;
              bool foundMCTrk=false;

              for (int imctrk=0; imctrk<mcTrk->GetEntries(); ++imctrk) {
                  iMCtrk = (rAPPNUWSGeantTrack *)mcTrk->At(imctrk);
                  if ( iMCtrk->GetfParentID()==0 && iMCtrk->GetfTrackID()==mcTrkId ) { foundMCTrk=true; break; }
              }

              if (foundMCTrk) {
                  TVector3 *mcTrkPosSt = iMCtrk->GetfPosStart();
                  TVector3 *rcBTTrkPos = iBTtrk->GetposCnt();
                  Double_t *rcBTTrkPosErr = iBTtrk->GeterrPosCnt();

                  TVector3 diff=(*rcBTTrkPos-*mcTrkPosSt);
                  hresX->Fill(diff.X());
                  hresZ->Fill(diff.Z());
                  hpullX->Fill( diff.X() / std::fmod(fabs(rcBTTrkPosErr[0]),TMath::TwoPi()) );
                  hpullZ->Fill(diff.Z() / std::fmod(fabs(rcBTTrkPosErr[2]),TMath::TwoPi()) );

//                  TVector3 *rcBTTrkPos1 = iBTtrk->GetposCnt1();
//                  TVector3 diff1=(*rcBTTrkPos1-*mcTrkPosSt);
//                  hresX1->Fill(diff1.X());
//                  hresZ1->Fill(diff1.Z());

                  double diffTh=( iBTtrk->GetthetaCnt() - iMCtrk->GetfMomentum()->Theta() );
                  double diffPhi=( iBTtrk->GetphiCnt() - iMCtrk->GetfMomentum()->Phi() );
                  hresTh->Fill(diffTh);
                  hresPhi->Fill(diffPhi);
                  //if (iBTtrk->Geterr_thetaCnt()<0.01/*TMath::PiOver2()*/)
                  //hpullTh->Fill( diffTh / fmod(iBTtrk->Geterr_thetaCnt(),TMath::Pi()) );
                  hpullTh->Fill( diffTh / iBTtrk->Geterr_thetaCnt() );
                  //if (iBTtrk->Geterr_phiCnt()<TMath::TwoPi())
                  //hpullPhi->Fill( diffPhi/ fmod(iBTtrk->Geterr_phiCnt(),TMath::Pi()) );
                  hpullPhi->Fill( diffPhi/ iBTtrk->Geterr_phiCnt() );

                  double diffMom = iBTtrk->GetMomentum() - iMCtrk->GetfMomentum()->Mag()/1000.0;
                  hresMom->Fill(diffMom);
                  hpullMom->Fill(diffMom/iBTtrk->GetErr_Momentum());
                  hmomErr->Fill(iBTtrk->GetErr_Momentum());
                  TVector3 &mom = *iBTtrk->Getmom();
                  TMatrixDSym &cov = *iBTtrk->Getcov();
                  double momErr1 = sqrt( mom[0]*mom[0]*cov[3][3] + mom[1]*mom[1]*cov[4][4] + mom[2]*mom[2]*cov[5][5] +
                                         2.0*mom[0]*mom[1]*cov[3][4] + 2.0*mom[0]*mom[2]*cov[3][5] + 2.0*mom[1]*mom[2]*cov[4][5] ) / iBTtrk->GetMomentum();
                  hmomDErr->Fill(iBTtrk->GetErr_Momentum() - momErr1);
              }
          }

      }


    }

    TCanvas *resCv= new TCanvas();
    resCv->Divide(2,2);
    resCv->cd(1);
    hresX->Draw();
    resCv->cd(2);
    hresZ->Draw();
    resCv->cd(3);
    hpullX->Draw();
    resCv->cd(4);
    hpullZ->Draw();

    TCanvas *resAngCv= new TCanvas();
//    resAngCv->Divide(2,1);
    resAngCv->Divide(2,2);
    resAngCv->cd(1);
    hresTh->Draw();
    resAngCv->cd(2);
    hresPhi->Draw();
    resAngCv->cd(3);
    hpullTh->Draw();
    resAngCv->cd(4);
    hpullPhi->Draw();

//    TCanvas *resCv1= new TCanvas();
//    resCv1->Divide(2,1);
//    resCv1->cd(1);
//    hresX1->Draw();
//    resCv1->cd(2);
//    hresZ1->Draw();

    TCanvas *resMomCv= new TCanvas();
    resMomCv->Divide(2,2);
    resMomCv->cd(1);
    hresMom->Draw();
    resMomCv->cd(2);
    hmomErr->Draw();
    resMomCv->cd(3);
    hpullMom->Draw();
    resMomCv->cd(4);
    hmomDErr->Draw();

}
