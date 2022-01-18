//
// Defines a generic CARGOracker sensitive detector
//
// Original author G. Tassielli
//

#ifndef CARGOtargetSD_hh
#define CARGOtargetSD_hh

// CARGO includes
#include "CARGOdescription.hh"
#include "GeomHandle.hh"
#include "SensitiveDetector.hh"

namespace crg {

  class CARGOtargetSD : public SensitiveDetector {

  public:
    CARGOtargetSD(const G4String &name, const G4String& hitsCollectionName, const crd::SimpleConfig& config);
    virtual ~CARGOtargetSD();

    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    void setCARGOCenterInDetSys(const G4ThreeVector &origin) {
            _crgDetCenter = origin + _systOrigin;
    }

  protected:

    int _elementId;

    GeomHandle<CARGOdescription> crgdesc;
    //CARGOdescription::GeomType _ittype;

    // CARGO point of origin in ref. System
    G4ThreeVector _crgDetCenter;

  };

} // namespace crg

#endif /* CARGOtargetSD_hh */
