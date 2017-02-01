#ifndef FAIRDBDEXPAR3_H
#define FAIRDBDEXPAR3_H

#include "FairDbParSet.h"
#include "DataType.h"                    // for EDataType::kData
#include "ValCondition.h"                 // for ValCondition
#include "ValTimeStamp.h"               // for ValTimeStamp
#include "db_detector_def.h"            // for FairDbDetector, etc

#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

#include "FairDbWriter.h"
#include "FairDbReader.h"


using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;


class FairDbExPar3 : public FairDbParSet
{
  using TObject::Compare;
  
  public :
  FairDbExPar3 (Int_t base=0,
		  const char* name="ExPar3",
                  const char* title="Tutorial  parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
  virtual ~FairDbExPar3(void);

  enum {datasize = 50 };
  
  void clear(void);
  void   putParams(FairParamList* list);
  Bool_t getParams(FairParamList* list);
  
  void Print();
  Bool_t IsValid() const;
    
  // Add-ons: SQL descriptors for the parameter class
  virtual std::string GetTableDefinition(const char* Name = 0);
  
  virtual FairDbObjTableMap* CreateObjTableMap() const {
    FairDbExPar3* p = new FairDbExPar3();
    return dynamic_cast<FairDbObjTableMap*> (p);
  }
  
  // Atomic IO (intrinsic)
  virtual void Fill(FairDbResultPool& res_in,
		    const FairDbValRecord* valrec);
  virtual void Store(FairDbOutTableBuffer& res_out,
		     const FairDbValRecord* valrec) const;
  // Global IO using run_id
  virtual void fill(UInt_t rid);
  virtual void store(UInt_t rid);
  
  // Validity frame definition
  virtual ValCondition GetContext(UInt_t rid) {
    return ValCondition(FairDbDetector::kSts,
			DataType::kData,
			ValTimeStamp(rid));
  }
  
  // SQL-IO Meta-Class Getters
  FairDbReader<FairDbExPar3>* GetParamReader();
  FairDbWriter<FairDbExPar3>* GetParamWriter();
  
 private:
  Int_t       fData[datasize];          
  
  // Parameter Container SQL Writer Meta-Class
  FairDbWriter<FairDbExPar3>* fParam_Writer; //!
  // Parameter Container SQL Writer Meta-Class
  FairDbReader<FairDbExPar3>* fParam_Reader; //!
  // Connection Pool
  FairDbConnectionPool* fMultConn;  //!
  
  ClassDef(FairDbExPar3,0); // FairDbExPar3: Example of aggregated data type.
};


inline  FairDbReader<FairDbExPar3>* FairDbExPar3::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<FairDbExPar3>();
    return fParam_Reader;
  }
}

inline FairDbWriter<FairDbExPar3>* FairDbExPar3::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<FairDbExPar3>();
    return fParam_Writer;
  }
}

#endif /* !FAIRDBEXPAR3_H*/
