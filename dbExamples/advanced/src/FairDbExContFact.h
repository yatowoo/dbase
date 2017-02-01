#ifndef FAIRDBEXCONTFACT_H
#define FAIRDBEXCONTFACT_H

#include "FairContFact.h" 
#include "Rtypes.h"    

class FairDbParSet;

class FairDbExContFact : public FairContFact
{
  public:
    FairDbExContFact();
    ~FairDbExContFact() {}
    FairParSet* createContainer(FairContainer* c);
  private:
    void setAllContainers();

    ClassDef( FairDbExContFact,0); // Factory for parameter containers
};

#endif  /* !FAIRDBEXCONTFACT_H */
