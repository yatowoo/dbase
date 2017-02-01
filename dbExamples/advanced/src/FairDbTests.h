#ifndef FAIRDBTESTS_H
#define FAIRDBTESTS_H


#include "FairParTSQLIo.h"
#include "FairDbParSet.h"
#include "DataType.h"                    // for EDataType::kData
#include "ValCondition.h"                 // for ValCondition
#include "ValTimeStamp.h"               // for ValTimeStamp
#include "db_detector_def.h"            // for FairDbDetector, etc

#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string


#include "FairDbExPar1.h"
#include "FairDbExPar2.h"


class FairDbTests
{

public:

          FairDbTests();
  virtual ~FairDbTests();

  
  Bool_t RunAllExamples();
  Bool_t RunExample(Int_t i_ex);

  // State changing member functions
  Bool_t test_caching();
  
  
 private:
  
  // Tests
  enum { kNUMTESTS = 1 };
  
   
  void PrepareDatabases();
  void CleanDatabases(Bool_t printWarnings = kTRUE);
  
 
  
  // Number of test failures
  Int_t fNumFail;             
  // Number of test successes
  Int_t fNumPass;           
  // Test Dbs Pool
  Bool_t fGoodPool;       
  // SQL IO
  FairParTSQLIo * fSql_io; 

  ClassDef(FairDbTests,0)
  
};

#endif  // FAIRDBTESTS
