//
// Original author G. Tassielli
//

#ifndef BoxkindId_hh
#define BoxkindId_hh

#include <iostream>

namespace genelmbs {

//class BoxkindId;
//
//std::ostream& operator<<(std::ostream& ost,
//                                const BoxkindId& b );

class BoxkindId{

public:

  BoxkindId();

  BoxkindId( long bxid );

  // Use compiler-generated copy c'tor, copy assignment, and d'tor

  long getBoxkind() const;

  bool operator==(const BoxkindId& b) const;

//  friend std::ostream& operator<<(std::ostream& ost,
//                                  const BoxkindId& b );

private:

  long _boxid;

};

inline std::ostream& operator<<(std::ostream& ost,
		const BoxkindId& b ){
	ost << "Boxkind Id: ("
			<< b.getBoxkind()
			<< " )";
	return ost;
}

}
#endif /* BoxkindId_hh */
