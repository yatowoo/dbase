#ifndef STSDEFECTCONTEXT_H
#define STSDEFECTCONTEXT_H

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsDefectContext : public FairDbGenericParSet<StsDefectContext>
{

using TObject::Compare;

  public :
    StsDefectContext(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsDefectContext", 
              const char* title = "Sts Defect Context Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsDefectContext(void);
    StsDefectContext(const StsDefectContext& from);
    StsDefectContext& operator=(const StsDefectContext& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    Int_t    GetId()              const { return fId; }
    string   GetContextName()     const { return fContextName; }
    Double_t GetWeight()          const { return fWeight; }

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetId(Int_t value)               { fId = value; }
    void SetContextName(string value)     { fContextName = value; }
    void SetWeight(Double_t value)        { fWeight = value; }

    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    Int_t      fId;
    string     fContextName;
    Double_t   fWeight;

    ClassDef(StsDefectContext,1); // ROOT class definition
};

#endif /* !STSDEFECTCONTEXT_H*/

