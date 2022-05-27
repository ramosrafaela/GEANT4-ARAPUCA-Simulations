/*Created by Rafaela Ramos Sarmento
abstract: generation of the alpha particle with specific energy, momentum and initial position
for more information contact me: rramos@ifi.unicamp.br
last update: 17/07/2019
*/

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0),
fEnvelopeBox(0)
{
    flag_alpha=true;
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



    G4double H_b = 43.5*cm;
    G4double esp = 20.0*mm;

    G4double dif_h = 10.0*cm;
    G4double espessura_maior = 8.5*cm;
    G4double pi = std::acos(-1.);
    G4double teta = 2*pi*G4UniformRand();
    G4double costeta = std::cos(teta);
    G4double sinteta = std::sin(teta);
    G4double raio = 0.5*cm;
    G4double esp_alpha = 1*cm;

    G4double myraio = raio*sqrt(G4UniformRand());

    G4double x0 = myraio*costeta;
    G4double z0 = myraio*sinteta + 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp);
    G4double y0 = 3.0*cm - 140*(G4UniformRand()-0.5)*um;

    //the initial position of the particle
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="alpha");
    fParticleGun->SetParticleDefinition(particle);


    //direçao da particula -> distribuicao isotropica
  /*
    G4double pi2 = 3.141595;
    G4double teta2 = 2*pi*G4UniformRand();
    G4double phi2 = pi2*G4UniformRand();
    G4double costeta2 = std::cos(teta2);
    G4double sinteta2 = std::sin(teta2);
    G4double cosphi2 = std::cos(phi2);
    G4double sinphi2 = std::sin(phi2);

    G4ThreeVector momento = G4ThreeVector(sinphi2*costeta2, sinphi2*sinteta2, cosphi2);
    */
    //G4ThreeVector momento = G4ThreeVector(0., -1.0, 0.);

    // -------------- angular distribution extraction ----//default


   G4double fi2 = 2*pi*G4UniformRand();
   G4double costheta = 2.0*(G4UniformRand() - 0.5);//isotropo e de 0 a -1
   G4double teta2 = std::acos(costheta);
   G4double costeta2 = std::cos(teta2);
   G4double sinteta2 = std::sin(teta2);
   G4double cosfi2 = std::cos(fi2);
   G4double sinfi2 = std::sin(fi2);
   //-- gera fuxo isotropo na direcao radial (z<->y)
   G4ThreeVector dir(sinteta2*cosfi2,costeta2,sinteta2*sinfi2);
   //--fluxo de cima para baixo
   //G4ThreeVector dir(0.,-1.,0.);


    fParticleGun->SetParticleMomentumDirection(dir);

    //sorteio das energia da fonte alpha
    G4double sorteio = 100*G4UniformRand();

    if ( sorteio <= 2.2 ){
        fParticleGun->SetParticleEnergy(4.464*MeV);

    } else if ( sorteio > 2.2 && sorteio < 51.1){
        fParticleGun->SetParticleEnergy(4.187*MeV);

    } else {
        fParticleGun->SetParticleEnergy(4.759*MeV);

    }

//    fParticleGun->SetParticleEnergy(4.187*MeV);
    fParticleGun->GeneratePrimaryVertex(anEvent);


    flag_alpha = true;
    flag_generate = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
