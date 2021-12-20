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

#include "APPNWG4ActionInitialization.hh"

#include "APPNWG4EventAction.hh"
#include "APPNWG4PrimaryGeneratorAction.hh"
#include "APPNWG4RunAction.hh"
#include "APPNWG4TrackingAction.hh"
#include "G4GeneralParticleSource.hh"
//#include "APPNWG4GPSPrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

APPNWG4ActionInitialization::APPNWG4ActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

APPNWG4ActionInitialization::~APPNWG4ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void APPNWG4ActionInitialization::BuildForMaster() const
{
  SetUserAction(new APPNWG4RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void APPNWG4ActionInitialization::Build() const
{
  SetUserAction(new APPNWG4PrimaryGeneratorAction);
//  SetUserAction(new APPNWG4GPSPrimaryGeneratorAction);
  SetUserAction(new APPNWG4RunAction);
  SetUserAction(new APPNWG4TrackingAction);
  SetUserAction(new APPNWG4EventAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
