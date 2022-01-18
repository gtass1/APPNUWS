//
// TRUCKBuilder builder class for the TRUCK in geant4
//
// Original author G. Tassielli
//

#include "TRUCKBuilder.hh"
#include "TRUCKdescription.hh"

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
#include "G4ExtrudedSolid.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UserLimits.hh"
#include "G4TwoVector.hh"

//using namespace std;

namespace trck {

bool checkOverlap, detailedCheck;
std::string trckName("TRUCKMother");
VolumeInfo CntInInfo;

VolumeInfo TRUCKBuilder::construct( G4LogicalVolume* mother/*, double zOff*/ ){

  // Master geometry for the tracker.
  GeomHandle<TRUCKdescription> trckdesc;
  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  VolumeInfo trckInfo;

  double x0    = CLHEP::mm * trckdesc->x0();
  double y0    = CLHEP::mm * trckdesc->y0();
  double z0    = CLHEP::mm * trckdesc->z0();
  G4ThreeVector trackerOffset(x0,y0,z0/*-zOff*/);

  checkOverlap = config.getBool("g4.doSurfaceCheck",false);
  detailedCheck = checkOverlap&&config.getBool("trck.doDetailedSurfCheck",false);

  if (trckdesc->isExternal()) {
//                throw cet::exception("GEOM") <<"This GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"TRUCK: This GDML file option is temporarily disabled\n";
    e.error();
  } else {

    G4VisAttributes* visAtt = new G4VisAttributes(true, G4Colour::White() );
    visAtt->SetForceSolid(true);
    visAtt->SetForceAuxEdgeVisible (false);
    visAtt->SetVisibility(false);
    visAtt->SetDaughtersInvisible(false);

    G4Material* matMother = gmsrv::findMaterialOrThrow( config.getString("trck.motherVolMat","G4_AIR") );

    trckInfo.solid = new G4Box("TRUCKdescription", trckdesc->halfWidth()+0.001,trckdesc->halfHeight()+0.001,trckdesc->zHalfLength()+0.001);

    trckInfo.logical = new G4LogicalVolume(trckInfo.solid , matMother, trckName,0,0,0);
    trckInfo.logical->SetVisAttributes(visAtt);

    G4VisAttributes* visAttTrlWall = new G4VisAttributes(true, G4Colour::Red() );
    visAttTrlWall->SetForceSolid(true);
    visAttTrlWall->SetForceAuxEdgeVisible (false);
    visAttTrlWall->SetVisibility(true);
    visAttTrlWall->SetDaughtersInvisible(false);

    bool debugShape =  config.getBool("trck.debugShape",false);

    char shape[50], vol[50];

    G4LogicalVolume*   trlPlanelogical=0x0;
    G4LogicalVolume*   tMotherlogical=0x0;

    if (trckdesc->geomType()==0) {

      genelmbs::Boxkind *cntin = trckdesc->getContainerElem();
//      VolumeInfo CntInInfo;
      sprintf(shape,"%s",cntin->getElmName().c_str());
      sprintf(vol,"%svol",cntin->getElmName().c_str());

      CntInInfo.solid = buildBoxElem(cntin,shape);
      G4Material* matCntIn = gmsrv::findMaterialOrThrow( cntin->getDetail()->materialName().c_str() );
      CntInInfo.logical = new G4LogicalVolume(CntInInfo.solid,matCntIn,vol,0,0,0);
      CntInInfo.physical = new G4PVPlacement(cntin->get3DTransfrom(),
          CntInInfo.logical,       // its logical volume
          vol,                     // its name
          trckInfo.logical,        // its mother  volume
          false,                   // no boolean operations
          0,                       // copy number
          checkOverlap);

      for (int iBx = 0; iBx < trckdesc->nBoxKindElem(); ++iBx){

        std::cout<<"Building box element n.: "<<iBx<<std::endl;

        genelmbs::Boxkind *ibx = trckdesc->getTrkBoxKindElem(iBx);
        if ( debugShape ) {
          std::cout<<"TRUCK Box type element: "<<ibx->Id()<<std::endl;
        }
        VolumeInfo BoxInfo;
        sprintf(shape,"%s-%d",ibx->getElmName().c_str(),iBx);
        sprintf(vol,"%svol-%03d",ibx->getElmName().c_str(),iBx);
//        std::string Shape(shape);

        //        BoxInfo.solid = new G4Tubs(shape, ibx->getDetail()->InnerRadius()-0.0005,
        //            ibx->getDetail()->OuterRadius()+0.0005,
        //            ibx->getDetail()->halfLength()+0.0005,
        //            0.0,360.0*CLHEP::degree);

        BoxInfo.solid = buildBoxElem(ibx,shape);
        G4Material* matBox = gmsrv::findMaterialOrThrow( ibx->getDetail()->materialName().c_str() );
        BoxInfo.logical = new G4LogicalVolume(BoxInfo.solid,matBox,vol,0,0,0);
        //            if (ibx->voxelizationFactor()==0.0) {
        //                    BoxInfo.logical->SetOptimisation(false);
        //            }
        //            else{
        //                            BoxInfo.logical->SetSmartless( 1.0/((float)(ibx->nPhiSectors() * ibx->nLaddersPerSector())) );
        //            }
        if (ibx->getElmName().compare("cntrWall")==0) {
                  BoxInfo.logical->SetVisAttributes(visAttTrlWall);
        }
        tMotherlogical = trckInfo.logical;
        if (ibx->getElmName().compare("trlrRbs")==0) {
          tMotherlogical = trlPlanelogical;
          std::cout<<"Ribs "<<ibx->Id()<<", mother volume for it "<<tMotherlogical<<std::endl;
        }

        BoxInfo.physical = new G4PVPlacement(ibx->get3DTransfrom(),
            BoxInfo.logical,       // its logical volume
            vol,                     // its name
            tMotherlogical,        // its mother  volume
            false,                   // no boolean operations
            0,                       // copy number
            checkOverlap);

        if (ibx->getElmName().compare("trlrPlane")==0) {
          trlPlanelogical = BoxInfo.logical;
          std::cout<<"Trailer Plane found "<<ibx->Id()<<", its volume is at "<<trlPlanelogical<<std::endl;
        }

      } // Box Loop

      for (int iCy = 0; iCy < trckdesc->nCylKindElem(); ++iCy){

        genelmbs::Cylkind *icy = trckdesc->getTrkCylKindElem(iCy);
        if ( debugShape ) {
          std::cout<<"TRUCK Cyl type element: "<<icy->Id()<<std::endl;
        }
        sprintf(shape,"%s-%d",icy->getElmName().c_str(),iCy);
        sprintf(vol,"%svol-%03d",icy->getElmName().c_str(),iCy);
//        std::string Shape(shape);

        //        CylInfo.solid = new G4Tubs(shape, icy->getDetail()->InnerRadius()-0.0005,
        //            icy->getDetail()->OuterRadius()+0.0005,
        //            icy->getDetail()->halfLength()+0.0005,
        //            0.0,360.0*CLHEP::degree);

        VolumeInfo CylInfo = buildCylElem(icy,iCy);
//        CylInfo.solid = buildCylElem(icy,shape);
//        G4Material* matCyl = gmsrv::findMaterialOrThrow( icy->getDetail()->materialName().c_str() );
//        CylInfo.logical = new G4LogicalVolume(CylInfo.solid,matCyl,vol,0,0,0);
        //            if (icy->voxelizationFactor()==0.0) {
        //                    CylInfo.logical->SetOptimisation(false);
        //            }
        //            else{
        //                            CylInfo.logical->SetSmartless( 1.0/((float)(icy->nPhiSectors() * icy->nLaddersPerSector())) );
        //            }
//        CylInfo.logical->SetVisAttributes(visAttCyl);

        CylInfo.physical = new G4PVPlacement(icy->get3DTransfrom(),
            CylInfo.logical,       // its logical volume
            vol,                     // its name
            trckInfo.logical,     // its mother  volume
            false,                   // no boolean operations
            0,                       // copy number
            checkOverlap);

      } // Cyl Loop

    } // geom 00

    trckInfo.physical =  new G4PVPlacement( 0,
                    trackerOffset,
                    trckInfo.logical,
                    trckName,
                    mother,
                    0,
                    0,
                    checkOverlap);

    if ( checkOverlap ) { std::cout<<"TRUCK Overlap Checking "<<trckInfo.physical->CheckOverlaps(100000,0.0001,true)<<std::endl; }

  }

  return trckInfo;

}

VolumeInfo TRUCKBuilder::containerInVol() { return CntInInfo; }

G4VSolid* TRUCKBuilder::buildBoxElem(genelmbs::Boxkind *ibx, char shape[]){
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

    std::vector<G4TwoVector> polygon(12);
    double w=ibx->getDetail()->halfWidth();
    double h=ibx->getDetail()->halfHeight();
    double te=ibx->getDetail()->wallThickness();
    double tc=ibx->getDetail()->centWallThickness()/2.0;
    polygon[0].set(-w ,-h);
    polygon[1].set(-w ,-h+te);
    polygon[2].set(-tc,-h+te);
    polygon[3].set(-tc, h-te);
    polygon[4].set(-w , h-te);
    polygon[5].set(-w , h);
    polygon[6].set( w , h);
    polygon[7].set( w , h-te);
    polygon[8].set( tc, h-te);
    polygon[9].set( tc,-h+te);
    polygon[10].set(w ,-h+te);
    polygon[11].set(w ,-h);
    tmpBox = new G4ExtrudedSolid(shape, polygon, ibx->getDetail()->halfLength(),0,1,0,1);

  } else if (ibx->getShapeType()==genelmbs::Boxkind::Cshape) {

    std::vector<G4TwoVector> polygon(8);
    double w=ibx->getDetail()->halfWidth();
    double h=ibx->getDetail()->halfHeight();
    double te=ibx->getDetail()->wallThickness();
    double tc=ibx->getDetail()->centWallThickness();
    polygon[0].set(-w   ,-h);
    polygon[1].set(-w   , h);
    polygon[2].set( w   , h);
    polygon[3].set( w   , h-te);
    polygon[4].set(-w+tc, h-te);
    polygon[5].set(-w+tc,-h+te);
    polygon[6].set( w   ,-h+te);
    polygon[7].set( w   ,-h);
    tmpBox = new G4ExtrudedSolid(shape, polygon, ibx->getDetail()->halfLength(),0,1,0,1);
  }
  return tmpBox;
}

VolumeInfo TRUCKBuilder::buildCylElem(genelmbs::Cylkind *icy, int iCy){

    G4VisAttributes* visAttTire = new G4VisAttributes(true, G4Colour::Black() );
    visAttTire->SetForceSolid(true);
    visAttTire->SetForceAuxEdgeVisible (false);
    visAttTire->SetVisibility(true);
    visAttTire->SetDaughtersInvisible(false);

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

      G4Material* matMot = gmsrv::findMaterialOrThrow( "G4_AIR" );
      CylInfo.logical = new G4LogicalVolume(CylInfo.solid,matMot,volName,0,0,0);

      G4Material* matCyl = gmsrv::findMaterialOrThrow(icy->getDetail()->materialName().c_str());

      char tShapeName[100], tVolName[100];

      sprintf(tShapeName,"%s_sub1",shapeName);
      sprintf(tVolName,"%s_sub1",volName);

      G4VSolid *tsubsh = new G4Tubs(tShapeName, icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->outerRadius(),icy->getDetail()->halfLength(),0.0,CLHEP::twopi);
      G4LogicalVolume *tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      G4VPhysicalVolume *tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

      sprintf(tShapeName,"%s_sub2",shapeName);
      sprintf(tVolName,"%s_sub2",volName);

      tsubsh = new G4Tubs(tShapeName, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->centWallThickness()/2.0,0.0,CLHEP::twopi);
      tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

    } else if (icy->getShapeType()==genelmbs::Cylkind::Cshape) {

      G4Material* matMot = gmsrv::findMaterialOrThrow( "G4_AIR" );
      CylInfo.logical = new G4LogicalVolume(CylInfo.solid,matMot,volName,0,0,0);

      G4Material* matCyl = gmsrv::findMaterialOrThrow(icy->getDetail()->materialName().c_str());

      char tShapeName[100], tVolName[100];

      sprintf(tShapeName,"%s_sub1",shapeName);
      sprintf(tVolName,"%s_sub1",volName);

      G4VSolid *tsubsh = new G4Tubs(tShapeName, icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->outerRadius(),icy->getDetail()->halfLength(),0.0,CLHEP::twopi);
      G4LogicalVolume *tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      G4VPhysicalVolume *tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

      sprintf(tShapeName,"%s_sub2",shapeName);
      sprintf(tVolName,"%s_sub2",volName);

      tsubsh = new G4Tubs(tShapeName, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->centWallThickness()/2.0,0.0,CLHEP::twopi);
      tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,icy->getDetail()->halfLength()-icy->getDetail()->centWallThickness()/2.0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

    } else if (icy->getShapeType()==genelmbs::Cylkind::Ushape) {

      G4Material* matMot = gmsrv::findMaterialOrThrow( "G4_AIR" );
      CylInfo.logical = new G4LogicalVolume(CylInfo.solid,matMot,volName,0,0,0);

      G4Material* matCyl = gmsrv::findMaterialOrThrow(icy->getDetail()->materialName().c_str());

      char tShapeName[100], tVolName[100];

      sprintf(tShapeName,"%s_sub1",shapeName);
      sprintf(tVolName,"%s_sub1",volName);

      G4VSolid *tsubsh = new G4Tubs(tShapeName, icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->outerRadius(),icy->getDetail()->halfLength(),0.0,CLHEP::twopi);
      G4LogicalVolume *tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      if (icy->getElmName().find("Tire")!=std::string::npos) {
        tlogicSubSh->SetVisAttributes(visAttTire);
      }

      G4VPhysicalVolume *tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

      sprintf(tShapeName,"%s_sub2",shapeName);
      sprintf(tVolName,"%s_sub2",volName);

      tsubsh = new G4Tubs(tShapeName, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->centWallThickness()/2.0,0.0,CLHEP::twopi);
      tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      if (icy->getElmName().find("Tire")!=std::string::npos) {
        tlogicSubSh->SetVisAttributes(visAttTire);
      }


      tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,icy->getDetail()->halfLength()-icy->getDetail()->centWallThickness()/2.0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

      sprintf(tShapeName,"%s_sub3",shapeName);
      sprintf(tVolName,"%s_sub3",volName);

      tsubsh = new G4Tubs(tShapeName, icy->getDetail()->innerRadius(),icy->getDetail()->outerRadius()-icy->getDetail()->wallThickness(),icy->getDetail()->centWallThickness()/2.0,0.0,CLHEP::twopi);
      tlogicSubSh = new G4LogicalVolume(tsubsh,matCyl,tVolName,0,0,0);

      if (icy->getElmName().find("Tire")!=std::string::npos) {
        tlogicSubSh->SetVisAttributes(visAttTire);
      }


      tphysLdSh = new G4PVPlacement(0,
          G4ThreeVector(0,0,-icy->getDetail()->halfLength()+icy->getDetail()->centWallThickness()/2.0),
          tlogicSubSh,         // its logical volume
          tVolName,           // its name
          CylInfo.logical, // its mother  volume
          false,              // no boolean operations
          0);                 // copy number
      tphysLdSh->GetCopyNo(); //just to remove the warning during compiling

    }

    return CylInfo;
}

//void TRUCKBuilder::instantiateSensitiveDetectors( const std::string hitsCollectionName){
//
//  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();
//
//  G4SDManager* SDman      = G4SDManager::GetSDMpointer();
//
//  // G4 takes ownership and will delete the detectors at the job end
//
//  GeomHandle<TRUCKdescription> trckdesc;
//  G4ThreeVector trckPos(0.,0.,CLHEP::mm * trckdesc->z0()/*-zOff*/);
//
//  TRUCKLadderSD* trckdescSD=0x0;
//  trckdescSD = new TRUCKLadderSD(SensitiveDetectorName::MPGDTRUCKrackerRO(), hitsCollectionName,  config);
//  trckdescSD->setTRUCKCenterInDetSys(trckPos);
//  SDman->AddNewDetector(trckdescSD);
//
//}

} // end namespace trck
