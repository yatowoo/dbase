#ifndef STSSENSOR_H
#define STSSENSOR_H

#include "StsSensorBatch.h"
#include "StsSensorLocation.h"
#include "StsSensorType.h"
#include "StsSensorVendor.h"

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsSensor : public FairDbGenericParSet<StsSensor>
{

using TObject::Compare;

  public :
    StsSensor(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsSensor", 
              const char* title = "Sts Sensor Entity", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsSensor(void);
    StsSensor(const StsSensor& from);
    StsSensor& operator=(const StsSensor& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsSensor* GetSensorById(Int_t sensorId, UInt_t runId = 0);
    static TObjArray* GetSensors(Int_t batchId, UInt_t runId = 0);
    
    StsSensorBatch* GetBatch() {
      if (!fBatch) fBatch = StsSensorBatch::GetBatchById(fBatchId);
      return fBatch;
    }

    TObjArray* GetOpticalInspections();

    StsSensorLocation* GetLocation() {
      if (!fLocation) fLocation = StsSensorLocation::GetLocationById(fLocationId);
      return fLocation;
    }

    StsSensorType* GetType() {
      if (!fType) fType = StsSensorType::GetTypeById(fTypeId);
      return fType;
    }

    StsSensorVendor* GetVendor() {
      if (!fVendor) fVendor = StsSensorVendor::GetVendorById(fVendorId);
      return fVendor;
    }

    Int_t  GetId()           const { return fId; }
    Int_t  GetBatchId()      const { return fBatchId; }
    Int_t  GetLocationId()   const { return fLocationId; }
    Int_t  GetTypeId()       const { return fTypeId; }
    Int_t  GetVendorId()     const { return fVendorId; }
    string GetReticleName()  const { return fReticleName; }
    Int_t  GetWaferNumber()  const { return fWaferNumber; }
    ValTimeStamp GetDate()   const { return fDate; }
    Double_t GetQuality()    const { return fQuality; }
    string GetQualityGrade() const { return fQualityGrade; }
    string GetComment()      const { return fComment; }


    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetBatch(StsSensorBatch& value)             { fBatch = new StsSensorBatch(value); fBatchId = value.GetId(); }
    void SetLocation(StsSensorLocation& value) { fLocation = new StsSensorLocation(value); fLocationId = value.GetId(); }
    void SetType(StsSensorType& value)         { fType = new StsSensorType(value); fTypeId = value.GetId(); }
    void SetVendor(StsSensorVendor& value)           { fVendor = new StsSensorVendor(value); fVendorId = value.GetId(); }

    void SetId(Int_t value)            { fId = value; }
    void SetBatchId(Int_t value)       { fBatchId = value; }
    void SetLocationId(Int_t value)    { fLocationId = value; }
    void SetTypeId(Int_t value)        { fTypeId = value; }
    void SetVendorId(Int_t value)      { fVendorId = value; }
    void SetReticleName(string value)  { fReticleName = value; }
    void SetWaferNumber(Int_t value)   { fWaferNumber = value; }
    void SetDate(ValTimeStamp value)   { fDate = value; }
    void SetQuality(Double_t value)    { fQuality = value; }
    void SetQualityGrade(string value) { fQualityGrade = value; }
    void SetComment(string value)      { fComment = value; }


    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    TObjArray*         fOpticalInspections; //! Transient data member
    StsSensorBatch*    fBatch;              //! Transient data member
    StsSensorLocation* fLocation;           //! Transient data member
    StsSensorType*     fType;               //! Transient data member
    StsSensorVendor*   fVendor;             //! Transient data member

    Int_t              fId;
    Int_t              fBatchId;
    Int_t              fLocationId;
    Int_t              fTypeId;
    Int_t              fVendorId;
    string             fReticleName;
    Int_t              fWaferNumber;
    ValTimeStamp       fDate;
    Double_t           fQuality;
    string             fQualityGrade;
    string             fComment;

    ClassDef(StsSensor,1); // ROOT class definition
};

#endif /* !STSSENSOR_H*/

