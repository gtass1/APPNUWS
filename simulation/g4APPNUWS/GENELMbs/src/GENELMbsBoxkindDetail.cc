//
// Original author G. Tassielli
//

#include "GENELMbsBoxkindDetail.hh"

namespace genelmbs {

  BoxkindDetail::BoxkindDetail():
          _halfLength(0.0)
          ,_halfWidth(0.0)
          ,_halfHeight(0.0)
          ,_wallThickness(0.0)
          ,_centWallThickness(0.0)
  {
    _materialName="";
  }

  BoxkindDetail::BoxkindDetail( double halfLength, double halfWidth, double halfHeight, std::string materialName, double wallThicknesses, double centWallThickness
                           ):
              _halfLength(halfLength)
              ,_halfWidth(halfWidth)
              ,_halfHeight(halfHeight)
              ,_wallThickness(wallThicknesses)
              ,_centWallThickness(centWallThickness)
  {
    _materialName = materialName;
  }

  BoxkindDetail::~BoxkindDetail (){
  }

} // namespace genelmbs
