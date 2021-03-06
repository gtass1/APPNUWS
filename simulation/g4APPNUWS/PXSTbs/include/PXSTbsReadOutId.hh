//
// Original author G. Tassielli
//

#ifndef ReadOutId_hh
#define ReadOutId_hh

#include <iostream>

#include "PXSTbsLadderId.hh"

namespace pxstbs {

//class LadderId;
//class ReadOutId;
//inline std::ostream& operator<<(std::ostream& ost,
//                                const ReadOutId& c );

class ReadOutId{

  friend class LadderId;

public:

  ReadOutId();

//  SensorId( SensorId &sid):
//    _sid(sid._sid)
//  {
//  }

  ReadOutId( LadderId *ladder, unsigned long n );

  // use compiler-generated copy c'tor, copy assignment, and d'tor

  const LadderId& getLadderId() const;

  int getLadder() const;

  unsigned long getSensor() const;

  bool operator==(const ReadOutId &c) const;

//  friend std::ostream& operator<<(std::ostream& ost,
//                                  const ReadOutId& c );

private:

  LadderId *_ldid;
  unsigned long _sid;

};

inline std::ostream& operator<<(std::ostream& ost,
                                const ReadOutId& c ){
  ost << "Sensor Id: ("
      << c.getLadderId() << " "
      << c.getSensor()
      << " )";
  return ost;
}

}
#endif /* ReadOutId_hh */
