#ifndef CBMSTSDBQARESULTSPAR_H
#define CBMSTSDBQARESULTSPAR_H

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


class CbmStsDbQaResultsPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaResultsPar (const char* name="StsQaResultsPar",
                  const char* title="Cbm Sts QA Results Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaResultsPar(void);
    void clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    void Print();

    Int_t        GetComboNo()            const {return fCompId;}
    Int_t        GetCompId()             const {return fCompId;}
    Int_t        GetUID()                const {return fUID;}
	  string       GetType()               const {return fType;}
    Int_t        GetWaferId()            const {return fWaferId;}
    Double_t     GetV_fd()               const {return fV_fd;}
    Double_t     GetI_150V_20C()         const {return fI_150V_20C;}
    Double_t     GetI_250V_20C()         const {return fI_250V_20C;}
    Int_t        GetSdefect_pside()      const {return fSdefect_pside;}
    Int_t        GetSdefect_nside()      const {return fSdefect_nside;}
    Int_t        GetQuality_grade()      const {return fQuality_grade;}
    Int_t        GetProblem()            const {return fProblem;}
    Int_t        GetQaPassed()           const {return fQaPassed;}
    Int_t        GetOpticalCheckPassed() const {return fOpticalCheckPassed;}
    string       GetComment()            const {return fComments;}


    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}
    void SetUID(Int_t x) {fUID  = x;}
    void SetType(string name)          { fType   = name;}
    void SetWaferId( Int_t i )         { fWaferId = i;}
    void SetVfd(Double_t v)            { fV_fd = v; }
    void SetI_150V_20C(Double_t i)     { fI_150V_20C = i; }
    void SetI_250V_20C(Double_t i)     { fI_250V_20C = i; }
    void SetSdefect_pside(Int_t i)     { fSdefect_pside = i; }
    void SetSdefect_nside(Int_t i)     { fSdefect_nside = i; }
    void SetQuality_grade(Int_t i)     { fQuality_grade = i; }
    void SetProblem(Int_t i)           { fProblem = i;}
    void SetQaPassed(Int_t i)          { fQaPassed = i;}
    void SetOpticalCheckPassed(Int_t i){ fOpticalCheckPassed = i;}
    void SetComment(string comment)    { fComments = comment;}


    UInt_t GetIndex(UInt_t /*def*/) const { return fUID; }

    // Logical (=)
    Bool_t Compare(const CbmStsDbQaResultsPar& that ) const;

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new CbmStsDbQaResultsPar();
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

    Bool_t Import( const vector<string>& value);

    // SQL-IO Meta-Class Getters
    FairDbReader<CbmStsDbQaResultsPar>* GetParamReader();
    FairDbWriter<CbmStsDbQaResultsPar>* GetParamWriter();
    FairDbWriter<CbmStsDbQaResultsPar>* ActivateWriter(Int_t rid);


  private:
    // Data Parameters
    Int_t fCompId;
    Int_t fUID;

    string    fType;
    Int_t     fWaferId;
    Double_t  fV_fd;
    Double_t  fI_150V_20C;
    Double_t  fI_250V_20C;
    Int_t     fSdefect_pside;
    Int_t     fSdefect_nside;
    Int_t     fQuality_grade;
    Int_t     fProblem;
    Int_t     fQaPassed;
    Int_t     fOpticalCheckPassed;
    string    fComments;

    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<CbmStsDbQaResultsPar>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<CbmStsDbQaResultsPar>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!

    ClassDef(CbmStsDbQaResultsPar,1); // CbmStsDbQaResultsPar Parameter Container example
};



inline  FairDbReader<CbmStsDbQaResultsPar>* CbmStsDbQaResultsPar::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<CbmStsDbQaResultsPar>();
    return fParam_Reader;
  }
}

inline FairDbWriter<CbmStsDbQaResultsPar>* CbmStsDbQaResultsPar::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<CbmStsDbQaResultsPar>();
    return fParam_Writer;
  }
}

#endif /* !CBMSTSDBQARESULTSPAR_H*/
