////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// rAPPNUWSTReadMCDataCSMBT                                                   //
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
//     CSMBTGeantStep                                                         //
//     CSMBTHit                                                               //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/* Generated header file containing necessary includes                        */
#include "generated/rAPPNUWSTReadMCDataCSMBTGeneratedIncludes.h"

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
#include "generated/rAPPNUWSCSMBTGeantStep.h"
#include "generated/rAPPNUWSCSMBTHit.h"

#include "generated/rAPPNUWSEventLoop.h"

#include "tasks/rAPPNUWSTReadMCDataCSMBT.h"

#include "CSMBTROGeometryHandle.hh"

ClassImp(rAPPNUWSTReadMCDataCSMBT)

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::Init() {

  //instance the geometry
  fGeometry = GeometrySrvHndl::Instance();
//  if(!fGeometry->GetCellHandle())
    fGeometry->makeDetectors();

  _minimumEnergy = GetSP()->GetHitEnergyCut();
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::BeginOfRun()
{
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::Event()
{

  if (gAnalyzer->GetCurrentEventNumber()%100==0 ) { printf("\n ***** ReadMCDataCSMBT Load event %lld ************************ \n",gAnalyzer->GetCurrentEventNumber()); }
  LoadEvent(gAnalyzer->GetCurrentEventNumber());

  gAnalyzer->SetCSMBTHitSize(0);
  DefineHitsInCSMBT();

}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::LoadEvent(Int_t nev) {

  fBrDataCSMBT=gAnalyzer->GetCSMBTGeantSteps();
  fBrDataTrk=gAnalyzer->GetGeantTracks();
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::DefineHitsInCSMBT() {

  Int_t NrHits = 0;

  CSMBTChHitMap hitmap;
  hitmap.clear();
  fillHitMap( hitmap );

  for(CSMBTChHitMap::const_iterator idchn = hitmap.begin(); idchn != hitmap.end(); ++idchn) {
    unsigned long dchn_id = idchn->first;
//    fGeometry->GetCellHandle()->SelectCellDet(dchn_id);
//    CLHEP::Hep3Vector const& mid   = fGeometry->GetCellHandle()->GetCellCenter();
//    CLHEP::Hep3Vector const& w     = fGeometry->GetCellHandle()->GetCellDirection();

    std::vector<rAPPNUWSCSMBTGeantStep *> const& ihits = idchn->second;

    std::map<int,rAPPNUWSCSMBTHit *> hitPerTrk;
    std::map<int,rAPPNUWSCSMBTHit *>::iterator hitPerTrk_it;

//    std::map<int,TVector3 *> hitEndPerTrk;
//    std::map<int,TVector3 *>::iterator hitEndPerTrk_it;

    for( size_t i=0; i<ihits.size(); i++ ) {

      rAPPNUWSCSMBTGeantStep& hit = *(ihits.at(i));

      hitPerTrk_it = hitPerTrk.find(hit.GetfTrackID());
//      hitEndPerTrk_it = hitEndPerTrk.find(hit.GetfTrackID());

      if ( hitPerTrk_it == hitPerTrk.end() ) {
        gAnalyzer->SetCSMBTHitSize(NrHits+1);
        rAPPNUWSCSMBTHit *ahit = gAnalyzer->GetCSMBTHitAt(NrHits);
        ++NrHits;
        hitPerTrk.insert( std::pair<int,rAPPNUWSCSMBTHit *>( hit.GetfTrackID(), ahit ) );
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
        rAPPNUWSCSMBTHit *ahit = hitPerTrk_it->second;
        ahit->SetfTotalEnergyLoss( ahit->GetfTotalEnergyLoss() + hit.GetfEdep() );
//        ahit->SetfLength( ahit->GetfLength() + hit.GetfStepLen() );
//        hitEndPerTrk_it->second=hit.GetfPos();
      }

    }

    //FIXME!!! Trackes Pile-up not taken into account!!!

//    for ( hitPerTrk_it=hitPerTrk.begin(); hitPerTrk_it!=hitPerTrk.end(); ++hitPerTrk_it ) {
//      rAPPNUWSCSMBTHit *ahit = hitPerTrk_it->second;
//    }

  }

  if (gAnalyzer->GetCurrentEventNumber()%100==0 ) { std::cout<<"nhits "<<gAnalyzer->GetCSMBTHitSize()<<std::endl; }

}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::fillHitMap ( CSMBTChHitMap& hitmap ) {
  int nentries=fBrDataCSMBT->GetEntries();
  for (int istep=0;istep<nentries;istep++) {
    rAPPNUWSCSMBTGeantStep *aStep = (rAPPNUWSCSMBTGeantStep *)fBrDataCSMBT->At(istep);
    if( (aStep->GetfEdep()-aStep->GetfNoIEdep())<_minimumEnergy ) continue; // Skip steps with very low energy deposition
    unsigned long unqChId = fGeometry->GetCSMBTROChanHandle()->computeDet(aStep->GetfChamberNr(),aStep->GetfChannelNr());
    hitmap[unqChId].push_back(aStep);
  }
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::EndOfRun()
{
}

//______________________________________________________________________________
void rAPPNUWSTReadMCDataCSMBT::Terminate()
{
}

