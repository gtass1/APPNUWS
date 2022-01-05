//
// CSMBTrackerBuilder builder class for the CSMBT in geant4
//
// Original author G. Tassielli
//

#ifndef CSMBTrackerBuilder_hh
#define CSMBTrackerBuilder_hh

#include "VolumeInfo.hh"
#include "SimpleConfig.hh"

// G4 includes
#include "G4LogicalVolume.hh"

// PXSTbs includes
#include "PXSTbsLadder.hh"

//namespace pxstbs {
//class Ladder;
//}

namespace csmbt {

  class CSMBTBuilder {
  public:
    static VolumeInfo constructTracker( G4LogicalVolume* mother/*, double zOff*/ );
    static void constructStepLimiters();
    static void instantiateSensitiveDetectors(const std::string hitsCollectionName="CSMBTHitsCollection");
    static void constructAbsorber( G4LogicalVolume* csmbtmother/*, double zOff*/ );
  private:
    static VolumeInfo buildLadder(pxstbs::Ladder &tld);
  };

} //namespace csmbt

#endif /* CSMBTrackerBuilder_hh */
