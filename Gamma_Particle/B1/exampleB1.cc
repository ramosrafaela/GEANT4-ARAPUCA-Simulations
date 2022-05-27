#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"

//#include "B1PhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4ScoringManager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//double contadorPhotons=0;
// ============================================================================

extern G4bool flag_generate = true;

int main(int argc,char** argv) {
    
    // Seed the random number generator manually
//    G4long myseed = 345354;
    G4long myseed = 3453540;
    CLHEP::HepRandom::setTheSeed(myseed);

    // ============================================================================
    // Run manager
    
    G4double RIndex = 1;
    if (argc==2) {
	RIndex = atof(argv [1]);
    }
    if (argc==3) {
	RIndex = atof(argv [2]);
    }

    G4RunManager* runManager = new G4RunManager;
    G4ScoringManager* scoringManager = G4ScoringManager::GetScoringManager();
    
    B1DetectorConstruction* detConstruction = new B1DetectorConstruction(RIndex);
    runManager->SetUserInitialization(detConstruction);
    
    //runManager->SetUserInitialization(new B1PhysicsList);
    
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option2());
    
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    physicsList->RegisterPhysics(opticalPhysics);
    
    opticalPhysics->Configure(kCerenkov, true);
    //opticalPhysics->SetCerenkovStackPhotons(false);
    opticalPhysics->Configure(kScintillation, true);
    physicsList->DumpList();

    runManager->SetUserInitialization(physicsList);
    
    B1ActionInitialization* actionInitialization
    = new B1ActionInitialization(detConstruction);
    
    runManager->SetUserInitialization(actionInitialization);
    
    








    #ifdef G4VIS_USE
    // visualization manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    #endif
    
    // Initialize G4 kernel
    runManager->Initialize();
    
    // ============================================================================
    // User Interface
    
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Define UI session for interactive mode
    if (argc < 2) {
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
      ui->SessionStart();
      delete ui;
#endif
    } else if (argc ==2) {
      
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
      
    }
    
    // ============================================================================
    // Delete
#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    return 0;
}

// ============================================================================
