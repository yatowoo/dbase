#ifndef STSDEFECTTYPE_H
#define STSDEFECTTYPE_H

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsDefectType : public FairDbGenericParSet<StsDefectType>
{

using TObject::Compare;

  public :
    StsDefectType(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsDefectType", 
              const char* title = "Sts Defect Type Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsDefectType(void);
    StsDefectType(const StsDefectType& from);
    StsDefectType& operator=(const StsDefectType& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    static StsDefectType* GetDefectTypeById(Int_t defectTypId, UInt_t runId = 0);

    Int_t    GetId()              const { return fId; }
    string   GetTypeName()        const { return fTypeName; }
    Double_t GetWeight()          const { return fWeight; }

    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetId(Int_t value)               { fId = value; }
    void SetTypeName(string value)        { fTypeName = value; }
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
    string     fTypeName;
    Double_t   fWeight;

    ClassDef(StsDefectType,1); // ROOT class definition
};

#endif /* !STSDEFECTTYPE_H*/

