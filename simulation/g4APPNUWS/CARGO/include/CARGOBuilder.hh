//
// CARGOBuilder builder class for the CARGO in geant4
//
// Original author G. Tassielli
//

#ifndef CARGOBuilder_hh
#define CARGOBuilder_hh

#include "VolumeInfo.hh"
#include "SimpleConfig.hh"

// GENELMbs includes
#include "GENELMbsBoxkind.hh"
#include "GENELMbsCylkind.hh"

// G4 includes
#include "G4LogicalVolume.hh"

#include <string>

namespace crg {

  class CARGOBuilder {
  public:
    static void construct( VolumeInfo cargoInfo );
    static void constructStepLimiters();
    static void instantiateSensitiveDetectors(const std::string hitsCollectionName="CARGOHitsCollection");
  private:
    static G4VSolid* buildBoxElem(genelmbs::Boxkind *ibx, char shape[]);
    static VolumeInfo buildCylElem(genelmbs::Cylkind *icy, int iCy);
  };

} //namespace crg

#endif /* CARGOBuilder_hh */
