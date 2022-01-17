//
// Original author G. Tassielli
//

#include "GENELMbsBoxkindId.hh"

namespace genelmbs {

  BoxkindId::BoxkindId():
    _boxid(-1)
  {
  }

  BoxkindId::BoxkindId( long bxid ):
    _boxid(bxid)
  {
  }

  // Use compiler-generated copy c'tor, copy assignment, and d'tor

  long BoxkindId::getBoxkind() const{
    return _boxid;
  }

  bool BoxkindId::operator==(const BoxkindId& b) const{
    return ( _boxid == b._boxid );
  }

//  std::ostream& operator<<(std::ostream& ost,
//                                  const BoxkindId& b ){
//    ost << "Boxkind Id: ("
//        << b._bxid
//        << " )";
//    return ost;
//  }

}
