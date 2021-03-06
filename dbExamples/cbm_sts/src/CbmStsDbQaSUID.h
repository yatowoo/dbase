#ifndef CBMSTSDBQASUID_H
#define CBMSTSDBQASUID_H

#include "FairDbParSet.h"          // for FairParGenericSet
#include "DataType.h"                    // for EDataType::kData
#include "ValCondition.h"                 // for ValCondition
#include "ValTimeStamp.h"               // for ValTimeStamp
#include "db_detector_def.h"            // for Detector, etc

#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

#include "TObjArray.h"                  // Store for vertices 
#include "TNamed.h"
#include "TVector3.h"                   // Vertex
#include "TGeoArb8.h"                   // Trap
#include "FairDbWriter.h"
#include "FairDbReader.h"

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;



class CbmStsDbQaSUID : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaSUID (const char* name="STSQASUID",
				 const char* title="Sensor uid table",
				 const char* context="DefaultContext",
				 Bool_t own=kTRUE);
    virtual ~CbmStsDbQaSUID(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    /** Accessor functions **/
    Int_t GetComboNo()       const {return fCompId;}
    Int_t GetCompId()   const {return fCompId;}
    void  SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}

    void  SetVendor(string v)  { fVendor = v;}
    void  SetBatchId(string id) { fBatchId = id;}  
    void  SetBatchTime(string batch_time) { fBatchTime = batch_time;}  
    void  SetWaferId(Int_t id)  { fWaferId = id;}
    void  SetSensorId(Int_t id) { fSensorId = id;}  

    string GetVendor()    { return fVendor;}
    string GetBatchId()   { return fBatchId;}
    string GetBatchTime() { return fBatchTime;}
    Int_t  GetWaferId()   { return fWaferId;}
    Int_t  GetSensorId()  { return fSensorId;}
    
    // Random Access via UID
    UInt_t GetIndex(UInt_t /*def*/) const { return fCompId; }
    Bool_t Compare(const CbmStsDbQaSUID& that ) const;
    Bool_t IsEqual(const TObject* that ) const; 

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaSUID();
    }

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;
    // Global IO using run_id
    virtual void fill(UInt_t rid);
    virtual void store(UInt_t rid);
    void AddSensor(const string& vendor, const Int_t& batch_id, const ValTimeStamp& btime,
                   const Int_t& wafer_id , const Int_t& sensor_id ){;}

    // Validity frame definition
    virtual ValCondition GetContext(UInt_t rid){
      return ValCondition(FairDbDetector::kSts,
                          DataType::kData,
                          ValTimeStamp(rid));
    }

    // Create Db Table
    void  CreateDbTable(Int_t db_entry);
    string CreateStringUID();
    
    // SQL-IO Meta-Class Getters
    FairDbReader<CbmStsDbQaSUID>* GetParamReader();
    FairDbWriter<CbmStsDbQaSUID>* GetParamWriter();
    FairDbWriter<CbmStsDbQaSUID>* ActivateWriter(Int_t rid=0);
    


 private:
    // Data Parameters
    Int_t  fCompId;    
    string fVendor;
    string fBatchId;
    string fBatchTime;   
    Int_t  fWaferId;
    Int_t  fSensorId;
    
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaSUID>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaSUID>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaSUID,1); // CbmStsDbQaSUID Parameter Container example
};



inline  FairDbReader<CbmStsDbQaSUID>* CbmStsDbQaSUID::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaSUID>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaSUID>* CbmStsDbQaSUID::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaSUID>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBSUID_H*/
