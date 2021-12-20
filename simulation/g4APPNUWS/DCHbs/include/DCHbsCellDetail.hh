// DCH base cells description
//
// Original author G. Tassielli
//

#ifndef CellDetail_BS_hh
#define CellDetail_BS_hh

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "DCHbsCellWallDetail.hh"
#include "DCHbsWireDetail.hh"
#include "manageException.hh"

// CDCH includes

namespace dchbs {

class CellDetail{

public:
  CellDetail():
          _circumscribedRadius(0.0),
          _inscribedCircleRadius(0.0)
  {}

  CellDetail( double circumscribedRadius, double inscribedCircleRadius, boost::shared_ptr<WireDetail> senseWire );

  CellDetail( double inscribedCircleRadius, boost::shared_ptr<WireDetail> senseWire, boost::shared_ptr<CellWallDetail> wall );

  ~CellDetail ();

  double      CirumscribedRadius()   const { return _circumscribedRadius;}
  double      InscribedCircleRadius()   const { return _inscribedCircleRadius;}
  double      wireRadius()/* const throw(cet::exception)*/ {
    //          try {
    //                  return _senseWire.get()->outerRadius();
    //          } catch (cet::exception e) {
    //              throw cet::exception("GEOM")
    //                << "No sense wire defined for the Cell \n";
    //          return 0.0;
    //          }
    if (!_senseWire) {
      exc::exceptionG4 e("GEOM","Fatal Error",0);
      e<<"No sense wire defined for the Cell \n";
      e.error();
      return 0.0;
    } else {
      return _senseWire->outerRadius();
    }
  }

  double      halfLength()/* const throw(cet::exception)*/ {
//          try {
//                  return _senseWire->halfLength();
//          } catch (cet::exception e) {
//              throw cet::exception("GEOM")
//                << "No sense wire defined for the Cell \n";
//          return 0.0;
//          }
    if (!_senseWire) {
      exc::exceptionG4 e("GEOM","Fatal Error",0);
      e<<"No sense wire defined for the Cell \n";
      e.error();
      return 0.0;
    } else {
      return _senseWire->halfLength();
    }
  }

  bool        hasWall() { return (_wall!=0);  }

  double      wallThick() {
    if (!_wall) {
      return 0.0;
    } else {
      return _wall->totThickness();
    }
  }

protected:

  double _circumscribedRadius;
  double _inscribedCircleRadius;
  boost::shared_ptr<WireDetail> _senseWire;
  boost::shared_ptr<CellWallDetail> _wall;

};

}  //namespace dchbs

#endif /* CellDetail_BS_hh */
