//
// Original author G. Tassielli
//

#ifndef CylkindDetail_hh
#define CylkindDetail_hh

#include <string>
#include <vector>

#include "manageException.hh"

namespace genelmbs {

class CylkindDetail{

public:
  CylkindDetail();

  CylkindDetail( double halfLength, double innerRadius, double outerRadius, /*double centralRadius,*/ std::string materialName, double wallThicknesses=0.0, double centWallThickness=0.0 );

  ~CylkindDetail ();

  double      halfLength()         const { return _halfLength;}
  double      innerRadius()        const { return _innerRadius;}
  double      outerRadius()        const { return _outerRadius;}
//  double      centralRadius()      const { return _centralRadius;}
  double      wallThickness()      const { return _wallThickness;}
  double      centWallThickness()  const { return _centWallThickness;}

  std::string const materialName() const { return _materialName;  }

private:

  double _halfLength;
  double _innerRadius;
  double _outerRadius;
//  double _centralRadius;
  double _wallThickness;
  double _centWallThickness;

  std::string _materialName;

};

}  //namespace genelmbs

#endif /* CylkindDetail_hh */
