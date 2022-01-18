//
// TRUCKBuilder builder class for the TRUCK in geant4
//
// Original author G. Tassielli
//

#ifndef TRUCKBuilder_hh
#define TRUCKBuilder_hh

#include "VolumeInfo.hh"
#include "SimpleConfig.hh"

// GENELMbs includes
#include "GENELMbsBoxkind.hh"
#include "GENELMbsCylkind.hh"

// G4 includes
#include "G4LogicalVolume.hh"

#include <string>

namespace trck {

  class TRUCKBuilder {
  public:
    static VolumeInfo construct( G4LogicalVolume* mother/*, double zOff*/ );
    static void constructStepLimiters();
//    static void instantiateSensitiveDetectors(const std::string hitsCollectionName="TRUCKHitsCollection");
    static VolumeInfo containerInVol();// { return CntInInfo; }
  private:
    static G4VSolid* buildBoxElem(genelmbs::Boxkind *ibx, char shape[]);
//    static G4VSolid* buildCylElem(genelmbs::Cylkind *ibx, char shape[]);
    static VolumeInfo buildCylElem(genelmbs::Cylkind *icy, int iCy);
//    static VolumeInfo buildLadder(pxstbs::Ladder &tld);
//    static VolumeInfo CntInInfo;

  };

} //namespace trck

#endif /* TRUCKBuilder_hh */
