#ifndef CBMSTSDBQACHANNELMAPPAR_H
#define CBMSTSDBQACHANNELMAPPAR_H

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


class CbmStsDbQaChannelMapPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaChannelMapPar (const char* name="CbmStsDbQaChannelMapPar",
                  const char* title="Cbm Sts Qa Channel Map Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaChannelMapPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    Int_t GetComboNo()     const {return fCompId;}    
    Int_t GetCompId()      const {return fCompId;}
    Int_t GetUID()         const {return fUID;}
    
    Int_t GetAC_Cap_Ok_pside()   const {return fAC_Cap_Ok_pside;}
    Int_t GetAC_Cap_Ok_nside()   const {return fAC_Cap_Ok_nside;}
    Double_t GetAC_Cap_Value_pside() const { return fAC_Cap_Value_pside;}
    Double_t GetAC_Cap_Value_nside() const { return fAC_Cap_Value_nside;}
    Double_t GetI_Strip_pside() const  {return fI_Strip_pside;}
    Double_t GetI_Strip_nside() const  {return fI_Strip_nside;}

    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}
    void SetUID(Int_t x) {fUID  = x;}
    
    void SetAC_Cap_Ok_pside(Int_t flag)     { fAC_Cap_Ok_pside = flag;}
    void SetAC_Cap_Ok_nside(Int_t flag)     { fAC_Cap_Ok_nside = flag;}
    void SetAC_Cap_Value_pside(Double_t v)  { fAC_Cap_Value_pside = v;}
    void SetAC_Cap_Value_nside(Double_t v)  { fAC_Cap_Value_nside = v;}
    void SetI_Strip_pside(Double_t v)       { fI_Strip_pside = v;}
    void SetI_Strip_nside(Double_t v)       { fI_Strip_nside = v;}

    
    UInt_t GetIndex(UInt_t /*def*/) const { return fUID; }

    // Logical (=) 
    Bool_t Compare(const CbmStsDbQaChannelMapPar& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaChannelMapPar();
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
    FairDbReader<CbmStsDbQaChannelMapPar>* GetParamReader();
    FairDbWriter<CbmStsDbQaChannelMapPar>* GetParamWriter();
    FairDbWriter<CbmStsDbQaChannelMapPar>* ActivateWriter(Int_t rid);
  

  private:
    // Data Parameters
    Int_t fCompId;
    Int_t fUID;
    Int_t  fAC_Cap_Ok_pside;
    Int_t  fAC_Cap_Ok_nside;
    Double_t  fAC_Cap_Value_pside;
    Double_t  fAC_Cap_Value_nside;
    Double_t  fI_Strip_pside;
    Double_t  fI_Strip_nside;

    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaChannelMapPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaChannelMapPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaChannelMapPar,1); // CbmStsDbQaChannelMapPar Parameter Container example
};



inline  FairDbReader<CbmStsDbQaChannelMapPar>* CbmStsDbQaChannelMapPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaChannelMapPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaChannelMapPar>* CbmStsDbQaChannelMapPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaChannelMapPar>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBQACHANNELMAPPAR_H */
