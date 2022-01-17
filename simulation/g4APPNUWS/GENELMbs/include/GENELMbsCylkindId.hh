//
// Original author G. Tassielli
//

#ifndef CylkindId_hh
#define CylkindId_hh

#include <iostream>

namespace genelmbs {

//class CylkindId;
//
//std::ostream& operator<<(std::ostream& ost,
//                                const CylkindId& b );

class CylkindId{

public:

  CylkindId();

  CylkindId( long cylid );

  // Use compiler-generated copy c'tor, copy assignment, and d'tor

  long getCylkind() const;

  bool operator==(const CylkindId& b) const;

//  friend std::ostream& operator<<(std::ostream& ost,
//                                  const CylkindId& b );

private:

  long _cylid;

};

inline std::ostream& operator<<(std::ostream& ost,
		const CylkindId& c ){
	ost << "Cylkind Id: ("
			<< c.getCylkind()
			<< " )";
	return ost;
}

}
#endif /* CylkindId_hh */
