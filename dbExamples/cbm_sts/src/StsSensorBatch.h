#ifndef STSSENSORBATCH_H
#define STSSENSORBATCH_H

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsSensorBatch : public FairDbGenericParSet<StsSensorBatch>
{

using TObject::Compare;

  public :
    StsSensorBatch(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsSensorBatch", 
              const char* title = "Sts Batch Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsSensorBatch(void);
    StsSensorBatch(const StsSensorBatch& from);
    StsSensorBatch& operator=(const StsSensorBatch& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsSensorBatch* GetBatchById(Int_t batchId, UInt_t runId = 0);
    TObjArray*             GetSensors();

    Int_t        GetId()            const { return fId; }
    string       GetNumber()        const { return fNumber; }
    ValTimeStamp GetDate()          const { return fDate; }
    string       GetComment()       const { return fComment; }

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetId(Int_t value)               { fId = value; }
    void SetNumber(string value)          { fNumber = value; }
    void SetDate(ValTimeStamp value)      { fDate = value; }
    void SetComment(string value)         { fComment = value; }

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    TObjArray*        fSensors; //! Transient data member

    Int_t             fId;
    string            fNumber;
    ValTimeStamp      fDate;
    string            fComment;

    ClassDef(StsSensorBatch,1); // ROOT class definition
};

#endif /* !STSSENSORBATCH_H*/

