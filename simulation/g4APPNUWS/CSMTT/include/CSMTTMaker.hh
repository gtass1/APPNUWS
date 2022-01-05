// CSMTT geometry maker
//
// Original author G. Tassielli
//

#ifndef CSMTTMaker_hh
#define CSMTTMaker_hh

#include <map>
#include <memory>
#include <string>
#include <vector>

//#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "CSMTtracker.hh"
#include "CSMTTAbsorber.hh"
#include "CLHEP/Vector/ThreeVector.h"


namespace crd {
class SimpleConfig;
}

namespace csmtt {

//class CSMTtracker;
//class CSMTTAbsorber;
//class Wall;

class CSMTTMaker{

public:

  CSMTTMaker( crd::SimpleConfig const& config );
  ~CSMTTMaker ();

  // This is the accessor that will remain.
  std::unique_ptr<CSMTtracker> getCSMTTrackerPtr() { return std::move(_lpst); }
  std::unique_ptr<CSMTTAbsorber> getCSMTTabsorberPtr() { return std::move(_lpsabs); }

private:

  void loadTracker( crd::SimpleConfig const& config );
  void Build();
  void loadAbsorber( crd::SimpleConfig const& config );
  void BuildAbsorber();

  // Name of external gdml geometry file description.
  std::string _extFile;
  bool _isExternal;

  // Basic geometry element parameter
  int _nLayers;                 //Number of Layers
  double _distIn;               //Nominal Inner height of the tracker
  double _distOut;              //Nominal Outer height of the tracker
  double _halfLength;           //Nominal Half-Length of the tracker along the z axis
  double _halfWidth;            //Nominal Half-Width of the tracker along the second axis
  double _halfThickness;        //Nominal Half-Thickness of the tracker along the third axis

  int _geomType;                //Version number (see CSMTTracker for definition)

//  double _x0;                   //Shift along x of the center of the tracker
//  double _y0;                   //Shift along y of the center of the tracker
//  double _z0;                   //Shift along z of the center of the tracker

  //Detailed parameters

  std::vector<double> _LayersVertPos;
  std::vector<double> _LayersFstSidePos;
  std::vector<double> _LayersSndSidePos;
  std::vector<int>    _nLaddersPerFstSide;
  std::vector<int>    _nLaddersPerSndSide;
  std::vector<double> _LaddersFstSideDim;
  std::vector<double> _LaddersDeadZoneFstSide;
  std::vector<double> _SpaceBtwnLadFstSide;
  std::vector<double> _LaddersSndSideDim;
  std::vector<double> _LaddersDeadZoneSndSide;
  std::vector<double> _SpaceBtwnLadSndSide;
  std::vector<double> _LaddersThickness;
  std::vector<int>    _ROTypes;
  std::vector<double> _ROfirstSideDim;
  std::vector<double> _ROfirstSideInsul;
  std::vector<double> _ROSecondSideDim;
  std::vector<double> _ROSecondSideInsul;
  std::vector<double> _ROangle;
  std::vector<unsigned int> _nROsFstSdPerLadder;
  std::vector<unsigned int> _nROsSndSdPerLadder;
  std::vector<unsigned long> _nROsPerLadder;

  bool                _InvertLadderShellOrder;
  std::vector<int>    _LaddersNmShells;
  std::vector< std::vector<std::string> > _LaddersShellsMaterial;
  std::vector< std::vector<double> > _LaddersShellsThick;
  std::vector<int>    _ReadoutsNmShells;
  std::vector< std::vector<int> > _ReadOutsShellIds;

  //Absorber material description
  //Barrel region
  int _nAbsorbLayers;
  std::vector<double> _AbsorbInRasius;
  std::vector<double> _AbsorbHalfLengths;
  std::vector<int>    _AbsorbNmShells;
  std::vector< std::vector<std::string> > _AbsorbShellsMaterial;
  std::vector< std::vector<double> > _AbsorbShellsThick;
  std::vector< double > _AbsorbersThickness;

  // Center of the tracker.
  CLHEP::Hep3Vector _center;

  std::unique_ptr<CSMTtracker> _lpst;
  std::unique_ptr<CSMTTAbsorber> _lpsabs;

};

}  //namespace csmtt

#endif /* CSMTTMaker_hh */
