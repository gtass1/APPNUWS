// CDCH wire description
//
// Original author G. Tassielli
//

#ifndef Wire_BS_hh
#define Wire_BS_hh

#include <deque>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "DCHbsWireDetail.hh"
#include "DCHbsWireId.hh"

namespace dchbs {

class Wire{

//        friend class CDCHtracker;
//        friend class CDCHMaker;


public:

        // A free function, returning void, that takes a const Wire& as an argument.
        typedef void (*WireFunction)( const Wire& s);

        enum Wtype {undefined=-1, field, sense};

        Wire();

        // Constructor using wire 3d transformation.
        Wire( boost::shared_ptr<WireId> id,
                        boost::shared_ptr<WireDetail> detail,
                        HepGeom::Transform3D *pos,
//                        double epsilon,
//                        double alpha,
                        Wtype wireType=undefined
        );

        virtual ~Wire ();

        virtual WireId& Id() const { return *(_id.get());}

        boost::shared_ptr<WireDetail> getDetail() const { return _detail;}

        const HepGeom::Transform3D& get3DTransfrom() const {return *_pos;}

        const HepGeom::Transform3D& get3DInvTransfrom() const {return _invpos;}

        const CLHEP::Hep3Vector& getMidPoint() const {return _c;}

        const CLHEP::Hep3Vector& getDirection() const { return _w;}

//        double getEpsilon() const { return _epsilon;}
//        double getAlpha() const { return _alpha;}

        Wtype getWireType() const { return _wireType; }

protected:

        // Identifier
        boost::shared_ptr<WireId> _id;

        Wtype _wireType;

        // Mid-point of the wire.
        CLHEP::Hep3Vector _c;

        // Detailed description of a wire.
        boost::shared_ptr<WireDetail> _detail;

        // Unit vector along the wire direction.
        // Need to add unit vectors along local u and v also.
        // Use Euler angle convention from G4.
        CLHEP::Hep3Vector _w;

        const HepGeom::Transform3D *_pos;
        HepGeom::Transform3D _invpos;
//        const double _epsilon;
//        const double _alpha;

};

inline std::ostream& operator<<(std::ostream& ost, const Wire& w ){
        ost <<w.Id()<<" type "<<w.getWireType()<<" radius "<< w.getDetail()->outerRadius() << " length "<< w.getDetail()->length() <<std::endl;
//        ost<<"epsilon "<< w.getEpsilon();
        ost<<" pos matrix: "<<std::endl;
        ost<<w.get3DTransfrom().xx()<<" "<<w.get3DTransfrom().xy()<<" "<<w.get3DTransfrom().xz()<<" "<<w.get3DTransfrom().dx()<<std::endl;
        ost<<w.get3DTransfrom().yx()<<" "<<w.get3DTransfrom().yy()<<" "<<w.get3DTransfrom().yz()<<" "<<w.get3DTransfrom().dy()<<std::endl;
        ost<<w.get3DTransfrom().zx()<<" "<<w.get3DTransfrom().zy()<<" "<<w.get3DTransfrom().zz()<<" "<<w.get3DTransfrom().dz()<<std::endl;
        return ost;
}

}  //namespace dchbs

#endif /* Wire_BS_hh */
