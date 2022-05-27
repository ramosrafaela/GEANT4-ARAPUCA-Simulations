/*Created by Rafaela Ramos Sarmento
abstract: this code does only the initialization
for more information contact me: rramos@ifi.unicamp.br
last update: 17/07/2019
*/
last update

#include "B1ActionInitialization.hh"
#include "B1DetectorConstruction.hh"
#include "B1EventAction.hh"

#include "B1PrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1SteppingAction.hh"

// ============================================================================

B1ActionInitialization::B1ActionInitialization
                                (B1DetectorConstruction* detConstruction)
: G4VUserActionInitialization(), fDetConstruction(detConstruction)
{;}

// ============================================================================

B1ActionInitialization::~B1ActionInitialization() {;}

// ============================================================================

void B1ActionInitialization::BuildForMaster() const {

    SetUserAction(new B1RunAction);

}

// ============================================================================

void B1ActionInitialization::Build() const {

    B1EventAction* eventAction = new B1EventAction;
    B1RunAction* runAction = new B1RunAction;
    B1PrimaryGeneratorAction* PrimaryGeneratorAction = new B1PrimaryGeneratorAction;

    SetUserAction(new B1PrimaryGeneratorAction);
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new B1SteppingAction(eventAction, runAction, PrimaryGeneratorAction));

}

// ============================================================================
