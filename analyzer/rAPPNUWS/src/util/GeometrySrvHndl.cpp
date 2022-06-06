/*
 *      Author: tassiell
 */

#include <util/GeometrySrvHndl.h>
#include "generated/rAPPNUWSAnalyzer.h"
#include "generated/rAPPNUWSGlobalSteering.h"
#include "GeomService.hh"
#include "GeomHandle.hh"
//// CDCH
//#include "CDCHMaker.hh"
//#include "CDCHtracker.hh"
////#include "CellGeometryHandle_v2.hh"
////#include "CellGeometryHandle_v3.hh"
//CSMBT
#include "CSMBTMaker.hh"
#include "CSMBtracker.hh"
#include "CSMBTROGeometryHandle.hh"
//CSMTT
#include "CSMTTMaker.hh"
#include "CSMTtracker.hh"
//#include "CSMTTRadiator.hh"
#include "CSMTTROGeometryHandle.hh"

#include <iostream>

GeometrySrvHndl *GeometrySrvHndl::_instance = 0;
GeomService *GeometrySrvHndl::fGSrvc = 0;

GeometrySrvHndl *GeometrySrvHndl::Instance() {
  if (_instance==0) {
    static GeometrySrvHndl manager;
    _instance = &manager;
  }
  if (fGSrvc == 0) {
    fGSrvc = GeomService::Instance(
        gAnalyzer->GetGSP()->GetGSrvAddFile().Data(),
        gAnalyzer->GetGSP()->GetGSrvAddFold().Data());
  }
  return _instance;
}

GeometrySrvHndl *GeometrySrvHndl::Instance(std::string geomfile) {
  if (_instance==0) {
    static GeometrySrvHndl manager;
    _instance = &manager;
  }
  if (fGSrvc == 0) {
    fGSrvc = GeomService::Instance(geomfile);
  }
  return _instance;
}

GeometrySrvHndl::GeometrySrvHndl() :
//    fch(NULL),
   fCSMBT(NULL),
   fCSMTT(NULL)
{
  // TODO Auto-generated constructor stub

}

GeometrySrvHndl::~GeometrySrvHndl() {
  // TODO Auto-generated destructor stub
}

void GeometrySrvHndl::makeDetectors(){
  if (fGSrvc != 0) {
//    if (fch==NULL && fGSrvc->getConfig().getBool("hasCDCH",false)) {
//      cdch::CDCHMaker cdchtm( fGSrvc->getConfig() );
//      fGSrvc->addDetector( cdchtm.getCDCHtrackerPtr() );
//      //    fCdchtracker=cdchtm.getCDCHtrackerPtr();
//      //if (fch==NULL) {
//        GeomHandle<cdch::CDCHtracker> cdchtrkhndl;
//        fch = cdchtrkhndl->getCellGeometryHandle();
//      //}
//
//    }
    if (fCSMBT==NULL && fGSrvc->getConfig().getBool("hasCSMBT",false)){
      csmbt::CSMBTMaker csmbtm( fGSrvc->getConfig() );
      fGSrvc->addDetector( csmbtm.getCSMBTrackerPtr() );
      //if (fCSMBT==NULL) {
        GeomHandle<csmbt::CSMBtracker> csmbtrkhndl;
        fCSMBT = csmbtrkhndl->getROGeometryHandle();
      //}
    }
    if (fCSMTT==NULL && fGSrvc->getConfig().getBool("hasCSMTT",false)){
      csmtt::CSMTTMaker csmttm( fGSrvc->getConfig() );
      fGSrvc->addDetector( csmttm.getCSMTTrackerPtr() );
      //if (fCSMTT==NULL) {
        GeomHandle<csmtt::CSMTtracker> csmttrkhndl;
        fCSMTT = csmttrkhndl->getROGeometryHandle();
      //}
    }
  } else {
    std::cout<<"Warning: GeomService not yet instantiated"<<std::endl;
  }
}

//void GeometrySrvHndl::GetCDCHtracker() {
//  GeomHandle<cdch::CDCHtracker> cdchtrkhndl;
//}

//const cdch::CDCHtracker *GeometrySrvHndl::GetCDCHtracker() {
////  if (cdchtracker==NULL) {
////    GeomHandle<CDCHtracker> cdchtrkhndl;
////    cdchtracker = &(*cdchtrkhndl);
////  }
////  return cdchtracker;
//  GeomHandle<cdch::CDCHtracker> cdchtrkhndl;
//  return &(*cdchtrkhndl);
////  return fCdchtracker.get();
//}

const csmbt::CSMBtracker *GeometrySrvHndl::GetCSMBtracker() {
  GeomHandle<csmbt::CSMBtracker> csmbtrkhndl;
  return &(*csmbtrkhndl);
//  return fCdchtracker.get();
}

const csmtt::CSMTtracker *GeometrySrvHndl::GetCSMTtracker() {
  GeomHandle<csmtt::CSMTtracker> csmttrkhndl;
  return &(*csmttrkhndl);
//  return fCdchtracker.get();
}
