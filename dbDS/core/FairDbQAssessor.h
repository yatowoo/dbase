#ifndef FAIRDBQASSESSOR_H
#define FAIRDBQASSESSOR_H

#include "Rtypes.h"
#include "FairDbFileSpotter.h"

class FairDbADS;
class FairDbStatement;

class FairDbQAssessor {
 public:
  FairDbQAssessor(FairDbADS* ads);
  virtual ~FairDbQAssessor();

  // State testing member functions
  Bool_t RunAllTests();
  Bool_t TestExportImport();
  Bool_t TestGlobaliseSeqNo();

 private:  
  Bool_t PrepareDb();

  FairDbFileSpotter  fFileLocater; 
  FairDbADS* fDbTask;      
  FairDbStatement* fStatement0;  
  FairDbStatement* fStatement1;  

  ClassDef(FairDbQAssessor,0)     // QA assessor
  
};

#endif // FAIRDBQASSESSOR
