//
// CSMVtrackerBuilder builder class for the CSMV in geant4
//
// Original author G. Tassielli
//

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
#include "G4VisAttributes.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4UserLimits.hh"

//#include "PXSTbsLadder.hh"
#include "PXSTbsLayer.hh"

#include "CSMVBuilder.hh"

#include "CSMVLadderSD.hh"
#include "CSMVtracker.hh"
#include "CSMVAbsorber.hh"

using namespace std;
//using namespace svx;

namespace csmv {

bool checkOverlap, detailedCheck;
string csmvName("CSMVMother");

VolumeInfo CSMVBuilder::constructTracker( G4LogicalVolume* mother/*, double zOff*/ ){

  // Master geometry for the tracker.
  GeomHandle<CSMVtracker> csmvtracker;
  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  VolumeInfo csmvInfo;

  double x0    = CLHEP::mm * csmvtracker->x0();
  double y0    = CLHEP::mm * csmvtracker->y0();
  double z0    = CLHEP::mm * csmvtracker->z0();
  G4ThreeVector trackerOffset(x0,y0,z0/*-zOff*/);

  checkOverlap = config.getBool("g4.doSurfaceCheck",false);
  detailedCheck = checkOverlap&&config.getBool("csmv.doDetailedSurfCheck",false);

  if (csmvtracker->isExternal()) {
//                throw cet::exception("GEOM") <<"This GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMV: This GDML file option is temporarily disabled\n";
    e.error();
  } else {

    G4VisAttributes* visAtt = new G4VisAttributes(true, G4Colour::White() );
    visAtt->SetForceSolid(true);
    visAtt->SetForceAuxEdgeVisible (false);
    visAtt->SetVisibility(false);
    visAtt->SetDaughtersInvisible(false);

    G4Material* matMother = gmsrv::findMaterialOrThrow( config.getString("csmv.motherVolMat","G4_AIR") );

    csmvInfo.solid = new G4Box("CSMTrackerTop", csmvtracker->halfWidth()+0.001,csmvtracker->halfThickness()+0.001,csmvtracker->zHalfLength()+0.001);

    csmvInfo.logical = new G4LogicalVolume(csmvInfo.solid , matMother, csmvName,0,0,0);
    csmvInfo.logical->SetVisAttributes(visAtt);

    G4VisAttributes* visAttLay = new G4VisAttributes(true, G4Colour::Cyan() );
    visAttLay->SetForceSolid(true);
    visAttLay->SetForceAuxEdgeVisible (false);
    visAttLay->SetVisibility(true);
    visAttLay->SetDaughtersInvisible(false);

    bool debugLayer =  config.getBool("csmv.debugLayer",false);

    char shape[50], vol[50];

    if (csmvtracker->geomType()==0) {

      for (int iLy = 0; iLy < csmvtracker->nLayers(); ++iLy){

    	pxstbs::Layer *ily = csmvtracker->getLayer(iLy);
        if ( debugLayer ) {
          cout<<"CSMV Layer: "<<ily->Id()<<endl;
        }
        VolumeInfo LayerInfo;
        sprintf(shape,"csmttly-L%d",iLy);
        sprintf(vol,"csmttlyvol-L%03d",iLy);

//        LayerInfo.solid = new G4Tubs(shape, ily->getDetail()->InnerRadius()-0.0005,
//            ily->getDetail()->OuterRadius()+0.0005,
//            ily->getDetail()->halfLength()+0.0005,
//            0.0,360.0*CLHEP::degree);
        LayerInfo.solid = new G4Box(shape, csmvtracker->halfWidth()+0.0005,(ily->getDetail()->OuterRadius()-ily->getDetail()->InnerRadius())+0.0005,csmvtracker->zHalfLength()+0.0005);
        LayerInfo.logical = new G4LogicalVolume(LayerInfo.solid,matMother,vol,0,0,0);
        //            if (ily->voxelizationFactor()==0.0) {
        //                    LayerInfo.logical->SetOptimisation(false);
        //            }
        //            else{
//                            LayerInfo.logical->SetSmartless( 1.0/((float)(ily->nPhiSectors() * ily->nLaddersPerSector())) );
        //            }
        LayerInfo.logical->SetVisAttributes(visAttLay);

        boost::shared_ptr<pxstbs::Ladder> ild = ily->getLadder(0);
        VolumeInfo LadderInfo = buildLadder(*ild);
        sprintf(vol,"csmttldvol-L%03dLd%05ld",ild->Id().getLayer(),ild->Id().getLadder());
//        ily->nLaddersPerSector();

        for (unsigned long iLd=0; iLd < ily->nLadders(); ++iLd ){

//          boost::shared_ptr<pxstbs::Ladder> ild = ily->getLadder(iLd);
          ild = ily->getLadder(iLd);

          //if (ild->Id().getLadder()/100>1) continue;
          //if (ild->Id().getLadder()%100>1) continue;
          //if (ild->Id().getLadder()%100<4 || ild->Id().getLadder()%100>10) continue;
          //std::cout<<"iLd "<<iLd<<" Phi "<<iLd/ily->nLaddersPerSector()<<std::endl;
          //if (iLd/ily->nLaddersPerSector()!=0) continue;

          if (ild->getLadderGeomType() == pxstbs::Ladder::plane) {

            if (debugLayer ) {
              cout<<"Ladder "<< ild->Id()<<" geom type "<<ild->getLadderGeomType()<<" type "<<ild->getLadderType()<<" nROs "<<ild->nReadOuts()<<endl;
              cout<<"Vol name "<<vol<<endl;
              cout<<*ild<<endl;
            }

            int copyNum = iLd;
            HepGeom::Transform3D absLadTransf = ild->get3DTransfrom();

            //            CLHEP::HepRotation ldRot = ild->get3DTransfrom().getRotation();
            LadderInfo.physical = new G4PVPlacement(absLadTransf,//ild->get3DTransfrom(),
                LadderInfo.logical,      // its logical volume
                vol,                     // its name
                LayerInfo.logical,       // its mother  volume
                false,                   // no boolean operations
                copyNum,                 // copy number
                checkOverlap);

            if (ild->getLadderType() == pxstbs::Ladder::pixel || ild->getLadderType() == pxstbs::Ladder::strip) {
              G4VSensitiveDetector *sd = G4SDManager::GetSDMpointer()->FindSensitiveDetector(SensitiveDetectorName::CSMTrackerRO());
              if(sd) {
                if (ild->getDetail()->nShells()>1) {
                  //for (int ishell=0; ishell<ild->getDetail()->nShells(); ++ishell){
                  //  LadderInfo.logical->GetDaughter(ishell)->GetLogicalVolume()->SetSensitiveDetector(sd);
                  //}
                  boost::shared_ptr<pxstbs::ReadOutDetail> ildROd = ild->getReadOut()->getDetail();
                  for (int ishell=0; ishell<ildROd->nShells(); ++ishell){
                    LadderInfo.logical->GetDaughter(ildROd->shellId(ishell))->GetLogicalVolume()->SetSensitiveDetector(sd);
                  }
                } else {
                  LadderInfo.logical->SetSensitiveDetector(sd);
                }
              }
            }

          } else {
            exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
            e<<"CSMV: Only plane ladder geometry is implemented yet\n";
            e.error();

          }
        } // Ladder loop

        LayerInfo.physical = new G4PVPlacement(0,               // no rotation
            G4ThreeVector(0,ily->getDetail()->yPosition(),ily->getDetail()->zPosition()),         // at (x,y,z)
            LayerInfo.logical,       // its logical volume
            vol,                     // its name
            csmvInfo.logical,     // its mother  volume
            false,                   // no boolean operations
            0,                       // copy number
            checkOverlap);

      } // Layer loop

    } // geom 00


    csmvInfo.physical =  new G4PVPlacement( 0,
                    trackerOffset,
                    csmvInfo.logical,
                    csmvName,
                    mother,
                    0,
                    0,
                    checkOverlap);

    if ( checkOverlap ) { cout<<"CSMV Overlap Checking "<<csmvInfo.physical->CheckOverlaps(100000,0.0001,true)<<endl; }


  }

  return csmvInfo;

}

//VolumeInfo CSMVtrackerBuilder::buildLadder(float radius, float length, char *shapeName, char *volName, const std::vector<std::string> &materialName, const std::vector<double> &thicknesses, bool activeWireSD, bool isSense){
VolumeInfo CSMVBuilder::buildLadder(pxstbs::Ladder &tld){

  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  G4VisAttributes* visAttLad = new G4VisAttributes(true, G4Colour::Brown() );
  visAttLad->SetForceSolid(true);
  visAttLad->SetForceAuxEdgeVisible (false);
  visAttLad->SetVisibility(true);
  visAttLad->SetDaughtersInvisible(false);

//  G4VisAttributes* visAttLad1 = new G4VisAttributes(true, G4Colour::Blue() );
//  visAttLad1->SetForceSolid(true);
//  visAttLad1->SetForceAuxEdgeVisible (false);
//  visAttLad1->SetVisibility(true);
//  visAttLad1->SetDaughtersInvisible(false);

  VolumeInfo LadderInfo;
  char shapeName[50],volName[50];
//  sprintf(shapeName,"%ld",tld.Id().getLadder());
  sprintf(shapeName,"csmttld-L%dLd%ld",tld.Id().getLayer(),tld.Id().getLadder());
  sprintf(volName,"csmttldvol-L%03dLd%05ld",tld.Id().getLayer(),tld.Id().getLadder());

//  if (debugLayer ) {
//    cout<<"Ladder "<< tld.Id()<<" geom type "<<tld.getLadderGeomType()<<" type "<<tld.getLadderType()<<" nROs "<<tld.nReadOuts()<<endl;
//    cout<<"Vol name "<<volName<<endl;
//    cout<<*tld<<endl;
//  }

  if (tld.getLadderGeomType() == pxstbs::Ladder::plane) {
    if (tld.isWedgeType()) {
      LadderInfo.solid = new G4Trd(shapeName,tld.getDetail()->width()*0.5,
          tld.getDetail()->scndWidth()*0.5,
          tld.getDetail()->thickness()*0.5,
          tld.getDetail()->thickness()*0.5,
          tld.getDetail()->halfLength());
    } else {
      LadderInfo.solid = new G4Box(shapeName,tld.getDetail()->width()*0.5,
          tld.getDetail()->thickness()*0.5,
          tld.getDetail()->halfLength());
    }
    if (tld.getDetail()->nShells()==1) {
      LadderInfo.logical = new G4LogicalVolume(LadderInfo.solid,gmsrv::findMaterialOrThrow( tld.getDetail()->materialName(0).c_str() ),volName,0,0,0);
    }
    else {
      G4Material* matMother = gmsrv::findMaterialOrThrow( config.getString("csmv.motherVolMat","G4_AIR") );
      LadderInfo.logical = new G4LogicalVolume(LadderInfo.solid,matMother/*gmsrv::findMaterialOrThrow( "G4_Galactic" )*/,volName,0,0,0);
      char tShapeName[100], tVolName[100];

      double iYpos = -0.5*tld.getDetail()->thickness();

      for (int ishell=0; ishell<tld.getDetail()->nShells(); ++ishell){
        sprintf(tShapeName,"%s_sub%i",shapeName,ishell);
        sprintf(tVolName,"%s_sub%i",volName,ishell);
        double shlThckHlf = 0.5*tld.getDetail()->shellThickness(ishell);
        iYpos += shlThckHlf;
        G4VSolid *tldsh=0x0;
        if (tld.isWedgeType()) {
          tldsh = new G4Trd(tShapeName,tld.getDetail()->width()*0.5,
              tld.getDetail()->scndWidth()*0.5,
              shlThckHlf,
              shlThckHlf,
              tld.getDetail()->halfLength());
        } else {
          tldsh = new G4Box(tShapeName,tld.getDetail()->width()*0.5,
              shlThckHlf,
              tld.getDetail()->halfLength());
        }

        G4LogicalVolume *tlogicLdSh = new G4LogicalVolume(tldsh,gmsrv::findMaterialOrThrow(tld.getDetail()->materialName(ishell).c_str()),tVolName,0,0,0);

        G4VPhysicalVolume *tphysLdSh = new G4PVPlacement(0,
            G4ThreeVector(0,iYpos,0),
            tlogicLdSh,         // its logical volume
            tVolName,           // its name
            LadderInfo.logical, // its mother  volume
            false,              // no boolean operations
            0);                 // copy number
        tphysLdSh->GetCopyNo(); //just to remove the warning during compiling
        //if (ishell==1) tlogicLdSh->SetVisAttributes(visAttLad1);
        //else tlogicLdSh->SetVisAttributes(visAttLad);
        iYpos += shlThckHlf;
      }
    }
    LadderInfo.logical->SetVisAttributes(visAttLad);

  }
  return LadderInfo;
}

void CSMVBuilder::instantiateSensitiveDetectors( const std::string hitsCollectionName){

  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  G4SDManager* SDman      = G4SDManager::GetSDMpointer();

  // G4 takes ownership and will delete the detectors at the job end

  GeomHandle<CSMVtracker> csmvtracker;
  G4ThreeVector csmvPos(0.,0.,CLHEP::mm * csmvtracker->z0()/*-zOff*/);

  CSMVLadderSD* csmvtrackerSD=0x0;
  csmvtrackerSD = new CSMVLadderSD(SensitiveDetectorName::CSMTrackerRO(), hitsCollectionName,  config);
  csmvtrackerSD->setCSMVCenterInDetSys(csmvPos);
  SDman->AddNewDetector(csmvtrackerSD);

}

void CSMVBuilder::constructAbsorber( G4LogicalVolume* csmvmother/*, double zOff*/ ){

  // Master geometry for the tracker.
  GeomHandle<CSMVtracker> csmvtracker;
  GeomHandle<CSMVAbsorber> csmvabsorber;
  crd::SimpleConfig const& config  = GeomService::Instance()->getConfig();

  checkOverlap = config.getBool("g4.doSurfaceCheck",false);

  if (csmvtracker->isExternal()) {
//                throw cet::exception("GEOM") <<"This GDML file option is temporarily disabled\n";
    exc::exceptionG4 e("GEOM","Fatal Error in Argument",1);
    e<<"CSMV: This GDML file option is temporarily disabled\n";
    e.error();
  } else {

    G4VisAttributes* visAtt = new G4VisAttributes(true, G4Colour::Grey() );
    visAtt->SetForceSolid(true);
    visAtt->SetForceAuxEdgeVisible (true);
    visAtt->SetVisibility(true);
    visAtt->SetDaughtersInvisible(false);

//    G4VisAttributes* visAtt1 = new G4VisAttributes(true, G4Colour::Green() );
//    visAtt1->SetForceSolid(true);
//    visAtt1->SetForceAuxEdgeVisible (true);
//    visAtt1->SetVisibility(true);
//    visAtt1->SetDaughtersInvisible(false);

    G4Material* matMother = gmsrv::findMaterialOrThrow( config.getString("csmv.motherVolMat","G4_AIR") );
//    bool debugLayer =  config.getBool("csmv.debugLayer",false);

    char shape[50], vol[50];

//    if (csmvtracker->geomType()==0) {

      for (int iLy = 0; iLy < csmvabsorber->getAbsorbLayers(); ++iLy){

        VolumeInfo LayerInfo;
        sprintf(shape,"csmttradly-L%d",iLy);
        sprintf(vol,"csmttradlyvol-L%02d",iLy);
        if (csmvabsorber->getAbsorbType()[iLy]==0) { //Barrel layers

          LayerInfo.solid = new G4Tubs(shape,csmvabsorber->getAbsorbInRasius()[iLy],
              csmvabsorber->getAbsorbInRasius()[iLy]+csmvabsorber->getAbsorbersThickness()[iLy],
              csmvabsorber->getAbsorbHalfLengths()[iLy],
              0.0,360.0*CLHEP::degree);
          LayerInfo.logical = new G4LogicalVolume(LayerInfo.solid,matMother,vol,0,0,0);

          char tShapeName[100], tVolName[100];

          double iInRad = csmvabsorber->getAbsorbInRasius()[iLy];

          for (int ishell=0; ishell<csmvabsorber->getAbsorbNmShells()[iLy]; ++ishell){
            sprintf(tShapeName,"%s_sub%i",shape,ishell);
            sprintf(tVolName,"%s_sub%i",vol,ishell);

            G4Tubs *tradsh = new G4Tubs(tShapeName,iInRad,
                iInRad+csmvabsorber->getAbsorbShellsThick()[iLy][ishell],
                csmvabsorber->getAbsorbHalfLengths()[iLy],
                0.0,360.0*CLHEP::degree);

            G4LogicalVolume *tlogicRadSh = new G4LogicalVolume(tradsh,gmsrv::findMaterialOrThrow(csmvabsorber->getAbsorbShellsMaterial()[iLy][ishell]),tVolName,0,0,0);

            G4VPhysicalVolume *tphysRadSh = new G4PVPlacement(0,
                G4ThreeVector(0,0,0),
                tlogicRadSh,        // its logical volume
                tVolName,           // its name
                LayerInfo.logical,  // its mother  volume
                false,              // no boolean operations
                0,                  // copy number
                checkOverlap);
            tphysRadSh->GetCopyNo(); //just to remove the warning during compiling
            tlogicRadSh->SetVisAttributes(visAtt);
            iInRad += csmvabsorber->getAbsorbShellsThick()[iLy][ishell];
          }

          LayerInfo.physical = new G4PVPlacement(0,               // no rotation
              G4ThreeVector(0,0,0),    // at (x,y,z)
              LayerInfo.logical,       // its logical volume
              vol,                     // its name
              csmvmother,              // its mother volume
              false,                   // no boolean operations
              0,                       // copy number
              checkOverlap);
        } else if (csmvabsorber->getAbsorbType()[iLy]==1) { //Forward layers

          LayerInfo.solid = new G4Tubs(shape,csmvabsorber->getAbsorbInRasius()[iLy],
              csmvabsorber->getAbsorbOutRasius()[iLy],
              0.5*csmvabsorber->getAbsorbersThickness()[iLy],
              0.0,360.0*CLHEP::degree);
          LayerInfo.logical = new G4LogicalVolume(LayerInfo.solid,matMother,vol,0,0,0);

          char tShapeName[100], tVolName[100];

          double iZpos = -0.5*csmvabsorber->getAbsorbersThickness()[iLy];

          for (int ishell=0; ishell<csmvabsorber->getAbsorbNmShells()[iLy]; ++ishell){
            sprintf(tShapeName,"%s_sub%i",shape,ishell);
            sprintf(tVolName,"%s_sub%i",vol,ishell);

            iZpos += 0.5*csmvabsorber->getAbsorbShellsThick()[iLy][ishell];

            G4Tubs *tradsh = new G4Tubs(tShapeName,csmvabsorber->getAbsorbInRasius()[iLy],
                csmvabsorber->getAbsorbOutRasius()[iLy],
                0.5*csmvabsorber->getAbsorbShellsThick()[iLy][ishell],
                0.0,360.0*CLHEP::degree);

            G4LogicalVolume *tlogicRadSh = new G4LogicalVolume(tradsh,gmsrv::findMaterialOrThrow(csmvabsorber->getAbsorbShellsMaterial()[iLy][ishell]),tVolName,0,0,0);

            G4VPhysicalVolume *tphysRadSh = new G4PVPlacement(0,
                G4ThreeVector(0,0,iZpos),
                tlogicRadSh,        // its logical volume
                tVolName,           // its name
                LayerInfo.logical,  // its mother  volume
                false,              // no boolean operations
                0,                  // copy number
                checkOverlap);
            tphysRadSh->GetCopyNo(); //just to remove the warning during compiling
            /*if (ishell==0)*/ tlogicRadSh->SetVisAttributes(visAtt);
//            if (ishell==1) tlogicRadSh->SetVisAttributes(visAtt1);
            iZpos += 0.5*csmvabsorber->getAbsorbShellsThick()[iLy][ishell];
          }

          LayerInfo.physical = new G4PVPlacement(0,               // no rotation
              G4ThreeVector(0,0,csmvabsorber->getAbsorbHalfLengths()[iLy]),    // at (x,y,z)
              LayerInfo.logical,       // its logical volume
              vol,                     // its name
              csmvmother,              // its mother volume
              false,                   // no boolean operations
              0,                       // copy number
              checkOverlap);
          HepGeom::Transform3D posBckw(HepGeom::TranslateZ3D(-csmvabsorber->getAbsorbHalfLengths()[iLy])*HepGeom::RotateY3D(CLHEP::pi));
          LayerInfo.physical = new G4PVPlacement(posBckw,
              LayerInfo.logical,       // its logical volume
              vol,                     // its name
              csmvmother,              // its mother volume
              false,                   // no boolean operations
              1,                       // copy number
              checkOverlap);

        }
      } // Layer loop

  }

}

} // end namespace csmv
