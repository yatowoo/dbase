#ifndef CBMSTSDBQASENSORPAR_H
#define CBMSTSDBQASENSORPAR_H

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


class CbmStsDbQaSensorPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaSensorPar (const char* name="CbmStsDbQaSensorPar",
                  const char* title="Cbm Sensor Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaSensorPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);



    Int_t      GetComboNo()          const {return fCompId;}
    Int_t      GetCompId()           const {return fCompId;}
    Int_t      GetUID()              const {return fUID;}
    string     GetSensorType()       const {return fSensor_type;}
    string     GetSensorName()       const {return fSensor_name;}
    Int_t      GetWaferId()          const {return fWafer_id;}
    string     GetBatchId()          const {return fBatch_id;}
    string     GetManufacturer()     const {return fManufacturer;}
    string     GetProductionDate()   const {return fProduction_date;}
    string     GetOwner()            const {return fCurrent_owner;}
    string     GetLocation()         const {return fCurrent_location;}
    Double_t   GetV_fd()             const {return fV_fd;}
    Double_t   GetI_150V_20C()       const {return fI_150V_20C;}
    Double_t   GetI_250V_20C()       const {return fI_250V_20C;}
    Int_t      GetSdefect_pside()    const {return fSdefect_pside;}
    Int_t      GetSdefect_nside()    const {return fSdefect_nside;}
    Int_t      GetQuality_grade()    const {return fQuality_grade;}
    string     GetProblem()          const {return fProblem;}
    string     GetPassed()           const {return fPassed;}
    string     GetComment()          const {return fComments;}



    void SetCompId(Int_t x)             {SetComboNo(x); fCompId=x;}
    void SetUID(Int_t uid)              {fUID  = uid;}
    void SetSensorType(string type)     {fSensor_type=type;}
    void SetSensorName(string name)     {fSensor_name=name;}
    void SetBatchId(string bid)         {fBatch_id=bid;}
    void SetWaferId(Int_t id)           {fWafer_id=id;}
    void SetManufacturer(string manuf)  {fManufacturer=manuf;}
    void SetProductionDate(string date) {fProduction_date=date;}
    void SetCurrentOwner(string owner)  {fCurrent_owner=owner;}
    void SetCurrentLocation(string loc) {fCurrent_location=loc;}
    void SetVfd(Double_t vv)            {fV_fd=vv;}
    void SetI_150V_20C(Double_t ii)     {fI_150V_20C=ii;}
    void SetI_250V_20C(Double_t ii)     {fI_250V_20C=ii;}
    void SetSdefect_pside(Int_t def)    {fSdefect_pside=def;}
    void SetSdefect_nside(Int_t def)    {fSdefect_nside=def;}
    void SetQuality_grade(Int_t grad)   {fQuality_grade=grad;}
    void SetProblem(string prob)        {fProblem=prob;}
    void SetPassed(string  pass)        {fPassed=pass;}
    void SetComments(string com)        {fComments=com;}


    void Print();
    Bool_t Import(const vector<string> & value);


    UInt_t GetIndex(UInt_t /*def*/) const { return fUID; }

    // Logical (=)
    Bool_t Compare(const CbmStsDbQaSensorPar& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaSensorPar();
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
    FairDbReader<CbmStsDbQaSensorPar>* GetParamReader();
    FairDbWriter<CbmStsDbQaSensorPar>* GetParamWriter();
    FairDbWriter<CbmStsDbQaSensorPar>* ActivateWriter(Int_t rid);

  private:

    // Data Parameters
    Int_t    fCompId;
    Int_t    fUID;
    string   fSensor_type;
    string   fBatch_id;
    Int_t    fWafer_id;
    string   fSensor_name;
    string   fManufacturer;
    string   fProduction_date;
    string   fCurrent_owner;
    string   fCurrent_location;
    Double_t fV_fd; // [Volt]
    Double_t fI_150V_20C; // [muA]
    Double_t fI_250V_20C; // [muA]
    Int_t    fSdefect_pside;
    Int_t    fSdefect_nside;
    Int_t    fQuality_grade;
    string   fProblem;
    string   fPassed;
    string   fComments;


    // Database Pool Index
    //Int_t fDbEntry; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaSensorPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaSensorPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaSensorPar,1); // CbmStsDbQaSensorPar Parameter Container example
};



inline  FairDbReader<CbmStsDbQaSensorPar>* CbmStsDbQaSensorPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaSensorPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaSensorPar>* CbmStsDbQaSensorPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaSensorPar>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBQASENSORPAR_H*/
