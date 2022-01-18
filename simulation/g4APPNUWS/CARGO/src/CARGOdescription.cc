// CARGO descriprion manager class

#include "CARGOdescription.hh"

#include <algorithm>

namespace crg {

CARGOdescription::CARGOdescription() : Detector("CARGOdescription")
{
  _isExternal = false;

  _geomType = 0;

  _nCrgBx = 0;
  _nCrgBxTrgt = 0;
  _nCrgCyl = 0;
  _nCrgCylTrgt = 0;

}

genelmbs::Boxkind* CARGOdescription::getCrgBoxKindElem(int n) const {
        if (n<0 || n>= _nCrgBx){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Cargo Box like element number: "<< n <<" not present";
          e.error();
        }
        return &(_crgBx[n]);
}

genelmbs::Cylkind* CARGOdescription::getCrgCylKindElem(int n) const {
        if (n<0 || n>= _nCrgCyl){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Cargo Cylinder like element number: "<< n <<" not present";
          e.error();
        }
        return &(_crgCyl[n]);
}


genelmbs::Boxkind* CARGOdescription::getCrgBoxKindTargetElem(int n) const {
        if (n<0 || n>= _nCrgBxTrgt){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Cargo Box like Target element number: "<< n <<" not present";
          e.error();
        }
        return &(_crgBx[_crgBxTrgtId[n]]);
}

genelmbs::Cylkind* CARGOdescription::getCrgCylKindTargetElem(int n) const {
        if (n<0 || n>= _nCrgCylTrgt){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Cargo Cylinder like Target element number: "<< n <<" not present";
          e.error();
        }
        return &(_crgCyl[_crgCylTrgtId[n]]);
}

bool CARGOdescription::checkBoxKindElemIsTarget (int ibxel ) const {
  bool isTarget = std::find(_crgBxTrgtId.begin(), _crgBxTrgtId.end(), ibxel) != _crgBxTrgtId.end();
  return isTarget;
}

bool CARGOdescription::checkCylKindElemIsTarget (int icylel ) const {
  bool isTarget = std::find(_crgCylTrgtId.begin(), _crgCylTrgtId.end(), icylel) != _crgCylTrgtId.end();
  return isTarget;
}

} // namespace crg
