#ifndef B1SteppingAction_h
#define B1SteppingAction_h 1

#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"

class B1EventAction;
class B1RunAction;
class B1PrimaryGeneratorAction;

extern G4bool flag_generate;
//Stepping action class
class B1SteppingAction : public G4UserSteppingAction {

   public:
       B1SteppingAction(B1EventAction *eventAction, B1RunAction* runAction, B1PrimaryGeneratorAction* PrimaryGeneratorAction);
	virtual ~B1SteppingAction ();

	virtual void UserSteppingAction (const G4Step*);

   private:
    G4int fEventNumber;
    B1EventAction* fEventAction;
    B1RunAction* fRunAction;
    B1PrimaryGeneratorAction* fPrimaryGeneratorAction;
    G4double contadorPhotons1;
    G4double contadorPhotons2;
    G4double contadorEnergia;
    G4double myEnergy;
    G4double x;
    G4double y;
    G4double z;
    G4int teste;
    G4bool firstStep;
    G4double initial_energy;
    G4double temp_edep;
    G4double temp_ID;
    G4bool isAlpha;
   
    G4bool firstOfAll = true;
};
#endif
