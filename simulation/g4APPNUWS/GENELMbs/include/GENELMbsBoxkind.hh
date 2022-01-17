// Box like generic element description
//
// Original author G. Tassielli
//

#ifndef Boxkind_hh
#define Boxkind_hh

#include <deque>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "manageException.hh"
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "GENELMbsBoxkindDetail.hh"
#include "GENELMbsBoxkindId.hh"
//#include "GENELMbsReadOut.hh"

namespace trck {
class TRUCKMaker;
}

namespace flr {
class FLOORMaker;
}

namespace genelmbs {

class Boxkind{

  friend class trck::TRUCKMaker;
  friend class flr::FLOORMaker;

public:

  enum BxShpType {full=0, shell, Ishape, Cshape};

  // A free function, returning void, that takes a const ReadOut& as an argument.
//  typedef void (*ReadOutFunction)( const Boxkind& s);

  Boxkind();

  // Constructor using read out/ladder info.
  Boxkind( BoxkindId& id,
         boost::shared_ptr<BoxkindDetail> &detail,
         HepGeom::Transform3D *pos,
//         boost::shared_ptr<ReadOut> &ros,
         BxShpType boxShType=full
         );

  ~Boxkind ();

  const std::string getElmName() { return _elmName; }

  const BoxkindId& Id() const { return _id;}

  boost::shared_ptr<BoxkindDetail> getDetail() const { return _detail;}

  /*const*/ BxShpType getShapeType() const { return _shapeType; }

  const HepGeom::Transform3D& get3DTransfrom() const {return *_pos;}

  const HepGeom::Transform3D& get3DInvTransfrom() const {return _invpos;}

  const CLHEP::Hep3Vector& getMidPoint() const {return _MidPoint;}

//  const CLHEP::Hep3Vector& getFstSdDirection() const { return _FstSdDirection; }
//  const CLHEP::Hep3Vector& getSndSdDirection() const { return _SndSdDirection; }
//
//  const CLHEP::Hep3Vector& getROChLoclMidPoint(unsigned long id) const;
//
//  int nReadOuts() const { return _nReadOuts; }
//  int nReadOutsFstSd() const { return _nReadOutsFstSd; }
//  int nReadOutsSndSd() const { return _nReadOutsSndSd; }
//  int nReadOutsFstSd2() const { return _nReadOutsFstSd2; }
//  int nSndSdDeltaRO() const { return _nSndSdDltRO; }
//  int nSndSdAddDeltaRO() const { return _nSndSdAddDltRO; }

//  double voxelizationFactor() const { return _voxelizationFactor; }

//  boost::shared_ptr<ReadOut> getReadOut() const /*throw(cet::exception)*/ {
//    return _ros;
//  }
//
//  boost::shared_ptr<ReadOut> getReadOut( const ReadOutId& id ) const {
//    return getReadOut(id.getSensor());
//  }

protected:

  // Identifier
  std::string _elmName;
  BoxkindId _id;

  // Detailed description of a ladder.
  boost::shared_ptr<BoxkindDetail> _detail;
//  int _detailIndex;

  BxShpType _shapeType;

  CLHEP::Hep3Vector _MidPoint;
//  CLHEP::Hep3Vector _FstSdDirection;
//  CLHEP::Hep3Vector _SndSdDirection;

  const HepGeom::Transform3D *_pos;
  HepGeom::Transform3D _invpos;


//  unsigned long _nReadOuts;
//  unsigned long _nReadOutsFstSd;
//  unsigned long _nReadOutsSndSd;
//  unsigned long _nReadOutsFstSd2;
//  int _nSndSdDltRO; // n. of Second Side rows increment to have one more ReadOut on the First Side
//  int _nSndSdAddDltRO; // n. of Second Side rows increment to have an additional one more ReadOut on the First Side (correction at second order)

//  double _voxelizationFactor;

  // Pointers to the ReadOut in this ladder.
//  boost::shared_ptr<ReadOut> _ros;
//  std::vector<boost::shared_ptr<ReadOut> > _ros;
//
//  void addReadOut(ReadOut *ro){
//          _ros.push_back(boost::shared_ptr<ReadOut>(ro));
//          _nReadOuts++;
//  }

  void setPosMatrix( HepGeom::Transform3D &lPos );

};

inline std::ostream& operator<<(std::ostream& ost, const Boxkind& bx ){
        ost <<bx.Id()<<" type "<<bx.getShapeType()<<" width "<< 2.0*bx.getDetail()->halfWidth() << " length "<< 2.0*bx.getDetail()->halfLength() <<std::endl;
        ost <<" Height "<<2.0*bx.getDetail()->halfHeight()<<" wallThickness "<< bx.getDetail()->wallThickness() << " centWallThickness "<< bx.getDetail()->centWallThickness() <<std::endl;
        ost<<bx.get3DTransfrom().xx()<<" "<<bx.get3DTransfrom().xy()<<" "<<bx.get3DTransfrom().xz()<<" "<<bx.get3DTransfrom().dx()<<std::endl;
        ost<<bx.get3DTransfrom().yx()<<" "<<bx.get3DTransfrom().yy()<<" "<<bx.get3DTransfrom().yz()<<" "<<bx.get3DTransfrom().dy()<<std::endl;
        ost<<bx.get3DTransfrom().zx()<<" "<<bx.get3DTransfrom().zy()<<" "<<bx.get3DTransfrom().zz()<<" "<<bx.get3DTransfrom().dz()<<std::endl;
        ost<<std::endl;
        ost<<"MidPoint "<< bx.getMidPoint();
//        ost<<" Direction "<< ld.getDirection()<<" "<<ld.get3DTransfrom().getRotation()<<" phi "<<ld.get3DTransfrom().getRotation().phi()<<" theta "<<ld.get3DTransfrom().getRotation().theta()<<std::endl;
        return ost;
}

}  //namespace genelmbs

#endif /* Boxkind_hh */
