// DCH foil description
//
// Original author G. Tassielli
//

#include "DCHbsFoil.hh"
#include "CLHEP/Geometry/Vector3D.h"

#ifndef __CINT__

namespace dchbs {

Foil::Foil():
  _id(new FoilId()),
//  _foilType(undefined),
  _c(CLHEP::Hep3Vector(0.,0.,0.)),
  _f(CLHEP::Hep3Vector(0.,0.,1.)),
  _pos(0x0),
  _invpos(HepGeom::Transform3D())
//,
//  _epsilon(0.0),
//  _alpha(0.0)
{
}


Foil::Foil( boost::shared_ptr<FoilId> id,
                boost::shared_ptr<FoilDetail> detail,
                HepGeom::Transform3D *pos//,
//                double epsilon,
//                double alpha,
//                Ftype foilType
            ):
//  _id(id)
//  _foilType(foilType)
//  ,
  _pos(pos)
//  ,_epsilon(epsilon)
//  ,_alpha(alpha)
{
  _id=id;
  _detail=detail;
  _c.set(_pos->dx(), _pos->dy(), _pos->dz());
  _invpos=_pos->inverse();
  _f.set(_pos->xz(),_pos->yz(),_pos->zz());

}

Foil::~Foil (){
//        try {
//                delete *_detail; *_detail=nullptr;
//                delete _pos;
//        } catch (cet::exception e) {
//            throw cet::exception("GEOM")
//                << "Error during deleting foil data \n";
//        }

  delete _pos;
}

} // namespace dchbs

#endif
