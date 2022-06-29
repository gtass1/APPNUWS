//******************************************************************************
// CRYPrimaryGenerator.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

// CRY coordinate system:
//
//           Z ^
//             |
//             |
//             ----->  Y
//            /
//         X /
//
//
// Geant4 coordinate system:
//
//           Y ^
//             | / X
//             |/
//             ----->  Z
//
// To translate: 180° rot around Z and 90° rot around X
// equivalent to: X' = -X ; Y' = Z; Z' = Y

#ifdef G4_USE_CRY

#include <iomanip>
#include <cstdlib>

#include "CRYPrimaryGenerator.hh"
#include "CRYPrimaryGeneratorMessenger.hh"

using namespace std;

#include "G4Event.hh"

//----------------------------------------------------------------------------//
CRYPrimaryGenerator::CRYPrimaryGenerator(const char *inputfile) :
       particleGun(nullptr)
       ,gen(nullptr)
       ,setup(nullptr)
       ,radialDist(0.0)
       ,extrapolPos(false)
       ,noCheckActiveArea(true)
       ,debug(false)
{
  // define a particle gun
  particleGun = new G4ParticleGun();

  offset.set(0,0,0);

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    if( *inputfile !=0)  //....only complain if a filename was given
      G4cout << "CRYPrimaryGenerator: Failed to open CRY input file= " << inputfile << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    /*CRYSetup **/setup=new CRYSetup(setupString,std::getenv("CRYDATAPATH"));//"../data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;

    offset.set(setup->param(CRYSetup::xoffset)*CLHEP::m,setup->param(CRYSetup::yoffset)*CLHEP::m,setup->param(CRYSetup::zoffset)*CLHEP::m);

  }
  // create a vector to store the CRY particle properties
  vect=new std::vector<CRYParticle*>;

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new CRYPrimaryGeneratorMessenger(this);

  activeAra[0]=activeAra[1]=0.0;
}

//----------------------------------------------------------------------------//
CRYPrimaryGenerator::~CRYPrimaryGenerator()
{
    if (gen) { delete gen; }
    if (setup) { delete setup; }
    if (particleGun) { delete particleGun; }
}

//----------------------------------------------------------------------------//
void CRYPrimaryGenerator::InputCRY()
{
  InputState=1;
}

//----------------------------------------------------------------------------//
void CRYPrimaryGenerator::UpdateCRY(std::string* MessInput)
{
//    std::cout<<"UpdateCRY"<<std::endl;

    if (gen) { delete gen; }
    if (setup) { delete setup; }
  /*CRYSetup **/setup=new CRYSetup(*MessInput,std::getenv("CRYDATAPATH"));//"../data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState=0;
//  std::cout<<"UpdateCRY done"<<std::endl;

  offset.set(setup->param(CRYSetup::xoffset)*CLHEP::m,setup->param(CRYSetup::yoffset)*CLHEP::m,setup->param(CRYSetup::zoffset)*CLHEP::m);

}

//----------------------------------------------------------------------------//
void CRYPrimaryGenerator::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,std::getenv("CRYDATAPATH"));//"../data");

    gen = new CRYGenerator(setup);

  // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
}

//----------------------------------------------------------------------------//
void CRYPrimaryGenerator::SetRadialDist(G4double radDist) {
    if (setup->param(CRYSetup::xoffset)!=0 || setup->param(CRYSetup::yoffset)!=0 || setup->param(CRYSetup::zoffset)!=0 ) {
        std::cout<<"CRY offset position already set, radialDist ignored"<<std::endl;
    } else {
        radialDist=radDist;
        std::cout<<"CRY radialDist "<<radialDist<<std::endl;
        extrapolPos=true;
    }
}

//----------------------------------------------------------------------------//
void CRYPrimaryGenerator::SetActiveArea(G4ThreeVector actArea) {
    if ( (setup->param(CRYSetup::subboxLength)*CLHEP::m)<actArea.x() || (setup->param(CRYSetup::subboxLength)*CLHEP::m)<actArea.z() ) {
        std::cout<<"CRY active Area bigger than the one defined by subboxLength, activeArea ignored"<<std::endl;
    } else {
        activeAra[0]=actArea.x()/2.0;
        activeAra[1]=actArea.z()/2.0;
        std::cout<<"CRY active area x "<<actArea.x()<<" z "<<actArea.z()<<std::endl;
        noCheckActiveArea=false;
    }
}

//----------------------------------------------------------------------------//
void CRYPrimaryGenerator::SetDebg(G4bool doDebug) {
    debug=doDebug;
}


//----------------------------------------------------------------------------//
//void CRYPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
void CRYPrimaryGenerator::GeneratePrimaryVertex(G4Event* anEvent)
{ 
//    std::cout<<"GeneratePrimaryVertex"<<std::endl;
  if (InputState != 0) {
    G4String* str = new G4String("CRY library was not successfully initialized");
    //G4Exception(*str);
    G4Exception("CRYPrimaryGenerator", "1",
                RunMustBeAborted, *str);
  }

  G4String particleName;

  G4bool redoEve=true;
  G4int jev=0;
  static const G4int maxEvTry=1000;

  while (jev<maxEvTry  && redoEve) {
      ++jev;

      vect->clear();
      gen->genEvent(vect);

      //  G4ThreeVector offset(setup->param(CRYSetup::xoffset)*CLHEP::m,setup->param(CRYSetup::yoffset)*CLHEP::m,setup->param(CRYSetup::zoffset)*CLHEP::m);

      if (debug) {
          G4cout << "\nEvent=" << anEvent->GetEventID() << " "
                  << "CRY generated nparticles=" << vect->size()
                  << G4endl;
      }

      for ( unsigned j=0; j<vect->size(); j++) {
          particleName=CRYUtils::partName((*vect)[j]->id());

          G4ThreeVector tmpPos((*vect)[j]->x()*CLHEP::m,(*vect)[j]->y()*CLHEP::m,(*vect)[j]->z()*CLHEP::m);

          if (noCheckActiveArea || ( fabs(tmpPos.x())<activeAra[0] && fabs(tmpPos.y())<activeAra[1] ) ) { //Y will be Z in geant system
              redoEve=false;
              if (extrapolPos) {
                  offset.set( (*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w() );
                  offset*=-radialDist;
              }

              tmpPos+=offset;

              if (debug) {
                  cout << "  "          << particleName << " "
                          << "charge="      << (*vect)[j]->charge() << " "
                          << setprecision(4)
                          << "energy (MeV)=" << (*vect)[j]->ke()*CLHEP::MeV << " "
                          << "pos (m)"
                          //         << G4ThreeVector((*vect)[j]->x()*CLHEP::m, (*vect)[j]->y()*CLHEP::m, (*vect)[j]->z()*CLHEP::m)
                          << G4ThreeVector(-tmpPos.x(), tmpPos.z(), tmpPos.y())
                          << " " << "direction cosines "
                          //         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
                          << G4ThreeVector(-(*vect)[j]->u(), (*vect)[j]->w(), (*vect)[j]->v())
                          << " " << endl;
              }

              particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
              particleGun->SetParticleEnergy((*vect)[j]->ke()*CLHEP::MeV);
              //    particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*CLHEP::m, (*vect)[j]->y()*CLHEP::m, (*vect)[j]->z()*CLHEP::m));
              particleGun->SetParticlePosition(G4ThreeVector(-tmpPos.x(), tmpPos.z(), tmpPos.y()));
              //    particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
              particleGun->SetParticleMomentumDirection(G4ThreeVector(-(*vect)[j]->u(), (*vect)[j]->w(), (*vect)[j]->v()));
              particleGun->SetParticleTime((*vect)[j]->t());
              particleGun->GeneratePrimaryVertex(anEvent);
          }
          if ((*vect)[j]) { delete (*vect)[j]; }
          //    std::cout<<"GeneratePrimaryVertex done"<<std::endl;
      }
  }

  if (jev>=maxEvTry) {
      G4cout << "\nEvent=" << anEvent->GetEventID() << " not properly simulated, no cosmic ray inside the selected area"<<endl;
  }

}

#endif
