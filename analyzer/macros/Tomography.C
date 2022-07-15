//
// Run it inside rappnuwsanalyzer: $RAPPNSDIR/rappnuwsanalyzer.exe -I
// Otherwise, to run in ROOT, you need add the class declaration and to load the dictionaries
//
//#include "generated/rAPPNUWSPreTomography.h"

#include "util/TomoUtils.h"

void Tomography(TString runList="0;1", TString foldBase=".", double thetaCut=0.0) {

    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1111);

    TChain *tr = new TChain("TomoData");

    TObjArray *RunList=runList.Tokenize(";");
    int nRuns=RunList->GetEntries();

    TString frunN, fTMName;

    for (Int_t irl=0; irl<nRuns; ++irl) {
      TString fRun = ((TObjString*)RunList->At(irl))->GetString();

      TObjArray *RunSeq=fRun.Tokenize("-");

      if (RunSeq->GetEntries()>1) {

          TString fRunStr = ((TObjString*)RunSeq->At(0))->GetString();
          TString fRunStp = ((TObjString*)RunSeq->At(1))->GetString();

          for (Int_t irsq=fRunStr.Atoi(); irsq<=fRunStp.Atoi(); ++irsq) {

              frunN=Form("%.05d.root",irsq);
              fTMName = foldBase+"/"+"TomoData"+frunN;
              tr->Add(fTMName.Data());
              cout<<"Analyzing run "<<irsq<<endl;
          }
      } else {
          frunN=Form("%.05d.root",fRun.Atoi());
          fTMName = foldBase+"/"+"TomoData"+frunN;
          tr->Add(fTMName.Data());
          cout<<"Analyzing run "<<fRun<<endl;
      }
    }

    TClonesArray *pretomo = new TClonesArray("rAPPNUWSPreTomography");
    tr->SetBranchAddress("PreTomography",&pretomo);

    std::unordered_map< unsigned long, std::vector<double> > voxHitMap;

    Int_t nEvents = tr->GetEntries();
    cout<<"Number of events Stored --> "<<nEvents<<endl;

    for (int ie=0; ie</*10*/nEvents; ++ie) {

      tr->GetEntry(ie);  //load ie event

      for (int iptd=0; iptd<pretomo->GetEntries(); ++iptd) {

          rAPPNUWSPreTomography *iVxHit = (rAPPNUWSPreTomography *)pretomo->At(iptd);
          std::vector<Double_t> &iVxAllTh = iVxHit->Getalltheta();
          for ( auto &itheta : iVxAllTh ) {
              if (itheta>thetaCut) {
                  voxHitMap[iVxHit->GethitId()].push_back(itheta);
              }
          }

//          int tmpId[3] = {-1,-1,-1};
//          UIDtoID(iVxHit->GethitId(), tmpId);

//          std::cout<<"preTomoHit Id "<<iVxHit->GethitId()<<" back to ID "<<tmpId[0]<<" "<<tmpId[1]<<" "<<tmpId[2]<<std::endl;

      }
    }

    std::cout<<"Vox Hit Map size "<<voxHitMap.size()<<std::endl;

    TH2F *htomoZX = new TH2F("htomoZX","Tomoraghy progection on orizontal plane",5000,0,5000,1000,0,1000);
    TH2F *htomoZY = new TH2F("htomoZY","Tomoraghy progection on vertical plane",5000,0,5000,1000,0,1000);

    for ( auto& voxHit : voxHitMap)  {
        int tmpId[3] = {-1,-1,-1};
        UIDtoID(voxHit.first, tmpId);
        double mtheta=0;
        double sgmtheta=0;
        for ( auto& itheta : voxHit.second ) {
            mtheta+=itheta;
            sgmtheta+=itheta*itheta;
        }
        mtheta/=(double) voxHit.second.size();
        sgmtheta/=(double) voxHit.second.size();
        sgmtheta=sqrt( sgmtheta-mtheta*mtheta );

        if (voxHit.second.size()>1) {
        std::cout<<"preTomoHit Id "<<voxHit.first<<" back to ID "<<tmpId[0]<<" "<<tmpId[1]<<" "<<tmpId[2]<<std::endl;
        std::cout<<"n Hits "<<voxHit.second.size()<<" meanTheta "<<mtheta<<" sigmaTheta "<<sgmtheta<<std::endl;
        }

        htomoZX->SetBinContent(tmpId[2],tmpId[0],voxHit.second.size());
        htomoZY->SetBinContent(tmpId[2],tmpId[1],voxHit.second.size());
    }


    TCanvas *tomoCv= new TCanvas();
    tomoCv->Divide(1,2);
    tomoCv->cd(1);
    htomoZX->Draw("colz");
    tomoCv->cd(2);
    htomoZY->Draw("colz");
    
}
