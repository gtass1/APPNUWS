//
// Original author G. Tassielli
//

#ifndef CellWallDetail_BS_hh
#define CellWallDetail_BS_hh

#include <vector>
#include <string>

// Framework includes
//#include "messagefacility/MessageLogger/MessageLogger.h"
#include "manageException.hh"

namespace dchbs {

class CellWallDetail{

public:
  CellWallDetail():
    _materialNames(),
    _shellsThicknesses(),
    _totThick(-1.)
  {}

  CellWallDetail( std::vector<double> & thicknesses, std::vector<std::string> & materialNames);

  ~CellWallDetail ();

  std::string const materialName(int idx) const /*throw(cet::exception)*/ {
//          try {
//                  return _materialNames.at(idx);
//          } catch (cet::exception e) {
//              throw cet::exception("GEOM")
//                << "No material defined for the wire \n";
//          }
    try {
      _materialNames.at(idx);
    } catch (std::exception &ex) {
      exc::exceptionG4 e("GEOM","Fatal Error in Arguments",1);
      e<< "No material defined for the cell wall \n";
      e.error();
    }
    return _materialNames.at(idx); //just to remove warning
  }

  const std::vector<std::string> &materialNames() const { return _materialNames;}

  double /*const*/ shellThickness(int idx) const/* throw(cet::exception)*/ {
//          try {
//              return _shellsThicknesses.at(idx);
//          } catch (cet::exception e) {
//              throw cet::exception("GEOM")
//                << "No shells thicknesses defined for the wire \n";
//          }
    try {
      /*return*/ _shellsThicknesses.at(idx);
    } catch (std::exception &ex) {
      exc::exceptionG4 e("GEOM","Fatal Error in Arguments",1);
      e<< "No shells thicknesses defined for the cell wall \n";
      e.error();
    }
    return _shellsThicknesses.at(idx); //just to remove warning
  }

  const std::vector<double> &shellsThicknesses() const { return _shellsThicknesses;}

  double      totThickness()  const { return _totThick;}

protected:

  // Order of materials and shells dimensions is:
  std::vector<std::string> _materialNames;
  std::vector<double> _shellsThicknesses;

  double _totThick;

};

}  //namespace dchbs

#endif /* CellWallDetail_BS_hh */
