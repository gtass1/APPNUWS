//
// FLOORBuilder builder class for the FLOOR in geant4
//
// Original author G. Tassielli
//

#include "FLOORBuilder.hh"
#include "FLOORdescription.hh"

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

namespace flr {

bool checkOverlap, detailedCheck;
std::string floorName("FLOORMother");

VolumeInfo FLOORBuilder::construct( G4LogicalVolume* mother/*, double zOff*/ ){

  // Master geometry for the floor.
  GeomHandle<FLOORdescription> floordesc;
  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  VolumeInfo floorInfo;

  double x0    = CLHEP::mm * floordesc->x0();
  double y0    = CLHEP::mm * floordesc->y0();
  double z0    = CLHEP::mm * floordesc->z0();
  G4ThreeVector floorOffset(x0,y0-0.002,z0/*-zOff*/); //0.002 to avoid overlaps with the truck mother volume

  checkOverlap = config.getBool("g4.doSurfaceCheck",false);
  detailedCheck = checkOverlap&&config.getBool("floor.doDetailedSurfCheck",false);

  if (floordesc->isExternal()) {
//                throw cet::exception("GEOM") <<"This GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"FLOOR: This GDML file option is temporarily disabled\n";
    e.error();
  } else {

    G4VisAttributes* visAtt = new G4VisAttributes(true, G4Colour::White() );
    visAtt->SetForceSolid(true);
    visAtt->SetForceAuxEdgeVisible (false);
    visAtt->SetVisibility(false);
    visAtt->SetDaughtersInvisible(false);

    G4Material* matMother = gmsrv::findMaterialOrThrow( config.getString("floor.motherVolMat","G4_AIR") );

    floorInfo.solid = new G4Box("FLOORdescription", floordesc->halfWidth()+0.001,floordesc->halfHeight()+0.001,floordesc->zHalfLength()+0.001);

    floorInfo.logical = new G4LogicalVolume(floorInfo.solid , matMother, floorName,0,0,0);
    floorInfo.logical->SetVisAttributes(visAtt);

//    G4VisAttributes* visAttTrlWall = new G4VisAttributes(true, G4Colour::Red() );
//    visAttTrlWall->SetForceSolid(true);
//    visAttTrlWall->SetForceAuxEdgeVisible (false);
//    visAttTrlWall->SetVisibility(true);
//    visAttTrlWall->SetDaughtersInvisible(false);

    bool debugShape =  config.getBool("floor.debugShape",false);

    char shape[50], vol[50];

    if (floordesc->geomType()==0) {

      for (int iBx = 0; iBx < floordesc->nBoxKindElem(); ++iBx){

        std::cout<<"Building box element n.: "<<iBx<<std::endl;

        genelmbs::Boxkind *ibx = floordesc->getFlrBoxKindElem(iBx);
        if ( debugShape ) {
          std::cout<<"FLOOR Box type element: "<<ibx->Id()<<std::endl;
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

        BoxInfo.physical = new G4PVPlacement(ibx->get3DTransfrom(),
            BoxInfo.logical,       // its logical volume
            vol,                     // its name
            floorInfo.logical,        // its mother  volume
            false,                   // no boolean operations
            0,                       // copy number
            checkOverlap);

      } // Box Loop

    } // geom 00

    floorInfo.physical =  new G4PVPlacement( 0,
                    floorOffset,
                    floorInfo.logical,
                    floorName,
                    mother,
                    0,
                    0,
                    checkOverlap);

    if ( checkOverlap ) { std::cout<<"FLOOR Overlap Checking "<<floorInfo.physical->CheckOverlaps(100000,0.0001,true)<<std::endl; }

  }

  return floorInfo;

}

G4VSolid* FLOORBuilder::buildBoxElem(genelmbs::Boxkind *ibx, char shape[]){
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

} // end namespace flr
