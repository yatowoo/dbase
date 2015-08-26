#ifndef CBMSTSDBQAIVPAR_H
#define CBMSTSDBQAIVPAR_H

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


class CbmStsDbQaIvPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaIvPar (const char* name="CbmStsDbQaIvPar",
                  const char* title="Cbm Sts IV Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaIvPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    Int_t GetComboNo()     const {return fCompId;}    
    Int_t GetCompId()      const {return fCompId;}
    Int_t GetUID()         const {return fUID;}
    string GetFileName()   const {return fName;}
    Int_t     GetN()        const {return fN;}  
    Double_t* GetI()       const {return fI;}   
    Double_t* GetV()       const {return fV;}   
        
    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}
    void SetUID(Int_t x) {fUID  = x;}
    void SetFileName(string name)  {fName=name;}
    void SetN(Int_t nele);          
    void SetI(Double_t* inp)  {for(Int_t i=0;i<fN;i++){fI[i]=inp[i];}}   
    void SetV(Double_t* inp)  {for(Int_t i=0;i<fN;i++){fV[i]=inp[i];}}   
    
    UInt_t GetIndex(UInt_t /*def*/) const { return fUID; }

    // Logical (=) 
    Bool_t Compare(const CbmStsDbQaIvPar& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaIvPar();
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
    FairDbReader<CbmStsDbQaIvPar>* GetParamReader();
    FairDbWriter<CbmStsDbQaIvPar>* GetParamWriter();
    FairDbWriter<CbmStsDbQaIvPar>* ActivateWriter(Int_t rid);
  

  private:
    // Data Parameters
    string fName; 
    Int_t fCompId;
    Int_t fUID;
    Int_t fN;         
    Double_t* fI; //[fN]
    Double_t* fV; //[fN]
     
    // Database Pool Index
    //Int_t fDbEntry; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaIvPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaIvPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaIvPar,1); // CbmStsDbQaIvPar Parameter Container example
};



inline  FairDbReader<CbmStsDbQaIvPar>* CbmStsDbQaIvPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaIvPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaIvPar>* CbmStsDbQaIvPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaIvPar>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBQAIVPAR_H*/
