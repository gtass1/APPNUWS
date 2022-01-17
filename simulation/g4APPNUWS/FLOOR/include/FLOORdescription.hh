// FLOORdescription manager class

#ifndef FLOORdescription_hh
#define FLOORdescription_hh

//#include <vector>

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "Detector.hh"

#include "GENELMbsBoxkind.hh"
//#include "GENELMbsCylkind.hh"

namespace flr {

class FLOORdescription : public Detector {

  friend class FLOORMaker;

public:
  FLOORdescription();
  ~FLOORdescription() {}

  double x0()            const { return _x0;}
  double y0()            const { return _y0;}
  double z0()            const { return _z0;}

  std::string extFile()  const { return _extFile; }
  bool isExternal()      const { return _isExternal; }

  double zHalfLength()   const { return _zHalfLength;}
  double halfWidth()   const { return _halfWidth;}
  double halfHeight()   const { return _halfHeight;}

  int geomType()         const { return _geomType; }

  int nBoxKindElem()  const { return _nFlrBx; }

  genelmbs::Boxkind* getFlrBoxKindElem(int n) const;

  boost::shared_array<genelmbs::Boxkind> getFlrBoxKindElemArray() const {
          return _flrBx;
  }

protected:
  // Nominal values.
  // _x0 = position of the center of the truck relative to the origin
  // _y0 = position of the center of the truck relative to the origin
  // _z0 = position of the center of the truck relative to the origin
  double _x0;
  double _y0;
  double _z0;

  // Name of external gdml geometry file description.
  std::string _extFile;
  bool _isExternal;

  double _zHalfLength;           //Nominal Half-Length of the truck along the z axis
  double _halfWidth;             //Nominal Half-Width of the truck along the second axis
  double _halfHeight;            //Nominal Half-Thickness of the truck along the third axis

  int _geomType;

  int _nFlrBx;
  boost::shared_array<genelmbs::Boxkind> _flrBx;

};

} //namespace flr

#endif /* FLOORdescription_hh */
