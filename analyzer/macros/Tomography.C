//
// Run it inside rappnuwsanalyzer: $RAPPNSDIR/rappnuwsanalyzer.exe -I
// Otherwise, to run in ROOT, you need add the class declaration and to load the dictionaries
//
//#include "generated/rAPPNUWSPreTomography.h"

#include <utility>

#include "util/TomoUtils.h"
#include "TomoFCut.C"

void Tomography(TString runList="0;1", TString foldBase=".", bool useCut=true/*, int fcBin=1*/) {

    //    gROOT->LoadMacro("$RAPPNSDIR/../macros/TomoFCut.C");

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

    TClonesArray *pretomoinfo = new TClonesArray("rAPPNUWSPreTomographyInfo");
    tr->SetBranchAddress("PreTomographyInfo",&pretomoinfo);

    TClonesArray *pretomo = new TClonesArray("rAPPNUWSPreTomography");
    tr->SetBranchAddress("PreTomography",&pretomo);

    std::unordered_map< unsigned long, std::vector< std::pair<double,double> > > voxHitMap;

    Int_t nEvents = tr->GetEntries();
    cout<<"Number of events Stored --> "<<nEvents<<endl;

    TF1 *ftmcut = new TF1("ftmcut",TomoFCut,0,500,4);
    ftmcut->SetParameters(0.0061,0.000465,150,20);

    double maxAngle=0.0;
    double maxDist=0.0;
    for (int ie=0; ie</*10*/nEvents; ++ie) {

      tr->GetEntry(ie);  //load ie event

      for (int iptd=0; iptd<pretomo->GetEntries(); ++iptd) {

          rAPPNUWSPreTomography *iVxHit = (rAPPNUWSPreTomography *)pretomo->At(iptd);
          std::vector<Double_t> &iVxAllAngle = iVxHit->GetallAngle();
          std::vector<Double_t> &iVxAllDist = iVxHit->GetallDist();
//          for ( auto &iAngle : iVxAllAngle ) {
          for ( int ih=0; ih< iVxHit->GetallAngleSize(); ++ih ) {
              double iAngle=iVxAllAngle.at(ih);
              double iDist=iVxAllDist.at(ih);
              if (useCut) {
                  if( iAngle<ftmcut->Eval(iDist) ) {
                      voxHitMap[iVxHit->GethitId()].push_back(make_pair(iAngle,iDist));
                      if (iAngle>maxAngle) {maxAngle=iAngle;}
                      if (iDist>maxDist) {maxDist=iDist;}
                  }
              } else {
                  voxHitMap[iVxHit->GethitId()].push_back(make_pair(iAngle,iDist));
                  if (iAngle>maxAngle) {maxAngle=iAngle;}
                  if (iDist>maxDist) {maxDist=iDist;}
              }
          }

//          int tmpId[3] = {-1,-1,-1};
//          UIDtoID(iVxHit->GethitId(), tmpId);

//          std::cout<<"preTomoHit Id "<<iVxHit->GethitId()<<" back to ID "<<tmpId[0]<<" "<<tmpId[1]<<" "<<tmpId[2]<<std::endl;

      }
    }

    std::cout<<"Vox Hit Map size "<<voxHitMap.size()<<std::endl;

    rAPPNUWSPreTomographyInfo *tminfo = (rAPPNUWSPreTomographyInfo *)pretomoinfo->At(0);

//    TH2F *htomoZX = new TH2F("htomoZX","Tomoraghy progection on orizontal plane",5000/fcBin,0,5000/fcBin,1000/fcBin,0,1000/fcBin);
//    TH2F *htomoZY = new TH2F("htomoZY","Tomoraghy progection on vertical plane",5000/fcBin,0,5000/fcBin,1000/fcBin,0,1000/fcBin);
//
//    TH2F *htomoZX_th = new TH2F("htomoZX_th","Tomoraghy progection on orizontal plane",5000/fcBin,0,5000/fcBin,1000/fcBin,0,1000/fcBin);
//    TH2F *htomoZY_th = new TH2F("htomoZY_th","Tomoraghy progection on vertical plane",5000/fcBin,0,5000/fcBin,1000/fcBin,0,1000/fcBin);
    TH2F *htomoZX = new TH2F("htomoZX","Tomoraghy progection on orizontal plane",tminfo->GetnVolexAt(2),tminfo->GetminVolexPosAt(2),tminfo->GetmaxVolexPosAt(2),tminfo->GetnVolexAt(0),tminfo->GetminVolexPosAt(0),tminfo->GetmaxVolexPosAt(0));
    TH2F *htomoZY = new TH2F("htomoZY","Tomoraghy progection on vertical plane",tminfo->GetnVolexAt(2),tminfo->GetminVolexPosAt(2),tminfo->GetmaxVolexPosAt(2),tminfo->GetnVolexAt(1),tminfo->GetminVolexPosAt(1),tminfo->GetmaxVolexPosAt(1));

    TH2F *htomoZX_th = new TH2F("htomoZX_th","Tomoraghy progection on orizontal plane",tminfo->GetnVolexAt(2),tminfo->GetminVolexPosAt(2),tminfo->GetmaxVolexPosAt(2),tminfo->GetnVolexAt(0),tminfo->GetminVolexPosAt(0),tminfo->GetmaxVolexPosAt(0));
    TH2F *htomoZY_th = new TH2F("htomoZY_th","Tomoraghy progection on vertical plane",tminfo->GetnVolexAt(2),tminfo->GetminVolexPosAt(2),tminfo->GetmaxVolexPosAt(2),tminfo->GetnVolexAt(1),tminfo->GetminVolexPosAt(1),tminfo->GetmaxVolexPosAt(1));

    for ( auto& voxHit : voxHitMap)  {
        int tmpId[3] = {-1,-1,-1};
        UIDtoID(voxHit.first, tmpId);
        double iAngle=0;
        double mAngle=0;
        double sgmAngle=0;
        double iDist=0;
        double mDist=0;
        double sgmDist=0;

        for ( auto& iAngDist : voxHit.second ) {
            iAngle = iAngDist.first;
            mAngle+= iAngle;
            sgmAngle+=iAngle*iAngle;

            iDist = iAngDist.second;
            mDist+= iDist;
            sgmDist+=iDist*iDist;
        }

        mAngle/=(double) voxHit.second.size();
        sgmAngle/=(double) voxHit.second.size();
        sgmAngle=sqrt( sgmAngle-mAngle*mAngle );

        mDist/=(double) voxHit.second.size();
        sgmDist/=(double) voxHit.second.size();
        sgmDist=sqrt( sgmDist-mDist*mDist );

        if (voxHit.second.size()>1) {
        std::cout<<"preTomoHit Id "<<voxHit.first<<" back to ID "<<tmpId[0]<<" "<<tmpId[1]<<" "<<tmpId[2]<<std::endl;
        std::cout<<"n Hits "<<voxHit.second.size()<<" meanAngle "<<mAngle<<" sigmaAngle "<<sgmAngle<<std::endl;
        }

        htomoZX->SetBinContent(tmpId[2],tmpId[0],voxHit.second.size());
        htomoZY->SetBinContent(tmpId[2],tmpId[1],voxHit.second.size());
        if (voxHit.second.size()>1) {
            htomoZX_th->SetBinContent(tmpId[2],tmpId[0],mAngle/maxAngle);
            htomoZY_th->SetBinContent(tmpId[2],tmpId[1],mAngle/maxAngle);
        }
    }


    TCanvas *tomoCv= new TCanvas();
    tomoCv->Divide(1,2);
    tomoCv->cd(1);
    htomoZX->Draw("colz");
    tomoCv->cd(2);
    htomoZY->Draw("colz");

    TCanvas *tomoThCv= new TCanvas();
    tomoThCv->Divide(1,2);
    tomoThCv->cd(1);
    htomoZX_th->Draw("colz");
    tomoThCv->cd(2);
    htomoZY_th->Draw("colz");
    
}
