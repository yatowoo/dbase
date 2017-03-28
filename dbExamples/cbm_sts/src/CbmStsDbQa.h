#ifndef CBMSTSDBQA_H
#define CBMSTSDBQA_H

#include "FairDbParSet.h"          // for FairDbParSet

#include "CbmStsDbQaSensorNewPar.h"
#include "CbmStsDbQaSensor.h"
#include "CbmStsDbQaIv.h"
#include "CbmStsDbQaGeometryPar.h"
#include "CbmStsDbQaOwnershipPar.h"
#include "CbmStsDbQaResultsPar.h"
#include "CbmStsDbQaChannelMapPar.h"
#include "CbmStsDbQaIvNewPar.h"
#include "CbmStsDbQaSUID.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string


#define MAX_MEAS 40


using namespace std;

class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;



struct MatchPathSeparator
{
  bool operator()( char ch ) const
  {
    return ch == '/';
  }
};


class CbmStsDbQa : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQa (const char* name="StsDbQa",
                  const char* title="Cbm Sts Database QA Parameter ( Version ) ",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQa(void);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);
    // Global IO using run_id
    virtual void fill(UInt_t rid);
    virtual void store(UInt_t rid);


    void   AddSUID(CbmStsDbQaSUID *s){fSUID->Add(s);}  
    void   AddSUIDAt(CbmStsDbQaSUID *s, int idx){fSUID->AddAtAndExpand(s,idx);}  
    TObjArray* GetListOfSUID() {return fSUID;}
    Int_t      GetNumSUID() const {return fSUID->GetEntries();} 
    CbmStsDbQaSUID* GetSUIDAt(Int_t idx){return (CbmStsDbQaSUID*) fSUID->At(idx);} 

	// List Summary    
    void   AddSensor(CbmStsDbQaSensorNewPar *s){fSensors->Add(s);}  
    void   AddSensorAt(CbmStsDbQaSensorNewPar *s, int idx){fSensors->AddAt(s,idx);}  
    TObjArray* GetListOfSensors() const {return fSensors;}
    Int_t     GetNumSensors() const  {return fSensors->GetEntries();} 
    CbmStsDbQaSensorNewPar* GetSensorAt(Int_t idx){return (CbmStsDbQaSensorNewPar*) fSensors->At(idx);} 

	// List Channel Map
    void   AddSChannelMap(CbmStsDbQaChannelMapPar *s)           {fChannelMap->Add(s);}  
    void   AddSChannelMapAt(CbmStsDbQaChannelMapPar *s, int idx){fChannelMap->AddAt(s,idx);}  
    TObjArray* GetListOfSChannelMap() {return fChannelMap;}
    Int_t GetNumSChannelMap() {return fChannelMap->GetEntries();} 
    CbmStsDbQaChannelMapPar* GetSChannelMapAt(Int_t idx){return (CbmStsDbQaChannelMapPar*) fChannelMap->At(idx);} 

    // List Geometry
    void   AddSGeometry(CbmStsDbQaGeometryPar *s)           {fGeometry->Add(s);}  
    void   AddSGeometryAt(CbmStsDbQaGeometryPar *s, int idx){fGeometry->AddAt(s,idx);}  
    TObjArray* GetListOfSGeometry() {return fGeometry;}
    Int_t GetNumSGeometry() {return fGeometry->GetEntries();} 
    CbmStsDbQaGeometryPar* GetSGeometryAt(Int_t idx){return (CbmStsDbQaGeometryPar*) fGeometry->At(idx);} 


   // List Ownership
    void   AddSOwnership(CbmStsDbQaOwnershipPar *s)           {fOwnership->Add(s);}  
    void   AddSOwnershipAt(CbmStsDbQaOwnershipPar *s, int idx){fOwnership->AddAt(s,idx);}  
    TObjArray* GetListOfSOwnership() {return fOwnership;}
    Int_t GetNumSOwnership() {return fOwnership->GetEntries();} 
    CbmStsDbQaOwnershipPar* GetSOwnershipAt(Int_t idx){return (CbmStsDbQaOwnershipPar*) fOwnership->At(idx);} 

   // List Results
    void   AddSResults(CbmStsDbQaResultsPar *s)           {fResults->Add(s);}  
    void   AddSResultsAt(CbmStsDbQaResultsPar *s, int idx){fResults->AddAt(s,idx);}  
    TObjArray* GetListOfSResults() {return fResults;}
    Int_t GetNumSResults() {return fResults->GetEntries();} 
    CbmStsDbQaResultsPar* GetSResultsAt(Int_t idx){return (CbmStsDbQaResultsPar*) fResults->At(idx);} 
   
   // IV curves
    void   AddIvMeas(CbmStsDbQaIvNewPar *s){fIvMeas->Add(s);}  
    void   AddIvMeasAt(CbmStsDbQaIvNewPar *s, int idx){fIvMeas->AddAt(s,idx);}  
    TObjArray* GetListOfIvMeas() const {return fIvMeas;}
    Int_t GetNumIvMeas() const {return fIvMeas->GetEntries();} 
    CbmStsDbQaIvNewPar* GetIvMeasAt(Int_t idx){return (CbmStsDbQaIvNewPar*) fIvMeas->At(idx);} 

   // Db Readers
    FairDbReader<CbmStsDbQaSUID>*          GetReaderSUID()      {return fReader_SUID;}
    FairDbReader<CbmStsDbQaSensor>*        GetReaderSensor()    {return fReader_Sensor;}
    FairDbReader<CbmStsDbQaIv>*            GetReaderIv()        {return fReader_Iv;}
    
    FairDbReader<CbmStsDbQaChannelMapPar>* GetReaderChannelMap(){return fReader_ChannelMap;}
    FairDbReader<CbmStsDbQaGeometryPar>*   GetReaderGeometry()  {return fReader_Geometry;}
    FairDbReader<CbmStsDbQaOwnershipPar>*  GetReaderOwnership() {return fReader_Ownership;}
    FairDbReader<CbmStsDbQaResultsPar>*    GetReaderResults()   {return fReader_Results;}


    
  // Dump Object
    void   Print();

  // Import utils function to read from differents input 
    void ImportFromCsvFile(const string& fname);

    void LoadDataFromIvDir(const string& dirname);
    void ImportIvFilesFromDir(const string& dirname);
    void LoadDataFromIvFile(const string& vendor, int& b_id, const string& fname); 
  // UID
    void GenerateSUIDFromCsvFile(const string& fname);
    void LoadSUIDFromDB();

  // Utils
    std::istream& safeGetline(std::istream& is, std::string& t);     
    std::string basename( std::string const& pathname );
    std::string remove_ext( std::string const& filename );

    
 private:
    // Qa Sensor UID 
    TObjArray* fSUID;
    // Qa Sensor Summary
    TObjArray* fSensors;
    // Qa-Iv Sensor Measurements 
    TObjArray* fIvMeas; //
    // Qa ChannelMap  
    TObjArray* fChannelMap;//
    // Qa Geometry
    TObjArray* fGeometry;//
    // Qa OwnerShip
    TObjArray* fOwnership;//
    // Qa Results 
    TObjArray* fResults;//
    // Last Index in SUID Array
    Int_t fLastIndexSUID; //!
    map< string, int> fMapIndex; //!
    
    FairDbReader<CbmStsDbQaSUID>*             fReader_SUID; //!
    FairDbReader<CbmStsDbQaSensor>*           fReader_Sensor; //!
    FairDbReader<CbmStsDbQaIv>*               fReader_Iv; //!
    FairDbReader<CbmStsDbQaChannelMapPar>*    fReader_ChannelMap; //!
    FairDbReader<CbmStsDbQaGeometryPar>*      fReader_Geometry; //!
    FairDbReader<CbmStsDbQaOwnershipPar>*     fReader_Ownership; //!
    FairDbReader<CbmStsDbQaResultsPar>*       fReader_Results; //!
    
    
    ClassDef(CbmStsDbQa,1); // CbmStsDbQa Parameter Container 
};

#endif /* !CBMSTSDBQA_H*/

