// FLOOR descriprion manager class

#include "FLOORdescription.hh"

namespace flr {

FLOORdescription::FLOORdescription() : Detector("FLOORdescription")
{
  _x0 = 0.0;
  _y0 = 0.0;
  _z0 = 0.0;

  _isExternal = false;

  _zHalfLength = 0.0;
  _halfWidth = 0.0;
  _halfHeight = 0.0;

  _geomType = 0;

  _nFlrBx = 0;

}

genelmbs::Boxkind* FLOORdescription::getFlrBoxKindElem(int n) const {
        if (n<0 || n>= _nFlrBx){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Floor Box like element number: "<< n <<" not present";
          e.error();
        }
        return &(_flrBx[n]);
}

} // namespace flr
