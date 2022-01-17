// Cyl like generic element description
//
// Original author G. Tassielli
//

#ifndef Cylkind_hh
#define Cylkind_hh

#include <deque>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "manageException.hh"
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "GENELMbsCylkindDetail.hh"
#include "GENELMbsCylkindId.hh"
//#include "GENELMbsReadOut.hh"

namespace trck {
class TRUCKMaker;
}

namespace genelmbs {

class Cylkind{

  friend class trck::TRUCKMaker;

public:

  enum CylShpType {full=0, shell, Ishape, Cshape, Ushape};

  // A free function, returning void, that takes a const ReadOut& as an argument.
//  typedef void (*ReadOutFunction)( const Cylkind& s);

  Cylkind();

  // Constructor using read out/ladder info.
  Cylkind( CylkindId& id,
         boost::shared_ptr<CylkindDetail> &detail,
         HepGeom::Transform3D *pos,
//         boost::shared_ptr<ReadOut> &ros,
         CylShpType boxShType=full
         );

  ~Cylkind ();

  const std::string getElmName() { return _elmName; }

  const CylkindId& Id() const { return _id;}

  boost::shared_ptr<CylkindDetail> getDetail() const { return _detail;}

  /*const*/ CylShpType getShapeType() const { return _shapeType; }

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
  CylkindId _id;

  // Detailed description of a ladder.
  boost::shared_ptr<CylkindDetail> _detail;
//  int _detailIndex;

  CylShpType _shapeType;

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

inline std::ostream& operator<<(std::ostream& ost, const Cylkind& cyl ){
        ost <<cyl.Id()<<" type "<<cyl.getShapeType()/*<<" width "<< cyl.getDetail()->width() << " length "<< 2.0*cyl.getDetail()->halfLength()*/ <<std::endl;
        ost<<cyl.get3DTransfrom().xx()<<" "<<cyl.get3DTransfrom().xy()<<" "<<cyl.get3DTransfrom().xz()<<" "<<cyl.get3DTransfrom().dx()<<std::endl;
        ost<<cyl.get3DTransfrom().yx()<<" "<<cyl.get3DTransfrom().yy()<<" "<<cyl.get3DTransfrom().yz()<<" "<<cyl.get3DTransfrom().dy()<<std::endl;
        ost<<cyl.get3DTransfrom().zx()<<" "<<cyl.get3DTransfrom().zy()<<" "<<cyl.get3DTransfrom().zz()<<" "<<cyl.get3DTransfrom().dz()<<std::endl;
        ost<<std::endl;
        ost<<"MidPoint "<< cyl.getMidPoint();
//        ost<<" Direction "<< ld.getDirection()<<" "<<ld.get3DTransfrom().getRotation()<<" phi "<<ld.get3DTransfrom().getRotation().phi()<<" theta "<<ld.get3DTransfrom().getRotation().theta()<<std::endl;
        return ost;
}

}  //namespace genelmbs

#endif /* Cylkind_hh */
