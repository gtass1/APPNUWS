//
// Defines a generic CSMVtracker sensitive detector
//
// Original author G. Tassielli
//

#ifndef CSMVLadderSD_hh
#define CSMVLadderSD_hh

// CSMV includes
#include "GeomHandle.hh"
#include "SensitiveDetector.hh"
#include "CSMVtracker.hh"

namespace csmv {

  class CSMVLadderSD : public SensitiveDetector {

  public:
    CSMVLadderSD(const G4String &name, const G4String& hitsCollectionName, const crd::SimpleConfig& config);
    virtual ~CSMVLadderSD();

    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    void setCSMVCenterInDetSys(const G4ThreeVector &origin) {
            _csmvDetCenter = origin + _systOrigin;
    }

  protected:

    int _layer;
    int _phiSec;
    int _ladder;
    int _roSubShell;
    unsigned long _roIdFstSide;
    unsigned long _roIdSndSide;
//    double _Dphi;

    GeomHandle<CSMVtracker> csmvtracker;
    //CSMVtracker::GeomType _ittype;

    // CSMV point of origin in ref. System
    G4ThreeVector _csmvDetCenter;

  };

} // namespace csmv

#endif /* CSMVLadderSD_hh */
