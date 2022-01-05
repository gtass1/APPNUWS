//
// CSMTTtrackerBuilder builder class for the CSMTT in geant4
//
// Original author G. Tassielli
//

#ifndef CSMTTrackerBuilder_hh
#define CSMTTrackerBuilder_hh

#include "VolumeInfo.hh"
#include "SimpleConfig.hh"

// G4 includes
#include "G4LogicalVolume.hh"

// PXSTbs includes
#include "PXSTbsLadder.hh"

//namespace pxstbs {
//class Ladder;
//}

namespace csmtt {

  class CSMTTBuilder {
  public:
    static VolumeInfo constructTracker( G4LogicalVolume* mother/*, double zOff*/ );
    static void constructStepLimiters();
    static void instantiateSensitiveDetectors(const std::string hitsCollectionName="CSMTTHitsCollection");
    static void constructAbsorber( G4LogicalVolume* csmttmother/*, double zOff*/ );
  private:
    static VolumeInfo buildLadder(pxstbs::Ladder &tld);
  };

} //namespace csmtt

#endif /* CSMTTrackerBuilder_hh */
