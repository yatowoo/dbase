#ifndef CBMSTSDBQAPAR_H
#define CBMSTSDBQAPAR_H

#include "FairDbParSet.h"          // for FairDbParSet

#include "CbmStsDbQaSensorPar.h"
#include "CbmStsDbQaIvPar.h"

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


class CbmStsDbQaPar : public FairDbParSet
{

using TObject::Compare;

  public :
    CbmStsDbQaPar (const char* name="CbmStsDbQaPar",
                  const char* title="Cbm Sts Database QA Parameter",
                  const char* context="TestDefaultContext",
                  Bool_t own=kTRUE);
    virtual ~CbmStsDbQaPar(void);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);
    // Global IO using run_id
    virtual void fill(UInt_t rid);
    virtual void store(UInt_t rid);

  
	// Lists handling    
    void   AddSensor(CbmStsDbQaSensorPar *s){fSensors->Add(s);}  
    void   AddSensorAt(CbmStsDbQaSensorPar *s, int idx){fSensors->AddAt(s,idx);}  
    TObjArray* GetListOfSensors() {return fSensors;}
    Int_t GetNumSensors() {return fSensors->GetEntries();} 
    CbmStsDbQaSensorPar* GetSensorAt(Int_t idx){return (CbmStsDbQaSensorPar*) fSensors->At(idx);} 

    void   AddIvMeas(CbmStsDbQaIvPar *s){fIvMeas->Add(s);}  
    void   AddIvMeasAt(CbmStsDbQaIvPar *s, int idx){fIvMeas->AddAt(s,idx);}  
    TObjArray* GetListOfIvMeas() {return fIvMeas;}
    Int_t GetNumIvMeas() {return fIvMeas->GetEntries();} 
    CbmStsDbQaIvPar* GetIvMeasAt(Int_t idx){return (CbmStsDbQaIvPar*) fIvMeas->At(idx);} 
   
     
    FairDbReader<CbmStsDbQaSensorPar>* GetReaderSensor(){return fReader_Sensor;}
    FairDbReader<CbmStsDbQaIvPar>*     GetReaderIv() {return fReader_Iv;} 

  // Dump Object
    void   Print();


  // Import utils function to read from differents input 
    void ImportFromCsvFile(const string& fname);
    void LoadDataFromIvFile(const string& fname);
    void ImportIvFilesFromDir();
    

  private:
   TObjArray* fSensors;
   TObjArray* fIvMeas;

   FairDbReader<CbmStsDbQaSensorPar>* fReader_Sensor;
   FairDbReader<CbmStsDbQaIvPar>*     fReader_Iv;
    

    ClassDef(CbmStsDbQaPar,1); // CbmStsDbQaPar Parameter Container 
};

#endif /* !CBMSTSDBQAPAR_H*/

