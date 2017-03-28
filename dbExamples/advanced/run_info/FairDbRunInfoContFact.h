#ifndef FAIRDBRUNINFOCONTFACT_H
#define FAIRDBRUNINFOCONTFACT_H

#include "FairContFact.h"               // for FairDbRunInfoContFact, etc

#include "Rtypes.h"                     // for FairDbRunInfoContFact::Class, etc

class FairParSet;

class FairDbRunInfoContFact : public FairContFact
{
  public:
    FairDbRunInfoContFact();
    ~FairDbRunInfoContFact() {}
    FairParSet* createContainer(FairContainer* c);
  private:
    void setAllContainers();

    ClassDef( FairDbRunInfoContFact,0); // Factory for parameter containers
};

#endif  /* !FairDbRunInfoCONTFACT_H */
