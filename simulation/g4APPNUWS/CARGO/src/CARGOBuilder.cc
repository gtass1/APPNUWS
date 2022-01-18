//
// CARGOBuilder builder class for the CARGO in geant4
//
// Original author G. Tassielli
//

#include "CARGOBuilder.hh"
#include "CARGOdescription.hh"
#include "CARGOtargetSD.hh"

#include "findMaterialOrThrow.hh"
#include "GeomHandle.hh"
#include "GeomService.hh"
#include "SensitiveDetectorName.hh"

#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
//#include "G4ExtrudedSolid.hh"
//#include "G4VisAttributes.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UserLimits.hh"
//#include "G4TwoVector.hh"

//using namespace std;

namespace crg {

bool checkOverlap, detailedCheck;

void CARGOBuilder::construct( VolumeInfo cargoInfo ){

  // Master geometry for the cargo.
  GeomHandle<CARGOdescription> cargodesc;
  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  checkOverlap = config.getBool("g4.doSurfaceCheck",false);
  detailedCheck = checkOverlap&&config.getBool("cargo.doDetailedSurfCheck",false);

  if (cargodesc->isExternal()) {
//                throw cet::exception("GEOM") <<"This GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CARGO: This GDML file option is temporarily disabled\n";
    e.error();
  } else {

//    G4VisAttributes* visAttTrlWall = new G4VisAttributes(true, G4Colour::Red() );
//    visAttTrlWall->SetForceSolid(true);
//    visAttTrlWall->SetForceAuxEdgeVisible (false);
//    visAttTrlWall->SetVisibility(true);
//    visAttTrlWall->SetDaughtersInvisible(false);

    G4VSensitiveDetector *sd = G4SDManager::GetSDMpointer()->FindSensitiveDetector(SensitiveDetectorName::CARGOtargetRO());

    bool debugShape =  config.getBool("cargo.debugShape",false);

    char shape[50], vol[50];

    if (cargodesc->geomType()==0) {

      for (int iBx = 0; iBx < cargodesc->nBoxKindElem(); ++iBx){

        std::cout<<"Building box element n.: "<<iBx<<std::endl;

        genelmbs::Boxkind *ibx = cargodesc->getCrgBoxKindElem(iBx);
        if ( debugShape ) {
          std::cout<<"CARGO Box type element: "<<ibx->Id()<<std::endl;
        }
        VolumeInfo BoxInfo;
        sprintf(shape,"%s-%d",ibx->getElmName().c_str(),iBx);
        sprintf(vol,"%svol-%03d",ibx->getElmName().c_str(),iBx);

        BoxInfo.solid = buildBoxElem(ibx,shape);
        G4Material* matBox = gmsrv::findMaterialOrThrow( ibx->getDetail()->materialName().c_str() );
        BoxInfo.logical = new G4LogicalVolume(BoxInfo.solid,matBox,vol,0,0,0);
        //            if (ibx->voxelizationFactor()==0.0) {
        //                    BoxInfo.logical->SetOptimisation(false);
        //            }
        //            else{
        //                            BoxInfo.logical->SetSmartless( 1.0/((float)(ibx->nPhiSectors() * ibx->nLaddersPerSector())) );
        //            }
        if (cargodesc->checkBoxKindElemIsTarget(iBx)) {
          BoxInfo.logical->SetSensitiveDetector(sd);
        }

        BoxInfo.physical = new G4PVPlacement(ibx->get3DTransfrom(),
            BoxInfo.logical,       // its logical volume
            vol,                     // its name
            cargoInfo.logical,        // its mother  volume
            false,                   // no boolean operations
            0,                       // copy number
            checkOverlap);

      } // Box Loop

      for (int iCy = 0; iCy < cargodesc->nCylKindElem(); ++iCy){

        genelmbs::Cylkind *icy = cargodesc->getCrgCylKindElem(iCy);
        if ( debugShape ) {
          std::cout<<"TRUCK Cyl type element: "<<icy->Id()<<std::endl;
        }
        sprintf(shape,"%s-%d",icy->getElmName().c_str(),cargodesc->nBoxKindElem()+iCy);
        sprintf(vol,"%svol-%03d",icy->getElmName().c_str(),cargodesc->nBoxKindElem()+iCy);

        VolumeInfo CylInfo = buildCylElem(icy,iCy);
        //            if (icy->voxelizationFactor()==0.0) {
        //                    CylInfo.logical->SetOptimisation(false);
        //            }
        //            else{
        //                            CylInfo.logical->SetSmartless( 1.0/((float)(icy->nPhiSectors() * icy->nLaddersPerSector())) );
        //            }
//        CylInfo.logical->SetVisAttributes(visAttCyl);
        if (cargodesc->checkCylKindElemIsTarget(iCy)) {
          CylInfo.logical->SetSensitiveDetector(sd);
        }

        CylInfo.physical = new G4PVPlacement(icy->get3DTransfrom(),
            CylInfo.logical,       // its logical volume
            vol,                     // its name
            cargoInfo.logical,     // its mother  volume
            false,                   // no boolean operations
            0,                       // copy number
            checkOverlap);

      } // Cyl Loop

    } // geom 00

    if ( checkOverlap ) { std::cout<<"CARGO Overlap Checking "<<cargoInfo.physical->CheckOverlaps(100000,0.0001,true)<<std::endl; }

  }

}

G4VSolid* CARGOBuilder::buildBoxElem(genelmbs::Boxkind *ibx, char shape[]){
  G4VSolid* tmpBox=0x0;
  if (ibx->getShapeType()==genelmbs::Boxkind::full) {

    tmpBox = new G4Box(shape, ibx->getDetail()->halfWidth(),ibx->getDetail()->halfHeight(),ibx->getDetail()->halfLength());

  } else if (ibx->getShapeType()==genelmbs::Boxkind::shell) {

    char shape1[100], shape2[100];
    sprintf(shape1,"%s-1",shape);
    sprintf(shape2,"%s-2",shape);
    G4Box *tmpBoxExt = new G4Box(shape1, ibx->getDetail()->halfWidth(),ibx->getDetail()->halfHeight(),ibx->getDetail()->halfLength());
    G4Box *tmpBoxIn = new G4Box(shape2, ibx->getDetail()->halfWidth()-ibx->getDetail()->wallThickness(),ibx->getDetail()->halfHeight()-ibx->getDetail()->wallThickness(),ibx->getDetail()->halfLength()-ibx->getDetail()->wallThickness());
    tmpBox = new G4SubtractionSolid(shape, tmpBoxExt, tmpBoxIn);

  } else if (ibx->getShapeType()==genelmbs::Boxkind::Ishape) {

    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"I shape type for Box is not used for Cargo";
    e.error();

  } else if (ibx->getShapeType()==genelmbs::Boxkind::Cshape) {

    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"C shape type for Box is not used for Cargo";
    e.error();

  }
  return tmpBox;
}

VolumeInfo CARGOBuilder::buildCylElem(genelmbs::Cylkind *icy, int iCy){

    VolumeInfo CylInfo;
    char shapeName[60],volName[60];

    sprintf(shapeName,"%s-%d",icy->getElmName().c_str(),iCy);
    sprintf(volName,"%svol-%03d",icy->getElmName().c_str(),iCy);

    CylInfo.solid = new G4Tubs(shapeName, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius(),icy->getDetail()->halfLength(),0.0,CLHEP::twopi);

    if (icy->getShapeType()==genelmbs::Cylkind::full) {

      G4Material* matCyl = gmsrv::findMaterialOrThrow( icy->getDetail()->materialName().c_str() );
      CylInfo.logical = new G4LogicalVolume(CylInfo.solid,matCyl,volName,0,0,0);

    } else if (icy->getShapeType()==genelmbs::Cylkind::shell) {

      G4Material* matMot = gmsrv::findMaterialOrThrow( "G4_AIR" );
      CylInfo.logical = new G4LogicalVolume(CylInfo.solid,matMot,volName,0,0,0);

      G4Material* matCyl = gmsrv::findMaterialOrThrow( icy->getDetail()->materialName().c_str() );

      char shape1Name[100], shape2Name[100], shapeSubName[100], subVolName[100];
      sprintf(shape1Name,"%s-1",shapeName);
      sprintf(shape2Name,"%s-2",shapeName);
      sprintf(shapeSubName,"%s_sub",shapeName);
      sprintf(subVolName,"%s_sub",volName);
      G4Tubs *tmpCylExt = new G4Tubs(shape1Name, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius(),icy->getDetail()->halfLength(),0.0,CLHEP::twopi);
      G4Tubs *tmpCylIn = new G4Tubs(shape2Name, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->halfLength()-icy->getDetail()->wallThickness(),0.0,CLHEP::twopi);
      G4VSolid *tmpCyl = new G4SubtractionSolid(shapeSubName, tmpCylExt, tmpCylIn);
      G4LogicalVolume *tlogicSubSh = new G4LogicalVolume(tmpCyl,matCyl,volName,0,0,0);

      G4VPhysicalVolume *tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,0),
          tlogicSubSh,         // its logical volume
          subVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling


    } else if (icy->getShapeType()==genelmbs::Cylkind::Ishape) {

      exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
      e<<"I shape type for Cylinder is not used for Cargo";
      e.error();

    } else if (icy->getShapeType()==genelmbs::Cylkind::Cshape) {

      exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
      e<<"C shape type for Cylinder is not used for Cargo";
      e.error();

    } else if (icy->getShapeType()==genelmbs::Cylkind::Ushape) {


      exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
      e<<"U shape type for Cylinder is not used for Cargo";
      e.error();

    }

    return CylInfo;
}

void CARGOBuilder::instantiateSensitiveDetectors( const std::string hitsCollectionName){

  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  G4SDManager* SDman      = G4SDManager::GetSDMpointer();

  // G4 takes ownership and will delete the detectors at the job end

//  GeomHandle<CARGOdescription> crgdesc;
  G4ThreeVector crgPos(0.,0.,0.);//CLHEP::mm * crgdesc->z0()/*-zOff*/);

  CARGOtargetSD* crgdescSD=0x0;
  crgdescSD = new CARGOtargetSD(SensitiveDetectorName::CARGOtargetRO(), hitsCollectionName,  config);
  crgdescSD->setCARGOCenterInDetSys(crgPos);
  SDman->AddNewDetector(crgdescSD);

}


} // end namespace crg
