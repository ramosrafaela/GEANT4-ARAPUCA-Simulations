#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class B1DetectorConstruction;

/// Action initialization class.
class B1ActionInitialization : public G4VUserActionInitialization {
    
  public:
    B1ActionInitialization(B1DetectorConstruction*);
    virtual ~B1ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    B1DetectorConstruction* fDetConstruction;
    
};

#endif

    