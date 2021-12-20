/*
 * SensitiveDetectorName.hh
 *
 *      Author: tassiell
 */

#ifndef SENSITIVEDETECTORNAME_HH_
#define SENSITIVEDETECTORNAME_HH_

//namespace gmsrv {

class SensitiveDetectorName {

public:
  enum SDtype { DCHtracker=0, DCHSWires,  DCHFWires, DCHWalls, CSMtracker };

  static char const * TrackerGas(){
    return name[DCHtracker];
  }

  static char const * TrackerSWires(){
    return name[DCHSWires];
  }

  static char const * TrackerFWires(){
    return name[DCHFWires];
  }

  static char const * TrackerWalls(){
    return name[DCHWalls];
  }

  static char const * CSMTrackerRO(){
    return name[CSMtracker];
  }

private:
  static const char* name[5];

};


//} // namespace gmsrv

#endif /* SENSITIVEDETECTORNAME_HH_ */
