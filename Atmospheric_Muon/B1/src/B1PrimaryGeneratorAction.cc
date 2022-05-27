/*Created by Rafaela Ramos Sarmento
abstract: generation of atmospheric muons with parameterization and specific energy, momentum and initial position
for more information contact me: rramos@ifi.unicamp.br
last update: 14/02/2020
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
    flag_mu=true;
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

    G4double H_half = 43.5*0.5*cm; //comprimento criostato
    G4double esp = 20.0*mm;

    G4double dif_h = 10.0*cm;
    G4double espessura_maior = 8.5*cm;
    G4double pi = std::acos(-1.);
    G4double teta1 = 0.;
    G4double costeta1 = 0.;
    G4double sinteta1 = 0.;
    G4double raio_emissao = 200*0.5*cm;
    G4double R = 26.5*0.5*cm;

    G4double esp_alpha = 2*cm;

    G4double myraio_emissao = 0.;

    G4double x0 = 0.;
    G4double y0 = 0.;
    G4double z0 = 50*cm+H_half; //os muons devem estar 50 cm da tampa do criostato, entao tem que somar mais a metade do comprimento do criostato


    //the initial position of the particle
 //   fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu-");
    fParticleGun->SetParticleDefinition(particle);

    //iniciando o sofrimento

    // -------------- angular distribution extraction ----//default

    G4double distrib_uniform = 0;

    G4double teta = 0;
    G4double i = 0;
    G4double angle;

    G4double fi = 0;
    G4double costeta = 0;
    G4double sinteta = 0;
    G4double cosfi = 0;
    G4double sinfi = 0;

    G4double time1=0, time2=0;

    G4double z1=0, z2=0;

  //  G4cout << x0*x0 << " " << y0*y0 << G4endl;

    bool particleHitDetector = false;

    while(1){
        particleHitDetector = false;


        myraio_emissao = raio_emissao*sqrt(G4UniformRand());

        teta1 = 2*pi*G4UniformRand();
        costeta1 = std::cos(teta1);
        sinteta1 = std::sin(teta1);

        x0 = myraio_emissao*costeta1;
        y0 = myraio_emissao*sinteta1;



        distrib_uniform = G4UniformRand();

        angle = std::acos(std::pow(1-distrib_uniform,1./3.)); //cos^2 distribution in theta
        teta = angle;

        fi = 2.*pi*G4UniformRand(); //uniform distribution in fi
        costeta = std::cos(teta);
        sinteta = std::sin(teta);
        cosfi = std::cos(fi);
        sinfi = std::sin(fi);


         if( (y0*cosfi - x0*sinfi)*(y0*cosfi - x0*sinfi) <= R*R ){
            time1 = ((x0*cosfi + y0*sinfi) - std::sqrt((x0*cosfi + y0*sinfi)*(x0*cosfi + y0*sinfi) - (x0*x0 + y0*y0 - R*R)))/(sinteta);
            time2 = ((x0*cosfi + y0*sinfi) + std::sqrt((x0*cosfi + y0*sinfi)*(x0*cosfi + y0*sinfi) - (x0*x0 + y0*y0 - R*R)))/(sinteta);
            if(time2<time1){
                G4cout << "WARNING !!!! \n\n\n WARNING \n\n\n Algo de errado nao esta certo \n\n\n WARNING \n\n\n WARNING" << G4endl;
            }



            z1 = (-1)*costeta*time1 + z0;
            z2 = (-1)*costeta*time2 + z0;


            if(z1 <= +H_half && z1 >=-H_half){ // primeiro caso, passou direto pela lateral
                particleHitDetector = true;
//                 G4cout << "Primeiro caso !!!" << G4endl;
                break;
            }
            else if(z1 > H_half && z2 < - H_half){ // passou pelo meio
                particleHitDetector = true;
//                 G4cout << "Segundo caso !!!" << G4endl;
                break;

            }
            else if(z1 > H_half && z2 < +H_half && z2 > -H_half){ //entrou fora, saiu dentro
                particleHitDetector = true;
//                 G4cout << "Segundo caso !!!" << G4endl;
                break;

            }
            else if(z1 <= +H_half && z2 >= - H_half){ // entrou pela lateral mas nao saiu
//                 G4cout << "Caso redundante, nao deveria passar !!! \n\n Algo errado \n\n LOL !!!! " << G4endl;
                particleHitDetector = true;
                break;

            }
            else{
//                  G4cout << "Cruza fora do cilidro !!" << G4endl;
            }
        }
        else{
//             G4cout << "Nunca bate nunca!!" << G4endl;
        }


    }



    if(particleHitDetector==true){
        G4ThreeVector dir(-sinteta*cosfi, -sinteta*sinfi,-costeta);
        fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
        fParticleGun->SetParticleMomentumDirection(dir);
    }
    else{
        G4cout << "WARNING !!!!! WHILE NAO ESTA FUNCIONANDO" << G4endl;
    }





    fParticleGun->SetParticleEnergy(4.0*GeV);
    fParticleGun->GeneratePrimaryVertex(anEvent);


    flag_mu = true;
    flag_generate = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
