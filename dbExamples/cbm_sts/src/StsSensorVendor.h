#ifndef STSSENSORVENDOR_H
#define STSSENSORVENDOR_H

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsSensorVendor : public FairDbGenericParSet<StsSensorVendor>
{

using TObject::Compare;

  public :
    StsSensorVendor(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsSensorVendor", 
              const char* title = "Sts Sensor Vendor Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsSensorVendor(void);
    StsSensorVendor(const StsSensorVendor& from);
    StsSensorVendor& operator=(const StsSensorVendor& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsSensorVendor* GetVendorByName(string vendorName, UInt_t runId = 0);

    Int_t    GetId()              const { return fId; }
    string   GetVendorName()      const { return fVendorName; }
    string   GetLocation()        const { return fLocation; }
    string   GetContactInfo()     const { return fContactInfo; }
    string   GetComment()         const { return fComment; }

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetId(Int_t value)               { fId = value; }
    void SetVendorName(string value)      { fVendorName = value; }
    void SetLocation(string value)        { fLocation = value; }
    void SetContactInfo(string value)     { fContactInfo = value; }
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
    string   fVendorName;
    string   fLocation;
    string   fContactInfo;
    string   fComment;

    ClassDef(StsSensorVendor,1); // ROOT class definition
};

#endif /* !STSSENSORVENDOR_H*/

