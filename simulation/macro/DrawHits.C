#include <algorithm>
#include <vector>
#include "TCanvas.h"
#include "TH2F.h"

//gSystem->Load("$PRJBASE/simulation/build/lib/libSimG4ClassesDict");

  TCanvas *cvView = 0x0;
  TCanvas *cvViewAll = 0x0;
  TH2F *tmpHxy = 0x0;
  TH2F *tmpHzy = 0x0;
  TGraph *xyView    = 0x0;
  TGraph *zyView    = 0x0;
  TGraph *xyViewSec = 0x0;
  TGraph *zyViewSec = 0x0;
  
  void DoPlot( std::vector<float> hit[], std::vector<float> hitSec[], int ev=-1 );

bool first=true;

void DrawHits(TString fileName="hits00001.root", Int_t ev=-1, bool debug=false){

  static const int nLayer = 3;

//  // initialize ROOT
//  TSystem ts;
////  gSystem->Load("$PRJBASE/simulation/build/lib/libSimG4ClassesDict");
//  ts->Load("$PRJBASE/simulation/build/lib/libSimG4ClassesDict");

  if (first) {
  gStyle->SetOptStat(0);
  cvView = new TCanvas();
  cvViewAll = new TCanvas();
  cvView->Divide(1,2);
  cvViewAll->Divide(1,2);
  tmpHxy = new TH2F("tmpHxy","XY hits view;X [mm]; Y [mm]",100,-2000,2000,100,-2000,7000);
  tmpHzy = new TH2F("tmpHzy","ZY hits view;Z [mm]; Y [mm]",100,-1000,1000,100,-2000,7000);
  first = false;
  }
  
  TFile *fIn= TFile::Open(fileName.Data());

  TString br1("MCStep");
  TString br2("MCTracks");

  TTree *mcstp;
  TTree *mctrk;

  std::vector<SimG4TrackerHit*> *hitscsmtt = new std::vector<SimG4TrackerHit*>();
  std::vector<SimG4TrackerHit*> *hitscsmbt = new std::vector<SimG4TrackerHit*>();
  std::vector<SimG4Particle*> *tracks = new std::vector<SimG4Particle*>();

  bool hitCSMTTIsPresent=false;
  bool hitCSMBTIsPresent=false;

  fIn->GetListOfKeys()->Print();

  TIter next(fIn->GetListOfKeys());
  TKey *key = NULL;

  //double tot_en;
  while ((key=(TKey*)next()))
  {

    if (br1.CompareTo(key->GetName()) == 0) {

      fIn->GetObject(key->GetName(), mcstp);

      if (mcstp->FindBranch("CSMTTHitsStepCh")!=0x0) {
        hitCSMTTIsPresent=true;
        mcstp->SetBranchAddress("CSMTTHitsStepCh",&hitscsmtt);
        cout<<"Found CSMTT hits"<<endl;
      }
      if (mcstp->FindBranch("CSMBTHitsStepCh")!=0x0) {
        hitCSMBTIsPresent=true;
        mcstp->SetBranchAddress("CSMBTHitsStepCh",&hitscsmbt);
        cout<<"Found CSMBT hits"<<endl;
      }

      cout << "Collection: " << mcstp->GetName() << endl;
      cout << "Number of events: " << mcstp->GetEntries() << endl;


    }

    if (br2.CompareTo(key->GetName()) == 0) {

      fIn->GetObject(key->GetName(), mctrk);

      mctrk->SetBranchAddress("Tracks",&tracks);
      cout << "Collection: " << mctrk->GetName() << endl;
      cout << "Number of events: " << mctrk->GetEntries() << endl;


    }
  }

  std::vector<float> hit[3];
  std::vector<float> hitSec[3];
//  std::vector<float> hitLy[nLayer][3];
  std::vector<float> csmtthit[nLayer][3];
  std::vector<float> csmbthit[nLayer][3];

  Int_t fstEv=0;
  Int_t lstEv=mctrk->GetEntries();
  if (ev>=0) {
      fstEv=ev;
      lstEv=ev+1;
  }
  for (int i=fstEv;i<lstEv;i++) {

     mctrk->GetEntry(i);

     mcstp->GetEntry(i);

     int ntracks = tracks->size();
     std::vector<int> primaryTrkId;

     //     if (i%50==0) cout <<"Ievent = "<<i<<"  nr. of tracks "<<ntracks<<endl;
     for (int k=0;k<ntracks;k++) {

//       G4ThreeVector start = tracks->at(k)->GetPosStart();
//       TVector3 vec_a(start.x(),start.y(),start.z());
//       G4ThreeVector end = tracks->at(k)->GetPosEnd();
//       TVector3 vec_b(end.x(),end.y(),end.z());
//       G4ThreeVector mom = tracks->at(k)->GetMomentum();
//       TVector3 vec_c(mom.x(),mom.y(),mom.z());

       if (tracks->at(k)->GetParentID()==0) {
         primaryTrkId.push_back(tracks->at(k)->GetTrackID());
         if (debug) { cout<<"Primary track ID: "<<primaryTrkId.back()<<endl; }
       }

//       new(ptrBr_trk[k]) GMCGeantTrack(tracks->at(k)->GetTrackID(),
//               tracks->at(k)->GetParentID(),
//               tracks->at(k)->GetParticleName(),
//               tracks->at(k)->GetParticleCharge(),
//               tracks->at(k)->GetPDGCode());
//
//       ((GMCGeantTrack*)ptrBr_trk[k])->SetfPosStart(vec_a);
//       ((GMCGeantTrack*)ptrBr_trk[k])->SetfPosEnd(vec_b);
//       ((GMCGeantTrack*)ptrBr_trk[k])->SetfMomentum(vec_c);
     }
	 
	 bool allPresent=false;

     if (/*false && */hitCSMTTIsPresent) {
       int nhits = hitscsmtt->size();
	   if (nhits>0) { allPresent=true; }

       for (int k=0;k<nhits;k++) {

         SimG4TrackerHit* spos = hitscsmtt->at(k);
//         G4ThreeVector s_pos = hitscsmtt->at(k)->GetPos();
//         TVector3 vec_p(s_pos.x(),s_pos.y(),s_pos.z());
//         G4ThreeVector q_pos = hitscsmtt->at(k)->GetPosEnding();
//         TVector3 vec_q(q_pos.x(),q_pos.y(),q_pos.z());
//         G4ThreeVector mom = hitscsmtt->at(k)->GetMomentum();
//         TVector3 vec_s(mom.x(),mom.y(),mom.z());

//         new(ptrBr_csmtt[k]) GMCPSHWGeantStep(
//             hitscsmtt->at(k)->GetTrackID(),
//             hitscsmtt->at(k)->GetChamberNb(),
//             hitscsmtt->at(k)->GetChannelNb(),
//             hitscsmtt->at(k)->GetEdep(),
//             hitscsmtt->at(k)->GetNoIEdep(),
//             hitscsmtt->at(k)->GetGlobalTime(),
//             hitscsmtt->at(k)->GetProperTime(),
//             hitscsmtt->at(k)->GetStepLength(),
//             hitscsmtt->at(k)->GetProcessCode()
//             );

         if ( std::find ( primaryTrkId.begin(), primaryTrkId.end(), hitscsmtt->at(k)->GetTrackID() ) != primaryTrkId.end() ) {
           int ilayer = hitscsmtt->at(k)->GetChamberNb()/10000-1;
           csmtthit[ilayer][0].push_back(spos->GetPos().x());
           csmtthit[ilayer][1].push_back(spos->GetPos().y());
           csmtthit[ilayer][2].push_back(spos->GetPos().z());
//           hitLy[ilayer][0].push_back(spos->GetPos().x());
//           hitLy[ilayer][1].push_back(spos->GetPos().y());
//           hitLy[ilayer][2].push_back(spos->GetPos().z());
           hit[0].push_back(spos->GetPos().x());
           hit[1].push_back(spos->GetPos().y());
           hit[2].push_back(spos->GetPos().z());
//           if (debug) { cout<<"CSMTT hit: "<<hit[0].back()<<" "<<hit[1].back()<<" "<<hit[2].back()<<endl; }
           if (debug) { cout<<"CSMTT hit: "<<spos->GetPos().x()<<" "<<spos->GetPos().y()<<" "<<spos->GetPos().z()<<endl; }
         } else {
			 
           hitSec[0].push_back(spos->GetPos().x());
           hitSec[1].push_back(spos->GetPos().y());
           hitSec[2].push_back(spos->GetPos().z());
		 }

       }

     }

     if (hitCSMBTIsPresent) {
       int nhits = hitscsmbt->size();
	   allPresent &= (nhits>0);

       for (int k=0;k<nhits;k++) {

         SimG4TrackerHit* spos = hitscsmbt->at(k);
//         G4ThreeVector s_pos = hitscsmbt->at(k)->GetPos();
//         TVector3 vec_p(s_pos.x(),s_pos.y(),s_pos.z());
//         G4ThreeVector q_pos = hitscsmbt->at(k)->GetPosEnding();
//         TVector3 vec_q(q_pos.x(),q_pos.y(),q_pos.z());
//         G4ThreeVector mom = hitscsmbt->at(k)->GetMomentum();
//         TVector3 vec_s(mom.x(),mom.y(),mom.z());

//         new(ptrBr_csmbt[k]) GMCPHCVGeantStep
//             (hitscsmbt->at(k)->GetTrackID(),
//             hitscsmbt->at(k)->GetChamberNb(),
//             hitscsmbt->at(k)->GetChannelNb(),
//             hitscsmbt->at(k)->GetEdep(),
//             hitscsmbt->at(k)->GetNoIEdep(),
//             hitscsmbt->at(k)->GetGlobalTime(),
//             hitscsmbt->at(k)->GetProperTime(),
//             hitscsmbt->at(k)->GetStepLength(),
//             hitscsmbt->at(k)->GetProcessCode()
//             );

         if ( std::find ( primaryTrkId.begin(), primaryTrkId.end(), hitscsmbt->at(k)->GetTrackID() ) != primaryTrkId.end() ) {
           int ilayer = hitscsmbt->at(k)->GetChamberNb()/10000-1;
           csmbthit[ilayer][0].push_back(spos->GetPos().x());
           csmbthit[ilayer][1].push_back(spos->GetPos().y());
           csmbthit[ilayer][2].push_back(spos->GetPos().z());
//           hitLy[ilayer][0].push_back(spos->GetPos().x());
//           hitLy[ilayer][1].push_back(spos->GetPos().y());
//           hitLy[ilayer][2].push_back(spos->GetPos().z());
           hit[0].push_back(spos->GetPos().x());
           hit[1].push_back(spos->GetPos().y());
           hit[2].push_back(spos->GetPos().z());
//           if (debug) { cout<<"CSMBT hit: "<<hit[0].back()<<" "<<hit[1].back()<<" "<<hit[2].back()<<endl; }
           if (debug) { cout<<"CSMTT hit: "<<spos->GetPos().x()<<" "<<spos->GetPos().y()<<" "<<spos->GetPos().z()<<endl; }
         } else {
			 
           hitSec[0].push_back(spos->GetPos().x());
           hitSec[1].push_back(spos->GetPos().y());
           hitSec[2].push_back(spos->GetPos().z());
		 }


       }

     }

     if (ev!=-1 && hit[0].size()>0 ) {
		 if (ev==-2 || ev==-3) { std::cout<<"Current ev: "<< i <<std::endl; }
		 if (xyView != 0x0) { xyView->Delete(); xyView = 0x0; }
		 if (zyView != 0x0) { zyView->Delete(); zyView = 0x0; }
		 if (xyViewSec != 0x0) { xyViewSec->Delete(); xyViewSec = 0x0; }
		 if (zyViewSec != 0x0) { zyViewSec->Delete(); zyViewSec = 0x0; }
		 if ( ev!=-3 || allPresent ) { DoPlot( hit, hitSec, ev ); }
		 hit[0].clear(); hit[1].clear(); hit[2].clear(); 
		 hitSec[0].clear(); hitSec[1].clear(); hitSec[2].clear(); 
     }

  }
  
  if (ev==-1) { DoPlot( hit, hitSec, ev ); }

}


void DoPlot( std::vector<float> hit[], std::vector<float> hitSec[], int ev ) {
//  TCanvas *cvView = new TCanvas();
//  cvView->Divide(1,2);

  /*TGraph */xyView = new TGraph(hit[0].size(),hit[0].data(),hit[1].data());
  xyView->SetTitle("XY view;X [mm]; Y [mm]");
  xyView->SetMarkerStyle(8);
//  xyView->SetMarkerSize(5);

  cvView->cd(1);
//  TH2F *tmpHxy = new TH2F("tmpHxy","XY hits view;X [mm]; Y [mm]",100,-2000,2000,100,-2000,7000);
  tmpHxy->Draw();  
  xyView->Draw("Psame");

  /*TGraph */zyView = new TGraph(hit[2].size(),hit[2].data(),hit[1].data());
  zyView->SetTitle("ZY view;Z [mm]; Y [mm]");
  zyView->SetMarkerStyle(8);
//  zyView->SetMarkerSize(5);

  cvView->cd(2);
//  TH2F *tmpHzy = new TH2F("tmpHzy","ZY hits view;Z [mm]; Y [mm]",100,-1000,1000,100,-2000,7000);
  tmpHzy->Draw();  
  zyView->Draw("Psame");
  cvView->Update();
  if (ev<=-2 && hitSec[0].size()==0 && hitSec[2].size()==0) {
	cvView->cd();
	cvView->WaitPrimitive();
  }

if (hitSec[0].size()>0 || hitSec[2].size()>0) {
//  TCanvas *cvViewAll = new TCanvas();
//  cvViewAll->Divide(1,2);

if (hitSec[0].size()>0) {
  /*TGraph */xyViewSec = new TGraph(hitSec[0].size(),hitSec[0].data(),hitSec[1].data());
  xyViewSec->SetTitle("XY view;X [mm]; Y [mm]");
  xyViewSec->SetMarkerStyle(8);
  xyViewSec->SetMarkerColor(kRed);

  cvViewAll->cd(1);
//  TH2F *tmpHxy = new TH2F("tmpHxy","XY hits view;X [mm]; Y [mm]",100,-2000,2000,100,-2000,7000);
  tmpHxy->Draw();  
  xyView->Draw("Psame");
  xyViewSec->Draw("Psame");
}

if (hitSec[2].size()>0) {
  /*TGraph */zyViewSec = new TGraph(hitSec[2].size(),hitSec[2].data(),hitSec[1].data());
  zyViewSec->SetTitle("ZY view;Z [mm]; Y [mm]");
  zyViewSec->SetMarkerStyle(8);
  zyViewSec->SetMarkerColor(kRed);

  cvViewAll->cd(2);
//  TH2F *tmpHzy = new TH2F("tmpHzy","ZY hits view;Z [mm]; Y [mm]",100,-1000,1000,100,-2000,7000);
  tmpHzy->Draw();  
  zyView->Draw("Psame");
  zyViewSec->Draw("Psame");
}
cvViewAll->Update();
  if (ev<=-2) {
    cvViewAll->cd();
	cvViewAll->WaitPrimitive();
  }
}

}