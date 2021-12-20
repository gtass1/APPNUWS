//
//
// Original author G. Tassielli
//

#include "DCHbsLayerId.hh"
#include "DCHbsWireId.hh"

namespace dchbs {

  WireId::WireId():
    _lid(nullptr),
    _n(-1){
  }

  WireId::WireId( boost::shared_ptr<LayerId> layer,
           int n
           ):
//    _lid(layer),
    _n(n)
  {
	  _lid=layer;
  }

  WireId::WireId( LayerId *layer,
           int n
           ):
//    _lid(layer),
    _n(n)
  {
	  _lid.reset(layer);
  }

  WireId::~WireId(){}

  // use compiler-generated copy c'tor, copy assignment, and d'tor

//  LayerId WireId::getLayerId() const {
//    return *(_lid.get());
//  }

  int WireId::getLayer() const{
    return _lid->getLayer();
  }

  int WireId::getWire() const{
    return _n;
  }

  int WireId::getUId() const{
//	int wUid = ((_lid->getSuperLayer()+1)*100000)+((_lid->getLayer()+1)*1000);
//	wUid*=10;
	int wUid = ((_lid->getLayer()+1)*1000);
	wUid*=10;
	wUid+=_n;
    return wUid;
  }

  bool WireId::operator==(const WireId w) const{
    return ( *_lid == *(w._lid) && _n == w._n );
  }

  std::ostream& operator<<(std::ostream& ost,
                                  const WireId& w ){
    ost << "Wire Id: ("
        << w.getLayerId() << " "
        << w._n
        << " )";
    return ost;
  }

}
