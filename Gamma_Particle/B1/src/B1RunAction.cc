/*Create by Rafaela Ramos Sarmento
abstract: create the histrogram and TTree's
for more information contact me: rramos@ifi.unicamp.br
last update: 19/02/2020
*/


#include "G4Timer.hh"

#include "B1RunAction.hh"
#include "B1Constantes.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "g4root.hh"
//#include "g4csv.hh"
#include <fstream>

// #include "G4NeutronHPManager.hh"

using namespace std;

//================================================================================

B1RunAction::B1RunAction() {

    timer = new G4Timer;
    nOfReflections_Total = 0;
    nOfDetections_Total = 0;
    TOF_Detections_Total = 0;
}

//================================================================================

B1RunAction::~B1RunAction() {

    delete timer;

}

//================================================================================

void B1RunAction::BeginOfRunAction(const G4Run* aRun) {

    // G4NeutronHPManager::GetInstance()->SetVerboseLevel(0);


    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4cout << "Using " << analysisManager->GetType() << " analysis manager." << G4endl;
    timer->Start();

    analysisManager->OpenFile("OutPut");
    analysisManager->SetVerboseLevel(1);


    // // Indica o id do primeiro histograma criado -- default = 0
    analysisManager->SetFirstHistoId(1);
    // // Cria histogramas    -- (nome, titulo, nbins, xmin, xmax)
    analysisManager->CreateH1 ("h1","Energy of scintillation photon",2000,0,20); //1
    analysisManager->CreateH1 ("h2","Muon Spectrum",100,-1.0, 6.0); //2
    analysisManager->CreateH1 ("h3","Photons/Muon",250,0,7000); //3
    // // Indica o id da primeira ntuple criada -- default = 0

     analysisManager->SetFirstNtupleId(1);
    // //Declara ntuples
    analysisManager->CreateNtuple("t1", "data");
    analysisManager->CreateNtupleDColumn("detPhotons1");
    analysisManager->CreateNtupleDColumn("detPhotons2");
    analysisManager->CreateNtupleDColumn("detPhotons");
    analysisManager->CreateNtupleDColumn("muonEnergy");
    analysisManager->CreateNtupleDColumn("axisX");
    analysisManager->CreateNtupleDColumn("axisY");
    analysisManager->CreateNtupleDColumn("axisZ");
    analysisManager->CreateNtupleDColumn("theta_distribuicao");
    analysisManager->FinishNtuple();

}

//================================================================================

void B1RunAction::EndOfRunAction(const G4Run* aRun) {
    // G4cout << "End of run." << G4endl;
    //G4cout << "Number of total reflections [Run] = " << nOfReflections_Total << G4endl;
    timer->Stop();
    // G4cout << "Eficiency = " << (G4double)nOfDetections_Total / aRun->GetNumberOfEvent() << G4endl;
    // G4cout << "Number of detected Photons = " << (G4double)nOfDetections_Total << G4endl;

    // G4cout << "Average Time of flight = "  << (G4double)TOF_Detections_Total / (G4double)nOfDetections_Total  << " ns" << G4endl;

    // G4cout << "Number of event = " << aRun->GetNumberOfEvent() << G4endl;


    if (PRINT_FILE == 1)
    {
       std::ofstream OutPut;

       OutPut.open(TXT_FILE, std::ios::app);
        //OutPut<<"Eficiency: " <<(G4double)nOfDetections_Total / aRun->GetNumberOfEvent()<<endl<<endl;
       OutPut<< setprecision(4) << fixed << (G4double)nOfDetections_Total <<endl;
       OutPut.close();
   }

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    delete G4AnalysisManager::Instance();

}

//================================================================================
