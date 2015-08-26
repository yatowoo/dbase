#include "CbmStsDbQaContFact.h"

#include "CbmStsDbQaSensorPar.h"         // 
#include "CbmStsDbQaIvPar.h"             // 
#include "CbmStsDbQaPar.h"             //
#include "FairDbParSet.h"                // for FairParSet
#include "FairRuntimeDb.h"              // for FairRuntimeDb

#include "Riosfwd.h"                    // for ostream
#include "TList.h"                      // for TList
#include "TString.h"                    // for TString

#include <string.h>                     // for strcmp, NULL
#include <iostream>                     // for operator<<, basic_ostream, etc

using namespace std;

ClassImp(CbmStsDbQaContFact);

static CbmStsDbQaContFact gCbmStsDbQaContFact;

CbmStsDbQaContFact::CbmStsDbQaContFact()
{
  // Constructor (called when the library is loaded)
  fName="CbmStsDbQaContFact";
  fTitle="Cbm Sts QA factory for parameter containers";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void CbmStsDbQaContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers.*/

  FairContainer* p1 = new FairContainer("CbmStsDbQaSensorPar", "Cbm Qa Global Parameters",
                                        "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");
  containers->Add(p1);

  FairContainer* p2 = new FairContainer("CbmStsDbQaIvPar", "Cbm Qa IV Parameters",
                                        "TestDefaultContext");
  p2->addContext("TestDefaultContext");
  containers->Add(p2);

  FairContainer* p3 = new FairContainer("CbmStsDbQaPar", "Cbm Qa Parameters",
                                        "TestDefaultContext");
  p3->addContext("TestDefaultContext");
  containers->Add(p3);

}

FairParSet* CbmStsDbQaContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */

  const char* name=c->GetName();
  cout << "-I-CbmStsDbQaContFact::createContainer " << name << endl;
  FairDbParSet* p=NULL;

  if (strcmp(name,"CbmStsDbQaSensorPar")==0) {
    p=new CbmStsDbQaSensorPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  } else if (strcmp(name,"CbmStsDbQaIvPar")==0) {
    p=new CbmStsDbQaSensorPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
   } else if (strcmp(name,"CbmStsDbQaPar")==0) {
    p=new CbmStsDbQaPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    // Set Arguments needed for SQL versioning managment
    p->SetVersion(0);
    p->SetDbEntry(0);
    p->SetLogTitle(name);
  }

  return p;
}
