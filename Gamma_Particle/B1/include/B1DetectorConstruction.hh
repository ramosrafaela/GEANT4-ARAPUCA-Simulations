/// \file B1DetectorConstruction.hh
/// \brief Definition of the B1DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VSolid.hh"
#include "G4BooleanSolid.hh"
#include "G4SubtractionSolid.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4SubtractionSolid;
/// Detector construction class to define materials and geometry.

class B1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    B1DetectorConstruction(G4double RIndex);
    virtual ~B1DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();


    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

    G4double Refr_Index;
  protected:
    G4LogicalVolume*  fScoringVolume;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif