//
// Original author G. Tassielli
//

#include "PXSTbsLayerId.hh"

namespace pxstbs {

  LayerId::LayerId():
    _id(-1){
  }

  LayerId::LayerId( int id ):
    _id(id)
  {
  }

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  int LayerId::getLayer() const{
    return _id;
  }

  bool LayerId::operator==(const LayerId s) const{
    return ( _id == s._id );
  }

//  std::ostream& operator<<(std::ostream& ost,
//                                  const LayerId& s ){
//    ost << "Layer Id: "<<s._id << " ";
//    return ost;
//  }

} //namespace pxstbs
