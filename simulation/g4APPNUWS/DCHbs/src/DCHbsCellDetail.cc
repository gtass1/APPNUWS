// DCH base cells description
//
// Original author G. Tassielli
//

#include "DCHbsCellDetail.hh"

using namespace std;

namespace dchbs {

  CellDetail::CellDetail( double circumscribedRadius, double inscribedCircleRadius, boost::shared_ptr<WireDetail> senseWire
                           ):
                           _circumscribedRadius(circumscribedRadius),
                           _inscribedCircleRadius(inscribedCircleRadius),
                           _senseWire(senseWire)
  {
  }

  CellDetail::CellDetail( double inscribedCircleRadius, boost::shared_ptr<WireDetail> senseWire
		                  , boost::shared_ptr<CellWallDetail> wall
                           ):
                           _inscribedCircleRadius(inscribedCircleRadius),
                           _senseWire(senseWire),
						   _wall(wall)
  {
      _circumscribedRadius=inscribedCircleRadius+wall->totThickness();
  }

  CellDetail::~CellDetail (){
//                try {
//                        delete *_senseWire; *_senseWire=nullptr;
//                } catch (cet::exception e) {
//                    throw cet::exception("GEOM")
//                        << "Error during deleting cell wire detail data \n";
//                }

  }

} // namespace dchbs
