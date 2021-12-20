//
// CSMVtrackerBuilder builder class for the CSMV in geant4
//
// Original author G. Tassielli
//

#ifndef CSMVtrackerBuilder_hh
#define CSMVtrackerBuilder_hh

#include "VolumeInfo.hh"
#include "SimpleConfig.hh"

// G4 includes
#include "G4LogicalVolume.hh"

// PXSTbs includes
#include "PXSTbsLadder.hh"

//namespace pxstbs {
//class Ladder;
//}

namespace csmv {

  class CSMVBuilder {
  public:
    static VolumeInfo constructTracker( G4LogicalVolume* mother/*, double zOff*/ );
    static void constructStepLimiters();
    static void instantiateSensitiveDetectors(const std::string hitsCollectionName="CSMVHitsCollection");
    static void constructAbsorber( G4LogicalVolume* csmvmother/*, double zOff*/ );
  private:
    static VolumeInfo buildLadder(pxstbs::Ladder &tld);
  };

} //namespace csmv

#endif /* CSMVtrackerBuilder_hh */
