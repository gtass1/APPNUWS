//******************************************************************************
// CRYPrimaryGenerator.hh
//
// This class is a class derived from G4VPrimaryGenerator for
// constructing the process used to generate incident particles.
//
// 1.00 JMV, LLNL, JAN-2007:  First version.
//******************************************************************************
// 

#ifdef G4_USE_CRY

#ifndef CRYPrimaryGenerator_h
#define CRYPrimaryGenerator_h 1

#include "G4VPrimaryGenerator.hh"

#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
//#include "CRYPrimaryGeneratorMessenger.hh"
#include "CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"

class G4Event;
class CRYPrimaryGeneratorMessenger;

class CRYPrimaryGenerator : public G4VPrimaryGenerator
{
  public:
    CRYPrimaryGenerator(const char * filename);
    ~CRYPrimaryGenerator();

  public:
//    void GeneratePrimaries(G4Event* anEvent);
    void InputCRY();
    void UpdateCRY(std::string* MessInput);
    void CRYFromFile(G4String newValue);
    void SetRadialDist(G4double radDist);
    void SetActiveArea(G4ThreeVector activeArea);
    void SetDebg(G4bool doDebug);
//    void SetActiveAreaY(G4double lenghty);
    virtual void GeneratePrimaryVertex(G4Event* anEvent);

  private:
    std::vector<CRYParticle*> *vect; // vector of generated particles
    G4ParticleTable* particleTable;
    G4ParticleGun* particleGun;
    CRYGenerator* gen;
    CRYSetup *setup;
    G4int InputState;
    G4ThreeVector offset;
    G4double radialDist;
    G4bool extrapolPos;
    G4double activeAra[2];
    G4bool noCheckActiveArea;
    CRYPrimaryGeneratorMessenger* gunMessenger;

    G4bool debug;
};

#endif

#endif
