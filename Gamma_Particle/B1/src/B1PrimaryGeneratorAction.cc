/*Created by Rafaela Ramos Sarmento
abstract: code to generate gamma rays from a position very close to the outer cylinder of the cryostat.
Gammas are generated from a point that is the center of the old alpha source and the direction is
isotropic, energy spectrum from Co60 (two lines of emission).
for more information contact me: rramos@ifi.unicamp.br
last update: 19/02/2020
*/

/*18 de fevereiro de 2020.
código para gerar raios gamma de uma posicao colada no cilindro externo do criostado. Os gammas sao gerados
de um ponto que é o centro da antiga fonte alfa e a direcao é isotropica, energia de 1.17 MeV.

Rafaela Ramos Sarmento.
*/


/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0),
fEnvelopeBox(0)
{
  flag_gamma=true;
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{


  //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4double H_b = 43.5*cm; //comprimento criostato
  G4double esp = 20.0*mm;

  G4double dif_h = 10.0*cm;
  G4double espessura_maior = 8.5*cm;
  G4double pi = std::acos(-1.);
  G4double teta1 = 2*pi*G4UniformRand();
  G4double costeta1 = std::cos(teta1);
  G4double sinteta1 = std::sin(teta1);
  G4double raio =  0.5*cm;
  G4double R = 0.5*26.5*cm; //26.5cm é o diametro da boca

  G4double esp_alpha = 2*cm;

  G4double myraio = raio;

  //sendo emitido do centro do disco da fonte alfa
  G4double x0 = 0;
  G4double y0 = R + 0.1*cm ;
  G4double z0 = 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp);


  //the initial position of the particle
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
  = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);

  //iniciando o sofrimento

  // -------------- angular distribution extraction ----//default


  G4double teta;
  G4double angle;


  angle = std::acos(1 - G4UniformRand()); //garante que vai ser gerado de maneira uniforme em todas as regioes
  teta = angle;

  //modelando as transformações de coordenadas cartesianas para as esfericas
  G4double fi = 2.*pi*G4UniformRand();
  G4double costeta = std::cos(teta);
  G4double sinteta = std::sin(teta);
  G4double cosfi = std::cos(fi);
  G4double sinfi = std::sin(fi);

  G4ThreeVector dir(-sinteta*cosfi, - costeta, -sinteta*sinfi);
  // G4ThreeVector dir(0, -1., 0);
  fParticleGun->SetParticleMomentumDirection(dir);




  fParticleGun->SetParticleEnergy(1.17*MeV);
  fParticleGun->GeneratePrimaryVertex(anEvent);


  flag_gamma = true;
  flag_generate = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
