////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTReadMCDataCSMTT                                                   //
//                                                                            //
// Begin_Html <!--
/*-->

<!--*/
// --> End_Html
//                                                                            //
//                                                                            //
// Please note: The following information is only correct after executing     //
// the ROMEBuilder.                                                           //
//                                                                            //
// This task accesses the following folders :                                 //
//     GeantTrack                                                             //
//     CSMTTGeantStep                                                         //
//     CSMTTHit                                                               //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/rAPPNUWSTReadMCDataCSMTTGeneratedIncludes.h"

////////////////////////////////////////////////////////////////////////////////
/*  This header was generated by ROMEBuilder. Manual changes above the        *
 * following line will be lost next time ROMEBuilder is executed.             */
/////////////////////////////////////----///////////////////////////////////////

#include "generated/rAPPNUWSAnalyzer.h"
#include "ROMEiostream.h"

// uncomment if you want to include headers of all folders
//#include "rAPPNUWSAllFolders.h"

#include "generated/rAPPNUWSConfigToForm.h"
#include "generated/rAPPNUWSMCRunHeader.h"
#include "generated/rAPPNUWSGeantTrack.h"
#include "generated/rAPPNUWSCSMTTGeantStep.h"
#include "generated/rAPPNUWSCSMTTHit.h"

#include "generated/rAPPNUWSEventLoop.h"

#include "tasks/rAPPNUWSTReadMCDataCSMTT.h"

#include "CSMTTROGeometryHandle.hh"

ClassImp(rAPPNUWSTReadMCDataCSMTT)

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::Init()
{

  //instance the geometry
  fGeometry = GeometrySrvHndl::Instance();
  fGeometry->makeDetectors();

  _minimumEnergy = GetSP()->GetHitEnergyCut();
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::BeginOfRun()
{
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::Event()
{

  if (gAnalyzer->GetCurrentEventNumber()%100==0 ) { printf("\n ***** ReadMCDataCSMTT Load event %lld ************************ \n",gAnalyzer->GetCurrentEventNumber()); }
  LoadEvent(gAnalyzer->GetCurrentEventNumber());

  gAnalyzer->SetCSMTTHitSize(0);
  DefineHitsInCSMTT();

}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::LoadEvent(Int_t nev) {

  fBrDataCSMTT=gAnalyzer->GetCSMTTGeantSteps();
  fBrDataTrk=gAnalyzer->GetGeantTracks();
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::DefineHitsInCSMTT() {

  Int_t NrHits = 0;

  CSMTTChHitMap hitmap;
  hitmap.clear();
  fillHitMap( hitmap );

  for(CSMTTChHitMap::const_iterator idchn = hitmap.begin(); idchn != hitmap.end(); ++idchn) {
    unsigned long dchn_id = idchn->first;
//    fGeometry->GetCellHandle()->SelectCellDet(dchn_id);
//    CLHEP::Hep3Vector const& mid   = fGeometry->GetCellHandle()->GetCellCenter();
//    CLHEP::Hep3Vector const& w     = fGeometry->GetCellHandle()->GetCellDirection();

    std::vector<rAPPNUWSCSMTTGeantStep *> const& ihits = idchn->second;

    std::map<int,rAPPNUWSCSMTTHit *> hitPerTrk;
    std::map<int,rAPPNUWSCSMTTHit *>::iterator hitPerTrk_it;

//    std::map<int,TVector3 *> hitEndPerTrk;
//    std::map<int,TVector3 *>::iterator hitEndPerTrk_it;

    for( size_t i=0; i<ihits.size(); i++ ) {

      rAPPNUWSCSMTTGeantStep& hit = *(ihits.at(i));

      hitPerTrk_it = hitPerTrk.find(hit.GetfTrackID());
//      hitEndPerTrk_it = hitEndPerTrk.find(hit.GetfTrackID());

      if ( hitPerTrk_it == hitPerTrk.end() ) {
        gAnalyzer->SetCSMTTHitSize(NrHits+1);
        rAPPNUWSCSMTTHit *ahit = gAnalyzer->GetCSMTTHitAt(NrHits);
        ++NrHits;
        hitPerTrk.insert( std::pair<int,rAPPNUWSCSMTTHit *>( hit.GetfTrackID(), ahit ) );
//        hitEndPerTrk.insert( std::pair<int,TVector3 *>( hit.GetfTrackID(), hit.GetfPosEnding() ) );

        ahit->SetfChanId( dchn_id );
        ahit->SetfTrkIDofHit( hit.GetfTrackID() );
        ahit->SetfEntranceX( hit.GetfPos()->X() );
        ahit->SetfEntranceY( hit.GetfPos()->Y() );
        ahit->SetfEntranceZ( hit.GetfPos()->Z() );
        ahit->SetfEntranceMomX( hit.GetfMomentum()->X() );
        ahit->SetfEntranceMomY( hit.GetfMomentum()->Y() );
        ahit->SetfEntranceMomZ( hit.GetfMomentum()->Z() );
        ahit->SetfTotalEnergyLoss( hit.GetfEdep() );
        ahit->SetfGlobalTime( hit.GetfGlobalTime() );
        ahit->SetfToF( hit.GetfProperTime() );

      } else {
        rAPPNUWSCSMTTHit *ahit = hitPerTrk_it->second;
        ahit->SetfTotalEnergyLoss( ahit->GetfTotalEnergyLoss() + hit.GetfEdep() );
//        ahit->SetfLength( ahit->GetfLength() + hit.GetfStepLen() );
//        hitEndPerTrk_it->second=hit.GetfPos();
      }

    }

    //FIXME!!! Trackes Pile-up not taken into account!!!

//    for ( hitPerTrk_it=hitPerTrk.begin(); hitPerTrk_it!=hitPerTrk.end(); ++hitPerTrk_it ) {
//      rAPPNUWSCSMTTHit *ahit = hitPerTrk_it->second;
//    }

  }

  if (gAnalyzer->GetCurrentEventNumber()%100==0 ) { std::cout<<"CSMTT nhits "<<gAnalyzer->GetCSMTTHitSize()<<std::endl; }

}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::fillHitMap ( CSMTTChHitMap& hitmap ) {
  int nentries=fBrDataCSMTT->GetEntries();
  for (int istep=0;istep<nentries;istep++) {
    rAPPNUWSCSMTTGeantStep *aStep = (rAPPNUWSCSMTTGeantStep *)fBrDataCSMTT->At(istep);
    if( (aStep->GetfEdep()-aStep->GetfNoIEdep())<_minimumEnergy ) continue; // Skip steps with very low energy deposition
    unsigned long unqChId = fGeometry->GetCSMTTROChanHandle()->computeDet(aStep->GetfChamberNr(),aStep->GetfChannelNr());
    hitmap[unqChId].push_back(aStep);
  }
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::EndOfRun()
{
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMTT::Terminate()
{
}

