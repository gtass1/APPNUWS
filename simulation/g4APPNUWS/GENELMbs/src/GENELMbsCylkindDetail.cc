//
// Original author G. Tassielli
//

#include "GENELMbsCylkindDetail.hh"

namespace genelmbs {

  CylkindDetail::CylkindDetail():
          _halfLength(0.0)
          ,_innerRadius(0.0)
          ,_outerRadius(0.0)
//          ,_centralRadius(0.0)
          ,_wallThickness(0.0)
          ,_centWallThickness(0.0)
  {
    _materialName="";
  }

  CylkindDetail::CylkindDetail( double halfLength, double innerRadius, double outerRadius, /*double centralRadius,*/ std::string materialName, double wallThicknesses, double centWallThickness
                           ):
              _halfLength(halfLength)
              ,_innerRadius(innerRadius)
              ,_outerRadius(outerRadius)
//              ,_centralRadius(centralRadius)
              ,_wallThickness(wallThicknesses)
              ,_centWallThickness(centWallThickness)
  {
    _materialName = materialName;
  }

  CylkindDetail::~CylkindDetail (){
  }

} // namespace genelmbs
