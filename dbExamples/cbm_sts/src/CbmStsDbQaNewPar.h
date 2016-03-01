#ifndef CBMSTSDBQAPARNEW_H
#define CBMSTSDBQAPARNEW_H

#include "FairDbParSet.h"          // for FairDbParSet

#include "CbmStsDbQaSensorNewPar.h"
#include "CbmStsDbQaGeometryPar.h"
#include "CbmStsDbQaOwnershipPar.h"
#include "CbmStsDbQaResultsPar.h"
#include "CbmStsDbQaChannelMapPar.h"
#include "CbmStsDbQaIvNewPar.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string


#define MAX_MEAS 40


using namespace std;

class FairDbOutTableBuffer;
class FairDbResultPool;
class FairDbObjTableMap;
class FairDbValRecord;
class FairParamList;


class CbmStsDbQaNewPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaNewPar (const char* name="StsQaPar",
                  const char* title="Cbm Sts Database QA Parameter ( Version ) ",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaNewPar(void);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);
    // Global IO using run_id
    virtual void fill(UInt_t rid);
    virtual void store(UInt_t rid);

  
	// List Summary    
    void   AddSensor(CbmStsDbQaSensorNewPar *s){fSensors->Add(s);}  
    void   AddSensorAt(CbmStsDbQaSensorNewPar *s, int idx){fSensors->AddAt(s,idx);}  
    TObjArray* GetListOfSensors() {return fSensors;}
    Int_t GetNumSensors() {return fSensors->GetEntries();} 
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
    TObjArray* GetListOfIvMeas() {return fIvMeas;}
    Int_t GetNumIvMeas() {return fIvMeas->GetEntries();} 
    CbmStsDbQaIvNewPar* GetIvMeasAt(Int_t idx){return (CbmStsDbQaIvNewPar*) fIvMeas->At(idx);} 

    std::istream& safeGetline(std::istream& is, std::string& t);     

   // Db Readers  
    FairDbReader<CbmStsDbQaSensorNewPar>*  GetReaderSensor()    {return fReader_Sensor;}
    FairDbReader<CbmStsDbQaChannelMapPar>* GetReaderChannelMap(){return fReader_ChannelMap;}
    FairDbReader<CbmStsDbQaGeometryPar>*   GetReaderGeometry()  {return fReader_Geometry;}
    FairDbReader<CbmStsDbQaOwnershipPar>*  GetReaderOwnership() {return fReader_Ownership;}
    FairDbReader<CbmStsDbQaResultsPar>*    GetReaderResults()   {return fReader_Results;}
    FairDbReader<CbmStsDbQaIvNewPar>*      GetReaderIv()        {return fReader_Iv;} 

    
  // Dump Object
    void   Print();

  // Import utils function to read from differents input 
    void ImportFromCsvFile(const string& fname);
    void LoadDataFromIvFile(const string& fname);
    void ImportIvFilesFromDir();
    

  private:
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


   FairDbReader<CbmStsDbQaSensorNewPar>*     fReader_Sensor; //!
   FairDbReader<CbmStsDbQaIvNewPar>*         fReader_Iv; //!
   FairDbReader<CbmStsDbQaChannelMapPar>*    fReader_ChannelMap; //!
   FairDbReader<CbmStsDbQaGeometryPar>*      fReader_Geometry; //!
   FairDbReader<CbmStsDbQaOwnershipPar>*     fReader_Ownership; //!
   FairDbReader<CbmStsDbQaResultsPar>*       fReader_Results; //!



    ClassDef(CbmStsDbQaNewPar,1); // CbmStsDbQaNewPar Parameter Container 
};

#endif /* !CBMSTSDBQAPARNEW_H*/

