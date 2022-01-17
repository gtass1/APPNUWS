//
// Original author G. Tassielli
//

#include "GENELMbsCylkindId.hh"

namespace genelmbs {

  CylkindId::CylkindId():
    _cylid(-1)
  {
  }

  CylkindId::CylkindId( long cylid ):
    _cylid(cylid)
  {
  }

  // Use compiler-generated copy c'tor, copy assignment, and d'tor

  long CylkindId::getCylkind() const{
    return _cylid;
  }

  bool CylkindId::operator==(const CylkindId& c) const{
    return ( _cylid == c._cylid );
  }

//  std::ostream& operator<<(std::ostream& ost,
//                                  const CylkindId& c ){
//    ost << "Cylkind Id: ("s
//        << c._cylid
//        << " )";
//    return ost;
//  }

}
