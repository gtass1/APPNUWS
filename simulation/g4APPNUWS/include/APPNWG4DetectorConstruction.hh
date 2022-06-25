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

#ifndef APPNWG4DetectorConstruction_h
#define APPNWG4DetectorConstruction_h 1

//#include <SimpleConfig.hh>

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4Cache.hh"

#include "tls.hh"

namespace crd {
  class SimpleConfig;
}

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;
//class G4GlobalMagFieldMessenger;

class APPNWG4DetectorMessenger;
class APPNWG4FieldSetup;

/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.

class APPNWG4DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    APPNWG4DetectorConstruction(G4String fGeomConfName="config/geom_01.txt", int runNum=0, G4String dataOutFold=".");
    virtual ~APPNWG4DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    G4VPhysicalVolume* GetWorld() { return fTheWorld; };

    // Set methods
    void ReadConfigGeomFile(G4String fGeomConfName);
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );
    void CreateWorld();

  private:
    // methods
    void DefineMaterials();
    void DefineVolumes();
    void ConstructCosmicRayTracker();
    void ConstructTruck();
    void ConstructCargo();
    void ConstructFloor();

    // data members
    G4double           fmaxStep;

    G4UserLimits*      fStepLimit;            // pointer to user step limits

    APPNWG4DetectorMessenger*  fMessenger;   // messenger
    G4VPhysicalVolume*       fTheWorld;

    const crd::SimpleConfig *cRd;

//    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger;
    G4Cache<APPNWG4FieldSetup*>    fEmFieldSetup;
                                        // magnetic field messenger
    
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps 

//    APPNWG4MegTAR *fMegTar;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
