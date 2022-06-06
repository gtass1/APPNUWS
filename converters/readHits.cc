//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: readHits.cc 68025 2013-03-13 13:43:46Z gcosmo $
//
/// \file persistency/P01/readHits.cc
/// \brief Main program of the persistency/P01 example
//
// Include files
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TKey.h"
#include "TTree.h"
#include "TBranch.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "Riostream.h"
//
//
#include "generated/rAPPNUWSDCGeantStep.h"
#include "generated/rAPPNUWSCSMBTGeantStep.h"
#include "generated/rAPPNUWSCSMTTGeantStep.h"
#include "generated/rAPPNUWSGeantTrack.h"
#include "ROMETreeInfo.h"
#include "IOSRVC/SimG4Particle.hh"
#include "IOSRVC/SimG4TrackerHit.hh"

using namespace std;

int main(int argc,char** argv) 
{
  // initialize ROOT
  TSystem ts;
//  gSystem->Load("$PRJBASE/simulation/g4APPNUWS/lib/libSimG4ClassesDict");
  gSystem->Load("$PRJBASE/simulation/build/lib/libSimG4ClassesDict");

  if(argc<2) cout << "Missing name of the file to read!" << endl;
 
  TFile fo(argv[1]);
   
  TString br1("MCStep");
  TString br2("MCTracks");
  
  TTree *a;
  TTree *b;
  std::vector<SimG4TrackerHit*> *hitsch = new std::vector<SimG4TrackerHit*>();
  std::vector<SimG4TrackerHit*> *hitsCSMBT = new std::vector<SimG4TrackerHit*>();
  std::vector<SimG4TrackerHit*> *hitsCSMTT = new std::vector<SimG4TrackerHit*>();
  std::vector<SimG4Particle*> *tracks = new std::vector<SimG4Particle*>();
  bool hitChIsPresent=false;
  bool hitCSMBTIsPresent=false;
  bool hitCSMTTIsPresent=false;

  fo.GetListOfKeys()->Print();
    
  TIter next(fo.GetListOfKeys());
  TKey *key = NULL;

  //double tot_en;
  while ((key=(TKey*)next()))
  {
    
    if (br1.CompareTo(key->GetName()) == 0) {
     
      fo.GetObject(key->GetName(), a);
      if (a->FindBranch("HitsStepCh")!=0x0) {
        hitChIsPresent=true;
        a->SetBranchAddress("HitsStepCh",&hitsch);
        cout<<"Found DCH hits"<<endl;
      }
      if (a->FindBranch("CSMBTHitsStepCh")!=0x0) {
        hitCSMBTIsPresent=true;
        a->SetBranchAddress("CSMBTHitsStepCh",&hitsCSMBT);
        cout<<"Found CSMBT hits"<<endl;
      }
      if (a->FindBranch("CSMTTHitsStepCh")!=0x0) {
        hitCSMTTIsPresent=true;
        a->SetBranchAddress("CSMTTHitsStepCh",&hitsCSMTT);
        cout<<"Found CSMTT hits"<<endl;
      }
      
  cout << "Collection: " << a->GetName() << endl;
  cout << "Number of events: " << a->GetEntries() << endl;


    } 
    
    if (br2.CompareTo(key->GetName()) == 0) {

      fo.GetObject(key->GetName(), b);
    
      b->SetBranchAddress("Tracks",&tracks);
  cout << "Collection: " << b->GetName() << endl;
  cout << "Number of events: " << b->GetEntries() << endl;
  
    
    }
  }
  
  int fOutNum=1;
  if (argc==3) { fOutNum = TString(argv[2]).Atoi(); }
  else {
    TString fIn(argv[1]);
    fIn.ReplaceAll(".root","");
    fIn.ReplaceAll("hits","");
    if (fIn.IsDec()) { fOutNum = fIn.Atoi(); }
  }

  TFile fOutput(Form("MCData%05d.root",fOutNum),"RECREATE");

  TTree *tr = new TTree("MCData","MCData");

  ROMETreeInfo fInfo;
  tr->Branch("Info",&fInfo);
  fInfo.SetRunNumber(fOutNum);

  TClonesArray *myBr_trk = new TClonesArray("rAPPNUWSGeantTrack",0);
  TClonesArray &ptrBr_trk = *myBr_trk;
  tr->Branch("MCTracks",&myBr_trk);

  TClonesArray *myBr_dch = new TClonesArray("rAPPNUWSDCGeantStep",0);
  TClonesArray &ptrBr_dch = *myBr_dch;
  if (hitChIsPresent) { tr->Branch("MCDCStep",&myBr_dch); }

  TClonesArray *myBr_CSMBT = new TClonesArray("rAPPNUWSCSMBTGeantStep",0);
  TClonesArray &ptrBr_CSMBT = *myBr_CSMBT;
  if (hitCSMBTIsPresent) { tr->Branch("MCCSMBTStep",&myBr_CSMBT); }

  TClonesArray *myBr_CSMTT = new TClonesArray("rAPPNUWSCSMTTGeantStep",0);
  TClonesArray &ptrBr_CSMTT = *myBr_CSMTT;
  if (hitCSMTTIsPresent) { tr->Branch("MCCSMTTStep",&myBr_CSMTT); }

  for (int i=0;i<b->GetEntries();i++) {

	   b->GetEntry(i);

	   a->GetEntry(i);

           fInfo.SetEventNumber(i);
     int ntracks = tracks->size();
     ptrBr_trk.Clear();
     if (i%50==0) cout <<"Ievent = "<<i<<"  nr. of tracks "<<ntracks<<endl;
     for (int k=0;k<ntracks;k++) { 
       
       G4ThreeVector start = tracks->at(k)->GetPosStart();
       TVector3 vec_a(start.x(),start.y(),start.z());
       G4ThreeVector end = tracks->at(k)->GetPosEnd();
       TVector3 vec_b(end.x(),end.y(),end.z());
       G4ThreeVector mom = tracks->at(k)->GetMomentum();
       TVector3 vec_c(mom.x(),mom.y(),mom.z());
       
       new(ptrBr_trk[k]) rAPPNUWSGeantTrack(tracks->at(k)->GetTrackID(),
				       tracks->at(k)->GetParentID(),
				       tracks->at(k)->GetParticleName(),
				       tracks->at(k)->GetParticleCharge(),
				       tracks->at(k)->GetPDGCode());

       ((rAPPNUWSGeantTrack*)ptrBr_trk[k])->SetfPosStart(vec_a);
       ((rAPPNUWSGeantTrack*)ptrBr_trk[k])->SetfPosEnd(vec_b);
       ((rAPPNUWSGeantTrack*)ptrBr_trk[k])->SetfMomentum(vec_c);
     }

     if (hitChIsPresent) {
       int nhits = hitsch->size();
       ptrBr_dch.Clear();
       for (int k=0;k<nhits;k++) {

         G4ThreeVector s_pos = hitsch->at(k)->GetPos();
         TVector3 vec_p(s_pos.x(),s_pos.y(),s_pos.z());
         G4ThreeVector q_pos = hitsch->at(k)->GetPosEnding();
         TVector3 vec_q(q_pos.x(),q_pos.y(),q_pos.z());
         G4ThreeVector mom = hitsch->at(k)->GetMomentum();
         TVector3 vec_s(mom.x(),mom.y(),mom.z());

         new(ptrBr_dch[k]) rAPPNUWSDCGeantStep(hitsch->at(k)->GetTrackID(),
             hitsch->at(k)->GetChamberNb(),
             hitsch->at(k)->GetEdep(),
             hitsch->at(k)->GetNoIEdep(),
             hitsch->at(k)->GetGlobalTime(),
             hitsch->at(k)->GetProperTime(),
             hitsch->at(k)->GetStepLength(),
             hitsch->at(k)->GetProcessCode() );
         ((rAPPNUWSDCGeantStep*)ptrBr_dch[k])->SetfPos(vec_p);
         ((rAPPNUWSDCGeantStep*)ptrBr_dch[k])->SetfPosEnding(vec_q);
         ((rAPPNUWSDCGeantStep*)ptrBr_dch[k])->SetfMomentum(vec_s);

       }
     }

     if (hitCSMBTIsPresent) {
       int nhits = hitsCSMBT->size();
       ptrBr_CSMBT.Clear();
       for (int k=0;k<nhits;k++) {

         G4ThreeVector s_pos = hitsCSMBT->at(k)->GetPos();
         TVector3 vec_p(s_pos.x(),s_pos.y(),s_pos.z());
         G4ThreeVector q_pos = hitsCSMBT->at(k)->GetPosEnding();
         TVector3 vec_q(q_pos.x(),q_pos.y(),q_pos.z());
         G4ThreeVector mom = hitsCSMBT->at(k)->GetMomentum();
         TVector3 vec_s(mom.x(),mom.y(),mom.z());

         new(ptrBr_CSMBT[k]) rAPPNUWSCSMBTGeantStep(hitsCSMBT->at(k)->GetTrackID(),
             hitsCSMBT->at(k)->GetChamberNb(),
             hitsCSMBT->at(k)->GetChannelNb(),
             hitsCSMBT->at(k)->GetEdep(),
             hitsCSMBT->at(k)->GetNoIEdep(),
             hitsCSMBT->at(k)->GetGlobalTime(),
             hitsCSMBT->at(k)->GetProperTime(),
             hitsCSMBT->at(k)->GetStepLength(),
             hitsCSMBT->at(k)->GetProcessCode() );

         
         ((rAPPNUWSCSMBTGeantStep*)ptrBr_CSMBT[k])->SetfPos(vec_p);
         ((rAPPNUWSCSMBTGeantStep*)ptrBr_CSMBT[k])->SetfPosEnding(vec_q);
         ((rAPPNUWSCSMBTGeantStep*)ptrBr_CSMBT[k])->SetfMomentum(vec_s);
         
       }
     }

     if (hitCSMTTIsPresent) {
       int nhits = hitsCSMTT->size();
       ptrBr_CSMTT.Clear();
       for (int k=0;k<nhits;k++) {

         G4ThreeVector s_pos = hitsCSMTT->at(k)->GetPos();
         TVector3 vec_p(s_pos.x(),s_pos.y(),s_pos.z());
         G4ThreeVector q_pos = hitsCSMTT->at(k)->GetPosEnding();
         TVector3 vec_q(q_pos.x(),q_pos.y(),q_pos.z());
         G4ThreeVector mom = hitsCSMTT->at(k)->GetMomentum();
         TVector3 vec_s(mom.x(),mom.y(),mom.z());

         new(ptrBr_CSMTT[k]) rAPPNUWSCSMTTGeantStep(hitsCSMTT->at(k)->GetTrackID(),
             hitsCSMTT->at(k)->GetChamberNb(),
             hitsCSMTT->at(k)->GetChannelNb(),
             hitsCSMTT->at(k)->GetEdep(),
             hitsCSMTT->at(k)->GetNoIEdep(),
             hitsCSMTT->at(k)->GetGlobalTime(),
             hitsCSMTT->at(k)->GetProperTime(),
             hitsCSMTT->at(k)->GetStepLength(),
             hitsCSMTT->at(k)->GetProcessCode() );
         ((rAPPNUWSCSMTTGeantStep*)ptrBr_CSMTT[k])->SetfPos(vec_p);
         ((rAPPNUWSCSMTTGeantStep*)ptrBr_CSMTT[k])->SetfPosEnding(vec_q);
         ((rAPPNUWSCSMTTGeantStep*)ptrBr_CSMTT[k])->SetfMomentum(vec_s);

       }
     }

     tr->Fill();
  }
  
  tr->Write();
  fOutput.Close();
  
}
