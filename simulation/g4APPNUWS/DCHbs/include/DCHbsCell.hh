#ifndef Cell_BS_hh
#define Cell_BS_hh

#include <deque>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "CLHEP/Vector/ThreeVector.h"
#include "DCHbsCellDetail.hh"
#include "DCHbsCellId.hh"
#include "DCHbsWire.hh"

namespace cdch {
//class CellGeometryHandle;
class CDCHCellGeometryHandle_v2;
class CDCHCellGeometryHandle_v3;
class CDCHCellGeometryHandle_v4;
//class CellGeometryHandle_v2_DBL;
//class CellGeometryHandle_v3_DBL;
//class CDCHtracker;
class CDCHMaker;
//class SuperLayer;
//class Layer;
}

namespace tdch {
//class TDCHtracker;
class TDCHMaker;
//class Station;
//class Plane;
//class Panel;
}

namespace dchbs {

class Cell /*: public Straw*/ {

//  friend class CellGeometryHandle;
  friend class cdch::CDCHCellGeometryHandle_v2;
  friend class cdch::CDCHCellGeometryHandle_v3;
  friend class cdch::CDCHCellGeometryHandle_v4;
//  friend class CellGeometryHandle_v2_DBL;
//  friend class CellGeometryHandle_v3_DBL;
//  friend class CDCHtracker;
  friend class cdch::CDCHMaker;
//  friend class SuperLayer;
//  friend class Layer;
//
  friend class tdch::TDCHMaker;
//  friend class tdch::Station;
//  friend class tdch::Plane;
//  friend class tdch::Panel;

public:

  // A free function, returning void, that takes a const Cell& as an argument.
  typedef void (*CellFunction)( const Cell& s);

  Cell();

  // Constructor using sense wire info.
  Cell( CellId id,
         boost::shared_ptr<CellDetail> detail,
         boost::shared_ptr<Wire> senseWire
         );

//  // Constructor using sense wire info.
//  Cell( CellId id,
//         CellIndex index,
//         boost::shared_ptr<CellDetail> detail,
//         int detailIndex,
//         boost::shared_ptr<Wire> senseWire
//         );

  ~Cell ();

  CellId Id() const { return _id;}
//  CellIndex Index() const { return _index;}

  boost::shared_ptr<CellDetail> getDetail() const { return _detail;}

  boost::shared_ptr<Wire> getWire() const { return  _senseWire; }

  const CLHEP::Hep3Vector& getMidPoint()  const;

  const CLHEP::Hep3Vector& getDirection() const;

  double getHalfLength() const { return _senseWire.get()->getDetail()->halfLength();}

  double getRadius() const { return getDetail()->CirumscribedRadius();}
  double getWallThickness() const { return getDetail()->wallThick();}

  size_t nFWires() const { return _fieldWires.size(); }
  boost::shared_ptr<Wire> getFWire(int iFw) const;
//  int hack;

protected:

  // Identifier
  CellId _id;

  // Detailed description of a cell.
  boost::shared_ptr<CellDetail> _detail;
  int _detailIndex;

  boost::shared_ptr<Wire> _senseWire;
  std::vector<boost::shared_ptr<Wire> > _fieldWires;

  CLHEP::Hep3Vector _tmpMidPoint;
  CLHEP::Hep3Vector _tmpDirection;

};

}  //namespace dchbs

#endif /* Cell_BS_hh */
