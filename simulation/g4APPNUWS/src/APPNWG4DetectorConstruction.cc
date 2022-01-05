//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************


// just to remove a Warning due to a similar definition bettwen a ROOT variable and a CLHEP one
// this warnign came out after splitting the code
#pragma GCC diagnostic ignored "-Wshadow"
 
// APPNW includes
#include "APPNWG4DetectorConstruction.hh"
#include "APPNWG4DetectorMessenger.hh"
#include "APPNWG4FieldSetup.hh"


// GEOMSRVC includes
#include "ConstructMaterials.hh"
#include "GeomService.hh"
#include "findMaterialOrThrow.hh"
#include "SensitiveDetectorName.hh"
#include "SimG4TrackerSD.hh"
#include "RootIO.hh"

// CSMTT includes
#include "CSMTTMaker.hh"
//#include "CSMTtracker.hh"
#include "CSMTTBuilder.hh"
//#include "CSMTTAbsorber.hh"

// CSMTT includes
#include "CSMBTMaker.hh"
//#include "CSMBtracker.hh"
#include "CSMBTBuilder.hh"
//#include "CSMBTAbsorber.hh"

// G4 includes
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
//#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4GDMLParser.hh"
#include "G4SystemOfUnits.hh"

#include "G4ios.hh"

//c++ includes
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
//G4ThreadLocal
//G4GlobalMagFieldMessenger* APPNWG4DetectorConstruction::fMagFieldMessenger = 0;

APPNWG4DetectorConstruction::APPNWG4DetectorConstruction(G4String fGeomConfName, int runNum, G4String dataOutFold)
: G4VUserDetectorConstruction()
 ,fStepLimit(NULL)
 ,fTheWorld(NULL)
 ,fCheckOverlaps(false)
// ,fMegTar(NULL)
 {
 
  fMessenger = new APPNWG4DetectorMessenger(this);
  std::cout<<"Messenger "<<fMessenger<<std::endl;
//  GeomService *geoms = GeomService::Instance("config/geom_01.txt");
//  /*const crd::SimpleConfig &*/cRd = &geoms->getConfig();

  ReadConfigGeomFile(fGeomConfName);

  //Load data saver
  RootIO::GetInstance(runNum,dataOutFold);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
APPNWG4DetectorConstruction::~APPNWG4DetectorConstruction() {

//  if (fMegTar!=NULL) delete fMegTar;

	  G4cout << "Deleting Detector "<< G4endl;
  delete fStepLimit;
  delete fMessenger;
  G4cout << "Detector deleted"<< G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 G4VPhysicalVolume* APPNWG4DetectorConstruction::Construct() {

  // Define materials
//  DefineMaterials();
  gmsrv::ConstructMaterials matConst;
  matConst.construct();

  //Create the world
  CreateWorld();

  // Define volumes
  DefineVolumes();

  // Visualization attributes
  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  fTheWorld->GetLogicalVolume()->SetVisAttributes(boxVisAtt);

  // Set step limit from configuration
  SetMaxStep(0.);
  
  return GetWorld();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void APPNWG4DetectorConstruction::CreateWorld() {

  G4Material* air  = gmsrv::findMaterialOrThrow(cRd->getString("world.materialName"));

  //G4double worldLength = cRd->getDouble("world.Length") ;
  std::vector<double> worldDims;
  cRd->getVectorDouble("world.dims",worldDims);

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(*std::max_element(worldDims.begin(),worldDims.end())/*worldLength*/);
  if (fCheckOverlaps) {
    G4cout << "Computed tolerance = "
        << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
        << " mm" << G4endl;
  }

  G4Box* worldS
    = new G4Box("world",                                    //its name
                  worldDims[0]/2,worldDims[1]/2,worldDims[2]/2); //its size
//                worldLength/2,worldLength/2,worldLength/2); //its size

  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 

  fTheWorld = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void APPNWG4DetectorConstruction::DefineVolumes() {

  ConstructCosmicRayTracker();

  //export geometry in GDML file
  if (cRd->getBool("writeGDML",false)) {
    G4GDMLParser parser;
    parser.Write(cRd->getString("GDMLFileName"), fTheWorld);
  }
  
  // Always return the physical world
  //  return TheWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void APPNWG4DetectorConstruction::ConstructCosmicRayTracker() {

  if (cRd->getBool("hasCSMTT",false)) {

    RootIO::GetInstance()->CreateMCStepBranches(SensitiveDetectorName::MPGDTrackerRO(),"CSMTTHitsStepCh");

    csmtt::CSMTTMaker csmttm( *cRd );
    GeomService::Instance()->addDetector( csmttm.getCSMTTrackerPtr() );

    csmtt::CSMTTBuilder::instantiateSensitiveDetectors("CSMTTrackerHitsCollection");
    VolumeInfo csmttvolinf = csmtt::CSMTTBuilder::constructTracker( fTheWorld->GetLogicalVolume() );

    GeomService::Instance()->addDetector( csmttm.getCSMTTabsorberPtr() );
    csmtt::CSMTTBuilder::constructAbsorber( csmttvolinf.logical );

  }

  if (cRd->getBool("hasCSMBT",false)) {

    RootIO::GetInstance()->CreateMCStepBranches(SensitiveDetectorName::MPGDTrackerRO(),"CSMBTHitsStepCh");

    csmbt::CSMBTMaker csmbtm( *cRd );
    GeomService::Instance()->addDetector( csmbtm.getCSMBTrackerPtr() );

    csmbt::CSMBTBuilder::instantiateSensitiveDetectors("CSMBTrackerHitsCollection");
    VolumeInfo csmbtvolinf = csmbt::CSMBTBuilder::constructTracker( fTheWorld->GetLogicalVolume() );

    GeomService::Instance()->addDetector( csmbtm.getCSMBTabsorberPtr() );
    csmbt::CSMBTBuilder::constructAbsorber( csmbtvolinf.logical );

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void APPNWG4DetectorConstruction::ConstructSDandField() {
  // Sensitive detectors

  // Construct the field creator - this will register the field it creates
  if (!fEmFieldSetup.Get()) {
    APPNWG4FieldSetup* fieldSetup
      = new APPNWG4FieldSetup(G4ThreeVector()/* 3.3*tesla, 0.0, 0.0 )*/ );
    G4AutoDelete::Register(fieldSetup); // Kernel will delete the F01FieldSetup
    fEmFieldSetup.Put(fieldSetup);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void APPNWG4DetectorConstruction::ReadConfigGeomFile(G4String fGeomConfName) {

  GeomService *geoms = GeomService::Instance(fGeomConfName);
  cRd = &geoms->getConfig();

  fCheckOverlaps = cRd->getBool("g4.doSurfaceCheck",false);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void APPNWG4DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit==NULL)&&(maxStep>0.)) fStepLimit = new G4UserLimits();
  if ((fStepLimit)&&(maxStep>0.)) {
    fStepLimit->SetMaxAllowedStep(maxStep);
    fTheWorld->GetLogicalVolume()->SetUserLimits(fStepLimit);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void APPNWG4DetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}  
