#ifndef GEOMSRVC_INCLUDE_findMaterialOrThrow_hh
#define GEOMSRVC_INCLUDE_findMaterialOrThrow_hh
//
// Free function wrapper around
//   G4NistManager::FindOrBuildMaterial
// The wrapper does the job of throwing if the pointer comes
// back null.
//
//      Author: tassiell
//
//

class G4Material;
class G4String;

namespace gmsrv {

  G4Material* findMaterialOrThrow( G4String const& name);

} // end namespace gmsrv
#endif /* GEOMSRVC_INCLUDE_findMaterialOrThrow_hh */
