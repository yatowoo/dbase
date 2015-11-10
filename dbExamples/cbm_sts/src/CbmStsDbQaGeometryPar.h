#ifndef CBMSTSDBQAGEOMETRYPAR_H
#define CBMSTSDBQAGEOMETRYPAR_H

#include "FairDbParSet.h"               // for FairParGenericSet
#include "DataType.h"                   // for EDataType::kData
#include "ValCondition.h"               // for ValCondition
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


class CbmStsDbQaGeometryPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaGeometryPar (const char* name="CbmStsDbQaGeometryPar",
                  const char* title="Cbm Sts Sensor Geometry Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaGeometryPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    Int_t GetComboNo()       const {return fCompId;}    
    Int_t GetCompId()        const {return fCompId;}
    Int_t GetUID()           const {return fUID;}
 
    string GetVendor()       const {return fVendor;}
    string GetType()         const {return fType;}  
    string GetReticleName()  const {return fReticleName;}   
    string GetProcessing()   const {return fProcessing;}
    Double_t GetHeight()     const {return fHeight;} 
    Double_t GetWidth()      const {return fWidth;}
    Double_t GetPitch()      const {return fPitch;}
    string GetStereoAngle()  const {return fStereoAngle;}
    Int_t GetStripsPerSide() const {return fStripsPerSide;} 
        
    void SetCompId( Int_t x )  {SetComboNo(x); fCompId=x;}
    void SetUID( Int_t x ) {fUID  = x;}
    
    void SetVendor( string st )      { fVendor = st; }
    void SetType( string st )        { fType = st; }
    void SetReticleName( string st ) { fReticleName = st; }  
    void SetProcessing( string st )  { fProcessing = st; }
    void SetHeight( Double_t val )   { fHeight = val; }
    void SetWidth( Double_t val )    { fWidth = val; }
    void SetPitch( Double_t val )    { fPitch = val; }
    void SetStereoAngle( string st ) { fStereoAngle = st; }
    void SetStripsPerSide( Int_t i ) { fStripsPerSide = i; }
    
    UInt_t GetIndex(UInt_t /*def*/) const { return fUID; }

    // Logical (=) 
    Bool_t Compare(const CbmStsDbQaGeometryPar& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaGeometryPar();
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
    FairDbReader<CbmStsDbQaGeometryPar>* GetParamReader();
    FairDbWriter<CbmStsDbQaGeometryPar>* GetParamWriter();
    FairDbWriter<CbmStsDbQaGeometryPar>* ActivateWriter(Int_t rid);
  

  private:
    // Data Parameters
    Int_t fCompId;
    Int_t fUID;

    string   fVendor;
    string   fType;
    string   fReticleName; 
    string   fProcessing;
    Double_t fHeight;
    Double_t fWidth;
    Double_t fPitch;
    string   fStereoAngle;
    Int_t    fStripsPerSide;    

    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaGeometryPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaGeometryPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaGeometryPar,1); // CbmStsDbQaGeometryPar Parameter Container example
};



inline  FairDbReader<CbmStsDbQaGeometryPar>* CbmStsDbQaGeometryPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaGeometryPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaGeometryPar>* CbmStsDbQaGeometryPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaGeometryPar>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBQAGEOMETRYPAR_H */
