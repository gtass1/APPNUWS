#ifndef VolumeInfo_hh
#define VolumeInfo_hh
//
// Information about a physical volume.
// The center information is not fully general: it does not know about rotations
// and is useful only for the top few levels of the detector.
//
// Author G. Tassielli
//

#include <string>

#include "CLHEP/Vector/ThreeVector.h"

class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

//namespace cdch {

  class VolumeInfo{

  public:

    // Depracated: this will go away.
    VolumeInfo():
      name(),
      solid(0),
      logical(0),
      physical(0),
      centerInParent(),
      centerInWorld(){}

    VolumeInfo( const std::string&       pName,
                const CLHEP::Hep3Vector& inParent,
                const CLHEP::Hep3Vector& parentInWorld);

    // Compiler written versions will be correct for:
    // destructor, copy constructor, assignment operator.

    // The name of this volume as known to G4.
    std::string name;

    // Non-owning pointers to volume information.
    G4VSolid*          solid;
    G4LogicalVolume*   logical;
    G4VPhysicalVolume* physical;

    // Location information in two coordinate systems.
    CLHEP::Hep3Vector      centerInParent;
    CLHEP::Hep3Vector      centerInWorld;

//
//  private:
//    static const CLHEP::Hep3Vector& mu2eOriginInWorld();
  };

//}

#endif /* VolumeInfo_hh */
