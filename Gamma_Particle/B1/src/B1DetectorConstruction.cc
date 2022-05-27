/*Created by Rafaela Ramos Sarmento
abstract: modeling code of the x-arapuca double side simulation with the alpha source support capped on the back
for more information contact me: rramos@ifi.unicamp.br
last update: 19/02/2020
*/

#include "B1DetectorConstruction.hh"
#include "B1Constantes.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Polyhedron.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4SubtractionSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TransportationManager.hh"

#include <fstream>
using namespace std;


//***********************************************************************************

B1DetectorConstruction::B1DetectorConstruction (G4double RIndex)
: G4VUserDetectorConstruction(), fScoringVolume(0), fCheckOverlaps(true), Refr_Index(RIndex) {

}

//***********************************************************************************

B1DetectorConstruction::~B1DetectorConstruction (){;}

//***********************************************************************************

G4VPhysicalVolume* B1DetectorConstruction::Construct () {

    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();

}

//***********************************************************************************

void B1DetectorConstruction::DefineMaterials() {}

//================================================================================

G4VPhysicalVolume* B1DetectorConstruction::DefineVolumes() {


    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CRIANDO MATERIAIS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //criando aço inoxidavel

    G4NistManager* man = G4NistManager::Instance();
    man->SetVerbose(1);
    // Define elements from NIST
    man->FindOrBuildElement("H");
    man->FindOrBuildElement("Be");
    G4Element* C  = man->FindOrBuildElement("C");
    man->FindOrBuildElement("N");
    man->FindOrBuildElement("O");
    man->FindOrBuildElement("Al");
    G4Element* Si = man->FindOrBuildElement("Si");
    man->FindOrBuildElement("Ti");
    G4Element* Cr = man->FindOrBuildElement("Cr");
    G4Element* Mn = man->FindOrBuildElement("Mn");
    G4Element* Fe = man->FindOrBuildElement("Fe");
    G4Element* Ni = man->FindOrBuildElement("Ni");
    G4Material* Air = man->FindOrBuildMaterial("G4_AIR");
    G4Material* Teflon = man->FindOrBuildMaterial("G4_TEFLON");
    // Define materials not in NIST
    G4double density3;
    G4int ncomponents;
    G4double fractionmass;
    G4Material* StainlessSteel = new G4Material("StainlessSteel", density3= 8.06*g/cm3, ncomponents=6);
    StainlessSteel->AddElement(C, fractionmass=0.001);
    StainlessSteel->AddElement(Si, fractionmass=0.007);
    StainlessSteel->AddElement(Cr, fractionmass=0.18);
    StainlessSteel->AddElement(Mn, fractionmass=0.01);
    StainlessSteel->AddElement(Fe, fractionmass=0.712);
    StainlessSteel->AddElement(Ni, fractionmass=0.09);

    //criando o PVC C3H4Cl

    G4double a, z; //numero atomico e massa molar
    G4double density_pvc;
    G4int natoms; //numero de atomos
    // Get nist material manager
    G4NistManager* nistManager = G4NistManager::Instance();

    //elementos para criar o acrilico
    G4Element* H = new G4Element("Hydrogen", "H", z = 1. , a = 1.01*g/mole);
    G4Element* Cl = new G4Element("Chlorine", "Cl", z = 17 , a = 35.45*g/mole);

    G4Material* PVC = new G4Material("pvc", density_pvc=1.16*g/cm3,3);
    PVC->AddElement(C, natoms = 5);
    PVC->AddElement(H, natoms = 4);
    PVC->AddElement(Cl, natoms = 1);


    //criando nitrogenio liquido

    G4double atomicNumber_NL2 = 7.0;
    G4double massOfMole_NL2   = 14.01*g/mole;
    G4double density_NL2     = 0.808*g/cm3;
    G4double temperature_NL2  = 77.0*kelvin;

    G4Material* NL2 = new G4Material("LiquidNitrogen", atomicNumber_NL2, massOfMole_NL2, density_NL2, kStateLiquid, temperature_NL2);

    //criando material aluminio

    G4double density_Al = 2.70*g/cm3;

    G4Element* Al = man->FindOrBuildElement("Al");
    G4Material* Aluminium = new G4Material("Aluminio", density_Al, ncomponents = 1);
    Aluminium->AddElement(Al, natoms = 1);

    //criando baixo vacuo

    G4Material* LAr = nistManager->FindOrBuildMaterial("G4_lAr");

    G4double atomicNumber = 1.0;
    G4double massOfMole   = 1.008*g/mole;
    G4double density1     = 1.e-25*g/cm3;
    G4double temperature  = 2.73*kelvin;
    G4double pressure     = 3.e-18*pascal;

    G4Material* Vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, density1, kStateGas, temperature, pressure);
    //G4Material* world_mat = G4Material::GetMaterial("G4_lAr");



    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@





    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ DECLARANDO VARIAVEIS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    const G4int NUMENTRIES = 14;

    G4double Scint_PP[NUMENTRIES] = { 9.2*eV, 9.3*eV, 9.4*eV, 9.5*eV,
                                    9.6*eV, 9.7*eV, 9.76377*eV, 9.8*eV,
                                    9.9*eV, 10.0*eV, 10.05*eV, 10.1*eV,
                                    10.2*eV, 10.3*eV};


    G4double Scint_FAST[NUMENTRIES] = { 0.000856175, 0.00839924, 0.0528321,
                                        0.213077, 0.551004, 0.913594, 1,
                                        0.971252, 0.66205, 0.161926, 0.289355,
                                        0.0810867, 0.0145697, 0.00167853 };

    G4double Scint_SLOW[NUMENTRIES] = { 0.000856175, 0.00839924, 0.0528321,
                                        0.213077, 0.551004, 0.913594, 1,
                                        0.971252, 0.66205, 0.161926, 0.289355,
                                        0.0810867, 0.0145697, 0.00167853 };



   //fotons no espectro VUV
    G4double PhotonEnergy [NUMENTRIES] = {4.96*eV, 5.39*eV, 6.20*eV, 6.89*eV,
                                          8.27*eV, 8.86*eV, 9.69*eV, 10.3*eV,
                                          11.3*eV, 12.4*eV, 13.8*eV, 15.5*eV, 24.8*eV, 24.9*eV};

    G4double reflec_ino = 0.20;
    G4double Reflec_inox [NUMENTRIES] = {reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino,
					 reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino, reflec_ino};


  //proprieta ottiche scint (da trovare)

const G4int nEntries = 5;
  G4double PhotonEnergyN[35] = {1.8002708291*eV, 1.9481638324*eV, 1.8712998009*eV, 2.031612752*eV, 2.122530612*eV, 2.2219671291*eV, 2.3311783976*eV, 2.4516802288*eV, 2.5853189354*eV, 2.7343666014*eV, 2.9016513225*eV, 3.0907383696*eV, 3.306187123*eV, 3.5539235215*eV, 3.8417935227*eV, 4.1804092554*eV, 4.5844859047*eV, 5.075036896*eV, 5.6831472915*eV, 6.456829363*eV, 7.3367183961*eV, 8.1468599034*eV, 8.7942508537*eV, 9.3735970883*eV, 9.7402642614*eV, 10.0346587111*eV, 10.2939327045*eV, 10.401432965*eV, 10.5669602239*eV, 10.7726825649*eV, 10.7960361308*eV, 10.8904717926*eV, 11.0108653265*eV, 11.1339505034*eV, 11.2217603734*eV};

  G4double n_lAr[35] = {1.2249193575067825, 1.224940655396017, 1.224940655396017, 1.2252814216237677, 1.22538791106994, 1.225686081519222, 1.2269000612055847, 1.2272834232118046, 1.2280714451134787, 1.2281779345596509, 1.2294345100244823, 1.2309466601601273, 1.2320328525110833, 1.2330125554158673, 1.2348441738900287, 1.2377406868259118, 1.2414465195527031, 1.2461320551842787, 1.2536928058625028, 1.2648954955998148, 1.28143463770843, 1.301258737209988, 1.3225062859789587, 1.345444112684444, 1.364484425660028, 1.3856545275590553, 1.4069737146827237, 1.4279201887447894, 1.4510731241315424, 1.4716468851320057, 1.5005409993414198, 1.5292534159333024, 1.550184981472904, 1.5699536822603057, 1.5907957966651227};

  G4double abs_pvc[NUMENTRIES] = {0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm, 0.00001*nm};


  G4double PhotonEnergy2[nEntries] = { 2.034*eV, 2.341*eV, 2.757*eV, 3.353*eV, 4.136*eV };

  G4double l_lAr[nEntries] = {3448*m,  4082*m,  6329*m,  9174*m, 12346*m };


    G4double env_sizeXY = 300*cm, env_sizeZ = 300*cm;
    G4double world_sizeXY = 5*env_sizeXY;
    G4double world_sizeZ  = 5*env_sizeZ;


    //raios ext e int do cilindro maior e sua altura
    G4double R_b = 26.5*cm;
    G4double R_a = 20.0*cm;
    G4double H_b = 43.5*cm;
    G4double espessura_maior = 8.5*cm;


    //raios ext e int do cilindro menor e sua altura
    G4double r_b = 15.3*cm;
    G4double r_a = 14.9*cm;
    G4double h_b = 35.0*cm;
    G4double espessura_menor = 1.5*cm;


    G4double   R = 145.0*mm;
    G4double esp = 20.0*mm;
    G4double dif_h = 10.0*cm;

    //arapuca
    G4double b_maior = 12.3*cm;
    G4double b_menor = 10.0*cm;
    G4double a_maior = 10.0*cm;
    G4double a_menor = 7.5*cm;
    G4double espeAra = 1.5*cm;

    //suporte fonte alpha

    G4double esp_alpha = 1.0*cm;
    G4double altura = 10.0*cm;
    G4double largura = 5.0*cm;
    G4double diam_buraco = 1.0*cm;
    G4double esp_aluminio = 140*um;

    G4ThreeVector pos1 = G4ThreeVector(0, 0, 0);
    G4ThreeVector pos2 = G4ThreeVector(0, 0, 0.5*(8.5*cm));
    G4ThreeVector pos3 = G4ThreeVector(0, 0, 0.5*(-0.5*H_b + 0.5*espessura_maior - 2.0*esp));
    G4ThreeVector pos4 = G4ThreeVector(0, 0, 0.5*(-0.5*H_b + 0.5*espessura_maior) + dif_h);
    G4ThreeVector pos5 = G4ThreeVector(0, -0.75*cm, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp + (b_maior - a_maior)));
   /* G4ThreeVector pos6 = G4ThreeVector(0, 3.0*cm - 0.5*esp_alpha, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp));*/
    G4ThreeVector pos6 = G4ThreeVector(0, 3.0*cm, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp));
    G4ThreeVector pos7 = G4ThreeVector(0, 3.5*cm-0.5*esp_aluminio, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp));
    G4ThreeVector pos8 = G4ThreeVector(0, 0, 0.5*(8.5*cm));
    G4ThreeVector pos9 = G4ThreeVector(0, 0, 0.5*(8.5*cm + espessura_menor));
    G4ThreeVector pos10 = G4ThreeVector(0, 4.0*cm - esp_aluminio, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp));
    G4ThreeVector pos11 = G4ThreeVector(0, -0.75*cm + 0.25*espeAra, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp + (b_maior - a_maior) ));
    G4ThreeVector pos12 = G4ThreeVector(0, -0.75*cm - 0.25*espeAra, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp + (b_maior - a_maior)));






    G4ThreeVector trans1 = G4ThreeVector(0, 0, -0.5*(H_b - espessura_maior));
    G4ThreeVector trans2 = G4ThreeVector(0, 0, -0.5*(h_b - espessura_menor));
    G4ThreeVector trans3 = G4ThreeVector(0, 0, 0);
    G4ThreeVector trans4 = G4ThreeVector(0, 3.0*cm, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h - esp));
    G4ThreeVector trans5 = G4ThreeVector(0, 0, -0.5*(h_b - espessura_menor)+0.5*esp);
    G4ThreeVector trans6 = G4ThreeVector(0, 0, 0.5*(-0.5*H_b + 0.5*espessura_maior + dif_h));
    G4ThreeVector trans7 = G4ThreeVector(0, -0.75*cm, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor + 0.5*(b_maior - a_maior));
    /*G4ThreeVector trans8 = G4ThreeVector(0, 3.0*cm - 0.5*esp_alpha, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor);*/
    G4ThreeVector trans8 = G4ThreeVector(0, 3.0*cm, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor);
    G4ThreeVector trans9 = G4ThreeVector(0, 3.5*cm-0.5*esp_aluminio, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor);
    G4ThreeVector trans10 = G4ThreeVector(0, 4.0*cm - esp_aluminio, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor);
    G4ThreeVector trans11 = G4ThreeVector(0, -0.75*cm + 0.25*espeAra, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor + 0.5*(b_maior - a_maior));
    G4ThreeVector trans12 = G4ThreeVector(0, -0.75*cm - 0.25*espeAra, -0.5*(h_b + espessura_menor)+0.5*esp + a_menor + 0.5*(b_maior - a_maior));

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@




    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ SETANDO PROPRIEDADES OPTICAS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@





    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONSTRUINDO O MUNDO E UM ENVELOPE @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


    G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

    //characterizing the world
    G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        Vacuum,           //its material
                        "World");            //its name
    //plot of the world
    G4VPhysicalVolume* physWorld = new G4PVPlacement(   0,                     //no rotation
                                                        G4ThreeVector(),       //at (0,0,0)
                                                        logicWorld,            //its logical volume
                                                        "World",               //its name
                                                        0,                     //its mother  volume
                                                        true,                  //no boolean operation
                                                        0,                     //copy number
                                                        fCheckOverlaps);       //overlaps checking

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CRIANDO O CRIOSTATO @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //criando a parte externa do criostato

    //("nome", raio interno, raio externo, segment angles)
    G4Tubs* Base_maior = new G4Tubs("base_maior", 0, 0.5*R_a, 0.5*espessura_maior, 0.0*deg, 360*deg); //disco base)
    G4Tubs* Criostato1_ext = new G4Tubs("criostato1_ext", 0.5*R_a, 0.5*R_b, 0.5*H_b, 0.0*deg, 360*deg);

    G4UnionSolid* Criostato_ext = new G4UnionSolid("criostato_ext", Criostato1_ext, Base_maior, 0, trans1);


    G4LogicalVolume* logicalCriostatoExt =
    new G4LogicalVolume(Criostato_ext,         //its solid
                        StainlessSteel,          //its material
                        "criostato_ext");

    G4VPhysicalVolume* physCriostatoExt = new G4PVPlacement(0,                       //no rotation
                                                            pos1,                    //at position
                                                            logicalCriostatoExt,     //its logical volume
                                                            "criostatoExt",          //its name
                                                            logicWorld,              //its mother  volume
                                                            true,                    //no boolean operation
                                                            0,                       //copy number
                                                            fCheckOverlaps);

    fScoringVolume = logicalCriostatoExt;

    //criando a parte interna do criostato e colocando ele dentro da parte externa


    G4Tubs* Base_menor = new G4Tubs("base_menor", 0, 0.5*r_a, 0.5*espessura_menor, 0.0*deg, 360*deg); //disco basa
    G4Tubs* Criostato1_int = new G4Tubs("criostato1_int", 0.5*r_a, 0.5*r_b, 0.5*h_b, 0.0*deg, 360*deg);

    G4UnionSolid* Criostato_int = new G4UnionSolid("criostato_int", Criostato1_int, Base_menor, 0, trans2);


    G4LogicalVolume* logicalCriostatoInt =
    new G4LogicalVolume(Criostato_int,         //its solid
                        StainlessSteel,          //its material
                        "criostato_int");


    G4VPhysicalVolume* physCriostatoInt = new G4PVPlacement(0,                       //no rotation
                                                            pos2,                    //at position
                                                            logicalCriostatoInt,     //its logical volume
                                                            "criostatoInt",          //its name
                                                            logicWorld,              //its mother  volume
                                                            true,                    //no boolean operation
                                                            0,                       //copy number
                                                            fCheckOverlaps);

    fScoringVolume = logicalCriostatoInt;

    //@@@@@@@@@@@@@@@@@@@@@@@@@@ CRIANDO O APARATO DO ARAPUCA @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //cilindros das bases
    G4Tubs* ARAbase1 = new G4Tubs("arabase1", 0, 0.5*R, 0.5*esp, 0.0*deg, 360*deg);

    G4LogicalVolume* logicalARAbase1 =
    new G4LogicalVolume(ARAbase1,         //its solid
                        PVC,          //its material
                        "arabase1");


    G4VPhysicalVolume* physARAbase1 = new G4PVPlacement(0,                       //no rotation
                                                        pos3,                    //at position
                                                        logicalARAbase1,         //its logical volume
                                                        "arapucabase1",          //its name
                                                        logicWorld,              //its mother  volume
                                                        true,                    //no boolean operation
                                                        0,                       //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logicalARAbase1;

 /*
    G4Tubs* ARAbase2 = new G4Tubs("arabase2", 0, 0.5*R, 0.5*esp, 0.0*deg, 360*deg);

    G4LogicalVolume* logicalARAbase2 =
    new G4LogicalVolume(ARAbase2,         //its solid
                        PVC,   //its material
                        "arabase2");

    G4VPhysicalVolume* physARAbase2 = new G4PVPlacement(0,                       //no rotation
                                                        pos4,                    //at position
                                                        logicalARAbase2,         //its logical volume
                                                        "arapucabase2",          //its name
                                                        logicWorld,              //its mother  volume
                                                        true,                    //no boolean operation
                                                        0,                       //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logicalARAbase2;
 */
    //onde vai ficar o arapuca

    G4Box* ArapucaExt = new G4Box("ArapucaExt", 0.5*b_maior, 0.5*espeAra, 0.5*b_menor);
    G4Box* ArapucaInt = new G4Box("ArapucaInt", 0.5*a_maior, 0.5*(espeAra + 1.0*cm), 0.5*a_menor);

    G4SubtractionSolid* Arapuca = new G4SubtractionSolid("arapuca", ArapucaExt, ArapucaInt, 0, trans3);

    G4RotationMatrix* rotacao1 = new G4RotationMatrix();
    rotacao1->rotateY(90.*deg);
    rotacao1->rotateX(0.*deg);
    rotacao1->rotateZ(0.*deg);

    G4LogicalVolume* logicalARApuca =
    new G4LogicalVolume(Arapuca,         //its solid
                        Teflon,   //its material
                        "LogArapuca");

    G4VPhysicalVolume* physARApuca = new G4PVPlacement(rotacao1,                   //no rotation
                                                        pos5,               //at position
                                                        logicalARApuca,     //its logical volume
                                                        "arapuca",          //its name
                                                        logicWorld,         //its mother  volume
                                                        true,               //no boolean operation
                                                        0,                  //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logicalARApuca;

    //detector arapuca

    //face para frente do detector
    G4Box* Detector1 = new G4Box("detector1", 0.5*a_maior, 0.25*(espeAra), 0.5*a_menor);


    G4LogicalVolume* logicalDetector1 =
    new G4LogicalVolume(Detector1,         //its solid
                        Teflon,   //its material
                        "LogDetector1");

    G4VPhysicalVolume* physDetector1 = new G4PVPlacement(rotacao1,                   //no rotation
                                                        pos11,               //at position
                                                        logicalDetector1,     //its logical volume
                                                        "detector1",          //its name
                                                        logicWorld,         //its mother  volume
                                                        true,               //no boolean operation
                                                        0,                  //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logicalDetector1;

    //face para fora do detector

    G4Box* Detector2 = new G4Box("detector2", 0.5*a_maior, 0.25*(espeAra), 0.5*a_menor);


    G4LogicalVolume* logicalDetector2 =
    new G4LogicalVolume(Detector2,         //its solid
                        Teflon,   //its material
                        "LogDetector2");

    G4VPhysicalVolume* physDetector2 = new G4PVPlacement(rotacao1,                   //no rotation
                                                        pos12,               //at position
                                                        logicalDetector2,     //its logical volume
                                                        "detector2",          //its name
                                                        logicWorld,         //its mother  volume
                                                        true,               //no boolean operation
                                                        0,                  //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logicalDetector2;



    //fonte alpha

    G4Box* SuporteAlphaBox = new G4Box("Suporte", 0.5*largura, 0.5*altura, 0.5*esp_alpha);

    G4Tubs* Buraco = new G4Tubs("buraco", 0, 0.5*diam_buraco, 0.5*(esp_alpha + 0.01*cm), 0.0*deg, 360*deg);

    G4RotationMatrix* rotacao2 = new G4RotationMatrix();
    rotacao2->rotateY(0.*deg);
    rotacao2->rotateX(90.*deg);
    rotacao2->rotateZ(180.*deg);

    G4SubtractionSolid* Alpha = new G4SubtractionSolid("SuporteAlpha",  SuporteAlphaBox, Buraco, 0, G4ThreeVector(0,0,0));

    G4LogicalVolume* logicalAlpha =
    new G4LogicalVolume(Alpha,         //its solid
                        PVC,   //its material
                        "logAlpha");

    G4VPhysicalVolume* physAlpha = new G4PVPlacement(rotacao2,                       //no rotation
                                                        pos7,                 //at position
                                                        logicalAlpha,         //its logical volume
                                                        "alpha",              //its name
                                                        logicWorld,           //its mother  volume
                                                        true,                 //no boolean operation
                                                        0,                    //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logicalAlpha;

    //fonte alfa propriamente dita

    G4Tubs* alumino_alpha = new G4Tubs("alumino_alpha", 0, 0.5*(diam_buraco - 0.001*cm), 0.5*esp_aluminio, 0.0*deg, 360*deg); //aqui controle do visualizador (bug)

    G4LogicalVolume* logical_aluminio_alpha =
    new G4LogicalVolume(alumino_alpha,         //its solid
                        Aluminium,   //its material
                        "logAluminio");

    G4VPhysicalVolume* phys_Alpha_aluminio = new G4PVPlacement(rotacao2,                       //no rotation
                                                        pos6,                 //at position
                                                        logical_aluminio_alpha,         //its logical volume
                                                        "aluminio_alpha2",              //its name
                                                        logicWorld,           //its mother  volume
                                                        true,                 //no boolean operation
                                                        0,                    //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logical_aluminio_alpha;

    //objeto para tampar a fonte alfa na parte traseira do dispositivo

    G4Tubs* tampa = new G4Tubs("tampa", 0, 0.5*diam_buraco, 0.5*esp_aluminio + 1.0*mm, 0.0*deg, 360*deg);

    G4LogicalVolume* logical_tampa =
    new G4LogicalVolume(tampa,         //its solid
                        PVC,   //its material
                        "logTampa");

    G4VPhysicalVolume* phys_Tampa = new G4PVPlacement(rotacao2,                       //no rotation
                                                        pos10,                 //at position
                                                        logical_tampa,         //its logical volume
                                                        "tampa",              //its name
                                                        logicWorld,           //its mother  volume
                                                        true,                 //no boolean operation
                                                        0,                    //copy number
                                                        fCheckOverlaps);

    fScoringVolume = logical_tampa;



    //preenchendo de nitrogenio liquido o espaço entre os cilindros


    G4Tubs* fill_step1 = new G4Tubs("fill_step1", 0.5*(r_b + 0.01*cm), 0.5*(R_a - 0.01*cm), 0.5*h_b, 0.0*deg, 360*deg);

    G4LogicalVolume* logical_NL2 =
    new G4LogicalVolume(fill_step1,         //its solid
                        NL2,   //its material
                        "logNL2");

    G4VPhysicalVolume* physNL2 = new G4PVPlacement(0,                       //no rotation
                                                    pos8,                   //at position
                                                    logical_NL2,            //its logical volume
                                                    "nitrogen",             //its name
                                                    logicWorld,             //its mother  volume
                                                    true,                   //no boolean operation
                                                    0,                      //copy number
                                                    fCheckOverlaps);

    fScoringVolume = logical_NL2;


    //preenchendo o cilindo interno de argonio líquido mas deixando o espaço para as geometrias internas p/ ñ ocorrer overlaps

    G4Tubs* fill_step2 = new G4Tubs("fill_step2", 0, 0.5*(r_a - 0.01*cm), 0.5*(h_b - espessura_menor), 0.0*deg, 360*deg);

    G4SubtractionSolid* sub1 = new G4SubtractionSolid("sub1", fill_step2, ARAbase1, 0, trans5);
//  G4SubtractionSolid* sub2 = new G4SubtractionSolid("sub2", sub1, ARAbase2, 0, trans6);
    G4SubtractionSolid* sub3 = new G4SubtractionSolid("sub3", sub1, Arapuca, rotacao1, trans7);
    G4SubtractionSolid* sub4 = new G4SubtractionSolid("sub4", sub3, Detector1, rotacao1, trans11);
    G4SubtractionSolid* sub5 = new G4SubtractionSolid("sub5", sub4, Detector2, rotacao1, trans12);
    G4SubtractionSolid* sub6 = new G4SubtractionSolid("sub6", sub5, alumino_alpha, rotacao2, trans8);
    G4SubtractionSolid* sub7 = new G4SubtractionSolid("sub7", sub6, tampa, rotacao2, trans10);
    G4SubtractionSolid* sub8 = new G4SubtractionSolid("sub8", sub7, Alpha, rotacao2, trans9);



    G4MaterialPropertiesTable* prop_argonio = new G4MaterialPropertiesTable();

    prop_argonio->AddProperty("FASTCOMPONENT", Scint_PP, Scint_FAST, NUMENTRIES);
    prop_argonio->AddProperty("SLOWCOMPONENT", Scint_PP, Scint_SLOW, NUMENTRIES);

    prop_argonio->AddConstProperty("SCINTILLATIONYIELD", 3978./MeV);
    prop_argonio->AddConstProperty("RESOLUTIONSCALE", 1.);


    prop_argonio->AddConstProperty("FASTTIMECONSTANT",  6.*ns);
    prop_argonio->AddConstProperty("SLOWTIMECONSTANT", 1.8*us);
    prop_argonio->AddConstProperty("YIELDRATIO", 0.77); // As here

    prop_argonio->AddProperty("RINDEX", PhotonEnergyN, n_lAr, nEntries);
    prop_argonio->AddProperty("ABSLENGTH", PhotonEnergy2, l_lAr, nEntries);

    LAr->SetMaterialPropertiesTable(prop_argonio);

    LAr->GetIonisation()->SetBirksConstant(0.0000001*mm/MeV);

    G4LogicalVolume* logical_LAr =
    new G4LogicalVolume(sub8,         //its solid
                        LAr,   //its material
                        "logLAr");

    G4VPhysicalVolume* physLAr = new G4PVPlacement(0,                       //no rotation
                                                    pos9,                   //at position
                                                    logical_LAr,            //its logical volume
                                                    "argon",             //its name
                                                    logicWorld,             //its mother  volume
                                                    true,                   //no boolean operation
                                                    0,                      //copy number
                                                   fCheckOverlaps);

    fScoringVolume = logical_LAr;


    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //propriedades para o pvc

    G4MaterialPropertiesTable* prop_pvc = new G4MaterialPropertiesTable();

    prop_pvc->AddProperty("ABSLENGTH", PhotonEnergy, abs_pvc, NUMENTRIES);

    PVC->SetMaterialPropertiesTable(prop_pvc);

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ PROPRIEDADES OPTICAS  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


    G4MaterialPropertiesTable* inox_pro = new G4MaterialPropertiesTable();

    inox_pro->AddProperty("REFLECTIVITY", Scint_PP, Reflec_inox, NUMENTRIES);

    //StainlessSteel->SetMaterialPropertiesTable(inox_pro);

    G4OpticalSurface *Inox_sur = new G4OpticalSurface("Inox_sur");

    Inox_sur->SetModel(glisur);//
    Inox_sur->SetType(dielectric_metal);
    Inox_sur->SetFinish(polished);

    Inox_sur->SetMaterialPropertiesTable(inox_pro);


    //new G4LogicalBorderSurface("InoxProperty", physLAr, physCriostatoInt, Inox_sur);
    new G4LogicalSkinSurface ("InoxProperty", logicWorld, Inox_sur);






    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  SETANDO CORES @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //red, green, blue
    G4VisAttributes* criostato_externo = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
    criostato_externo->SetForceSolid(false);
    logicalCriostatoExt->SetVisAttributes(criostato_externo);


    G4VisAttributes* criostato_interno = new G4VisAttributes(G4Colour(0.87, 0.87, 0.87, 0.2));
    criostato_interno->SetForceSolid(false);
    logicalCriostatoInt->SetVisAttributes(criostato_interno);

    G4VisAttributes* ARAbase_1 = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    logicalARAbase1->SetVisAttributes(ARAbase_1);

//     G4VisAttributes* ARAbase_2 = new G4VisAttributes(G4Colour(0.8, 0.3, 0.0));
//     logicalARAbase2->SetVisAttributes(ARAbase_2);
//
    G4VisAttributes* Arapuca_color = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
    logicalARApuca->SetVisAttributes(Arapuca_color);

    G4VisAttributes* alpha_color = new G4VisAttributes(G4Colour(0.41, 0.41, 0.41, 0.7));
    logicalAlpha->SetVisAttributes(alpha_color);

    G4VisAttributes* NL2_color = new G4VisAttributes(G4Colour(0., 0., 0., 0.1));
    logical_NL2->SetVisAttributes(NL2_color);

    G4VisAttributes* LAr_color = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0, 0.1));
    logical_LAr->SetVisAttributes(LAr_color);

    G4VisAttributes* Aluminium_color = new G4VisAttributes(G4Colour(0.66, 0.66, 0.66, 0.8));
    logical_aluminio_alpha->SetVisAttributes(Aluminium_color);

    G4VisAttributes* Detector_color1 = new G4VisAttributes(G4Colour(0.80, 0.36, 0.36, 0.4));
    logicalDetector1->SetVisAttributes(Detector_color1);

    G4VisAttributes* Detector_color2 = new G4VisAttributes(G4Colour(0.4, 1.0, 0.7, 0.4));
    logicalDetector2->SetVisAttributes(Detector_color2);

    G4VisAttributes* Tampa_color = new G4VisAttributes(G4Colour(1.0, 0, 0));
    logical_tampa->SetVisAttributes(Tampa_color);





    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     The End    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    return physWorld;

}

//
//================================================================================
