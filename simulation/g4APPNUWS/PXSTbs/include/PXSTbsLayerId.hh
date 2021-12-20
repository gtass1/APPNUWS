//
// Original author G. Tassielli
//

#ifndef PXSTbs_LayerId_hh
#define PXSTbs_LayerId_hh

#include <iostream>

namespace pxstbs {

//class LayerId;
//  std::ostream& operator<<(std::ostream& ost,
//                                  const LayerId& s );

class LayerId{

public:

  LayerId();

  LayerId( int id );

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  int getLayer() const;

  bool operator==(const LayerId s) const;

//  friend std::ostream& operator<<(std::ostream& ost,
//                                  const LayerId& s );

private:

  int _id;

};

inline std::ostream& operator<<(std::ostream& ost,
                                const LayerId& s ){
  ost << "Layer Id: "<<s.getLayer() << " ";
  return ost;
}

} //namespace pxstbs

#endif /* PXSTbs_LayerId_hh */
