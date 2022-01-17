// TRUCKdescription manager class

#ifndef TRUCKdescription_hh
#define TRUCKdescription_hh

//#include <vector>

#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include "Detector.hh"

#include "GENELMbsBoxkind.hh"
#include "GENELMbsCylkind.hh"

namespace trck {

class TRUCKdescription : public Detector {

  friend class TRUCKMaker;

public:
  TRUCKdescription();
  ~TRUCKdescription() {}

  double x0()            const { return _x0;}
  double y0()            const { return _y0;}
  double z0()            const { return _z0;}

  std::string extFile()  const { return _extFile; }
  bool isExternal()      const { return _isExternal; }

  double zHalfLength()   const { return _zHalfLength;}
  double halfWidth()   const { return _halfWidth;}
  double halfHeight()   const { return _halfHeight;}

  int geomType()         const { return _geomType; }

  int nBoxKindElem()  const { return _nTrkBx; }
  int nCylKindElem()  const { return _nTrkCyl; }

  genelmbs::Boxkind* getTrkBoxKindElem(int n) const;

  boost::shared_array<genelmbs::Boxkind> getTrkBoxKindElemArray() const {
          return _trkBx;
  }

  genelmbs::Cylkind* getTrkCylKindElem(int n) const;

  boost::shared_array<genelmbs::Cylkind> getTrkCylKindElemArray() const {
          return _trkCyl;
  }

  genelmbs::Boxkind* getContainerElem() const;

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

//  double _containerInX0;
//  double _containerIny0;
//  double _containerInz0;
//  double _containerInLength;           //Nominal Half-Length of the truck along the z axis
//  double _containerInWidth;             //Nominal Half-Width of the truck along the second axis
//  double _containerInHeight;            //Nominal Half-Thickness of the truck along the third axis

  int _nTrkBx;
  int _nTrkCyl;
  boost::shared_array<genelmbs::Boxkind> _trkBx;
  boost::shared_array<genelmbs::Cylkind> _trkCyl;
  boost::shared_ptr<genelmbs::Boxkind> _containerBx; //Description of the inside volume of the Container useful for the cargo

};

} //namespace trck

#endif /* TRUCKdescription_hh */
