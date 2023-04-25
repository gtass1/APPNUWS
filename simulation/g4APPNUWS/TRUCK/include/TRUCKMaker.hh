// TRUCK geometry maker
//
// Original author G. Tassielli
//

#ifndef TRUCKMaker_hh
#define TRUCKMaker_hh

#include <map>
#include <memory>
#include <string>
#include <vector>

//#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "CLHEP/Vector/ThreeVector.h"

#include "TRUCKdescription.hh"

namespace crd {
class SimpleConfig;
}

namespace trck {

class TRUCKMaker{

public:

  TRUCKMaker( crd::SimpleConfig const& config );
  ~TRUCKMaker ();

  // This is the accessor that will remain.
  std::unique_ptr<TRUCKdescription> getTRUCKdescPtr() { return std::move(_ltrkd); }

private:

  void loadTruck( crd::SimpleConfig const& config );
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

  int _geomType;                //Version number (see TRUCKracker for definition)

//  double _x0;                   //Shift along x of the center of the truck
//  double _y0;                   //Shift along y of the center of the truck
//  double _z0;                   //Shift along z of the center of the truck

//  //Detailed parameters

  int _nTrlBxKindEl;
  int _nTrlCylKindEl;
  int _nTrkBxKindEl;
  int _nTrkCylKindEl;
  int _nCntBxKindEl;

  //Trailer Plane description
  double _TrailerPlaneLength;
  double _TrailerPlaneWidth;
  double _TrailerPlaneThicknes;
  std::string _TrailerFullPlaneEqMaterial;
  double _TrailerPlaneHeight; //Trailer Plane distance from the floor
  int    _nTrailerRibs;
  std::vector<double> _TrailerRibPos;
  double _TrailerRibsWidth;
  double _TrailerRibsHeight; //Shaft with I profile, Height is its total Height/Thickness
  double _TrailerRibsThicknesExt;
  double _TrailerRibsThicknesCent;
  std::string _TrailerRibsEqMaterial;

  //Trailer Chassis description
  double _TrailerShaftsInterDist;
  double _TrailerShaftsFullHeightLength;
  double _TrailerShaftsWidth;
  double _TrailerShaftsHeight; //Shaft with I profile, Height is its total Height/Thickness
  double _TrailerShaftsThicknesExt;
  double _TrailerShaftsThicknesCent;
  std::string _TrailerShaftsEqMaterial;
  double _TrailerReducedShaftsHeight;

  //Trailer wheels description
  int _nTrailerWheels;
  double _TrlWheelRimRadius;
  double _TrlWheelRimWidth;
  double _TrlWheelRimShellThickness;
  double _TrlWheelRimEqThickness;
  double _TrlTireHeight;
  double _TrlWheelPitch;
  double _TrlWheelDistFromBack;
  double _TrlWheelAxisRadius;
  std::string _TrlWheelRimMaterial;
  std::string _TrlWheelAxisMaterial;
  std::string _TrlTireMaterial;
  double _TrlTireEqThickness;

  //Container description
  double _ContainerLength;
  double _ContainerWidth;
  double _ContainerHeight;
  std::string _ContainerInFillMaterial;
  double _ContainerWallThicknes;
  double _ContainerRibsThicknes;
  double _ContainerBasementThicknes;
  std::string _ContainerWallEqMaterial;
  std::string _ContainerRibsEqMaterial;
  std::string _ContainerBasementEqMaterial;

  double _ContainerInLength;
  double _ContainerInWidth;
  double _ContainerInHeight;
  CLHEP::Hep3Vector _ContainerInRelCenter;

  //Truck Chassis description
  double _TruckShaftsInterDist;
  double _TruckShaftsLength;
  double _TruckShaftsWidth;
  double _TruckShaftsHeight; //Shaft with C profile, Height is its total Height/Thickness
  double _TruckShaftsThicknesExt;
  double _TruckShaftsThicknesCent;
  double _TruckChassistsHeight;
  std::string _TruckChassisEqMaterial;
  int _nTruckRibs;
  std::vector<double> _TruckRibPos;
  double _TruckTrailerOverlap; //Truck chassis portion under the trailer

  double _TruckWheelRimRadius;
  double _TruckWheelRimWidth;
  double _TruckWheelRimShellThickness;
  double _TruckWheelRimEqThickness;
  double _TruckTireHeight;
  double _TruckWheelDistFromBack;
  double _TruckWheelDistFromFront;
  double _TruckWheelAxisRadius;
  std::string _TruckWheelRimMaterial;
  std::string _TruckWheelAxisMaterial;
  std::string _TruckTireMaterial;
  double _TruckTireEqThickness;

  double _TrckTrnWheelJnRadius;
  double _TrckTrnWheelJnThickness;
  double _TrckTrnWheelJnDistFromBack;
  std::string _TrckTrnWheelJnEqMaterial;

  // Center of the truck.
  CLHEP::Hep3Vector _center;

  std::unique_ptr<TRUCKdescription> _ltrkd;

};

}  //namespace trck

#endif /* TRUCKMaker_hh */
