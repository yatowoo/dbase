#ifndef CBMSTSDBQASENSOR_H
#define CBMSTSDBQASENSOR_H

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


using namespace std;


class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;


class CbmStsDbQaSensor : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaSensor (const char* name="StsQaSensorPar",
                  const char* title="Cbm Sensor Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaSensor(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    
    
    Int_t      GetComboNo()          const {return fCompId;}    
    Int_t      GetCompId()           const {return fCompId;}
    Int_t      GetUID()              const {return fUID;}
    string     GetSensorType()       const {return fSensor_type;}
    string     GetVendorName()       const {return fVendor_name;}
    Int_t      GetWaferId()          const {return fWafer_id;} 
    string     GetBatchId()          const {return fBatch_id;}    
    string     GetReticleName()      const {return fReticle_name;}
    string     GetProcessing()       const {return fProcessing;}
    Double_t   GetHeight()           const {return fHeight;}
    Double_t   GetWidth()            const {return fWidth;}
    Int_t      GetStripsPerSide()    const {return fStripsPerSide;} 
    Int_t      GetYear()             const {return fYear;}
    string     GetOwner()            const {return fCurrent_owner;}
    string     GetLocation()         const {return fCurrent_location;}
    Double_t   GetV_fd()             const {return fV_fd;}
    Double_t   GetI_150V_20C()       const {return fI_150V_20C;}
    Double_t   GetI_250V_20C()       const {return fI_250V_20C;}
    Int_t      GetSdefect_pside()    const {return fSdefect_pside;}
    Int_t      GetSdefect_nside()    const {return fSdefect_nside;}
    Int_t      GetQuality_grade()    const {return fQuality_grade;}
    Int_t      GetProblem()          const {return fProblem;}
    Int_t      GetPassed()           const {return fPassed;}
    Int_t      GetOpticalCheck()     const {return fOptPassed;}
    string     GetComment()          const {return fComments;}


    void SetCompId(Int_t x)             {SetComboNo(x); fCompId=x;}
    void SetUID(Int_t uid)              {fUID  = uid;}
    void SetSensorType(string type)     {fSensor_type=type;}
    void SetBatchId(string bid)         {fBatch_id=bid;}
    void SetWaferId(Int_t wid)          {fWafer_id=wid;}     
    void SetReticleName(string name)    {fReticle_name=name;}
    void SetVendorName(string name)     {fVendor_name=name;}
    void SetProcessing(string name)     {fProcessing=name;}
    void SetHeight(Double_t val)        {fHeight=val;}
    void SetWidth(Double_t val)         {fWidth=val;}
    void SetStripsPerSide(Int_t i)      {fStripsPerSide=i;}
    void SetYear(Int_t i)               {fYear=i;}
    void SetCurrentOwner(string owner)  {fCurrent_owner=owner;}    
    void SetCurrentLocation(string loc) {fCurrent_location=loc;}
    void SetVfd(Double_t vv)            {fV_fd=vv;}
    void SetI_150V_20C(Double_t ii)     {fI_150V_20C=ii;}
    void SetI_250V_20C(Double_t ii)     {fI_250V_20C=ii;}
    void SetSdefect_pside(Int_t def)    {fSdefect_pside=def;}
    void SetSdefect_nside(Int_t def)    {fSdefect_nside=def;}
    void SetQuality_grade(Int_t grad)   {fQuality_grade=grad;}
    void SetProblem(Int_t prob)         {fProblem=prob;}
    void SetPassed(Int_t  pass)         {fPassed=pass;}
    void SetOpticalCheck(Int_t pass)    {fOptPassed=pass;}
    void SetComments(string com)        {fComments=com;} 


    void Print();

    Bool_t Import(Int_t compid, const vector<string> & value);

    UInt_t GetIndex(UInt_t /*def*/) const { return fCompId; }

    // Logical (=) 
    Bool_t Compare(const CbmStsDbQaSensor& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaSensor();
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
    
    // Create Db Table
    void  CreateDbTable(Int_t db_entry=0);
    
    // SQL-IO Meta-Class Getters
    FairDbReader<CbmStsDbQaSensor>* GetParamReader();
    FairDbWriter<CbmStsDbQaSensor>* GetParamWriter();
    FairDbWriter<CbmStsDbQaSensor>* ActivateWriter(Int_t rid=0);

  private:

    // Data Parameters
    Int_t    fCompId;
    Int_t    fUID;
    string   fSensor_type;
    string   fBatch_id;
    Int_t    fWafer_id;
    string   fReticle_name;
    string   fVendor_name;
    string   fProcessing;    
    Double_t fHeight;
    Double_t fWidth;
    Int_t    fStripsPerSide;
    Int_t    fYear;
    string   fCurrent_owner;
    string   fCurrent_location;
    Double_t fV_fd; // [Volt]
    Double_t fI_150V_20C; // [muA]
    Double_t fI_250V_20C; // [muA]
    Int_t    fSdefect_pside;
    Int_t    fSdefect_nside;
    Int_t    fQuality_grade;
    Int_t    fProblem;
    Int_t    fPassed;
    Int_t    fOptPassed;
    string   fComments; 
    
    // Database Pool Index
    //Int_t fDbEntry; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaSensor>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaSensor>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaSensor,1); // CbmStsDbQaSensor Parameter Container example
};



inline  FairDbReader<CbmStsDbQaSensor>* CbmStsDbQaSensor::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaSensor>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaSensor>* CbmStsDbQaSensor::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaSensor>();
    return fParam_Writer;
  }
}

#endif /* !STSDBQASENSOR_H*/
