//
// Original author G. Tassielli
//

#ifndef FoilId_BS_hh
#define FoilId_BS_hh

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "DCHbsLayerId.hh"

namespace dchbs {

//class LayerId;
//class WireId;
//
//std::ostream& operator<<(std::ostream& ost,
//                                const WireId& w );

class FoilId{

//        friend class LayerId;

public:

  FoilId();

  FoilId( boost::shared_ptr<LayerId> layer, int n );

  FoilId( LayerId *layer, int n );

  virtual ~FoilId();

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  virtual LayerId& getLayerId() const { return *(_lid.get()); }

  virtual int getLayer() const;

  virtual int getFoil() const;

  virtual int getUId() const;

  bool operator==(const FoilId f) const;

  friend std::ostream& operator<<(std::ostream& ost,
                                  const FoilId& f );

protected:

  boost::shared_ptr<LayerId> _lid;
  int _n;

};

//class LayerId;
//class WireId;
//
//std::ostream& operator<<(std::ostream& ost,
//                                const WireId& w );
//
//
//inline std::ostream& operator<<(std::ostream& ost,
//                                const WireId& w ){
//  ost << "Wire Id: ("
//      << w.getLayerId() << " "
//      << w.getWire()
//      << " )";
//  return ost;
//}

}
#endif /* FoilId_BS_hh */
