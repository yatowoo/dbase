#ifndef STSSENSORLOCATION_H
#define STSSENSORLOCATION_H

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsSensorLocation : public FairDbGenericParSet<StsSensorLocation>
{

using TObject::Compare;

  public :
    StsSensorLocation(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsSensorLocation", 
              const char* title = "Sts Sensor Location Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsSensorLocation(void);
    StsSensorLocation(const StsSensorLocation& from);
    StsSensorLocation& operator=(const StsSensorLocation& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsSensorLocation* GetLocationById(Int_t locationId, UInt_t runId = 0);

    Int_t    GetId()              const { return fId; }
    string   GetLocation()        const { return fLocation; }
    string   GetOwner()           const { return fOwner; }
    string   GetComment()         const { return fComment; }

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetId(Int_t value)               { fId = value; }
    void SetLocation(string value)        { fLocation = value; }
    void SetOwner(string value)           { fOwner = value; }
    void SetComment(string value)         { fComment = value; }

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    Int_t    fId;
    string   fLocation;
    string   fOwner;
    string   fComment;

    ClassDef(StsSensorLocation,1); // ROOT class definition
};

#endif /* !STSSENSORLOCATION_H*/

