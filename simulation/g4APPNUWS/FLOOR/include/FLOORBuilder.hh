//
// FLOORBuilder builder class for the FLOOR in geant4
//
// Original author G. Tassielli
//

#ifndef FLOORBuilder_hh
#define FLOORBuilder_hh

#include "VolumeInfo.hh"
#include "SimpleConfig.hh"

// GENELMbs includes
#include "GENELMbsBoxkind.hh"
#include "GENELMbsCylkind.hh"

// G4 includes
#include "G4LogicalVolume.hh"

#include <string>

namespace flr {

  class FLOORBuilder {
  public:
    static VolumeInfo construct( G4LogicalVolume* mother/*, double zOff*/ );
    static void constructStepLimiters();
//    static void instantiateSensitiveDetectors(const std::string hitsCollectionName="FLOORHitsCollection");
  private:
    static G4VSolid* buildBoxElem(genelmbs::Boxkind *ibx, char shape[]);
  };

} //namespace flr

#endif /* FLOORBuilder_hh */
