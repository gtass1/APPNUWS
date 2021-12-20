//
// Original author G. Tassielli
//

#include "DCHbsCellId.hh"
#include "DCHbsWireId.hh"

namespace dchbs {

  CellId::CellId():
    _swid(nullptr)
  {
  }

  CellId::CellId( boost::shared_ptr<WireId> swid)
  {
	_swid=swid;
  }


  CellId::CellId( WireId *swid)
//    _swid(&swid)
  {
	  _swid.reset(swid);
  }

//  CellId::CellId( LayerId *layer,
//          int n
//        ):
//    _swid(new WireId(layer,n))
//  {
//  }

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  const LayerId& CellId::getLayerId() const {
    return _swid->getLayerId();
  }

  int CellId::getWireN() const{
	return _swid->getWire();
  }

  int CellId::getLayer() const{
    return _swid->getLayer();
  }

  int CellId::getCell() const{
    return _swid->getWire();
  }

  bool CellId::operator==(const CellId c) const{
    return ( (*_swid) == (*(c._swid)) );
  }

//  std::ostream& operator<<(std::ostream& ost,
//                                  const CellId& c ){
//    ost << "Cell Id: ("
//        << c.getLayerId() << " "
//        << c.getWireN()
//        << " )";
//    return ost;
//  }

}
