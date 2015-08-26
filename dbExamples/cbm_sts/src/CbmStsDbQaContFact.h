#ifndef CBMSTSDBQACONTFACT_H
#define CBMSTSDBQACONTFACT_H

#include "FairContFact.h"               // for CbmStsDbQaContFact, etc

#include "Rtypes.h"                     // for CbmStsDbQaContFact::Class, etc

class FairParSet;

class CbmStsDbQaContFact : public FairContFact
{
  public:
    CbmStsDbQaContFact();
    ~CbmStsDbQaContFact() {}
    FairParSet* createContainer(FairContainer* c);
  private:
    void setAllContainers();

    ClassDef( CbmStsDbQaContFact,0); // Factory for Cbm Sts Qa parameter containers
};

#endif  /* !CBMSTSDBQACONTFACT_H */
