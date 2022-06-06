/*
 *      Author: tassiell
 */

#ifndef GEOMETRYSRVHNDL_H
#define GEOMETRYSRVHNDL_H
#include <string>

class GeomService;
//namespace cdch {
//class CDCHtracker;
//class CellGeometryHandle;
//}
namespace pxstbs {
class ROGeometryHandle;
}
namespace csmbt {
class CSMBtracker;
}
namespace csmtt {
class CSMTtracker;
}

class GeometrySrvHndl {
public:
  static GeometrySrvHndl* Instance();
  static GeometrySrvHndl* Instance(std::string geomfile);
  virtual ~GeometrySrvHndl();
  void makeDetectors();

  GeomService *GetGeomService() {return fGSrvc;}
//  const cdch::CDCHtracker *GetCDCHtracker();
//  cdch::CellGeometryHandle *GetCellHandle() {return fch;}
//  void GetCDCHtracker();
  const csmbt::CSMBtracker *GetCSMBtracker();
  pxstbs::ROGeometryHandle *GetCSMBTROChanHandle() {return fCSMBT;}
  const csmtt::CSMTtracker *GetCSMTtracker();
  pxstbs::ROGeometryHandle *GetCSMTTROChanHandle() {return fCSMTT;}

private:
  GeometrySrvHndl();

  static GeometrySrvHndl *_instance;
  static GeomService *fGSrvc;
  //CDCHtracker *cdchtracker;
//  std::unique_ptr<CDCHtracker> fCdchtracker;

//  cdch::CellGeometryHandle *fch;
  pxstbs::ROGeometryHandle *fCSMBT;
  pxstbs::ROGeometryHandle *fCSMTT;
};

#endif /* GEOMETRYSRVHNDL_H */
