//
//
// Original author G. Tassielli
//

#include "DCHbsFoilId.hh"
#include "DCHbsLayerId.hh"

namespace dchbs {

  FoilId::FoilId():
    _lid(nullptr),
    _n(-1){
  }

  FoilId::FoilId( boost::shared_ptr<LayerId> layer,
           int n
           ):
//    _lid(layer),
    _n(n)
  {
	  _lid=layer;
  }

  FoilId::FoilId( LayerId *layer,
           int n
           ):
//    _lid(layer),
    _n(n)
  {
	  _lid.reset(layer);
  }

  FoilId::~FoilId(){}

  // use compiler-generated copy c'tor, copy assignment, and d'tor

//  LayerId WireId::getLayerId() const {
//    return *(_lid.get());
//  }

  int FoilId::getLayer() const{
    return _lid->getLayer();
  }

  int FoilId::getFoil() const{
    return _n;
  }

  int FoilId::getUId() const{
//	int wUid = ((_lid->getSuperLayer()+1)*100000)+((_lid->getLayer()+1)*1000);
//	wUid*=10;
	int fUid = ((_lid->getLayer()+1)*100);
//	fUid*=10;
	fUid+=_n;
    return fUid;
  }

  bool FoilId::operator==(const FoilId f) const{
    return ( *_lid == *(f._lid) && _n == f._n );
  }

  std::ostream& operator<<(std::ostream& ost,
                                  const FoilId& f ){
    ost << "Foil Id: ("
        << f.getLayerId() << " "
        << f._n
        << " )";
    return ost;
  }

}
