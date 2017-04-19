#ifndef STSSENSORTYPE_H
#define STSSENSORTYPE_H

#include "DataType.h"         
#include "FairDbGenericParSet.h"

#include "TObjArray.h"
#include "TGeoMaterial.h" 
#include "Rtypes.h"                     // for Double_t, Int_t, UInt_t, etc

#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for string

using namespace std;

class StsSensorType : public FairDbGenericParSet<StsSensorType>
{

using TObject::Compare;

  public :
    StsSensorType(FairDbDetector::Detector_t detid = FairDbDetector::kSts, 
              DataType::DataType_t dataid = DataType::kData, 
              const char* name = "StsSensorType", 
              const char* title = "Sts Sensor Type Static Data", 
              const char* context = "StsDefaultContext", 
              Bool_t ownership=kTRUE);

    virtual ~StsSensorType(void);
    StsSensorType(const StsSensorType& from);
    StsSensorType& operator=(const StsSensorType& from);

    // RuntimeDB interfaces 
    void   clear(void);
    void   putParams(FairParamList* list);
    Bool_t getParams(FairParamList* list);

    // Dump Object
    void   Print();

    /// Getter Functions
    Int_t    GetId()            const { return fId; }
    string   GetTypeName()      const { return fTypeName; }
    string   GetProcessing()    const { return fProcessing; }
    Double_t GetWidth()         const { return fWidth; }
    Double_t GetHeight()        const { return fHeight; }
    Double_t GetAliMarkWidth()  const { return fAliMarkWidth; }
    Double_t GetAliMarkHeight() const { return fAliMarkHeight; }
    Int_t    GetStripsPerSide() const { return fStripsPerSide; }
    Double_t GetPitch()         const { return fPitch; }
    Double_t GetThickness()     const { return fThickness; }
    Double_t GetStereoAngleP()  const { return fStereoAngleP; }
    Double_t GetStereoAngleN()  const { return fStereoAngleN; }
    string   GetComment()       const { return fComment; }


    UInt_t GetIndex(UInt_t /*def*/) const { return fId; }

    /// Setter Functions
    void SetId(Int_t value)               { fId = value; }
    void SetTypeName(string value)        { fTypeName = value; }
    void SetProcessing(string value)      { fProcessing = value; }
    void SetWidth(Double_t value)         { fWidth = value; }
    void SetHeight(Double_t value)        { fHeight = value; }
    void SetAliMarkWidth(Double_t value)  { fAliMarkWidth = value; }
    void SetAliMarkHeight(Double_t value) { fAliMarkHeight = value; }
    void SetStripsPerSide(Int_t value)    { fStripsPerSide = value; }
    void SetPitch(Double_t value)            { fPitch = value; }
    void SetThickness(Double_t value)     { fThickness = value; }
    void SetStereoAngleP(Double_t value)  { fStereoAngleP = value; }
    void SetStereoAngleN(Double_t value)  { fStereoAngleN = value; }
    void SetComment(string value)         { fComment = value; }


    // Add-ons: SQL descriptors for the parameter class
    virtual std::string GetTableDefinition(const char* Name = 0);

    // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;

  private:
    TObjArray* fOpticalInspections;

    Int_t    fId;
    string   fTypeName;
    string   fProcessing;
    Double_t fWidth;
    Double_t fHeight;
    Double_t fAliMarkWidth;
    Double_t fAliMarkHeight;
    Int_t    fStripsPerSide;
    Double_t    fPitch;
    Double_t fThickness;
    Double_t fStereoAngleP;
    Double_t fStereoAngleN;
    string   fComment;

    ClassDef(StsSensorType,1); // ROOT class definition
};

#endif /* !STSSENSORTYPE_H*/

