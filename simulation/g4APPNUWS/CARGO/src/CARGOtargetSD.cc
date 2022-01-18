//
// Original author G. Tassielli
//

// GEOMSRVC includes
#include "manageException.hh"
#include "GeomService.hh"

#include "G4ThreeVector.hh"
#include "G4VProcess.hh"

#include <string>

#include "CARGOtargetSD.hh"

namespace crg {

  CARGOtargetSD::CARGOtargetSD(const G4String &name, const G4String& hitsCollectionName, const crd::SimpleConfig& config) :
                  SensitiveDetector(name,hitsCollectionName,config)
                  ,_elementId(0)
  {
    if ( !GeomService::Instance()->hasElement<CARGOdescription>() ) {
      exc::exceptionG4 e("GEOM","Fatal Error",0);
      e<< "Expected CARGO description but found neither.\n";
      e.error();
    }
  }

  CARGOtargetSD::~CARGOtargetSD(){ }

  G4bool CARGOtargetSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ){

          _currentSize += 1;

          if( _sizeLimit>0 && _currentSize>_sizeLimit ) {
                  if( (_currentSize - _sizeLimit)==1 ) {
//                          mf::LogWarning("G4") << "Maximum number of particles reached "
//                                          << SensitiveDetectorName
//                                          << ": "
//                                          << _currentSize << endl;
                          exc::exceptionG4 e("G4","Warning",4);
                          e<< "CARGO: Maximum number of particles reached "
                              << SensitiveDetectorName
                              << ": "
                              << _currentSize << std::endl;
                          e.error();
                  }
                  return false;
          }

          G4double edep  = aStep->GetTotalEnergyDeposit();
          G4double nidep = aStep->GetNonIonizingEnergyDeposit();
          G4double step  = aStep->GetStepLength();
          //G4double idep  = edep-nidep;

          std::string volName = aStep->GetTrack()->GetVolume()->GetName();
          if ( verboseLevel>=2 )  std::cout<<"edep "<<edep<<" nidep "<<nidep<<" step "<<step<<std::endl;
          // I am not sure why we get these cases but we do.  Skip them.
          if ( (edep == 0. /*|| idep == 0.*/)/*&& step == 0.*/ ) {
                  if ( verboseLevel>=2 )  std::cout<<"Skipped in vol "<<volName<<std::endl;
                  return false;
          }
          _elementId = atoi(volName.substr(12,3).c_str());

//          string volName = aStep->GetTrack()->GetVolume()->GetName();
          int volCpNum = 0;
//          _roSubShell = 0;
//          pxstbs::Layer* tmpLayer = crgdesc->getLayer(_elementId);
//          if (tmpLayer->getLadder(0)->getDetail()->nShells()>1) {
////            volCpNum = aTouch->GetCopyNumber(1);
//            G4TouchableHandle touch1 = aStep->GetPreStepPoint()->GetTouchableHandle();
//            volCpNum = touch1->GetCopyNumber(1);
//            _roSubShell = atoi(volName.substr(24/*,1*/).c_str());
//          } else {
            volCpNum = aStep->GetTrack()->GetVolume()->GetCopyNo();
//          }
          if ( verboseLevel>=2 )  std::cout<<"Step vol name "<<volName<<" Copy num "<<volCpNum<<std::endl;

          // Position at start of step point, in world system and in
          // a system in which the center of the tracking detector is the origin.
          G4ThreeVector prePosWorld = aStep->GetPreStepPoint()->GetPosition();
          if ( verboseLevel>=2 )  std::cout<<"CARGO: G4 hit pos in World "<<prePosWorld[0]<<" "<<prePosWorld[1]<<" "<<prePosWorld[2]<<std::endl;
          G4ThreeVector prePosTracker = prePosWorld - _crgDetCenter;
          G4ThreeVector postPosTracker = aStep->GetPostStepPoint()->GetPosition() - _crgDetCenter;

          G4ThreeVector preMomWorld = aStep->GetPreStepPoint()->GetMomentum();
          HepGeom::Point3D<double> prePosGbl(prePosTracker);
//          HepGeom::Point3D<double> prePosLoc;
//          CARGOROGeometryHandle *roghndl = crgdesc->getROGeometryHandle();
//          roghndl->SelectLadder(_elementId,_phiSec,_ladder);
//          roghndl->Global2Local(prePosGbl,prePosLoc);

//          if (verboseLevel>=2) {
//            cout<<"Selected Ladder: "<<roghndl->GetLadder()->Id()<<"Sub shell "<<_roSubShell<<endl;
////            //cout<<*roghndl->GetLadder()<<endl;
//            cout<<"Ladder Mid Point "<<roghndl->GetLadder()->getMidPoint()<<endl;
//            cout<<"Hit Global "<<prePosGbl<<" Local "<<prePosLoc<<endl;
////
////            cout<<"Ladder width: "<<roghndl->GetLadder()->getDetail()->width()<<" width2: "<<roghndl->GetLadder()->getDetail()->scndWidth();
////            cout<<" halfLength "<<roghndl->GetLadder()->getDetail()->halfLength()<<endl;
//          }

//          cout<<" referred to top right corner "<<prePosLoc<<endl;

          try {

            fHitsCollection->insert( new SimG4TrackerHit(aStep->GetTrack()->GetTrackID(),
                                                _elementId,
                                                /*idep,*/edep,
                                                aStep->GetNonIonizingEnergyDeposit(),
                                                aStep->GetPreStepPoint()->GetGlobalTime(),
                                                aStep->GetPreStepPoint()->GetProperTime(),
                                                prePosTracker,
                                                postPosTracker,
                                                preMomWorld,
                                                step,
                                                aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()
                                                ));

            if (verboseLevel>=2) {
              std::cout<<"--------- Hit in CARGO target element ---------"<<std::endl;
              std::cout<<"Element ID "<<_elementId<<std::endl;
              std::cout<<"HitPos "<<prePosGbl<<std::endl;
              std::cout<<"Edep "<<edep<<std::endl;
            }

            return true;
    }catch (std::exception &e) {
            std::cerr<<e.what();
            return false;
    }

  }

} //namespace crg
