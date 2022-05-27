/*Created by Rafaela Ramos Sarmento
for more information contact me: rramos@ifi.unicamp.br
last update: 17/07/2019
*/
#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "B1Constantes.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

//================================================================================

B1EventAction::B1EventAction() {


}

//================================================================================

B1EventAction::~B1EventAction() {}

//================================================================================

void B1EventAction::BeginOfEventAction(const G4Event* /*event*/) {

    nOfReflections = 0;
    nOfDetections = 0;

}

//================================================================================

void B1EventAction::EndOfEventAction(const G4Event* event) {

    // Print number of reflections
    if (IMPRIMIR_EVENTOS == false) {
        G4cout << "Number of reflections [Event] = " << nOfReflections << G4endl;
        G4cout << "Number of detections [Event] = " << nOfDetections << G4endl;
//      G4out <<Numero de fotons detectados [Event] = " <<
    }

    // Print per event (modulo n)
    //
    G4int eventID = event->GetEventID();
    G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
        G4cout << "---> End of event: " << eventID << G4endl;
    }

}

//================================================================================
