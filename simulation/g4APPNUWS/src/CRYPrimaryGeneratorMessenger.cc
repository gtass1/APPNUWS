//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: CRYPrimaryGeneratorMessenger.cc,v 1.8 2002/12/16 16:37:27 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef G4_USE_CRY

#include "CRYPrimaryGeneratorMessenger.hh"

#include "CRYPrimaryGenerator.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CRYPrimaryGeneratorMessenger::CRYPrimaryGeneratorMessenger(
                                          CRYPrimaryGenerator* Gun)
  :Action(Gun)
{
  CRYDir = new G4UIdirectory("/CRY/");
  CRYDir->SetGuidance("CRY initialization");
   
  FileCmd = new G4UIcmdWithAString("/CRY/file",this);
  FileCmd->SetGuidance("This reads the CRY definition from a file");
  FileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
   
  InputCmd = new G4UIcmdWithAString("/CRY/input",this);
  InputCmd->SetGuidance("CRY input lines");
  InputCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/CRY/update",this);
  UpdateCmd->SetGuidance("Update CRY definition.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed the CRY definition.");
  UpdateCmd->AvailableForStates(G4State_Idle);

  MessInput = new std::string;

  distanceCmd = new G4UIcmdWithADoubleAndUnit("/CRY/radialDist",this);
  distanceCmd->SetGuidance("CRY particle starting point is reported on a circumference with radius radialDist");
  distanceCmd->SetParameterName("radialDist",true);
  distanceCmd->SetRange("radialDist>=0. && radialDist<1000000.");
  distanceCmd->SetDefaultValue(10.);
  distanceCmd->SetDefaultUnit("m");
  distanceCmd->AvailableForStates(G4State_Idle);
//  distanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  activeAreaCmd = new G4UIcmdWith3VectorAndUnit("/CRY/activeArea",this);
  activeAreaCmd->SetGuidance("CRY active area for the particle generation, X and Z must be equal or less then subboxLength.");
  activeAreaCmd->SetGuidance("Y directions is ignored.");
  activeAreaCmd->SetParameterName("alx","aly","alz",false,false);
  activeAreaCmd->SetDefaultUnit("m");
  activeAreaCmd->AvailableForStates(G4State_Idle);

  debugCmd = new G4UIcmdWithABool("/CRY/debug",this);
  debugCmd->SetGuidance("CRY active debug option");
  debugCmd->SetParameterName("debug",true);
  debugCmd->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CRYPrimaryGeneratorMessenger::~CRYPrimaryGeneratorMessenger()
{
  delete CRYDir;
  delete InputCmd;
  delete UpdateCmd;
  delete FileCmd;
  delete distanceCmd;
  delete activeAreaCmd;
  delete debugCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CRYPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == InputCmd )
   { 
     Action->InputCRY();
     (*MessInput).append(newValue);
     (*MessInput).append(" ");
   }

  if( command == UpdateCmd )
   { 
     Action->UpdateCRY(MessInput); 
     *MessInput = "";
   }

  if( command == FileCmd )
   { Action->CRYFromFile(newValue); }

  if (command == distanceCmd)
  {
      Action->SetRadialDist( distanceCmd->GetNewDoubleValue(newValue) );
  }

  if (command == activeAreaCmd)
  {
      Action->SetActiveArea(activeAreaCmd->GetNew3VectorValue(newValue) );
  }

  if (command == debugCmd)
  {
      Action->SetDebg(debugCmd->GetNewBoolValue(newValue) );
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
