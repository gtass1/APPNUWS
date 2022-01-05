//
// Defines a generic CSMBTracker sensitive detector
//
// Original author G. Tassielli
//

#ifndef CSMBTLadderSD_hh
#define CSMBTLadderSD_hh

// CSMBT includes
#include "CSMBtracker.hh"
#include "GeomHandle.hh"
#include "SensitiveDetector.hh"

namespace csmbt {

  class CSMBTLadderSD : public SensitiveDetector {

  public:
    CSMBTLadderSD(const G4String &name, const G4String& hitsCollectionName, const crd::SimpleConfig& config);
    virtual ~CSMBTLadderSD();

    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    void setCSMBTCenterInDetSys(const G4ThreeVector &origin) {
            _csmbtDetCenter = origin + _systOrigin;
    }

  protected:

    int _layer;
    int _phiSec;
    int _ladder;
    int _roSubShell;
    unsigned long _roIdFstSide;
    unsigned long _roIdSndSide;
//    double _Dphi;

    GeomHandle<CSMBtracker> csmbtracker;
    //CSMBtracker::GeomType _ittype;

    // CSMBT point of origin in ref. System
    G4ThreeVector _csmbtDetCenter;

  };

} // namespace csmbt

#endif /* CSMBTLadderSD_hh */
