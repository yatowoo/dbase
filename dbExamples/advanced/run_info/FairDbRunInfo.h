#ifndef FAIRDBRUNINFO_H
#define FAIRDBRUNINFO_H

#include "FairDbParSet.h"          // for FairParGenericSet
#include "DataType.h"                    // for EDataType::kData
#include "ValCondition.h"                 // for ValCondition
#include "ValTimeStamp.h"               // for ValTimeStamp
#include "db_detector_def.h"            // for Detector, etc

#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

#include "TObjArray.h"                  // Store for vertices 
#include "TVector3.h"                   // Vertex
#include "TGeoArb8.h"                   // Trap
#include "FairDbWriter.h"
#include "FairDbReader.h"

#define NCHMAX 500

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;



class FairDbRunInfo : public FairDbParSet
{

using TObject::Compare;

  public :
  FairDbRunInfo (const char* name="FairDbRunInfo",
				 const char* title="Run Info",
				 const char* context="TestDefaultContext",
				 Bool_t own=kTRUE);
    virtual ~FairDbRunInfo(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    /** Accessor functions **/
    Int_t GetComboNo()       const {return fCompId;}
    Int_t GetCompId()   const {return fCompId;}
    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}

    void   SetRunId(Int_t runId) {fRunId = runId;}  
    void   SetRunTime(ValTimeStamp run_time) {fRunTime = run_time;}  
    Int_t  GetRunId() {return fRunId;}
    ValTimeStamp  GetRunTime() {return fRunTime;}  

    UInt_t GetIndex(UInt_t /*def*/) const { return fRunId; }

    Bool_t Compare(const FairDbRunInfo& that ) const; 

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new FairDbRunInfo();
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
      return ValCondition(FairDbDetector::kUnknown,
                          DataType::kData,
                          ValTimeStamp(rid));
    }

    // SQL-IO Meta-Class Getters
    FairDbReader<FairDbRunInfo>* GetParamReader();
    FairDbWriter<FairDbRunInfo>* GetParamWriter();
    FairDbWriter<FairDbRunInfo>* ActivateWriter(Int_t rid);
    
    ValTimeStamp GetRunTime(Int_t runId);
    Bool_t Commit();
    
    Int_t        GetSeqNo() const
    { return CalcUniqueSeqNo(fRunId); }
    static Int_t  CalcUniqueSeqNo(Int_t run);
    
 private:
    // Data Parameters
    Int_t fCompId;
    Int_t fRunId;
    ValTimeStamp fRunTime;   
    

    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<FairDbRunInfo>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<FairDbRunInfo>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(FairDbRunInfo,1); // FairDbRunInfo Parameter Container example
};



inline  FairDbReader<FairDbRunInfo>* FairDbRunInfo::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<FairDbRunInfo>();
    return fParam_Reader;
  }
}

inline FairDbWriter<FairDbRunInfo>* FairDbRunInfo::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<FairDbRunInfo>();
    return fParam_Writer;
  }
}

#endif /* !FairDbRunInfo_H*/
