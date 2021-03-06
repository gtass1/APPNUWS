// CSMBT tracker manager class
//
// Original author G. Tassielli
//

#include "CSMBtracker.hh"
#include "manageException.hh"

//#include "Layer.hh"
//#include "ROGeometryHandle.hh"

//using namespace std;

namespace csmbt {

CSMBtracker::CSMBtracker(std::string name) : Detector(name) {
        _x0              =0.0;
        _y0              =0.0;
        _z0              =0.0;
        _distIn          =0.0;
        _distOut         =0.0;

        _extFile         ="";
        _isExternal      =false;
        _nLayers         =0;
        _zHalfLength     =0.0;
        _geomType        =00;
//        _sensType        =CSMBtracker::Pixel;
}

//pxstbs::ROGeometryHandle* CSMBtracker::getROGeometryHandle() const {
CSMBTROGeometryHandle* CSMBtracker::getROGeometryHandle() const {
  return _roChhnd.get();
}


pxstbs::Layer* CSMBtracker::getLayer(int n) const /*throw(cet::exception)*/ {
//        if (n>=0 && n< _nSuperLayers){
//                return &(_sprlr[n]);
//        } else {
//          throw cet::exception("GEOM")<< "Super Layer number: "<< n <<" not present";
        if (n<0 || n>= _nLayers){
          exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
          e<<"Super Layer number: "<< n <<" not present";
          e.error();
        }
        return &(_lr[n]);
}

} // namespace csmbt
