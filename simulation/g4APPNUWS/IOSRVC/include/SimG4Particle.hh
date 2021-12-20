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

#ifndef SimG4Particle_h
#define SimG4Particle_h 1

#include "G4ThreeVector.hh"
#include "tls.hh"

//#include "TObject.h"
/// Particle info class
///

class SimG4Particle {

  public:
    SimG4Particle();
    SimG4Particle(G4int trackID, G4int parentID, G4String name, G4double charge, G4int pdgcode, G4ThreeVector posstart, G4ThreeVector posend, G4ThreeVector momentum);

    SimG4Particle(const SimG4Particle&);

    virtual ~SimG4Particle();

    // operators
    const SimG4Particle& operator=(const SimG4Particle&);
    G4int operator==(const SimG4Particle&) const;


    // methods from base class
    void SetTrackID         (G4int track)      { fTrackID = track; }
    void SetParentID        (G4int parent)     { fParentID = parent; }
    void SetParticleName    (G4String pname)   { fParticleName = pname; }
    void SetParticleCharge  (G4double charge)  { fCharge = charge; }
    void SetPDGCode         (G4int pcode)      { fPDGCode = pcode; }
    void SetPosStart        (G4ThreeVector init_xyz) { fPosStart = init_xyz; }
    void SetPosEnd          (G4ThreeVector end_xyz)  { fPosEnd = end_xyz; }
    void SetMomentum        (G4ThreeVector mom)      { fMomentum = mom; }

    // Get methods
    G4int         GetTrackID() const         { return fTrackID; }
    G4int         GetParentID() const        { return fParentID; }
    G4String      GetParticleName() const    { return fParticleName; }
    G4double      GetParticleCharge() const  { return fCharge; }
    G4int         GetPDGCode() const         { return fPDGCode; }
    G4ThreeVector GetPosStart() const        { return fPosStart; }
    G4ThreeVector GetPosEnd() const          { return fPosEnd; }
    G4ThreeVector GetMomentum() const        { return fMomentum; }

  public:

      G4int         fTrackID;
      G4int         fParentID;
      G4String      fParticleName;
      G4double      fCharge;
      G4int         fPDGCode;
      G4ThreeVector fPosStart;
      G4ThreeVector fPosEnd;      
      G4ThreeVector fMomentum;
//   ClassDef(SimG4Particle,1);
   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif
