//
// Original author G. Tassielli
//

#ifndef WireId_BS_hh
#define WireId_BS_hh

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "DCHbsLayerId.hh"

namespace dchbs {

//class LayerId;
//class WireId;
//
//std::ostream& operator<<(std::ostream& ost,
//                                const WireId& w );

class WireId{

//        friend class LayerId;

public:

  WireId();

  WireId( boost::shared_ptr<LayerId> layer, int n );

  WireId( LayerId *layer, int n );

  virtual ~WireId();

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  virtual LayerId& getLayerId() const { return *(_lid.get()); }

  virtual int getLayer() const;

  virtual int getWire() const;

  virtual int getUId() const;

  bool operator==(const WireId w) const;

  friend std::ostream& operator<<(std::ostream& ost,
                                  const WireId& w );

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
#endif /* WireId_BS_hh */
