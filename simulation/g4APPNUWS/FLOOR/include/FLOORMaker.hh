// FLOOR geometry maker
//
// Original author G. Tassielli
//

#ifndef FLOORMaker_hh
#define FLOORMaker_hh

#include <map>
#include <memory>
#include <string>
#include <vector>

//#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "CLHEP/Vector/ThreeVector.h"

#include "FLOORdescription.hh"

namespace crd {
class SimpleConfig;
}

namespace flr {

class FLOORMaker{

public:

  FLOORMaker( crd::SimpleConfig const& config );
  ~FLOORMaker ();

  // This is the accessor that will remain.
  std::unique_ptr<FLOORdescription> getFLOORdescPtr() { return std::move(_lflrd); }

private:

  void load( crd::SimpleConfig const& config );
  void Build();
//  void loadAbsorber( crd::SimpleConfig const& config );
//  void BuildAbsorber();

  // Name of external gdml geometry file description.
  std::string _extFile;
  bool _isExternal;

  // Basic geometry element parameter
  double _halfLength;           //Nominal Half-Length of the truck along the z axis
  double _halfWidth;            //Nominal Half-Width of the truck along the second axis
  double _halfHeight;           //Nominal Half-Thickness of the truck along the third axis

  int _geomType;                //Version number (see FLOORracker for definition)

//  double _x0;                   //Shift along x of the center of the truck
//  double _y0;                   //Shift along y of the center of the truck
//  double _z0;                   //Shift along z of the center of the truck

//  //Detailed parameters

  int _nBxKindEl;

  //Plane description
  double _Length;
  double _PlaneWidth;
  double _PlaneThicknes;
  std::string _FullPlaneEqMaterial;
  //Optional Skeleton Plane description
  int    _nSkeletonShafts;
  genelmbs::Boxkind::BxShpType _SkeletonShaftShape;
  double _SkeletonShaftsInterDist;
  std::vector<double> _SkeletonShaftsPos;
  double _SkeletonShaftWidth;
  double _SkeletonShaftHeight; //Shaft with I profile, Height is its total Height/Thickness
  double _SkeletonShaftThicknesExt;
  double _SkeletonShaftThicknesCent;
  std::string _SkeletonEqMaterial;
  int    _nSkeletonRibs;
  genelmbs::Boxkind::BxShpType _SkeletonRibShape;
  double _SkeletonRibsInterDist;
  std::vector<double> _SkeletonRibsPos;
  double _SkeletonRibWidth;
  double _SkeletonRibHeight; //Shaft with I profile, Height is its total Height/Thickness
  double _SkeletonRibThicknesExt;
  double _SkeletonRibThicknesCent;

  // Center of the truck.
  CLHEP::Hep3Vector _center;

  std::unique_ptr<FLOORdescription> _lflrd;

};

}  //namespace flr

#endif /* FLOORMaker_hh */
