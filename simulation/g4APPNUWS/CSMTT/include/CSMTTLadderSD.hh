//
// Defines a generic CSMTTracker sensitive detector
//
// Original author G. Tassielli
//

#ifndef CSMTTLadderSD_hh
#define CSMTTLadderSD_hh

// CSMTT includes
#include "CSMTtracker.hh"
#include "GeomHandle.hh"
#include "SensitiveDetector.hh"

namespace csmtt {

  class CSMTTLadderSD : public SensitiveDetector {

  public:
    CSMTTLadderSD(const G4String &name, const G4String& hitsCollectionName, const crd::SimpleConfig& config);
    virtual ~CSMTTLadderSD();

    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    void setCSMTTCenterInDetSys(const G4ThreeVector &origin) {
            _csmttDetCenter = origin + _systOrigin;
    }

  protected:

    int _layer;
    int _phiSec;
    int _ladder;
    int _roSubShell;
    unsigned long _roIdFstSide;
    unsigned long _roIdSndSide;
//    double _Dphi;

    GeomHandle<CSMTtracker> csmttracker;
    //CSMTTtracker::GeomType _ittype;

    // CSMTT point of origin in ref. System
    G4ThreeVector _csmttDetCenter;

  };

} // namespace csmtt

#endif /* CSMTTLadderSD_hh */
