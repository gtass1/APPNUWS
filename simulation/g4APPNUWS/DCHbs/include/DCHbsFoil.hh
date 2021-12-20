// DCH Foil description
//
// Original author G. Tassielli
//

#ifndef Foil_BS_hh
#define Foil_BS_hh

#include <deque>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "DCHbsFoilDetail.hh"
#include "DCHbsFoilId.hh"

namespace dchbs {

class Foil{

//        friend class CDCHtracker;
//        friend class CDCHMaker;


public:

        // A free function, returning void, that takes a const Foil& as an argument.
        typedef void (*FoilFunction)( const Foil& s);

//        enum Ftype {undefined=-1, field, sense};

        Foil();

        // Constructor using foil 3d transformation.
        Foil( boost::shared_ptr<FoilId> id,
                        boost::shared_ptr<FoilDetail> detail,
                        HepGeom::Transform3D *pos//,
//                        double epsilon,
//                        double alpha,
//                        Ftype foilType=undefined
        );

        virtual ~Foil ();

        virtual FoilId& Id() const { return *(_id.get());}

        boost::shared_ptr<FoilDetail> getDetail() const { return _detail;}

        const HepGeom::Transform3D& get3DTransfrom() const {return *_pos;}

        const HepGeom::Transform3D& get3DInvTransfrom() const {return _invpos;}

        const CLHEP::Hep3Vector& getMidPoint() const {return _c;}

        const CLHEP::Hep3Vector& getDirection() const { return _f;}

//        double getEpsilon() const { return _epsilon;}
//        double getAlpha() const { return _alpha;}

//        Ftype getFoilType() const { return _foilType; }

protected:

        // Identifier
        boost::shared_ptr<FoilId> _id;

//        Ftype _foilType;

        // Mid-point of the foil.
        CLHEP::Hep3Vector _c;

        // Detailed description of a foil.
        boost::shared_ptr<FoilDetail> _detail;

        // Unit vector along the foil direction.
        // Need to add unit vectors along local u and v also.
        // Use Euler angle convention from G4.
        CLHEP::Hep3Vector _f;

        const HepGeom::Transform3D *_pos;
        HepGeom::Transform3D _invpos;
//        const double _epsilon;
//        const double _alpha;

};

inline std::ostream& operator<<(std::ostream& ost, const Foil& f ){
        ost <<f.Id()/*<<" type "<<f.getFoilType()*/<<" totalThickness "<< f.getDetail()->totThickness() << " length "<< f.getDetail()->length() <<std::endl;
//        ost<<"epsilon "<< f.getEpsilon();
        ost<<" pos matrix: "<<std::endl;
        ost<<f.get3DTransfrom().xx()<<" "<<f.get3DTransfrom().xy()<<" "<<f.get3DTransfrom().xz()<<" "<<f.get3DTransfrom().dx()<<std::endl;
        ost<<f.get3DTransfrom().yx()<<" "<<f.get3DTransfrom().yy()<<" "<<f.get3DTransfrom().yz()<<" "<<f.get3DTransfrom().dy()<<std::endl;
        ost<<f.get3DTransfrom().zx()<<" "<<f.get3DTransfrom().zy()<<" "<<f.get3DTransfrom().zz()<<" "<<f.get3DTransfrom().dz()<<std::endl;
        return ost;
}

}  //namespace dchbs

#endif /* Foil_BS_hh */
