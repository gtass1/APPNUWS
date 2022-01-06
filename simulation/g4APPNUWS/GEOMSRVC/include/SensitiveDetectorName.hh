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
  enum SDtype { DCHtracker=0, DCHSWires,  DCHFWires, DCHWalls, MPGDCSMTtracker, MPGDCSMBtracker };

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

  static char const * MPGDCSMTTrackerRO(){
    return name[MPGDCSMTtracker];
  }

  static char const * MPGDCSMBTrackerRO(){
    return name[MPGDCSMBtracker];
  }

private:
  static const char* name[6];

};


//} // namespace gmsrv

#endif /* SENSITIVEDETECTORNAME_HH_ */
