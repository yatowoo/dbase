#ifndef CBMSTSDBQAOWNERSHIPPAR_H
#define CBMSTSDBQAOWNERSHIPPAR_H

#include "FairDbParSet.h"          // for FairParGenericSet
#include "DataType.h"                    // for EDataType::kData
#include "ValCondition.h"                 // for ValCondition
#include "ValTimeStamp.h"               // for ValTimeStamp
#include "db_detector_def.h"            // for Detector, etc

#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

#include "FairDbWriter.h"
#include "FairDbReader.h"

#define MAX_M 50

using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;


class CbmStsDbQaOwnershipPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaOwnershipPar (const char* name="CbmStsDbQaOwnershipPar",
                  const char* title="Cbm Sts Ownership Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaOwnershipPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();


    Int_t GetComboNo()     const {return fCompId;}    
    Int_t GetCompId()      const {return fCompId;}
    Int_t GetUID()         const {return fUID;}
    string GetVendor()   const {return fVendor;}
    string GetType()   const {return fType;}
    string GetReticleName()   const {return fReticleName;}
    string GetYear()   const {return fYear;}
    string GetOwner()   const {return fOwner;}
    string GetLocation()   const {return fLocation;}
    

    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}
    void SetUID(Int_t x) {fUID  = x;}
    void SetVendor( string name)      {fVendor = name;}
    void SetType(string name)         {fType   = name;}
    void SetReticleName(string name)  {fReticleName = name;}
    void SetYear(string name)         {fYear = name;}
    void SetOwner(string name)        {fOwner = name;}
    void SetLocation(string name)     {fLocation = name;}
  
    
    UInt_t GetIndex(UInt_t /*def*/) const { return fUID; }

    // Logical (=) 
    Bool_t Compare(const CbmStsDbQaOwnershipPar& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaOwnershipPar();
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
    FairDbReader<CbmStsDbQaOwnershipPar>* GetParamReader();
    FairDbWriter<CbmStsDbQaOwnershipPar>* GetParamWriter();
    FairDbWriter<CbmStsDbQaOwnershipPar>* ActivateWriter(Int_t rid);
  

  private:
    // Data Parameters
    Int_t fCompId;
    Int_t fUID;
    string fVendor; 
    string fType;         
    string fReticleName; 
    string fYear; 
    string fOwner;
    string fLocation;  

    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaOwnershipPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaOwnershipPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaOwnershipPar,1); // CbmStsDbQaOwnershipPar Parameter Container example
};



inline  FairDbReader<CbmStsDbQaOwnershipPar>* CbmStsDbQaOwnershipPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaOwnershipPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaOwnershipPar>* CbmStsDbQaOwnershipPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaOwnershipPar>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBQAOWNERSHIPPAR_H*/
