// TRUCK descriprion manager class

#include "TRUCKdescription.hh"

namespace trck {

TRUCKdescription::TRUCKdescription() : Detector("TRUCKdescription")
{
  _x0 = 0.0;
  _y0 = 0.0;
  _z0 = 0.0;

  _isExternal = false;

  _zHalfLength = 0.0;
  _halfWidth = 0.0;
  _halfHeight = 0.0;

  _geomType = 0;

  _nTrkBx = 0;
  _nTrkCyl = 0;

}

genelmbs::Boxkind* TRUCKdescription::getTrkBoxKindElem(int n) const {
        if (n<0 || n>= _nTrkBx){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Truck Box like element number: "<< n <<" not present";
          e.error();
        }
        return &(_trkBx[n]);
}

genelmbs::Cylkind* TRUCKdescription::getTrkCylKindElem(int n) const {
        if (n<0 || n>= _nTrkCyl){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Truck Cylinder like element number: "<< n <<" not present";
          e.error();
        }
        return &(_trkCyl[n]);
}

genelmbs::Boxkind* TRUCKdescription::getContainerElem() const {
  return _containerBx.get();
}

} // namespace trck
