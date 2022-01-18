// CARGO geometry maker
//
// Original author G. Tassielli
//

#ifndef CARGOMaker_hh
#define CARGOMaker_hh

#include <map>
#include <memory>
#include <string>
#include <vector>

//#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "CLHEP/Vector/ThreeVector.h"

#include "CARGOdescription.hh"

namespace crd {
class SimpleConfig;
}

namespace crg {

class CARGOMaker{

public:

  CARGOMaker( crd::SimpleConfig const& config );
  ~CARGOMaker ();

  // This is the accessor that will remain.
  std::unique_ptr<CARGOdescription> getCARGOdescPtr() { return std::move(_lcrgd); }

private:

  void load( crd::SimpleConfig const& config );
  void Build();
//  void loadAbsorber( crd::SimpleConfig const& config );
//  void BuildAbsorber();

  // Name of external gdml geometry file description.
  std::string _extFile;
  bool _isExternal;

  int _geomType;                //Version number (see CARGOracker for definition)

//  //Detailed parameters

  int _nBxKindEl;

  //Box elements description
  std::vector<double> _BxKindLength;
  std::vector<double> _BxKindWidth;
  std::vector<double> _BxKindHeight;
  std::vector<int> _BxKindShape;
  std::vector<double> _BxKindThickness;
  std::vector<std::string> _BxKindEqMaterial;
  std::vector<int> _BxKindIsTarget;
  std::vector<double> _BxKindPosX;
  std::vector<double> _BxKindPosY;
  std::vector<double> _BxKindPosZ;

  int _nCylKindEl;

  //Cylinder elements description
  std::vector<double> _CylKindLength;
  std::vector<double> _CylKindInnerRadius;
  std::vector<double> _CylKindOuterRadius;
  std::vector<int> _CylKindShape;
  std::vector<double> _CylKindThickness;
  std::vector<int> _CylKindOrientation;
  std::vector<std::string> _CylKindEqMaterial;
  std::vector<int> _CylKindIsTarget;
  std::vector<double> _CylKindPosX;
  std::vector<double> _CylKindPosY;
  std::vector<double> _CylKindPosZ;

  std::unique_ptr<CARGOdescription> _lcrgd;

};

}  //namespace crg

#endif /* CARGOMaker_hh */
